#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
//#include <tetrominoes.h>
/*Windows: (for testing only)*/
//#include <windows.h>
//#include <conio.h>
/*Linux:*/
#include <unistd.h>

/*Tetris pieces: (might be stored in tetrominoes.h later)*/
const bool TETROMINOES[7][8][8] = {
{
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1},//I
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{1,1,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0},
{1,1,1,1,1,1,0,0},
{1,1,1,1,1,1,0,0},//J
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{0,0,0,0,1,1,0,0},
{0,0,0,0,1,1,0,0},
{1,1,1,1,1,1,0,0},
{1,1,1,1,1,1,0,0},//L
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{0,0,1,1,1,1,0,0},
{0,0,1,1,1,1,0,0},
{0,0,1,1,1,1,0,0},
{0,0,1,1,1,1,0,0},//O
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{0,0,1,1,1,1,0,0},
{0,0,1,1,1,1,0,0},
{1,1,1,1,0,0,0,0},
{1,1,1,1,0,0,0,0},//S
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{0,0,1,1,0,0,0,0},
{0,0,1,1,0,0,0,0},
{1,1,1,1,1,1,0,0},
{1,1,1,1,1,1,0,0},//T
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{1,1,1,1,0,0,0,0},
{1,1,1,1,0,0,0,0},
{0,0,1,1,1,1,0,0},
{0,0,1,1,1,1,0,0},//Z
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
};

/*Variables used by all games:*/
const float ARRAY_HEIGHT = 38.0;
const float ARRAY_WIDTH = 76.0;
float TOP_MARGIN; /*The margins bound the area controlled by the game.*/
float BOT_MARGIN;
float LEFT_MARGIN;
float RIGHT_MARGIN;
float BOT_END; /*= ARRAY_HEIGHT - BOTTOM_MARGIN - 1.0*/
float RIGHT_END; /*= ARRAY_WIDTH - RIGHT_MARGIN - 1.0*/

/*Functions usable by all games:*/
bool** make2DArray(float HEIGHT, float WIDTH) {
  int i;
  bool** ledArray = (bool**) malloc(HEIGHT*sizeof(bool*));
  for (i = 0; i < HEIGHT; i++) {
    ledArray[i] = (bool*) malloc(WIDTH*sizeof(bool));
  }
  return ledArray;
}
void fill2DArray(bool** ledArray, float HEIGHT, float WIDTH, bool lightsOn) {
  int i, j;
  for(i = 0; i < HEIGHT; i++) {
    for(j = 0; j < WIDTH; j++) {
      ledArray[i][j] = lightsOn;
    }
  }
  return;
}
void free2DArray(bool** ledArray, int HEIGHT) {
  int i;
  for (i = 0; i < HEIGHT; i++) {
    free(ledArray[i]);
  }
  free(ledArray);
  return;
}
int* make1DArray(int LENGTH) {
  int* longArray = (int*) malloc(LENGTH*sizeof(int));
  int i;
  for(i = 0; i < LENGTH; i++) {
    longArray[i] = -1;
  }
  return longArray;
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
  /*
  if(kbhit()){
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
int getRightInput(void) { /*This is a placeholder for a lodev function*/
  return 0;
}
void printTest(bool** ledArray) {
  int i, j;
  for(i = TOP_MARGIN; i <= BOT_END; i++) {
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
void frameTest(bool** ledArray) {
  /*Windows:*/
  //Sleep(5);
  //system("cls");
  /*Linux:*/
  usleep(50000);
  system("clear");

  printTest(ledArray);
}

/*Tetris-specific functions:*/
void refillBag(int* doubleBag, bool firstFill) {
  int i, j;
  if(firstFill == true) {
    for(i = 0; i < 7; i++) {
      do {
        doubleBag[i] = rand()%7;
        for(j = 0; j < i; j++) {
          if(doubleBag[i] == doubleBag[j]) {
            break;
          }
        }
      } while(i != j);
    }
  }
  for(i = 7; i < 14; i++) {
    do {
      doubleBag[i] = rand()%7;
      for(j = 7; j < i; j++) {
        if(doubleBag[i] == doubleBag[j]) {
          break;
        }
      }
    } while(i != j);
  }
}
void shiftBag(int* doubleBag) {
  int i;
  for(i = 0; i < 13; i++) {
    doubleBag[i] = doubleBag[i + 1];
  }
  doubleBag[i] = -1;
}
int pluckBag(int* doubleBag) {
  int i = doubleBag[0];
  shiftBag(doubleBag);
  if(doubleBag[7] == -1){
    refillBag(doubleBag, false);
  }
  return i;
}
void importPiece(bool** curPiece, int curType, int pieceOrien, int PIECE_WIDTH) {
  int i, j;
  if(curType == 3) {
    pieceOrien = 1; /*Rotation for "O" is purposely broken for efficiency and conformation.*/
  }
  if(curType != 0) {
    fill2DArray(curPiece, PIECE_WIDTH, PIECE_WIDTH, false);
    PIECE_WIDTH -= (PIECE_WIDTH / 4); /*Moves the pivot for rotation.*/
  }
  for(j = PIECE_WIDTH - 1; j >= 0; j--) {
    for(i = PIECE_WIDTH - 1; i >= 0; i--) {
      if(pieceOrien == 1) {
        curPiece[j][i] = TETROMINOES[curType][j][i];
      }
      else if (pieceOrien == 2) {
        curPiece[j][i] = TETROMINOES[curType][PIECE_WIDTH - 1 - i][j];
      }
      else if (pieceOrien == 3) {
        curPiece[j][i] = TETROMINOES[curType][PIECE_WIDTH - 1 - j][PIECE_WIDTH - 1 - i];
      }
      else if (pieceOrien == 4) {
        curPiece[j][i] = TETROMINOES[curType][i][PIECE_WIDTH - 1 - j];
      }
    }
  }
  return;
}
void copyPiece(bool** destPiece, bool** sourcePiece, int PIECE_WIDTH) {
  int i, j;
  for(j = PIECE_WIDTH - 1; j >= 0; j--) {
    for(i = PIECE_WIDTH - 1; i >= 0; i--) {
      destPiece[j][i] = sourcePiece[j][i];
    }
  }
  return;
}
void drawPiece(bool** ledArray, bool** curPiece, bool lightsOn, int curY, int curX, int PIECE_WIDTH) {
  int i, j;
  for(j = PIECE_WIDTH - 1; j >= 0; j--) {
    for(i = PIECE_WIDTH - 1; i >= 0; i--) {
      if(curPiece[j][i] == true) {
        ledArray[curY + j][curX + i] = lightsOn;
      }
    }
  }
  return;
}
int checkOverlap(bool** ledArray, bool** projPiece, bool** curPiece, int projY, int projX, int curY, int curX, int PIECE_WIDTH, int SQUARE_WIDTH, bool spawn) {
  int i, j;
  for(j = PIECE_WIDTH - 1; j >= 0; j -= SQUARE_WIDTH) {
    for(i = PIECE_WIDTH - 1; i >= 0; i -= SQUARE_WIDTH) {
      if(projPiece[j][i] == true && ledArray[projY + j][projX + i] == true) {
        if(spawn == false
           && i + projX - curX < PIECE_WIDTH
           && i + projX - curX >= 0
           && j + projY - curY < PIECE_WIDTH
           && j + projY - curY >= 0
           && curPiece[j + projY - curY][i + projX - curX] == true) {
            continue;
        }
        return 1;
      }
    }
  }
  return 0;
}
int checkLines(bool** ledArray, int leftBound, int rightBound, int botBound, int topBound, int score, int SQUARE_WIDTH) {
  int i, j;
  bool breaker = false;
  for(j = botBound; j >= topBound; j-= SQUARE_WIDTH) {
    for(i = rightBound; i >= leftBound; i -= SQUARE_WIDTH) {
      if(ledArray[j][i] == false) {
        breaker = true;
        break;
      }
    }
    if(breaker == true) {
      break;
    }
  }
  for(j -= 1; j <= botBound; j++) {
    for(i = rightBound; i >= leftBound; i--) {
      ledArray[j][i] = false;
    }
    score += 50; /*Points per full line clear divided by SQUARE_WIDTH*/
  }
  return score;
}

void tetris(bool** ledArray) {
  /*Setting values of the global variables for Tetris:*/
  TOP_MARGIN = 0.0;
  BOT_MARGIN = 0.0;
  LEFT_MARGIN = 0.0;
  RIGHT_MARGIN = 0.0;
  BOT_END = ARRAY_HEIGHT - BOT_MARGIN - 1.0;
  RIGHT_END = ARRAY_WIDTH - RIGHT_MARGIN - 1.0;

  /*Tetris-specific constants:*/
  const int PIECE_WIDTH = 8; /*Must be evenly divisible by 4*/
  const int SQUARE_WIDTH = PIECE_WIDTH / 4;
  const int LEFT_BORDER = 2; /*Should be a multiple of SQUARE_WIDTH*/
  const int RIGHT_BORDER = 2;
  const int TOP_BORDER = 0;
  const int BOT_BORDER = 2;
  const int INIT_X = LEFT_MARGIN + (RIGHT_END - LEFT_MARGIN - PIECE_WIDTH) / 2 - 1;
  const int INIT_Y = TOP_BORDER;
  srand(time(NULL));

  /*"Bag" of upcoming piece types: ("double" indicating two sets)*/
  int* doubleBag = make1DArray(14);
  refillBag(doubleBag, true);

  /*Tetris-specific variables:*/
  float curX = INIT_X;
  float curY = INIT_Y;
  float projX = curX;
  float projY = curY;
  int curType = pluckBag(doubleBag);
  int pieceOrien = 1; /*1-4 corresponds to north, east, south, west*/
  int score = 0;
  int input = 0;
  int timer = 1;
  int dropTime = 5; /*Should be > 1; may be decreased for difficulty*/

  /*Solid borders:*/
  drawRectangle(ledArray, true, TOP_MARGIN, LEFT_MARGIN, BOT_END - TOP_MARGIN + 1, LEFT_BORDER);
  drawRectangle(ledArray, true, TOP_MARGIN, RIGHT_END + 1 - RIGHT_BORDER, BOT_END - TOP_MARGIN + 1, RIGHT_BORDER);
  drawRectangle(ledArray, true, TOP_MARGIN, LEFT_MARGIN, TOP_BORDER, RIGHT_END - LEFT_MARGIN + 1);
  drawRectangle(ledArray, true, BOT_END + 1 - BOT_BORDER, LEFT_MARGIN, BOT_BORDER, RIGHT_END - LEFT_MARGIN + 1);

  /*Current piece state and its next projected state:*/
  bool** curPiece = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  importPiece(curPiece, curType, pieceOrien, PIECE_WIDTH);
  bool** projPiece = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  copyPiece(projPiece, curPiece, PIECE_WIDTH);

  while(1) {
    drawPiece(ledArray, curPiece, false, curY, curX, PIECE_WIDTH);
    copyPiece(curPiece, projPiece, PIECE_WIDTH);
    curX = projX;
    curY = projY;
    drawPiece(ledArray, curPiece, true, curY, curX, PIECE_WIDTH);
    frameTest(ledArray);
    input = 0;
    input = getLeftInput();
    if (input == 1) { /*Hard drop*/
      while(checkOverlap(ledArray, projPiece, curPiece, projY + SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, SQUARE_WIDTH, false) == 0) {
        projY += SQUARE_WIDTH;
        drawPiece(ledArray, curPiece, false, curY, curX, PIECE_WIDTH);
        copyPiece(curPiece, projPiece, PIECE_WIDTH);
        curX = projX;
        curY = projY;
        drawPiece(ledArray, curPiece, true, curY, curX, PIECE_WIDTH);
        frameTest(ledArray);
      }
      projX = INIT_X;
      projY = INIT_Y;
      curX = projX;
      curY = projY;
      curType = pluckBag(doubleBag);
      pieceOrien = 1;
      timer = 1;
      importPiece(curPiece, curType, pieceOrien, PIECE_WIDTH);
      copyPiece(projPiece, curPiece, PIECE_WIDTH);
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX, curY, curX, PIECE_WIDTH, SQUARE_WIDTH, true)) {
        break; /*Game loss*/
      }
    }
    else if(input == 5 || timer++ % dropTime == 0) { /*Soft drop*/
      if(checkOverlap(ledArray, projPiece, curPiece, projY + SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, SQUARE_WIDTH, false)) {
        projX = INIT_X;
        projY = INIT_Y;
        curX = projX;
        curY = projY;
        curType = pluckBag(doubleBag);
        pieceOrien = 1;
        timer = 1;
        importPiece(curPiece, curType, pieceOrien, PIECE_WIDTH);
        copyPiece(projPiece, curPiece, PIECE_WIDTH);
        if(checkOverlap(ledArray, projPiece, curPiece, projY, projX, curY, curX, PIECE_WIDTH, SQUARE_WIDTH, true)) {
          break; /*Game loss*/
        }
      }
      else {
        projY += SQUARE_WIDTH;
        timer = 1;
      }
    }
    else if(input == 9) { /*Spin clockwise (non-functional)*/
      pieceOrien++;
      if(pieceOrien > 4) {
        pieceOrien -= 4;
      }
      importPiece(projPiece, curType, pieceOrien, PIECE_WIDTH);
      /*checkOverlap() -> if it returns true then try to bump it?*/
    }
    else if(input == 10) { /*Spin counterclockwise (non-functional)*/
      pieceOrien--;
      if(pieceOrien < 1) {
        pieceOrien += 4;
      }
      importPiece(projPiece, curType, pieceOrien, PIECE_WIDTH);
      /*checkOverlap() -> if it returns true then try to bump it?*/
    }
    else if(input == 3) { /*Move right*/
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX + SQUARE_WIDTH, curY, curX, PIECE_WIDTH, SQUARE_WIDTH, false) == 0) {
        projX += SQUARE_WIDTH;
      }
    }
    else if(input == 7) { /*Move left*/
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX - SQUARE_WIDTH, curY, curX, PIECE_WIDTH, SQUARE_WIDTH, false) == 0) {
        projX -= SQUARE_WIDTH;
      }
    }
  }
  drawPiece(ledArray, curPiece, true, curY, curX, PIECE_WIDTH);
  frameTest(ledArray);
  free(doubleBag);
  free2DArray(curPiece, PIECE_WIDTH);
  free2DArray(projPiece, PIECE_WIDTH);
  return;
}

int main (void) {
  /*This function's contents are placeholders for a lodev menu.*/
  bool** ledArray;
  ledArray = make2DArray(ARRAY_HEIGHT, ARRAY_WIDTH);
  int input = 0;
  do {
    printf("Welcome to Tetris (official bootleg edition).\nEnter \"1\" to play Tetris.\nEnter \"0\" to exit.");
    scanf("%d", &input);
  } while(input < 0 || input > 1);
  while (input == 1) {
      fill2DArray(ledArray, ARRAY_HEIGHT, ARRAY_WIDTH, false);
      tetris(ledArray);
      do {
        printf("Enter \"1\" to play again.\nEnter \"0\" to exit.\n");
        scanf("%d", &input);
      } while (input < 0 || input > 1);
  }
  free2DArray(ledArray, ARRAY_HEIGHT);
  return 0;
}
