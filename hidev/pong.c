#include "pong.h"

//Game-agnostic functions (require lodev support):
int getLeftInput(void) { //This is a placeholder for a lodev function
/*
  if(kbhit()) {
    int input = getch();
    if(input == 'w') {
      return 1;
    }
    else if(input == 'd') {
      return 3;
    }
    else if(input == 's') {
      return 5;
    }
    else if(input == 'a') {
      return 7;
    }
    else if(input == 'e') {
      return 9;
    }
    else if(input == 'q') {
      return 10;
    }
  }
*/
  return 0;
}
int getRightInput(void) { //This is a placeholder for a lodev function
/*
  if(kbhit()) {
    int input = getch();
    if(input == 'i') {
      return 1;
    }
    else if(input == 'l') {
      return 3;
    }
    else if(input == 'k') {
      return 5;
    }
    else if(input == 'j') {
      return 7;
    }
    else if(input == 'o') {
      return 9;
    }
    else if(input == 'u') {
      return 10;
    }
  }
*/
  return 0;
}

void printWinner(int winner) { //This is a placeholder for a lodev function
  //This should take 1 or 2 and congradulate the corresponding player.
}


/*Pong-specific functions:*/
void drawBall(bool** ledArray, bool lightsOn, int topY, int leftX, int BALL_DIAMETER, float LEFT_MARGIN, float RIGHT_END) {
  int i, j;
  for(j = BALL_DIAMETER - 1; j >= 0; j--) {
    if(leftX + j <= RIGHT_END && leftX + j >= LEFT_MARGIN) {
      for(i = BALL_DIAMETER - 1; i >= 0; i--) {
        ledArray[topY + i][leftX + j] = lightsOn;
      }
    }
  }
  return;
}
int checkWallImpact(float* ballY, int BALL_DIAMETER, float TOP_MARGIN, float BOT_END) {
  //Modifies ballY and returns -1 if there was an impact
  if(*ballY < TOP_MARGIN) {
    *ballY = TOP_MARGIN + (TOP_MARGIN - *ballY);
    return - 1;
  }
  else if(*ballY + BALL_DIAMETER - 1 > BOT_END) {
    *ballY = BOT_END - (BALL_DIAMETER - 1) - (*ballY + BALL_DIAMETER - 1 - BOT_END);
    return - 1;
  }
  return 1;
}
void checkLeftImpact(float ballY, float* ballX, float LEFT_IMPACT_X, float* ballDeltaY, float* ballDeltaX, int BALL_DIAMETER, float leftPadY, int PAD_HEIGHT, float MAX_BALL_Y_SPEED) {
  float ballImpactY = ballY + (LEFT_IMPACT_X - *ballX) * (*ballDeltaY / *ballDeltaX);
  //Checking if the ball hit the inner side of left_paddle:
  //(two conditions for vertical position, one checking if impact occurred this cycle
  if(ballImpactY + BALL_DIAMETER - 1 >= leftPadY
     && ballImpactY <= leftPadY + PAD_HEIGHT - 1
     && *ballX - *ballDeltaX >= LEFT_IMPACT_X) {
    float impactSection = (ballImpactY - (leftPadY - BALL_DIAMETER + 1)) * 8 / (PAD_HEIGHT + BALL_DIAMETER);
    if(impactSection <= 1){
      *ballDeltaY = - MAX_BALL_Y_SPEED;
    }
    else if(impactSection <= 2){
      *ballDeltaY = - MAX_BALL_Y_SPEED * 3 / 4;
    }
    else if(impactSection <= 3){
      *ballDeltaY = - MAX_BALL_Y_SPEED * 2 / 4;
    }
    else if(impactSection <= 4){
      *ballDeltaY = - MAX_BALL_Y_SPEED / 4;
    }
    else if(impactSection <= 5){
      *ballDeltaY = MAX_BALL_Y_SPEED / 4;
    }
    else if(impactSection <= 6){
      *ballDeltaY = MAX_BALL_Y_SPEED * 2 / 4;
    }
    else if(impactSection <= 7){
      *ballDeltaY = MAX_BALL_Y_SPEED * 3 / 4;
    }
    else if(impactSection <= 8){
      *ballDeltaY = MAX_BALL_Y_SPEED;
    }
    *ballX = LEFT_IMPACT_X + (LEFT_IMPACT_X - *ballX);
    if(- *ballDeltaX * 1.05 < 1) {
      *ballDeltaX *= 1.05;
    }
    *ballDeltaX *= - 1.0;
  }
  return;
}
void checkRightImpact(float ballY, float* ballX, float RIGHT_IMPACT_X, float* ballDeltaY, float* ballDeltaX, int BALL_DIAMETER, float rightPadY, int PAD_HEIGHT, float MAX_BALL_Y_SPEED) {
  float ballImpactY = ballY - (*ballX - RIGHT_IMPACT_X) * (*ballDeltaY / *ballDeltaX);
  //Checking if the ball hit the inner side of right_paddle:
  //(two conditions for vertical position, one checking if impact occurred this cycle)
  if(ballImpactY + BALL_DIAMETER - 1 >= rightPadY
     && ballImpactY <= rightPadY + PAD_HEIGHT - 1
     && *ballX - *ballDeltaX <= RIGHT_IMPACT_X) {
    float impactSection = (ballImpactY - (rightPadY - BALL_DIAMETER + 1)) * 8 / (PAD_HEIGHT + BALL_DIAMETER);
    if(impactSection <= 1){
      *ballDeltaY = - MAX_BALL_Y_SPEED;
    }
    else if(impactSection <= 2){
      *ballDeltaY = - MAX_BALL_Y_SPEED * 3 / 4;
    }
    else if(impactSection <= 3){
      *ballDeltaY = - MAX_BALL_Y_SPEED * 2 / 4;
    }
    else if(impactSection <= 4){
      *ballDeltaY = - MAX_BALL_Y_SPEED / 4;
    }
    else if(impactSection <= 5){
      *ballDeltaY = MAX_BALL_Y_SPEED / 4;
    }
    else if(impactSection <= 6){
      *ballDeltaY = MAX_BALL_Y_SPEED * 2 / 4;
    }
    else if(impactSection <= 7){
      *ballDeltaY = MAX_BALL_Y_SPEED * 3 / 4;
    }
    else if(impactSection <= 8){
      *ballDeltaY = MAX_BALL_Y_SPEED;
    }
    *ballX = RIGHT_IMPACT_X - (*ballX - RIGHT_IMPACT_X);
    if(*ballDeltaX * 1.05 < 1) {
      *ballDeltaX *= 1.05;
    }
    *ballDeltaX *= - 1.0;
  }
  return;
}
void resetBall(float* ballX, float* ballY, float* ballDeltaX, int BALL_DIAMETER, float TOP_MARGIN, float BOT_END, float LEFT_MARGIN, float RIGHT_END, float INIT_BALL_SPEED) {
  *ballX = LEFT_MARGIN + (RIGHT_END - BALL_DIAMETER - LEFT_MARGIN) / 2;
  *ballY = TOP_MARGIN + (BOT_END - BALL_DIAMETER - TOP_MARGIN) / 2;
  *ballDeltaX = INIT_BALL_SPEED;
  return;
}
int movePaddle(int padDir, float* topY, float MOVE_DISTANCE, int PAD_HEIGHT, bool** ledArray, float LEFT_X, int PAD_WIDTH, float TOP_MARGIN, float BOT_END) {
  if(padDir != 1 && padDir != 5) {
    padDir = 0;
    return padDir;
  }
  else if(padDir == 1 && (int) (*topY + 0.5) - MOVE_DISTANCE < TOP_MARGIN) {
    padDir = 0;
    return padDir;
  }
  else if(padDir == 5 && (int) (*topY + 0.5) + PAD_HEIGHT - 1 + MOVE_DISTANCE > BOT_END) {
    padDir = 0;
    return padDir;
  }
  if(padDir == 1 && (int) (*topY) != (int) (*topY - MOVE_DISTANCE)) {
    drawRectangle(ledArray, false, *topY + 0.5, LEFT_X, PAD_HEIGHT, PAD_WIDTH);
    *topY -= MOVE_DISTANCE;
    drawRectangle(ledArray, true, *topY + 0.5, LEFT_X, PAD_HEIGHT, PAD_WIDTH);
  }
  else if(padDir == 5 && (int) (*topY) != (int) (*topY + MOVE_DISTANCE)) {
    drawRectangle(ledArray, false, *topY + 0.5, LEFT_X, PAD_HEIGHT, PAD_WIDTH);
    *topY += MOVE_DISTANCE;
    drawRectangle(ledArray, true, *topY + 0.5, LEFT_X, PAD_HEIGHT, PAD_WIDTH);
  }
  return padDir;
}

void pong(bool** ledArray) {

  //Playfield constants:
  const float TOP_MARGIN = 0.0; //The margins bound the area controlled by the game.
  const float BOT_MARGIN = 0.0;
  const float LEFT_MARGIN = 0.0;
  const float RIGHT_MARGIN = 0.0;
  const float BOT_END = ARRAY_HEIGHT - BOT_MARGIN - 1.0;
  const float RIGHT_END = ARRAY_WIDTH - RIGHT_MARGIN - 1.0;

  //Object constants:
  const int BALL_DIAMETER = 2;
  const float INIT_BALL_X_SPEED = 0.5;
  const float MAX_BALL_Y_SPEED = 0.5;

  const int PAD_HEIGHT = 8;
  const int PAD_WIDTH = 2;
  const int PAD_DISTANCE = 3; //The distance from each paddle to its side.
  const float LEFT_PAD_X = LEFT_MARGIN + PAD_DISTANCE; //Refers to the innermost pixel on the top of the paddle.
  const float RIGHT_PAD_X = RIGHT_END - (PAD_WIDTH - 1.0) - PAD_DISTANCE;
  const float LEFT_IMPACT_X = LEFT_PAD_X + PAD_WIDTH;
  const float RIGHT_IMPACT_X = RIGHT_PAD_X - BALL_DIAMETER;
  const float PAD_MOVE_DISTANCE = 1.0;

  //Object initialization:
  float ballDeltaX = INIT_BALL_X_SPEED;
  float ballDeltaY = 0;
  float ballX = LEFT_MARGIN + (RIGHT_END - BALL_DIAMETER - LEFT_MARGIN) / 2;
  float ballY = TOP_MARGIN + (BOT_END - BALL_DIAMETER - TOP_MARGIN) / 2; /*the last number is to test stationary impacts*/
  float prevBallX = ballX;
  float prevBallY = ballY;

  float leftPadY = TOP_MARGIN + (BOT_END + 1 - TOP_MARGIN) / 2 - PAD_HEIGHT / 2;
  float rightPadY = TOP_MARGIN + (BOT_END + 1 - TOP_MARGIN) / 2 - PAD_HEIGHT / 2;
  int leftPadDir = 0;
  int rightPadDir = 0;
  drawRectangle(ledArray, true, rightPadY + 0.5, RIGHT_PAD_X, PAD_HEIGHT, PAD_WIDTH);
  drawRectangle(ledArray, true, leftPadY + 0.5, LEFT_PAD_X, PAD_HEIGHT, PAD_WIDTH);

  //Score initialization
  int leftScore = 0;
  int rightScore = 0;
  int scoreDelay = 10; //Deactivates ball movement for a number of cycles after someone scores

  while(1) {
    frameTest(ledArray, TOP_MARGIN, LEFT_MARGIN, BOT_END, RIGHT_END);

    //Paddle movement:
    leftPadDir = getLeftInput();
    leftPadDir = movePaddle(leftPadDir, &leftPadY, PAD_MOVE_DISTANCE, PAD_HEIGHT, ledArray, LEFT_PAD_X, PAD_WIDTH, TOP_MARGIN, BOT_END);
    rightPadDir = getRightInput();
    rightPadDir = movePaddle(rightPadDir, &rightPadY, PAD_MOVE_DISTANCE, PAD_HEIGHT, ledArray, RIGHT_PAD_X, PAD_WIDTH, TOP_MARGIN, BOT_END);

    //Ball movement:
    if (scoreDelay > 0) {
      scoreDelay--;
    }
    else {
      drawBall(ledArray, false, prevBallY + 0.5, prevBallX + 0.5, BALL_DIAMETER, LEFT_MARGIN, RIGHT_END);
      drawBall(ledArray, true, ballY + 0.5, ballX + 0.5, BALL_DIAMETER, LEFT_MARGIN, RIGHT_END);
      prevBallX = ballX;
      prevBallY = ballY;

      ballY += ballDeltaY;
      ballDeltaY *= checkWallImpact(&ballY, BALL_DIAMETER, TOP_MARGIN, BOT_END);

      ballX += ballDeltaX;
      if(ballX <= LEFT_IMPACT_X) { //A paddle collision or score is possible
        drawRectangle(ledArray, true, leftPadY + 0.5, LEFT_PAD_X, PAD_HEIGHT, PAD_WIDTH);
        checkLeftImpact(ballY, &ballX, LEFT_IMPACT_X, &ballDeltaY, &ballDeltaX, BALL_DIAMETER, leftPadY, PAD_HEIGHT, MAX_BALL_Y_SPEED);

        //Check if the ball hit the edge of the paddle (not yet done) (non-critical functionality)

        //Checking if the ball hit the left edge of the screen:
        if(ballX + BALL_DIAMETER < LEFT_MARGIN) {
          drawBall(ledArray, false, prevBallY + 0.5, prevBallX + 0.5, BALL_DIAMETER, LEFT_MARGIN, RIGHT_END);
          rightScore++; //Also change displayed score, if it exists.
          if(rightScore == 3) {
            drawRectangle(ledArray, false, rightPadY + 0.5, RIGHT_PAD_X, PAD_HEIGHT, PAD_WIDTH);
            drawRectangle(ledArray, false, leftPadY + 0.5, LEFT_PAD_X, PAD_HEIGHT, PAD_WIDTH);
            printWinner(1);
            break;
          }
          resetBall(&ballX, &ballY, &ballDeltaX, BALL_DIAMETER, TOP_MARGIN, BOT_END, LEFT_MARGIN, RIGHT_END, INIT_BALL_X_SPEED);
          scoreDelay = 10;
        }
      }
      else if(ballX > RIGHT_IMPACT_X) { //A paddle collision or score is possible
        drawRectangle(ledArray, true, rightPadY + 0.5, RIGHT_PAD_X, PAD_HEIGHT, PAD_WIDTH);
        checkRightImpact(ballY, &ballX, RIGHT_IMPACT_X, &ballDeltaY, &ballDeltaX, BALL_DIAMETER, rightPadY, PAD_HEIGHT, MAX_BALL_Y_SPEED);

        //Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)

        //Checking if the ball hit the right edge of the screen:
        if(ballX > RIGHT_END) {
          drawBall(ledArray, false, prevBallY + 0.5, prevBallX + 0.5, BALL_DIAMETER, LEFT_MARGIN, RIGHT_END);
          leftScore++; //Also change displayed score, if it exists.
          if(leftScore == 3) {
            drawRectangle(ledArray, false, rightPadY + 0.5, RIGHT_PAD_X, PAD_HEIGHT, PAD_WIDTH);
            drawRectangle(ledArray, false, leftPadY + 0.5, LEFT_PAD_X, PAD_HEIGHT, PAD_WIDTH);
            printWinner(2);
            break;
          }
          resetBall(&ballX, &ballY, &ballDeltaX, BALL_DIAMETER, TOP_MARGIN, BOT_END, LEFT_MARGIN, RIGHT_END, INIT_BALL_X_SPEED);
          scoreDelay = 10;
        }
      }
    }
  }
  return;
}

/*int main (void) {
  //This function's contents are throwaway. They mimic a lodev menu.
  const float ARRAY_HEIGHT = 38.0;
  const float ARRAY_WIDTH = 76.0;
  bool** ledArray;
  ledArray = make2DArray(ARRAY_HEIGHT, ARRAY_WIDTH);
  fill2DArray(ledArray, ARRAY_HEIGHT, ARRAY_WIDTH, false);
  pong(ledArray);
  free2DArray(ledArray, ARRAY_HEIGHT);
  return 0;
}*/
