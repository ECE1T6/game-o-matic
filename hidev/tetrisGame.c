#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
//#include <tetrominoes.h>
/*Windows:*/
//#include <windows.h>
//#include <conio.h> /*For testing only*/
/*Linux:*/
#include <unistd.h>

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
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,1,1,0,0,0,0},
{0,0,1,1,0,0,0,0},//J
{0,0,1,1,1,1,1,1},
{0,0,1,1,1,1,1,1},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,1,1,0,0},
{0,0,0,0,1,1,0,0},//L
{1,1,1,1,1,1,0,0},
{1,1,1,1,1,1,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,1,1,1,1,0,0},
{0,0,1,1,1,1,0,0},//O
{0,0,1,1,1,1,0,0},
{0,0,1,1,1,1,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,1,1,1,1},
{0,0,0,0,1,1,1,1},//S
{0,0,1,1,1,1,0,0},
{0,0,1,1,1,1,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,1,1,0,0,0,0},
{0,0,1,1,0,0,0,0},//T
{1,1,1,1,1,1,0,0},
{1,1,1,1,1,1,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
,{
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{1,1,1,1,0,0,0,0},
{1,1,1,1,0,0,0,0},//Z
{0,0,1,1,1,1,0,0},
{0,0,1,1,1,1,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}}
};


/*Variables used by all games:*/
const float ARRAY_HEIGHT = 38.0;
const float ARRAY_WIDTH = 76.0;
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
void freeArray(bool** ledArray, int HEIGHT) {
  int i;
  for (i = 0; i < HEIGHT; i++) {
    free(ledArray[i]);
  }
  free(ledArray);
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
  }
  */
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
void testFrame(bool** ledArray) {
  /*Windows:*/
  //Sleep(5);
  //system("cls");
  /*Linux:*/
  usleep(50000);
  system("clear");

  printTest(ledArray);
}

/*Tetris-specific functions:*/
void drawRectangle(bool** ledArray, bool lightsOn, int topY, int leftX, int HEIGHT, int WIDTH) {
  int i, j;
  for(j = HEIGHT - 1; j >= 0; j--) {
    for(i = WIDTH - 1; i >= 0; i--) {
      ledArray[topY + j][leftX + i] = lightsOn;
    }
  }
  return;
}
void importPiece(bool** currentPiece, int currentType, int pieceOrientation, int PIECE_WIDTH) {
  int i, j;
  for(j = PIECE_WIDTH - 1; j >= 0; j--) {
    for(i = PIECE_WIDTH - 1; i >= 0; i--) {
      if(pieceOrientation == 1) {
        currentPiece[j][i] = TETROMINOES[currentType][j][i];
      }
      else if (pieceOrientation == 2) {
        currentPiece[j][i] = TETROMINOES[currentType][i][PIECE_WIDTH - 1 - j];
      }
      else if (pieceOrientation == 3) {
        currentPiece[j][i] = TETROMINOES[currentType][PIECE_WIDTH - 1 - j][PIECE_WIDTH - 1 - i];
      }
      else if (pieceOrientation == 4) {
        currentPiece[j][i] = TETROMINOES[currentType][PIECE_WIDTH - 1 - i][j];
      }
    }
  }
  return;
}
void copyPiece(bool** currentPiece, bool** nextPiece, int PIECE_WIDTH) {
  int i, j;
  for(j = PIECE_WIDTH - 1; j >= 0; j--) {
    for(i = PIECE_WIDTH - 1; i >= 0; i--) {
      currentPiece[j][i] = nextPiece[j][i];
    }
  }
  return;
}
void drawPiece(bool** ledArray, bool** currentPiece, bool lightsOn, int pieceY, int pieceX, int PIECE_WIDTH) {
  int i, j;
  for(j = PIECE_WIDTH - 1; j >= 0; j--) {
    for(i = PIECE_WIDTH - 1; i >= 0; i--) {
      if(currentPiece[j][i] == true) {
        ledArray[pieceY + j][pieceX + i] = lightsOn;
      }
    }
  }
  return;
}
int checkOverlap(bool** ledArray, bool** nextPiece, bool** currentPiece, int nextPieceY, int nextPieceX, int currentPieceY, int currentPieceX, int PIECE_WIDTH, int toggle) {
  int i, j;
  for(j = PIECE_WIDTH - 1; j >= 0; j -= PIECE_WIDTH / 4) {
    for(i = PIECE_WIDTH - 1; i >= 0; i -= PIECE_WIDTH / 4) {
      if(nextPiece[j][i] == true && ledArray[nextPieceY + j][nextPieceX + i] == true) {
        if(toggle == 1) {
          return 1;
        }
        else if(i + nextPieceX - currentPieceX < PIECE_WIDTH && i + nextPieceX - currentPieceX >= 0
             && j + nextPieceY - currentPieceY < PIECE_WIDTH && j + nextPieceY - currentPieceY >= 0) {
          if(currentPiece[j + nextPieceY - currentPieceY][i + nextPieceX - currentPieceX] == true) { /*Must be separate condition*/
            continue;
          }
        }
        return 1;
      }
    }
  }
  return 0;
}

void tetris(bool** ledArray) {
  /*Setting values of the global variables for Tetris:*/
  TOP_MARGIN = 0.0;
  BOTTOM_MARGIN = 0.0;
  LEFT_MARGIN = 0.0;
  RIGHT_MARGIN = 0.0;
  BOTTOM_END = ARRAY_HEIGHT - BOTTOM_MARGIN - 1.0;
  RIGHT_END = ARRAY_WIDTH - RIGHT_MARGIN - 1.0;

  /*Tetris-specific constants:*/
  const int PIECE_WIDTH = 8; /*Should be divisible by 4*/
  const int INITIAL_X = LEFT_MARGIN + (RIGHT_END - LEFT_MARGIN - PIECE_WIDTH) / 2 - 1;
  srand(time(NULL));

  /*Tetris-specific variables:*/
  float currentPieceX = INITIAL_X;
  float currentPieceY = 0;
  float nextPieceX = currentPieceX;
  float nextPieceY = currentPieceY;
  int currentType = rand()%7; /*0-6 corresponds to I, J, L, O, S, T, Z*/
  int nextType = rand()%7;
  int pieceOrientation = 1; /*1-4 corresponds to north, east, south, west*/
  int score = 0;
  int counter = 1;
  int input = 0;
  int dropTimer = 5; /*Should be > 1; may be decreased for difficulty*/

  drawRectangle(ledArray, true, TOP_MARGIN, LEFT_MARGIN, BOTTOM_END - TOP_MARGIN, 2);
  drawRectangle(ledArray, true, TOP_MARGIN, RIGHT_END - 1, BOTTOM_END - TOP_MARGIN, 2);
  drawRectangle(ledArray, true, BOTTOM_END - 1, LEFT_MARGIN, 2, RIGHT_END - LEFT_MARGIN);

  bool** currentPiece = makeArray(PIECE_WIDTH, PIECE_WIDTH);
  importPiece(currentPiece, currentType, pieceOrientation, PIECE_WIDTH);
  bool** nextPiece = makeArray(PIECE_WIDTH, PIECE_WIDTH);
  copyPiece(nextPiece, currentPiece, PIECE_WIDTH);

  while(1) {
    drawPiece(ledArray, currentPiece, false, currentPieceY, currentPieceX, PIECE_WIDTH);
    copyPiece(currentPiece, nextPiece, PIECE_WIDTH);
    currentPieceX = nextPieceX;
    currentPieceY = nextPieceY;
    drawPiece(ledArray, currentPiece, true, currentPieceY, currentPieceX, PIECE_WIDTH);
    testFrame(ledArray);
    input = 0;
    input = getLeftInput();
    if (input == 11) { /*Hard drop*/
      while(checkOverlap(ledArray, nextPiece, currentPiece, nextPieceY + PIECE_WIDTH / 4, nextPieceX, currentPieceY, currentPieceX, PIECE_WIDTH, 0) == 0) {
        nextPieceY += PIECE_WIDTH / 4;
      }
      nextPieceX = INITIAL_X;
      nextPieceY = 0;
      currentPieceX = nextPieceX;
      currentPieceY = nextPieceY;
      currentType = nextType;
      nextType = rand()%7;
      pieceOrientation = 1;
      counter = 1;
      importPiece(currentPiece, currentType, pieceOrientation, PIECE_WIDTH);
      copyPiece(nextPiece, currentPiece, PIECE_WIDTH);
      if(checkOverlap(ledArray, nextPiece, currentPiece, nextPieceY, nextPieceX, currentPieceY, currentPieceX, PIECE_WIDTH, 1)) {
        break; /*Game loss*/
      }
    }
    else if(input == 5 || counter++ % dropTimer == 0) { /*Soft drop*/
      if(checkOverlap(ledArray, nextPiece, currentPiece, nextPieceY + PIECE_WIDTH / 4, nextPieceX, currentPieceY, currentPieceX, PIECE_WIDTH, 0)) {
        nextPieceX = INITIAL_X;
        nextPieceY = 0;
        currentPieceX = nextPieceX;
        currentPieceY = nextPieceY;
        currentType = nextType;
        nextType = rand()%7;
        pieceOrientation = 1;
        counter = 1;
        importPiece(currentPiece, currentType, pieceOrientation, PIECE_WIDTH);
        copyPiece(nextPiece, currentPiece, PIECE_WIDTH);
        if(checkOverlap(ledArray, nextPiece, currentPiece, nextPieceY, nextPieceX, currentPieceY, currentPieceX, PIECE_WIDTH, 1)) {
          break; /*Game loss*/
        }
      }
      else {
        nextPieceY += PIECE_WIDTH / 4;
        counter = 1;
      }
    }
    else if(input == 9) { /*Spin clockwise (non-functional)*/
      pieceOrientation++;
      if(pieceOrientation > 4) {
        pieceOrientation -= 4;
      }
      importPiece(nextPiece, currentType, pieceOrientation, PIECE_WIDTH);
      /*checkOverlap() -> if it returns true then try to bump it?*/
    }
    else if(input == 10) { /*Spin counterclockwise (non-functional)*/
      pieceOrientation--;
      if(pieceOrientation < 1) {
        pieceOrientation += 4;
      }
      importPiece(nextPiece, currentType, pieceOrientation, PIECE_WIDTH);
      /*checkOverlap() -> if it returns true then try to bump it?*/
    }
    else if(input == 3) { /*Move right*/
      if(checkOverlap(ledArray, nextPiece, currentPiece, nextPieceY, nextPieceX + PIECE_WIDTH / 4, currentPieceY, currentPieceX, PIECE_WIDTH, 0) == 0) {
        nextPieceX += PIECE_WIDTH / 4;
      }
    }
    else if(input == 7) { /*Move left*/
      if(checkOverlap(ledArray, nextPiece, currentPiece, nextPieceY, nextPieceX - PIECE_WIDTH / 4, currentPieceY, currentPieceX, PIECE_WIDTH, 0) == 0) {
        nextPieceX -= PIECE_WIDTH / 4;
      }
    }
  }
  drawPiece(ledArray, currentPiece, true, currentPieceY, currentPieceX, PIECE_WIDTH);
  testFrame(ledArray);
  //endGame();
  freeArray(currentPiece, PIECE_WIDTH);
  freeArray(nextPiece, PIECE_WIDTH);
  return;
}

int main (void) {
  /*This function's contents are placeholders for a lodev menu.*/
  bool** ledArray;
  ledArray = makeArray(ARRAY_HEIGHT, ARRAY_WIDTH);
  int input = 0;
  do {
    printf("Welcome to Tetris (official bootleg edition).\nEnter \"1\" to play Tetris.\nEnter \"0\" to exit.");
    scanf("%d", &input);
  } while(input < 0 || input > 1);
  while (input == 1) {
      fillArray(ledArray, ARRAY_HEIGHT, ARRAY_WIDTH, false);
      tetris(ledArray);
      do {
        printf("Enter \"1\" to play again.\nEnter \"0\" to exit.\n");
        scanf("%d", &input);
      } while (input < 0 || input > 1);
  }
  freeArray(ledArray, ARRAY_HEIGHT);
  return 0;
}
