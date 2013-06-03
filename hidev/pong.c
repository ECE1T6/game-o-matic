#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
/*Windows:*/
//#include <windows.h>
/*Linux:*/
#include <unistd.h>
/*C++:*/
//#include "stdafx.h"
//#include "targetver.h"

/*Variables used by all games:*/
const float ARRAY_HEIGHT = 48.0;
const float ARRAY_WIDTH = 64.0;
float TOP_MARGIN; /*The margins bound the playable area of the game.*/
float BOTTOM_MARGIN;
float LEFT_MARGIN;
float RIGHT_MARGIN;
float BOTTOM_END; /*= ARRAY_HEIGHT - BOTTOM_MARGIN - 1.0*/
float RIGHT_END; /*= ARRAY_WIDTH - RIGHT_MARGIN - 1.0*/

/*Functions usable by all games:*/
bool** makeArray(float HEIGHT, float WIDTH) {
  int i;
  bool** ledArray = (bool**) malloc(HEIGHT*sizeof(bool*));
  for (i = 0; i < HEIGHT; i++) {
    ledArray[i] = (bool*) malloc(WIDTH*sizeof(bool));
  }
  return ledArray;
}
void fillArray(bool** ledArray, float HEIGHT, float WIDTH, bool lightsOn) {
  int i, j;
  for(i = 0; i < HEIGHT; i++) {
    for(j = 0; j < WIDTH; j++) {
      ledArray[i][j] = lightsOn;
    }
  }
  return;
}
freeArray(bool** ledArray) {
  int i;
  for (i = 0; i < ARRAY_HEIGHT; i++) {
    free(ledArray[i]);
  }
  free(ledArray);
  return;
}
void drawRectangle(bool** ledArray, bool lightsOn, int topY, int leftX, int HEIGHT, int WIDTH) {
  int i, j;
  for(j = HEIGHT - 1; j >= 0; j--) {
    for(i = WIDTH - 1; i >= 0; i--) {
      ledArray[topY + j][leftX + i] = lightsOn;
    }
  }
  return;
}
int getLeftInput(void) { /*This is a placeholder for a lodev function*/
  return 0;
}
int getRightInput(void) { /*This is a placeholder for a lodev function*/
  return 0;
}
void printTest(bool** ledArray) {
  int i, j;
  for(i = TOP_MARGIN; i <= BOTTOM_END; i++) {
    for(j = LEFT_MARGIN; j <= RIGHT_END; j++) {
      if(ledArray[i][j] == true) {
        printf("O", ledArray[i][j]);
      }
      else {
        printf("*", ledArray[i][j]);
      }
    }
    printf("\n");
  }
  return;
}

/*Pong-specific functions:*/
void drawBall(bool** ledArray, bool lightsOn, int topY, int leftX, int BALL_DIAMETER) {
  int i, j;
  for(j = BALL_DIAMETER; j >= 0; j--) {
    if(leftX + j <= RIGHT_END && leftX + BALL_DIAMETER + j >= LEFT_MARGIN) {
      for(i = BALL_DIAMETER; i >= 0; i--) {
        ledArray[topY + i][leftX + j] = lightsOn;
      }
    }
  }
  return;
}
int checkWallImpact(float* ballY, int BALL_DIAMETER) {
  if(*ballY < TOP_MARGIN) { /*Deflection off of the top side*/
    *ballY = TOP_MARGIN + (TOP_MARGIN - *ballY);
    return - 1;
  }
  else if(*ballY + BALL_DIAMETER > BOTTOM_END) { /*Deflection off of the bottom side*/
    *ballY = BOTTOM_END - BALL_DIAMETER - (*ballY - BOTTOM_END + BALL_DIAMETER);
    return - 1;
  }
  return 1;
}
int checkLeftImpact(float ballY, float* ballX, float LEFT_IMPACT_X, float ballDeltaY, float ballDeltaX, int BALL_DIAMETER, float leftPaddleY, int PADDLE_HEIGHT) {
  int ballImpactY = ballY - (LEFT_IMPACT_X - *ballX) * ballDeltaY / ballDeltaX;
  /*Checking if the ball hit the inner side of left_paddle: (two conditions for vertical position, one checking if impact occurred this cycle*/
  if(ballImpactY + BALL_DIAMETER >= leftPaddleY && ballImpactY <= leftPaddleY + PADDLE_HEIGHT - 1 && *ballX - ballDeltaX >= LEFT_IMPACT_X) {
    /*These ignore imparted paddle momentum right now:*/
    *ballX = LEFT_IMPACT_X + (LEFT_IMPACT_X - *ballX);
    return - 1;
  }
  return 1;
}
int checkRightImpact(float ballY, float* ballX, float RIGHT_IMPACT_X, float ballDeltaY, float ballDeltaX, int BALL_DIAMETER, float rightPaddleY, int PADDLE_HEIGHT) {
  int ballImpactY = ballY - (*ballX - RIGHT_IMPACT_X) * (ballDeltaY / ballDeltaX);
  /*Checking if the ball hit the inner side of right_paddle: (two conditions for vertical position, one checking if impact occurred this cycle)*/
  if(ballImpactY + BALL_DIAMETER >= rightPaddleY && ballImpactY <= rightPaddleY + PADDLE_HEIGHT - 1 && *ballX - ballDeltaX <= RIGHT_IMPACT_X) {
    /*These ignore imparted paddle momentum right now:*/
    *ballX = RIGHT_IMPACT_X - (*ballX - RIGHT_IMPACT_X);
    return - 1;
  }
  return 1;
}
void resetBall(float* ballX, float* ballY, int BALL_DIAMETER) {
  *ballX = LEFT_MARGIN + (RIGHT_END - BALL_DIAMETER - LEFT_MARGIN) / 2;
  *ballY = TOP_MARGIN + (BOTTOM_END - BALL_DIAMETER - TOP_MARGIN) / 2;
  return;
}
void displayWinner(int leftScore, int rightScore) {
  if(leftScore == 5) {
    /*print to screen that the left player won, or do some visual effect*/
  }
  else {
    /*print to screen that the right player won, or do some visual effect*/
  }
  return;
}
int movePaddle(int paddleDir, float* topY, float MOVE_DISTANCE, int PADDLE_HEIGHT, bool** ledArray, float LEFT_X, int PADDLE_WIDTH) {
  if(paddleDir == 1 && *topY + 0.5 - MOVE_DISTANCE < TOP_MARGIN) { /*Assuming: no move == 0, up == 1, right == 2, down == 3, left == 4*/
    paddleDir = 0;
  }
  else if(paddleDir == 3 && *topY + 0.5 + PADDLE_HEIGHT - 1 + MOVE_DISTANCE > BOTTOM_END) {
    paddleDir = 0;
  }
  drawRectangle(ledArray, false, *topY + 0.5, LEFT_X, PADDLE_HEIGHT, PADDLE_WIDTH);
  if(paddleDir == 1) {
    *topY -= MOVE_DISTANCE;
  }
  else if(paddleDir == 3) {
    *topY += MOVE_DISTANCE;
  }
  drawRectangle(ledArray, true, *topY + 0.5, LEFT_X, PADDLE_HEIGHT, PADDLE_WIDTH);
  return paddleDir;
}
void pong(bool** ledArray) {
  /*Setting values of the global variables for pong:*/
  TOP_MARGIN = 6.0;
  BOTTOM_MARGIN = 6.0;
  LEFT_MARGIN = 0.0;
  RIGHT_MARGIN = 0.0;
  BOTTOM_END = ARRAY_HEIGHT - BOTTOM_MARGIN - 1.0;
  RIGHT_END = ARRAY_WIDTH - RIGHT_MARGIN - 1.0;

  /*Pong-specific constants:*/
  const int BALL_DIAMETER = 1; /*Let this equal the intended diameter minus 1.*/
  const int PADDLE_HEIGHT = 8;
  const int PADDLE_WIDTH = 2;
  const int PADDLE_DISTANCE = 3; /*The distance between each paddle and the edge of the screen.*/
  const float PADDLE_MOVE_DISTANCE = 1.0;
  const float LEFT_PADDLE_X = LEFT_MARGIN + PADDLE_DISTANCE; /*Refers to the innermost pixel on the top of the paddle.*/
  const float RIGHT_PADDLE_X = RIGHT_END - (PADDLE_WIDTH - 1.0) - PADDLE_DISTANCE;
  const float LEFT_IMPACT_X = LEFT_PADDLE_X + PADDLE_WIDTH;
  const float RIGHT_IMPACT_X = RIGHT_PADDLE_X - BALL_DIAMETER - 1.0;

  /*Pong-specific variables:*/
  float ballDeltaX = sqrt(0.5);
  float ballDeltaY = - sqrt(0.5);
  float ballX = LEFT_MARGIN + (RIGHT_END - BALL_DIAMETER - LEFT_MARGIN) / 2;
  float ballY = TOP_MARGIN + (BOTTOM_END - BALL_DIAMETER - TOP_MARGIN) / 2 - 4.0; /*the last number is to test stationary impacts*/
  float leftPaddleY = TOP_MARGIN + (BOTTOM_END - BALL_DIAMETER - TOP_MARGIN) / 2 - PADDLE_HEIGHT / 2;
  float rightPaddleY = TOP_MARGIN + (BOTTOM_END - BALL_DIAMETER - TOP_MARGIN) / 2 - PADDLE_HEIGHT / 2;
  int leftScore = 0;
  int rightScore = 0;
  int scoreDelay = 10; /*Deactivates ball movement for a number of cycles after someone scores*/
  int leftPaddleDir = 0; /**/
  int rightPaddleDir = 0;

  drawRectangle(ledArray, true, rightPaddleY + 0.5, RIGHT_PADDLE_X, PADDLE_HEIGHT, PADDLE_WIDTH);
  drawRectangle(ledArray, true, leftPaddleY + 0.5, LEFT_PADDLE_X, PADDLE_HEIGHT, PADDLE_WIDTH);

  while (1) {
    printTest(ledArray);
    //Sleep(25);
    //system("cls");
    usleep(50000);
    system("clear");

    leftPaddleDir = getLeftInput();
    leftPaddleDir = movePaddle(leftPaddleDir, &leftPaddleY, PADDLE_MOVE_DISTANCE, PADDLE_HEIGHT, ledArray, LEFT_PADDLE_X, PADDLE_WIDTH);
    rightPaddleDir = getRightInput();
    rightPaddleDir = movePaddle(rightPaddleDir, &rightPaddleY, PADDLE_MOVE_DISTANCE, PADDLE_HEIGHT, ledArray, RIGHT_PADDLE_X, PADDLE_WIDTH);

    /*Ball movement:*/
    if (scoreDelay > 0) {
      scoreDelay--;
    }
    else if(scoreDelay == 0) {
      drawBall(ledArray, false, ballY + 0.5, ballX + 0.5, BALL_DIAMETER); /*Erase ball from array*/
      ballY += ballDeltaY;
      ballX += ballDeltaX;
      ballDeltaY *= checkWallImpact(&ballY, BALL_DIAMETER);
      if(ballX <= LEFT_IMPACT_X) { /*The ball has pierced the plane of left_paddle*/
        ballDeltaX *= checkLeftImpact(ballY, &ballX, LEFT_IMPACT_X, ballDeltaY, ballDeltaX, BALL_DIAMETER, leftPaddleY, PADDLE_HEIGHT);
        /*Check if the ball hit the edge of the paddle (not yet done) (non-critical functionality)*/
        /*Checking if the ball hit the left edge of the screen:*/
        if(ballX + BALL_DIAMETER < LEFT_MARGIN) {
          leftScore++; /*Also change displayed score*/
          if(leftScore == 3) {
            break; /*Goes to displayWinner()*/
          }
          resetBall(&ballX, &ballY, BALL_DIAMETER);
          scoreDelay = 10;
        }
      }
      else if(ballX >= RIGHT_IMPACT_X) { /*The ball has pierced the plane of right_paddle*/
        ballDeltaX *= checkRightImpact(ballY, &ballX, RIGHT_IMPACT_X, ballDeltaY, ballDeltaX, BALL_DIAMETER, rightPaddleY, PADDLE_HEIGHT);

        /*Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)*/

        /*Checking if the ball hit the left edge of the screen:*/
        if(ballX > RIGHT_END) {
          rightScore++; /*Also change displayed score*/
          if(rightScore == 3) {
            break; /*Goes to display_winner()*/
          }
          resetBall(&ballX, &ballY, BALL_DIAMETER);
          scoreDelay = 10;
        }
      }
      drawBall(ledArray, true, ballY + 0.5, ballX + 0.5, BALL_DIAMETER); /*Draw ball to array*/
    }
  }
  displayWinner(leftScore, rightScore);
}

int main (void) {
  bool** ledArray;
  ledArray = makeArray(ARRAY_HEIGHT, ARRAY_WIDTH);
  fillArray(ledArray, ARRAY_HEIGHT, ARRAY_WIDTH, false);

  /*Here we will assign the lodev display function to the array and decorate it*/

  int input = 0;
  do {
    printf("Enter \"1\" to play pong!");
    scanf("%d", &input);
  } while(input != 1);
  if(input == 1) {
    pong(ledArray);
  }

  freeArray(ledArray);
  return 0;
}
