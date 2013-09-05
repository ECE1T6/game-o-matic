/*
This code includes test input-handling which can be enabled in Windows by:
- de-commenting everything below the two "Windows:" tags
- commenting everything below the "Linux:" tags
- de-commenting the bodies of the definitions for getLeftInput() and getRightInput()

Each player's game is then controlled with the WASD+QE+R and IJKL+UO+P keys respectively.
For example, the control scheme for the left side of the board is as follows.
A - shift left
D - shift right
S - "soft drop" (shift one space downward)
W - "hard drop" (shift all the way downward and spawn a new piece)
Q - spin counter-clockwise
E - spin clockwise
R - hold piece

Note that translations are relative to the descent of the pieces (e.g. for the left-side player,
shifting "left" moves the piece closer to the top of the array.)

Also note that considerations for buffered keyboard input have not been made, so only the left
playfield may be controlled without joysticks (using WASD+QE+R). With input-gathering occuring in another
thread in the final version both sides will be functional. To test the right-side player's game, replace
"input = getLeftInput();" with "input = 0;".
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <tetrominoes.h>
//Windows (for testing only):
//#include <windows.h>
//#include <conio.h>
//Linux:
#include <unistd.h>

//Tetris pieces (to be kept in tetrominoes.h later):
const bool allTetrominoes[7][8][8] = {
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

//Game-agnostic functions (not final, and to be kept in helper.h later):
int getLeftInput(void) { //This will call getLeftButton(1), getRightButton(1), and getJoystick(1)
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
    else if(input == 'r') {
      return 11;
    }
  }
*/
  return 0;
}
int getRightInput(void) { //This will call getLeftButton(2), getRightButton(2), and getJoystick(2)
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
    else if(input == 'p') {
      return 11;
    }
  }
*/
  return 0;
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

//Test functions (remove in final version):
void printTest(bool** ledArray, float TOP_MARGIN, float LEFT_MARGIN, float BOT_END, float RIGHT_END) {
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
void frameTest(bool** ledArray, float TOP_MARGIN, float LEFT_MARGIN, float BOT_END, float RIGHT_END) {
  //Windows:
  //Sleep(5);
  //system("cls");
  //Linux:
  //usleep(20000);
  system("clear");

  printTest(ledArray, TOP_MARGIN, LEFT_MARGIN, BOT_END, RIGHT_END);
  return;
}

//Tetris-specific functions:
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
void drawCheckerboard(bool** ledArray, int topY, int leftX, int HEIGHT, int WIDTH) {
  //The bottom right pixel is always made true.
  int i, j;
  for(j = HEIGHT - 1; j >= 0; j--) {
    for(i = WIDTH - 1; i >= 0; i--) {
      if((j + i) % 2 == 0) {
        ledArray[topY + j][leftX + i] = true;
      }
      else {
        ledArray[topY + j][leftX + i] = false;
      }
    }
  }
  return;
}
void generateNextPieces(int* nextPieces) {
  //Adds integers representing tetromino types to an array, without modifying existing valid types.
  //Each of the 7 types is placed at most once in each half of the array.
  int i, j;
  if(nextPieces[0] == -1) {
    for(i = 0; i < 7; i++) {
      do {
        nextPieces[i] = rand()%7;
        for(j = 0; j < i; j++) {
          if(nextPieces[i] == nextPieces[j]) {
            break;
          }
        }
      } while(i != j);
    }
  }
  for(i = 7; i < 14; i++) {
    do {
      nextPieces[i] = rand()%7;
      for(j = 7; j < i; j++) {
        if(nextPieces[i] == nextPieces[j]) {
          break;
        }
      }
    } while(i != j);
  }
  return;
}
void shiftNextPieces(int* nextPieces) {
  //Moves each integer in the piece type array forward, discarding the first and replacing the last with -1 (invalid)
  int i;
  for(i = 0; i < 13; i++) {
    nextPieces[i] = nextPieces[i + 1];
  }
  nextPieces[i] = -1;
  return;
}
int takeNextPiece(int* nextPieces) {
  //Returns the first piece from the piece type array. It also shifts them and, if necessary, generates new pieces.
  int pieceType = nextPieces[0];
  shiftNextPieces(nextPieces);
  if(nextPieces[7] == -1){
    generateNextPieces(nextPieces);
  }
  return pieceType;
}
void importPiece(bool** curPiece, int curType, int pieceOrien, int PIECE_WIDTH, int player) {
  //Copies a piece from tetromino.h to the curPiece array, with a specified orientation.
  int i, j, topGap = 0, leftGap = 0, range = PIECE_WIDTH - 1, SQUARE_WIDTH = PIECE_WIDTH / 4;
  if(curType == 3) {
    //Rotation for "O" is purposely broken for simplicity.
    if(player == 1) {
      pieceOrien = 2;
    }
    else {
      pieceOrien = 4;
    }
  }
  if(curType != 0) {
    //The area considered for rotation is confined, and the outer area is blanked.
    if(player == 1) {
      leftGap = PIECE_WIDTH / 4;
      drawRectangle(curPiece, false, 0, 0, PIECE_WIDTH, SQUARE_WIDTH);
      drawRectangle(curPiece, false, SQUARE_WIDTH * 3, SQUARE_WIDTH, SQUARE_WIDTH, SQUARE_WIDTH * 3);
    }
    else{
      topGap = PIECE_WIDTH / 4;
      drawRectangle(curPiece, false, 0, SQUARE_WIDTH * 3, PIECE_WIDTH, SQUARE_WIDTH);
      drawRectangle(curPiece, false, 0, 0, SQUARE_WIDTH, SQUARE_WIDTH * 3);
    }
    range -= SQUARE_WIDTH;
  }
  if(pieceOrien == 1) {
    for(j = range; j >= 0; j--) {
      for(i = range; i >= 0; i--) {
        curPiece[j + topGap][i + leftGap] = allTetrominoes[curType][j][i];
      }
    }
  }
  else if (pieceOrien == 2) {
    for(j = range; j >= 0; j--) {
      for(i = range; i >= 0; i--) {
        curPiece[j + topGap][i + leftGap] = allTetrominoes[curType][range - i][j];
      }
    }
  }
  else if (pieceOrien == 3) {
    for(j = range; j >= 0; j--) {
      for(i = range; i >= 0; i--) {
        curPiece[j + topGap][i + leftGap] = allTetrominoes[curType][range - j][range - i];
      }
    }
  }
  else {
    for(j = range; j >= 0; j--) {
      for(i = range; i >= 0; i--) {
        curPiece[j + topGap][i + leftGap] = allTetrominoes[curType][i][range - j];
      }
    }
  }
  return;
}
void copyPiece(bool** destPiece, bool** sourcePiece, int PIECE_WIDTH) {
  //Copies one array's elements onto another equally sized array.
  int i, j;
  for(j = PIECE_WIDTH - 1; j >= 0; j--) {
    for(i = PIECE_WIDTH - 1; i >= 0; i--) {
      destPiece[j][i] = sourcePiece[j][i];
    }
  }
  return;
}
void drawPiece(bool** ledArray, bool** curPiece, int curType, bool lightsOn, int curY, int curX, int PIECE_WIDTH, int player) {
  //Draws the true elements of one array to another array, or erases them.
  //It is assumed that the content being drawn will fit on the destination array.
  int i, j, firstGap = 0, secondGap = 0;
  if(curType != 0) {
    if(player == 1) {
      firstGap = PIECE_WIDTH / 4;
    }
    else {
      secondGap = PIECE_WIDTH / 4;
    }
  }
  for(j = PIECE_WIDTH - 1 - firstGap; j >= secondGap; j--) {
    for(i = PIECE_WIDTH - 1 - secondGap; i >= firstGap; i--) {
      if(curPiece[j][i] == true) {
        ledArray[curY + j][curX + i] = lightsOn;
      }
    }
  }
  return;
}
void drawShadow(bool** ledArray, bool** curPiece, int curType, bool lightsOn, int curY, int curX, int PIECE_WIDTH, int player) {
  //Draws a staggered selection of one array's true elements to another array, or erases them.
  //It is assumed that the content being drawn will fit on the destination array.
  int i, j, firstGap = 0, secondGap = 0, SQUARE_WIDTH = PIECE_WIDTH / 4;
  if(curType != 0) {
    if(player == 1) {
      firstGap = SQUARE_WIDTH;
    }
    else {
      secondGap = SQUARE_WIDTH;
    }
  }
  //A shadow block is only lit in the bottom right, which is relative to the player:
  if(player == 1) {
    for(j = PIECE_WIDTH - 1; j >= 0; j -= SQUARE_WIDTH) {
      for(i = 0; i < PIECE_WIDTH; i += SQUARE_WIDTH) {
        if(curPiece[j][i] == true) {
          ledArray[curY + j][curX + i] = lightsOn;
        }
      }
    }
  }
  else {
    for(j = 0; j < PIECE_WIDTH; j += SQUARE_WIDTH) {
      for(i = PIECE_WIDTH - 1; i >= 0; i -= SQUARE_WIDTH) {
        if(curPiece[j][i] == true) {
          ledArray[curY + j][curX + i] = lightsOn;
        }
      }
    }
  }
  return;
}
int checkOverlap(bool** ledArray, bool** projPiece, bool** curPiece, int projY, int projX, int curY, int curX, int PIECE_WIDTH, bool spawning) {
  //Returns 1 if projPiece would overlap an object on ledArray.
  //Ignores the projected piece colliding with the current piece, except when spawning as the current piece is not drawn.
  int i, j, SQUARE_WIDTH = PIECE_WIDTH / 4;
  for(j = 0; j < PIECE_WIDTH; j += SQUARE_WIDTH) {
    for(i = 0; i < PIECE_WIDTH; i += SQUARE_WIDTH) {
      if(projPiece[j][i] == true && ledArray[projY + j][projX + i] == true) {
        if(spawning == false
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
int checkLines(bool** ledArray, int leftBound, int rightBound, int botBound, int topBound, int curX, int PIECE_WIDTH, int player) {
  //Checks for complete lines and clears them. Returns the number of lines cleared.
  int i, j, a, b, linesCleared = 0, SQUARE_WIDTH = PIECE_WIDTH / 4;
  bool fullLine;
  if(player == 1){
    for(i = curX + PIECE_WIDTH - 1; i >= leftBound && i >= curX; i -= SQUARE_WIDTH) {
      fullLine = true;
      for(j = topBound; j <= botBound; j += SQUARE_WIDTH) {
        if(ledArray[j][i] == false) {
          fullLine = false;
          break;
        }
      }
      if(fullLine == true) {
        for(a =  i - SQUARE_WIDTH + 1; a <= rightBound - SQUARE_WIDTH; a++) {
          for(b = topBound; b <= botBound; b++) {
            ledArray[b][a] = ledArray[b][a + SQUARE_WIDTH];
          }
        }
        for(a = rightBound - SQUARE_WIDTH + 1; a <= rightBound; a++) {
          for(b = topBound; b <= botBound; b++) {
            ledArray[b][a] = false;
          }
        }
        linesCleared++;
      }
    }
  }
  else{
    for(i = curX + SQUARE_WIDTH - 1; i <= rightBound && i <= curX + PIECE_WIDTH - 1; i += SQUARE_WIDTH) {
      fullLine = true;
      for(j = topBound; j <= botBound; j += SQUARE_WIDTH) {
        if(ledArray[j][i] == false) {
          fullLine = false;
          break;
        }
      }
      if(fullLine == true) {
        for(a =  i; a >= leftBound + SQUARE_WIDTH; a--) {
          for(b = topBound; b <= botBound; b++) {
            ledArray[b][a] = ledArray[b][a - SQUARE_WIDTH];
          }
        }
        for(a = leftBound + SQUARE_WIDTH - 1; a >= leftBound; a--) {
          for(b = topBound; b <= botBound; b++) {
            ledArray[b][a] = false;
          }
        }
        linesCleared++;
      }
    }
  }
  return linesCleared;
}
int addGarbage(bool** ledArray, int leftBound, int rightBound, int botBound, int topBound, int SQUARE_WIDTH, int player) {
  //Adds an unclearable, semi-solid line of blocks to the bottom of the play area.
  //Returns 1 if this would end the game with a "top out".
  int i, j;
  bool topOut = false;
  if(player == 1) {
    i = rightBound;
    for(j = topBound; j <= botBound; j += SQUARE_WIDTH) {
      if(ledArray[j][i]) {
        topOut = true;
        break;
      }
    }
    for(i = rightBound - SQUARE_WIDTH; i >= leftBound; i--) {
      for(j = topBound; j <= botBound; j++) {
        ledArray[j][i + SQUARE_WIDTH] = ledArray[j][i];
      }
    }
    drawCheckerboard(ledArray, topBound, leftBound, botBound + 1 - topBound, SQUARE_WIDTH);
    if(topOut) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    i = leftBound;
    for(j = topBound; j <= botBound; j += SQUARE_WIDTH) {
      if(ledArray[j][i]) {
        topOut = true;
        break;
      }
    }
    for(i = leftBound + SQUARE_WIDTH; i <= rightBound; i++) {
      for(j = topBound; j <= botBound; j++) {
        ledArray[j][i - SQUARE_WIDTH] = ledArray[j][i];
      }
    }
    drawCheckerboard(ledArray, topBound, rightBound + 1 - SQUARE_WIDTH, botBound + 1 - topBound, SQUARE_WIDTH);
    if(topOut) {
      return 1;
    }
    else {
      return 0;
    }
  }
}

void tetrisTwoPlayer(bool** ledArray) {

  //Universal constants:
  const float ARRAY_HEIGHT = 38.0;
  const float ARRAY_WIDTH = 76.0;

  const float TOP_MARGIN = 0.0; //The margins bound the area controlled by the game.
  const float BOT_MARGIN = 0.0;
  const float LEFT_MARGIN = 0.0;
  const float RIGHT_MARGIN = 0.0;
  const float BOT_END = ARRAY_HEIGHT - BOT_MARGIN - 1.0;
  const float RIGHT_END = ARRAY_WIDTH - RIGHT_MARGIN - 1.0;

  const int PIECE_WIDTH = 8; //Must be evenly divisible by 4
  const int SQUARE_WIDTH = PIECE_WIDTH / 4;

  srand(time(NULL));

  int dropTime = 10; //Should be > 1; may be decreased for difficulty, but decreasing it might also reduce responsiveness

  //Drawing checkboard border base to the large array:
  drawCheckerboard(ledArray, 0, 0, BOT_END + 1 - TOP_MARGIN, RIGHT_END + 1 - LEFT_MARGIN);

  //Player 1: Playfield constraints:
  const int LEFT_BORDER = 2; //Should be a multiple of SQUARE_WIDTH
  const int RIGHT_BORDER = 38;
  const int TOP_BORDER = 2;
  const int BOT_BORDER = 16;

  //Player 1: Arrays of upcoming piece types:
  int* nextPieces = make1DArray(14);
  generateNextPieces(nextPieces);

  //Player 1: Object constants:
  const int INIT_X = RIGHT_END + 1 - RIGHT_BORDER - SQUARE_WIDTH * 4;
  const int INIT_Y = TOP_BORDER + (BOT_END + 1 - BOT_BORDER - TOP_BORDER) / 2 - SQUARE_WIDTH * 2;

  //Player 1: Object variables:
  float curX = INIT_X;
  float curY = INIT_Y;
  float projX = curX;
  float projY = curY;
  float shadX = curX;
  int curType = takeNextPiece(nextPieces);
  int pieceOrien = 2; //1-4 corresponds to north, east, south, west
  int input = 0;
  int timer = 1;
  int linesCleared = 0;
  int garbageLines = 0;
  bool pieceMoved = true;

  //Player 1: Initializing held piece and next pieces:
  const int NEXT_PIECES_Y = BOT_END + 1 - SQUARE_WIDTH * 6;
  const int HELD_PIECE_X = SQUARE_WIDTH * 13;
  const int FIRST_NEXT_PIECE_X = HELD_PIECE_X - SQUARE_WIDTH * 7;
  const int SECOND_NEXT_PIECE_X = FIRST_NEXT_PIECE_X - SQUARE_WIDTH * 5;
  int heldPieceType = - 1;
  bool holdAllowed = true;
  bool** heldPiece = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  fill2DArray(heldPiece, PIECE_WIDTH, PIECE_WIDTH, false);
  bool** firstNextPiece = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  importPiece(firstNextPiece, nextPieces[0], 2, PIECE_WIDTH, 1);
  bool** secondNextPiece = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  importPiece(secondNextPiece, nextPieces[1], 2, PIECE_WIDTH, 1);

  //Player 1: Erasing parts of the checkerboard pattern on the array:
  drawRectangle(ledArray, false, TOP_BORDER, LEFT_BORDER, BOT_END + 1 - BOT_BORDER - TOP_BORDER, RIGHT_END + 1 - RIGHT_BORDER - LEFT_BORDER);
  drawRectangle(ledArray, false, NEXT_PIECES_Y, HELD_PIECE_X, PIECE_WIDTH, PIECE_WIDTH);
  drawRectangle(ledArray, false, NEXT_PIECES_Y, FIRST_NEXT_PIECE_X, PIECE_WIDTH, PIECE_WIDTH);
  drawRectangle(ledArray, false, NEXT_PIECES_Y, SECOND_NEXT_PIECE_X, PIECE_WIDTH, PIECE_WIDTH);

  //Player 1: Drawing next pieces:
  drawPiece(ledArray, firstNextPiece, nextPieces[0], true, NEXT_PIECES_Y, FIRST_NEXT_PIECE_X, PIECE_WIDTH, 1);
  drawPiece(ledArray, secondNextPiece, nextPieces[1], true, NEXT_PIECES_Y, SECOND_NEXT_PIECE_X, PIECE_WIDTH, 1);

  //Player 1: Current state and projected state of active pieces:
  bool** curPiece = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  importPiece(curPiece, curType, pieceOrien, PIECE_WIDTH, 1);
  bool** projPiece = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  copyPiece(projPiece, curPiece, PIECE_WIDTH);
  while(checkOverlap(ledArray, projPiece, curPiece, curY, shadX - SQUARE_WIDTH, curY, curX, PIECE_WIDTH, false) == 0) {
    shadX -= SQUARE_WIDTH;
  }

  ////Player 2 initialization:

  //Player 2: Playfield constraints:
  const int LEFT_BORDER_2 = 38; //Should be a multiple of SQUARE_WIDTH
  const int RIGHT_BORDER_2 = 2;
  const int TOP_BORDER_2 = 16;
  const int BOT_BORDER_2 = 2;

  //Player 2: "Bags" of upcoming piece types ("double" indicating two sets of types each):
  int* nextPieces2 = make1DArray(14);
  generateNextPieces(nextPieces2);

  //Player 2: Object constants:
  const int INIT_X_2 = LEFT_MARGIN + LEFT_BORDER_2;
  const int INIT_Y_2 = TOP_BORDER_2 + (BOT_END + 1 - BOT_BORDER_2 - TOP_BORDER_2) / 2 - SQUARE_WIDTH * 2;

  //Player 2: Object variables:
  float curX2 = INIT_X_2;
  float curY2 = INIT_Y_2;
  float projX2 = curX2;
  float projY2 = curY2;
  float shadX2 = curX2;
  int curType2 = takeNextPiece(nextPieces2);
  int pieceOrien2 = 4; //1-4 corresponds to north, east, south, west
  int input2 = 0;
  int timer2 = 1;
  int linesCleared2 = 0;
  int garbageLines2 = 0;
  bool pieceMoved2 = true;

  //Player 2: Initializing held piece and next pieces:
  const int NEXT_PIECES_Y_2 = TOP_MARGIN + SQUARE_WIDTH * 2;
  const int HELD_PIECE_X_2 = LEFT_BORDER_2 + SQUARE_WIDTH * 2;
  const int FIRST_NEXT_PIECE_X_2 = HELD_PIECE_X_2 + SQUARE_WIDTH * 7;
  const int SECOND_NEXT_PIECE_X_2 = FIRST_NEXT_PIECE_X_2 + SQUARE_WIDTH * 5;
  int heldPieceType2 = - 1;
  bool holdAllowed2 = true;
  bool** heldPiece2 = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  fill2DArray(heldPiece2, PIECE_WIDTH, PIECE_WIDTH, false);
  bool** firstNextPiece2 = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  importPiece(firstNextPiece2, nextPieces2[0], 4, PIECE_WIDTH, 2);
  bool** secondNextPiece2 = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  importPiece(secondNextPiece2, nextPieces2[1], 4, PIECE_WIDTH, 2);

  //Player 2: Erasing parts of the checkerboard pattern on the array:
  drawRectangle(ledArray, false, TOP_BORDER_2, LEFT_BORDER_2, BOT_END + 1 - BOT_BORDER_2 - TOP_BORDER_2, RIGHT_END + 1 - RIGHT_BORDER_2 - LEFT_BORDER_2);
  drawRectangle(ledArray, false, NEXT_PIECES_Y_2, HELD_PIECE_X_2, PIECE_WIDTH, PIECE_WIDTH);
  drawRectangle(ledArray, false, NEXT_PIECES_Y_2, FIRST_NEXT_PIECE_X_2, PIECE_WIDTH, PIECE_WIDTH);
  drawRectangle(ledArray, false, NEXT_PIECES_Y_2, SECOND_NEXT_PIECE_X_2, PIECE_WIDTH, PIECE_WIDTH);

  //Player 2: Drawing next pieces:
  drawPiece(ledArray, firstNextPiece2, nextPieces2[0], true, NEXT_PIECES_Y_2, FIRST_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
  drawPiece(ledArray, secondNextPiece2, nextPieces2[1], true, NEXT_PIECES_Y_2, SECOND_NEXT_PIECE_X_2, PIECE_WIDTH, 2);

  //Player 2: Current state and projected state of active pieces:
  bool** curPiece2 = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  importPiece(curPiece2, curType2, pieceOrien2, PIECE_WIDTH, 2);
  bool** projPiece2 = make2DArray(PIECE_WIDTH, PIECE_WIDTH);
  copyPiece(projPiece2, curPiece2, PIECE_WIDTH);
  while(checkOverlap(ledArray, projPiece2, curPiece2, curY2, shadX2 + SQUARE_WIDTH, curY2, curX2, PIECE_WIDTH, false) == 0) {
    shadX2 += SQUARE_WIDTH;
  }

  while(1) {

    if(pieceMoved) {
      //Redrawing for player 1:
      drawPiece(ledArray, curPiece, curType, false, curY, curX, PIECE_WIDTH, 1);
      drawShadow(ledArray, curPiece, curType, false, curY, shadX, PIECE_WIDTH, 1);
      drawPiece(ledArray, projPiece, curType, true, projY, projX, PIECE_WIDTH, 1);
      copyPiece(curPiece, projPiece, PIECE_WIDTH);
      curX = projX;
      curY = projY;
      shadX = curX;
      while(checkOverlap(ledArray, projPiece, curPiece, curY, shadX - SQUARE_WIDTH, curY, curX, PIECE_WIDTH, false) == 0) {
        shadX -= SQUARE_WIDTH;
      }
      drawShadow(ledArray, curPiece, curType, true, curY, shadX, PIECE_WIDTH, 1);
    }
    pieceMoved = true;

    if(pieceMoved2) {
      //Redrawing for player 2:
      drawPiece(ledArray, curPiece2, curType2, false, curY2, curX2, PIECE_WIDTH, 2);
      drawShadow(ledArray, curPiece2, curType2, false, curY2, shadX2, PIECE_WIDTH, 2);
      drawPiece(ledArray, projPiece2, curType2, true, projY2, projX2, PIECE_WIDTH, 2);
      copyPiece(curPiece2, projPiece2, PIECE_WIDTH);
      curX2 = projX2;
      curY2 = projY2;
      shadX2 = curX2;
      while(checkOverlap(ledArray, projPiece2, curPiece2, curY2, shadX2 + SQUARE_WIDTH, curY2, curX2, PIECE_WIDTH, false) == 0) {
        shadX2 += SQUARE_WIDTH;
      }
      drawShadow(ledArray, curPiece2, curType2, true, curY2, shadX2, PIECE_WIDTH, 2);
    }
    pieceMoved2 = true;

    frameTest(ledArray, TOP_MARGIN, LEFT_MARGIN, BOT_END, RIGHT_END);

    //Player 1 input handling:
    input = getLeftInput();
    if (input == 1) { //Hard drop
      while(checkOverlap(ledArray, projPiece, curPiece, projY, projX - SQUARE_WIDTH, curY, curX, PIECE_WIDTH, false) == 0) {
        projX -= SQUARE_WIDTH;
        drawPiece(ledArray, curPiece, curType, false, curY, curX, PIECE_WIDTH, 1);
        copyPiece(curPiece, projPiece, PIECE_WIDTH);
        curX = projX;
        curY = projY;
        drawPiece(ledArray, curPiece, curType, true, curY, curX, PIECE_WIDTH, 1);
        frameTest(ledArray, TOP_MARGIN, LEFT_MARGIN, BOT_END, RIGHT_END);
      }
      linesCleared += checkLines(ledArray, LEFT_MARGIN + LEFT_BORDER, RIGHT_END - RIGHT_BORDER, BOT_END - BOT_BORDER, TOP_MARGIN + TOP_BORDER, curX, PIECE_WIDTH, 1);
      if(linesCleared2 > 3){
        linesCleared2 -= 4;
        if(addGarbage(ledArray, LEFT_MARGIN + LEFT_BORDER, RIGHT_END - RIGHT_BORDER, BOT_END - BOT_BORDER, TOP_MARGIN + TOP_BORDER, SQUARE_WIDTH, 1)){
          break;
        }
        if(addGarbage(ledArray, LEFT_MARGIN + LEFT_BORDER, RIGHT_END - RIGHT_BORDER, BOT_END - BOT_BORDER, TOP_MARGIN + TOP_BORDER, SQUARE_WIDTH, 1)){
          break;
        }
      }
      projX = INIT_X;
      projY = INIT_Y;
      curX = projX;
      curY = projY;
      shadX = curX;
      drawPiece(ledArray, firstNextPiece, nextPieces[0], false, NEXT_PIECES_Y, FIRST_NEXT_PIECE_X, PIECE_WIDTH, 1);
      drawPiece(ledArray, secondNextPiece, nextPieces[1], false, NEXT_PIECES_Y, SECOND_NEXT_PIECE_X, PIECE_WIDTH, 1);
      curType = takeNextPiece(nextPieces);
      copyPiece(curPiece, firstNextPiece, PIECE_WIDTH);
      copyPiece(firstNextPiece, secondNextPiece, PIECE_WIDTH);
      importPiece(secondNextPiece, nextPieces[1], 2, PIECE_WIDTH, 1);
      drawPiece(ledArray, firstNextPiece, nextPieces[0], true, NEXT_PIECES_Y, FIRST_NEXT_PIECE_X, PIECE_WIDTH, 1);
      drawPiece(ledArray, secondNextPiece, nextPieces[1], true, NEXT_PIECES_Y, SECOND_NEXT_PIECE_X, PIECE_WIDTH, 1);
      pieceOrien = 2;
      timer = 1;
      copyPiece(projPiece, curPiece, PIECE_WIDTH);
      holdAllowed = true;
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX, curY, curX, PIECE_WIDTH, true)) {
        break; //Game loss
      }
    }
    else if(input == 5 || timer++ % dropTime == 0) { //Soft drop
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX - SQUARE_WIDTH, curY, curX, PIECE_WIDTH, false)) {
        linesCleared += checkLines(ledArray, LEFT_MARGIN + LEFT_BORDER, RIGHT_END - RIGHT_BORDER, BOT_END - BOT_BORDER, TOP_MARGIN + TOP_BORDER, curX, PIECE_WIDTH, 1);
        if(linesCleared2 > 3){
          linesCleared2 -= 4;
          if(addGarbage(ledArray, LEFT_MARGIN + LEFT_BORDER, RIGHT_END - RIGHT_BORDER, BOT_END - BOT_BORDER, TOP_MARGIN + TOP_BORDER, SQUARE_WIDTH, 1)){
            break;
          }
          if(addGarbage(ledArray, LEFT_MARGIN + LEFT_BORDER, RIGHT_END - RIGHT_BORDER, BOT_END - BOT_BORDER, TOP_MARGIN + TOP_BORDER, SQUARE_WIDTH, 1)){
            break;
          }
        }
        projX = INIT_X;
        projY = INIT_Y;
        curX = projX;
        curY = projY;
        shadX = curX;
        drawPiece(ledArray, firstNextPiece, nextPieces[0], false, NEXT_PIECES_Y, FIRST_NEXT_PIECE_X, PIECE_WIDTH, 1);
        drawPiece(ledArray, secondNextPiece, nextPieces[1], false, NEXT_PIECES_Y, SECOND_NEXT_PIECE_X, PIECE_WIDTH, 1);
        curType = takeNextPiece(nextPieces);
        copyPiece(curPiece, firstNextPiece, PIECE_WIDTH);
        copyPiece(firstNextPiece, secondNextPiece, PIECE_WIDTH);
        importPiece(secondNextPiece, nextPieces[1], 2, PIECE_WIDTH, 1);
        drawPiece(ledArray, firstNextPiece, nextPieces[0], true, NEXT_PIECES_Y, FIRST_NEXT_PIECE_X, PIECE_WIDTH, 1);
        drawPiece(ledArray, secondNextPiece, nextPieces[1], true, NEXT_PIECES_Y, SECOND_NEXT_PIECE_X, PIECE_WIDTH, 1);
        pieceOrien = 2;
        timer = 1;
        copyPiece(projPiece, curPiece, PIECE_WIDTH);
        holdAllowed = true;
        if(checkOverlap(ledArray, projPiece, curPiece, projY, projX, curY, curX, PIECE_WIDTH, true)) {
          break; //Game loss
        }
      }
      else {
        projX -= SQUARE_WIDTH;
        timer = 1;
      }
    }
    else if(input == 9) { //Spin clockwise
      pieceOrien++;
      if(pieceOrien > 4) {
        pieceOrien -= 4;
      }
      importPiece(projPiece, curType, pieceOrien, PIECE_WIDTH, 1);
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX, curY, curX, PIECE_WIDTH, false)) {
        if(!checkOverlap(ledArray, projPiece, curPiece, projY + SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, false)) {
          projY += SQUARE_WIDTH;
        }
        else if(curType == 0 && !checkOverlap(ledArray, projPiece, curPiece, projY + 2 * SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, false)) {
          projY += 2 * SQUARE_WIDTH;
        }
        else if(!checkOverlap(ledArray, projPiece, curPiece, projY - SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, false)) {
          projY -= SQUARE_WIDTH;
        }
        else if(curType == 0 && !checkOverlap(ledArray, projPiece, curPiece, projY - 2 * SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, false)) {
          projY -= 2 * SQUARE_WIDTH;
        }
        else {
          pieceOrien--;
          if(pieceOrien < 1) {
            pieceOrien += 4;
          }
          importPiece(projPiece, curType, pieceOrien, PIECE_WIDTH, 1);
          pieceMoved = false;
        }
      }
    }
    else if(input == 10) { //Spin counterclockwise
      pieceOrien--;
      if(pieceOrien < 1) {
        pieceOrien += 4;
      }
      importPiece(projPiece, curType, pieceOrien, PIECE_WIDTH, 1);
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX, curY, curX, PIECE_WIDTH, false)) {
        if(!checkOverlap(ledArray, projPiece, curPiece, projY + SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH,  false)) {
          projY += SQUARE_WIDTH;
        }
        else if(curType == 0 && !checkOverlap(ledArray, projPiece, curPiece, projY + 2 * SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, false)) {
          projY += 2 * SQUARE_WIDTH;
        }
        else if(!checkOverlap(ledArray, projPiece, curPiece, projY - SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, false)) {
          projY -= SQUARE_WIDTH;
        }
        else if(curType == 0 && !checkOverlap(ledArray, projPiece, curPiece, projY - 2 * SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, false)) {
          projY -= 2 * SQUARE_WIDTH;
        }
        else {
          pieceOrien++;
          if(pieceOrien > 4) {
            pieceOrien -= 4;
          }
          importPiece(projPiece, curType, pieceOrien, PIECE_WIDTH, 1);
          pieceMoved = false;
        }
      }
    }
    else if(input == 3) { //Move right
      if(checkOverlap(ledArray, projPiece, curPiece, projY + SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, false) == 0) {
        projY += SQUARE_WIDTH;
      }
      else {
        pieceMoved = false;
      }
    }
    else if(input == 7) { //Move left
      if(checkOverlap(ledArray, projPiece, curPiece, projY - SQUARE_WIDTH, projX, curY, curX, PIECE_WIDTH, false) == 0) {
        projY -= SQUARE_WIDTH;
      }
      else {
        pieceMoved = false;
      }
    }
    else if(input == 11 && holdAllowed) {
      if(heldPieceType == -1) {
        drawPiece(ledArray, curPiece, curType, false, curY, curX, PIECE_WIDTH, 1);
        drawShadow(ledArray, curPiece, curType, false, curY, shadX, PIECE_WIDTH, 1);
        projX = INIT_X;
        projY = INIT_Y;
        curX = projX;
        curY = projY;
        shadX = curX;
        drawPiece(ledArray, firstNextPiece, nextPieces[0], false, NEXT_PIECES_Y, FIRST_NEXT_PIECE_X, PIECE_WIDTH, 1);
        drawPiece(ledArray, secondNextPiece, nextPieces[1], false, NEXT_PIECES_Y, SECOND_NEXT_PIECE_X, PIECE_WIDTH, 1);
        heldPieceType = curType;
        importPiece(heldPiece, curType, 2, PIECE_WIDTH, 1);
        drawPiece(ledArray, heldPiece, heldPieceType, true, NEXT_PIECES_Y, HELD_PIECE_X, PIECE_WIDTH, 1);
        curType = takeNextPiece(nextPieces);
        copyPiece(curPiece, firstNextPiece, PIECE_WIDTH);
        copyPiece(firstNextPiece, secondNextPiece, PIECE_WIDTH);
        importPiece(secondNextPiece, nextPieces[1], 2, PIECE_WIDTH, 1);
        drawPiece(ledArray, firstNextPiece, nextPieces[0], true, NEXT_PIECES_Y, FIRST_NEXT_PIECE_X, PIECE_WIDTH, 1);
        drawPiece(ledArray, secondNextPiece, nextPieces[1], true, NEXT_PIECES_Y, SECOND_NEXT_PIECE_X, PIECE_WIDTH, 1);
        pieceOrien = 2;
        timer = 1;
        copyPiece(projPiece, curPiece, PIECE_WIDTH);
      }
      else{
        drawPiece(ledArray, heldPiece, heldPieceType, false, NEXT_PIECES_Y, HELD_PIECE_X, PIECE_WIDTH, 1);
        drawPiece(ledArray, curPiece, curType, false, curY, curX, PIECE_WIDTH, 1);
        drawShadow(ledArray, curPiece, curType, false, curY, shadX, PIECE_WIDTH, 1);
        projX = INIT_X;
        projY = INIT_Y;
        curX = projX;
        curY = projY;
        shadX = curX;
        copyPiece(curPiece, heldPiece, PIECE_WIDTH);
        importPiece(heldPiece, curType, 2, PIECE_WIDTH, 1);
        int tempType = heldPieceType;
        heldPieceType = curType;
        curType = tempType;
        drawPiece(ledArray, heldPiece, heldPieceType, true, NEXT_PIECES_Y, HELD_PIECE_X, PIECE_WIDTH, 1);
        pieceOrien = 2;
        timer = 1;
        copyPiece(projPiece, curPiece, PIECE_WIDTH);
      }
      holdAllowed = false;
    }
    else {
      pieceMoved = false;
    }

    //Player 2 input handling:
    input2 = getRightInput();
    if (input2 == 1) { //Hard drop
      while(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2 + SQUARE_WIDTH, curY2, curX2, PIECE_WIDTH, false) == 0) {
        projX2 += SQUARE_WIDTH;
        drawPiece(ledArray, curPiece2, curType2, false, curY2, curX2, PIECE_WIDTH, 2);
        copyPiece(curPiece2, projPiece2, PIECE_WIDTH);
        curX2 = projX2;
        curY2 = projY2;
        drawPiece(ledArray, curPiece2, curType2, true, curY2, curX2, PIECE_WIDTH, 2);
        frameTest(ledArray, TOP_MARGIN, LEFT_MARGIN, BOT_END, RIGHT_END);
      }
      linesCleared2 += checkLines(ledArray, LEFT_MARGIN + LEFT_BORDER_2, RIGHT_END - RIGHT_BORDER_2, BOT_END - BOT_BORDER_2, TOP_MARGIN + TOP_BORDER_2, curX2, PIECE_WIDTH,  2);
      if(linesCleared > 3){
        linesCleared -= 4;
        if(addGarbage(ledArray, LEFT_MARGIN + LEFT_BORDER_2, RIGHT_END - RIGHT_BORDER_2, BOT_END - BOT_BORDER_2, TOP_MARGIN + TOP_BORDER_2, SQUARE_WIDTH, 2)) {
          break;
        }
        if(addGarbage(ledArray, LEFT_MARGIN + LEFT_BORDER_2, RIGHT_END - RIGHT_BORDER_2, BOT_END - BOT_BORDER_2, TOP_MARGIN + TOP_BORDER_2, SQUARE_WIDTH, 2)) {
          break;
        }
      }
      projX2 = INIT_X_2;
      projY2 = INIT_Y_2;
      curX2 = projX2;
      curY2 = projY2;
      shadX2 = curX2;
      drawPiece(ledArray, firstNextPiece2, nextPieces2[0], false, NEXT_PIECES_Y_2, FIRST_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
      drawPiece(ledArray, secondNextPiece2, nextPieces2[1], false, NEXT_PIECES_Y_2, SECOND_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
      curType2 = takeNextPiece(nextPieces2);
      copyPiece(curPiece2, firstNextPiece2, PIECE_WIDTH);
      copyPiece(firstNextPiece2, secondNextPiece2, PIECE_WIDTH);
      importPiece(secondNextPiece2, nextPieces2[1], 4, PIECE_WIDTH, 2);
      drawPiece(ledArray, firstNextPiece2, nextPieces2[0], true, NEXT_PIECES_Y_2, FIRST_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
      drawPiece(ledArray, secondNextPiece2, nextPieces2[1], true, NEXT_PIECES_Y_2, SECOND_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
      pieceOrien2 = 4;
      timer2 = 1;
      copyPiece(projPiece2, curPiece2, PIECE_WIDTH);
      holdAllowed2 = true;
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2, curY2, curX2, PIECE_WIDTH, true)) {
        break; //Game loss
      }
    }
    else if(input2 == 5 || timer2++ % dropTime == 0) { //Soft drop
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2 + SQUARE_WIDTH, curY2, curX2, PIECE_WIDTH, false)) {
        linesCleared2 += checkLines(ledArray, LEFT_MARGIN + LEFT_BORDER_2, RIGHT_END - RIGHT_BORDER_2, BOT_END - BOT_BORDER_2, TOP_MARGIN + TOP_BORDER_2, curX2, PIECE_WIDTH, 2);
        if(linesCleared > 3){
          linesCleared -= 4;
          if(addGarbage(ledArray, LEFT_MARGIN + LEFT_BORDER_2, RIGHT_END - RIGHT_BORDER_2, BOT_END - BOT_BORDER_2, TOP_MARGIN + TOP_BORDER_2, SQUARE_WIDTH, 2)) {
            break;
          }
          if(addGarbage(ledArray, LEFT_MARGIN + LEFT_BORDER_2, RIGHT_END - RIGHT_BORDER_2, BOT_END - BOT_BORDER_2, TOP_MARGIN + TOP_BORDER_2, SQUARE_WIDTH, 2)) {
            break;
          }
        }
        projX2 = INIT_X_2;
        projY2 = INIT_Y_2;
        curX2 = projX2;
        curY2 = projY2;
        shadX2 = curX2;
        drawPiece(ledArray, firstNextPiece2, nextPieces2[0], false, NEXT_PIECES_Y_2, FIRST_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
        drawPiece(ledArray, secondNextPiece2, nextPieces2[1], false, NEXT_PIECES_Y_2, SECOND_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
        curType2 = takeNextPiece(nextPieces2);
        copyPiece(curPiece2, firstNextPiece2, PIECE_WIDTH);
        copyPiece(firstNextPiece2, secondNextPiece2, PIECE_WIDTH);
        importPiece(secondNextPiece2, nextPieces2[1], 4, PIECE_WIDTH, 2);
        drawPiece(ledArray, firstNextPiece2, nextPieces2[0], true, NEXT_PIECES_Y_2, FIRST_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
        drawPiece(ledArray, secondNextPiece2, nextPieces2[1], true, NEXT_PIECES_Y_2, SECOND_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
        pieceOrien2 = 4;
        timer2 = 1;
        copyPiece(projPiece2, curPiece2, PIECE_WIDTH);
        holdAllowed2 = true;
        if(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2, curY2, curX2, PIECE_WIDTH, true)) {
          break; //Game loss
        }
      }
      else {
        projX2 += SQUARE_WIDTH;
        timer2 = 1;
      }
    }
    else if(input2 == 9) { //Spin clockwise
      pieceOrien2++;
      if(pieceOrien2 > 4) {
        pieceOrien2 -= 4;
      }
      importPiece(projPiece2, curType2, pieceOrien2, PIECE_WIDTH, 2);
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2, curY2, curX2, PIECE_WIDTH, false)) {
        if(!checkOverlap(ledArray, projPiece2, curPiece2, projY2 - SQUARE_WIDTH, projX2, curY2, curX2, PIECE_WIDTH, false)) {
          projY2 -= SQUARE_WIDTH;
        }
        else if(curType2 == 0 && !checkOverlap(ledArray, projPiece2, curPiece2, projY2 - 2 * SQUARE_WIDTH, projX2, curY2, curX2, PIECE_WIDTH, false)) {
          projY2 -= 2 * SQUARE_WIDTH;
        }
        else if(!checkOverlap(ledArray, projPiece2, curPiece2, projY2 + SQUARE_WIDTH, projX2, curY2, curX2, PIECE_WIDTH, false)) {
          projY2 += SQUARE_WIDTH;
        }
        else if(curType2 == 0 && !checkOverlap(ledArray, projPiece2, curPiece2, projY2 + 2 * SQUARE_WIDTH, projX2, curY2, curX2, PIECE_WIDTH, false)) {
          projY2 += 2 * SQUARE_WIDTH;
        }
        else {
          pieceOrien2--;
          if(pieceOrien2 < 1) {
            pieceOrien2 += 4;
          }
          importPiece(projPiece2, curType2, pieceOrien2, PIECE_WIDTH, 2);
          pieceMoved2 = false;
        }
      }
    }
    else if(input2 == 10) { //Spin counterclockwise
      pieceOrien2--;
      if(pieceOrien2 < 1) {
        pieceOrien2 += 4;
      }
      importPiece(projPiece2, curType2, pieceOrien2, PIECE_WIDTH, 2);
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2, curY2, curX2, PIECE_WIDTH, false)) {
        if(!checkOverlap(ledArray, projPiece2, curPiece2, projY2 - SQUARE_WIDTH, projX2, curY2, curX2, PIECE_WIDTH, false)) {
          projY2 -= SQUARE_WIDTH;
        }
        else if(curType2 == 0 && !checkOverlap(ledArray, projPiece2, curPiece2, projY2 - 2 * SQUARE_WIDTH, projX2, curY2, curX2, PIECE_WIDTH, false)) {
          projY2 -= 2 * SQUARE_WIDTH;
        }
        else if(!checkOverlap(ledArray, projPiece2, curPiece2, projY2 + SQUARE_WIDTH, projX2, curY2, curX2, PIECE_WIDTH, false)) {
          projY2 += SQUARE_WIDTH;
        }
        else if(curType2 == 0 && !checkOverlap(ledArray, projPiece2, curPiece2, projY2 + 2 * SQUARE_WIDTH, projX2, curY2, curX2, PIECE_WIDTH, false)) {
          projY2 += 2 * SQUARE_WIDTH;
        }
        else {
          pieceOrien2++;
          if(pieceOrien2 > 4) {
            pieceOrien2 -= 4;
          }
          pieceMoved2 = false;
          importPiece(projPiece2, curType2, pieceOrien2, PIECE_WIDTH, 2);
        }
      }
    }
    else if(input2 == 3) { //Move right
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2 - SQUARE_WIDTH, projX2, curY2, curX2, PIECE_WIDTH, false) == 0) {
        projY2 -= SQUARE_WIDTH;
      }
      else {
        pieceMoved2 = false;
      }
    }
    else if(input2 == 7) { //Move left
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2 + SQUARE_WIDTH, projX2, curY2, curX2, PIECE_WIDTH, false) == 0) {
        projY2 += SQUARE_WIDTH;
      }
      else {
        pieceMoved2 = false;
      }
    }
    else if(input2 == 11 && holdAllowed2) {
      if(heldPieceType2 == -1) {
        drawPiece(ledArray, curPiece2, curType2, false, curY2, curX2, PIECE_WIDTH, 2);
        drawShadow(ledArray, curPiece2, curType2, false, curY2, shadX2, PIECE_WIDTH, 2);
        projX2 = INIT_X_2;
        projY2 = INIT_Y_2;
        curX2 = projX2;
        curY2 = projY2;
        shadX2 = curX2;
        drawPiece(ledArray, firstNextPiece2, nextPieces2[0], false, NEXT_PIECES_Y_2, FIRST_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
        drawPiece(ledArray, secondNextPiece2, nextPieces2[1], false, NEXT_PIECES_Y_2, SECOND_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
        heldPieceType2 = curType2;
        importPiece(heldPiece2, curType2, 4, PIECE_WIDTH, 2);
        drawPiece(ledArray, heldPiece2, heldPieceType2, true, NEXT_PIECES_Y_2, HELD_PIECE_X_2, PIECE_WIDTH, 2);
        curType2 = takeNextPiece(nextPieces2);
        copyPiece(curPiece2, firstNextPiece2, PIECE_WIDTH);
        copyPiece(firstNextPiece2, secondNextPiece2, PIECE_WIDTH);
        importPiece(secondNextPiece2, nextPieces2[1], 4, PIECE_WIDTH, 2);
        drawPiece(ledArray, firstNextPiece2, nextPieces2[0], true, NEXT_PIECES_Y_2, FIRST_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
        drawPiece(ledArray, secondNextPiece2, nextPieces2[1], true, NEXT_PIECES_Y_2, SECOND_NEXT_PIECE_X_2, PIECE_WIDTH, 2);
        pieceOrien2 = 4;
        timer2 = 1;
        copyPiece(projPiece2, curPiece2, PIECE_WIDTH);
      }
      else{
        drawPiece(ledArray, heldPiece2, heldPieceType2, false, NEXT_PIECES_Y_2, HELD_PIECE_X_2, PIECE_WIDTH, 2);
        drawPiece(ledArray, curPiece2, curType2, false, curY2, curX2, PIECE_WIDTH, 2);
        drawShadow(ledArray, curPiece2, curType2, false, curY2, shadX2, PIECE_WIDTH, 2);
        projX2 = INIT_X_2;
        projY2 = INIT_Y_2;
        curX2 = projX2;
        curY2 = projY2;
        shadX2 = curX2;
        copyPiece(curPiece2, heldPiece2, PIECE_WIDTH);
        importPiece(heldPiece2, curType2, 4, PIECE_WIDTH, 2);
        int tempType2 = heldPieceType2;
        heldPieceType2 = curType2;
        curType2 = tempType2;
        drawPiece(ledArray, heldPiece2, heldPieceType2, true, NEXT_PIECES_Y_2, HELD_PIECE_X_2, PIECE_WIDTH, 2);
        pieceOrien2 = 4;
        timer2 = 1;
        copyPiece(projPiece2, curPiece2, PIECE_WIDTH);
      }
      holdAllowed2 = false;
    }
    else {
      pieceMoved2 = false;
    }
  }
  drawPiece(ledArray, curPiece, curType, true, curY, curX, PIECE_WIDTH, 1);
  drawPiece(ledArray, curPiece2, curType2, true, curY2, curX2, PIECE_WIDTH, 2);
  frameTest(ledArray, TOP_MARGIN, LEFT_MARGIN, BOT_END, RIGHT_END);
  free(nextPieces);
  free2DArray(curPiece, PIECE_WIDTH);
  free2DArray(projPiece, PIECE_WIDTH);
  free(nextPieces2);
  free2DArray(curPiece2, PIECE_WIDTH);
  free2DArray(projPiece2, PIECE_WIDTH);
  return;
}

int main (void) {
  //This function's contents are throwaway. They mimic a lodev menu.
  const float ARRAY_HEIGHT = 38.0;
  const float ARRAY_WIDTH = 76.0;
  bool** ledArray;
  ledArray = make2DArray(ARRAY_HEIGHT, ARRAY_WIDTH);
  fill2DArray(ledArray, ARRAY_HEIGHT, ARRAY_WIDTH, false);
  tetrisTwoPlayer(ledArray);
  free2DArray(ledArray, ARRAY_HEIGHT);
  return 0;
}
