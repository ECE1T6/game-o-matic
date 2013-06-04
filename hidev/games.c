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

/*Snake-specific structures:*/
struct segment {
  int x;
  int y;
  struct segment *next;
};

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

  while(1) {
    /*Windows:*/
    //Sleep(25);
    //system("cls");
    /*Linux:*/
    usleep(50000);
    system("clear");

    printTest(ledArray);

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

/*Snake-specific functions:*/
struct segment *addToList(struct segment *first, int newX, int newY, bool **ledArray) {
  struct segment *newSegment;
  newSegment = malloc(sizeof(struct segment));
  newSegment->x = newX;
  newSegment->y = newY;
  ledArray[newY][newX] = true;
  newSegment->next = first;
  return newSegment;
}
struct segment *resetList(int INITIAL_X, int INITIAL_Y, int INITIAL_SIZE, bool **ledArray) {
  struct segment *first = NULL;
  int i;
  for(i = 0; i < INITIAL_SIZE; i++) {
    first = addToList(first, INITIAL_X + i, INITIAL_Y, ledArray);
  }
  return first;
}
int checkCollision(int newX, int newY, struct segment *first) {
  struct segment *place;
  place = first;
  while (place != NULL) {
    if(newX == place->x && newY == place->y) {
      return 1;
    }
    place = place->next;
  }
  return 0;
}
int checkOutBounds(int headX, int headY){
  if(headY < TOP_MARGIN || headY > BOTTOM_END){
    return 1;
  }
  if(headX < LEFT_MARGIN || headX > RIGHT_END){
    return 1;
  }
  return 0;
}
void placeFood(int *foodX, int *foodY, struct segment *first, int HEIGHT, int WIDTH, int FIRST_TOP, int FIRST_LEFT, bool **ledArray) {
  do {
  *foodY = rand() % HEIGHT + FIRST_TOP;
  *foodX = rand() % WIDTH + FIRST_LEFT;
  } while (checkCollision(*foodX, *foodY, first)); /*Returns true if collision occurred.*/
  ledArray[*foodY][*foodX] = true;
  return;
}
int moveHeadCoords(int moveDir, float *headX, float *headY, float moveDistance) {
  int previousMoveDir = moveDir;
  moveDir = getLeftInput(); /*Assuming the left joystick will be used for single player games.*/
  if (moveDir != 1 && moveDir != 2 && moveDir != 3 && moveDir != 4) {
    moveDir = previousMoveDir;
  }
  else if(moveDir - previousMoveDir == 2 || previousMoveDir - moveDir == 2) { /*Reversing direction disallowed*/
    moveDir = previousMoveDir;
  }
  if (moveDir == 1) {
    *headY -= moveDistance;
  }
  else if (moveDir == 2) {
    *headX += moveDistance;
  }
  else if (moveDir == 3) {
    *headY += moveDistance;
  }
  else if (moveDir == 4) {
    *headX -= moveDistance;
  }
  return moveDir;
}
struct segment *truncateList(struct segment *first, bool **ledArray){
  struct segment *current, *previous;
  current = first;
  previous = NULL;
  while (current->next != NULL) {
    previous = current;
    current = current->next;
  }
  if (previous == NULL) { /*This allows truncateList() to be looped to delete the entire list.*/
    ledArray[current->y][current->x] = false;
    free(current);
    first = NULL;
    return first;
  }
  ledArray[current->y][current->x] = false;
  previous->next = NULL;
  free(current);
  return first;
}

void snake(bool** ledArray) {
  /*Setting values of the global variables for snake:*/
  TOP_MARGIN = 6.0;
  BOTTOM_MARGIN = 6.0;
  LEFT_MARGIN = 0.0;
  RIGHT_MARGIN = 0.0;
  BOTTOM_END = ARRAY_HEIGHT - BOTTOM_MARGIN - 1.0;
  RIGHT_END = ARRAY_WIDTH - RIGHT_MARGIN - 1.0;

  /*Snake-specific constants:*/
  const int INITIAL_X = LEFT_MARGIN + (RIGHT_END - LEFT_MARGIN) / 2;
  const int INITIAL_Y = TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2;
  const int INITIAL_SIZE = 5;
  srand(time(NULL));

  /*Snake-specific variables:*/
  float moveDistance = 0.2; /*May change over time/score or based on a difficulty selection. Must be < 1.*/
  struct segment *first = resetList(INITIAL_X, INITIAL_Y, INITIAL_SIZE, ledArray);
  float headX = first->x;
  float headY = first->y;
  int moveDir = 2; /*Assuming: no move == 0, up == 1, right == 2, down == 3, left == 4*/
  int foodX;
  int foodY;
  placeFood(&foodX, &foodY, first, BOTTOM_END - TOP_MARGIN, RIGHT_END - LEFT_MARGIN, TOP_MARGIN, LEFT_MARGIN, ledArray);
  int score = 0;

  while(1) {
    /*Windows:*/
    //Sleep(5);
    //system("cls");
    /*Linux:*/
    usleep(50000);
    system("clear");

    printTest(ledArray);

    moveDir = moveHeadCoords(moveDir, &headX, &headY, moveDistance);

    if(checkOutBounds(headX + 0.5, headY + 0.5)){
      /*Game loss. Clear everything:*/
      ledArray[foodY][foodX] = false;
      while(first != NULL) {
        first = truncateList(first, ledArray);
        /*Windows:*/
        //Sleep(5);
        //system("cls");
        /*Linux:*/
        usleep(5000);
        system("clear");
        printTest(ledArray);
      }
      break;
    }
    if((int) (headX + 0.5) == foodX && (int) (headY + 0.5) == foodY) {
      first = addToList(first, headX + 0.5, headY + 0.5, ledArray);
      placeFood(&foodX, &foodY, first, BOTTOM_END - TOP_MARGIN, RIGHT_END - LEFT_MARGIN, TOP_MARGIN, LEFT_MARGIN, ledArray);
      score += 10; /*Also change displayed score.*/
    }
    else if((int) (headX + 0.5) != first->x || (int) (headY + 0.5) != first->y) {
      first = addToList(first, headX + 0.5, headY + 0.5, ledArray);
      first = truncateList(first, ledArray); /*Also erases the last segment from the array*/
    }
    if(checkCollision(first->x, first->y, first->next)){ /*This should check the head against all body segments.*/
      /*Game loss. Clear everything:*/
      ledArray[foodY][foodX] = false;
      while(first != NULL) {
        first = truncateList(first, ledArray);

        /*Windows:*/
        //Sleep(5);
        //system("cls");
        /*Linux:*/
        usleep(5000);
        system("clear");
        printTest(ledArray);
      }
      break;
    }
  }
  /*Display final score.*/
  return;
}

int main (void) {
  bool** ledArray;
  ledArray = makeArray(ARRAY_HEIGHT, ARRAY_WIDTH);
  fillArray(ledArray, ARRAY_HEIGHT, ARRAY_WIDTH, false);

  /*Here we will assign the lodev display function to the array and decorate it*/

  int input = 0;
  do {
    printf("Welcome to the game-o-matic!\nEnter \"1\" to play pong!\nEnter \"2\" to play snake!\nEnter \"0\" to exit.");
    scanf("%d", &input);
  } while(input < 0 || input > 2);
  if(input == 1) {
    pong(ledArray);
  }
  if(input == 2) {
    snake(ledArray);
  }
  freeArray(ledArray);
  return 0;
}
