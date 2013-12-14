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
thread in the final version both sides will be functional. To test the right-side player's game with a
keyboard, replace "input = getLeftInput();" with "input = 0;".
*/

#include "tetris.h"

//Game-agnostic functions (not final, and to be kept in helpers.h later):
static int getLeftInput(void) { //This will call getLeftButton(1), getRightButton(1), and getJoystick(1)
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
static int getRightInput(void) { //This will call getLeftButton(2), getRightButton(2), and getJoystick(2)
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

static void generateNextTypes(int* nextTypes) {
  //Adds integers representing piece types to an array, without modifying existing types.
  //Each of the 7 types is placed at most once in each half of the 14-integer array.
  int i, j;
  if(nextTypes[0] == -1) { //-1 marks an unspecified type. This condition is only true at initialization.
    for(i = 0; i < 7; i++) {
      do {
        nextTypes[i] = rand()%7;
        for(j = 0; j < i; j++) {
          if(nextTypes[i] == nextTypes[j]) {
            break;
          }
        }
      } while(i != j);
    }
  }
  for(i = 7; i < 14; i++) {
    do {
      nextTypes[i] = rand()%7;
      for(j = 7; j < i; j++) {
        if(nextTypes[i] == nextTypes[j]) {
          break;
        }
      }
    } while(i != j);
  }
  return;
}
static void shiftNextTypes(int* nextTypes) {
  //Moves each piece type forward, discarding the first and replacing the last with -1 (unspecified type)
  int i;
  for(i = 0; i < 13; i++) {
    nextTypes[i] = nextTypes[i + 1];
  }
  nextTypes[i] = -1;
  return;
}
static int takeNextType(int* nextTypes) {
  //Returns the first piece type from the array, shifts them, and, if necessary, generates new pieces.
  int pieceType = nextTypes[0];
  shiftNextTypes(nextTypes);
  if(nextTypes[7] == -1){
    generateNextTypes(nextTypes);
  }
  return pieceType;
}
static void importPiece(bool** curPiece, int curType, int pieceOrien, int pieceWidth, int player) {
  //Copies a piece from tetromino.h to the curPiece array, with a specified orientation.
  int i, j, topGap = 0, leftGap = 0, range = pieceWidth - 1, squareWidth = pieceWidth / 4;
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
      leftGap = pieceWidth / 4;
      drawRectangle(curPiece, false, 0, 0, pieceWidth, squareWidth);
      drawRectangle(curPiece, false, squareWidth * 3, squareWidth, squareWidth, squareWidth * 3);
    }
    else{
      topGap = pieceWidth / 4;
      drawRectangle(curPiece, false, 0, squareWidth * 3, pieceWidth, squareWidth);
      drawRectangle(curPiece, false, 0, 0, squareWidth, squareWidth * 3);
    }
    range -= squareWidth;
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
static void copyPiece(bool** destPiece, bool** sourcePiece, int pieceWidth) {
  //Copies one array's elements onto another equally sized array.
  int i, j;
  for(j = pieceWidth - 1; j >= 0; j--) {
    for(i = pieceWidth - 1; i >= 0; i--) {
      destPiece[j][i] = sourcePiece[j][i];
    }
  }
  return;
}
static void drawPiece(bool** ledArray, bool** curPiece, int curType, bool lightsOn, int curY, int curX, int pieceWidth, int player) {
  //Draws the true elements of one array to another array, or erases them.
  //It is assumed that the content being drawn will fit on the destination array.
  int i, j, firstGap = 0, secondGap = 0;
  if(curType != 0) {
    if(player == 1) {
      firstGap = pieceWidth / 4;
    }
    else {
      secondGap = pieceWidth / 4;
    }
  }
  for(j = pieceWidth - 1 - firstGap; j >= secondGap; j--) {
    for(i = pieceWidth - 1 - secondGap; i >= firstGap; i--) {
      if(curPiece[j][i]) {
        ledArray[curY + j][curX + i] = lightsOn;
      }
    }
  }
  return;
}
static void drawShadow(bool** ledArray, bool** curPiece, int curType, bool lightsOn, int curY, int curX, int pieceWidth, int player) {
  //Draws a staggered selection of one array's true elements to another array, or erases them.
  //It is assumed that the content being drawn will fit on the destination array.
  int i, j, firstGap = 0, secondGap = 0, squareWidth = pieceWidth / 4;
  if(curType != 0) {
    if(player == 1) {
      firstGap = squareWidth;
    }
    else {
      secondGap = squareWidth;
    }
  }
  //A shadow block is only lit in the bottom right, so the drawing loops are player(perspective)-dependent:
  if(player == 1) {
    for(j = pieceWidth - 1; j >= 0; j -= squareWidth) {
      for(i = 0; i < pieceWidth; i += squareWidth) {
        if(curPiece[j][i]) {
          ledArray[curY + j][curX + i] = lightsOn;
        }
      }
    }
  }
  else {
    for(j = 0; j < pieceWidth; j += squareWidth) {
      for(i = pieceWidth - 1; i >= 0; i -= squareWidth) {
        if(curPiece[j][i]) {
          ledArray[curY + j][curX + i] = lightsOn;
        }
      }
    }
  }
  return;
}
static int checkOverlap(bool** ledArray, bool** projPiece, bool** curPiece, int projY, int projX, int curY, int curX, float leftBorder, float rightEnd, float topBorder, float botEnd, int pieceWidth, bool spawning) {
  //Returns 1 if projPiece would overlap elements of ledArray that are already true.
  //Ignores the projected piece colliding with the current piece during movement.
  int i, j, squareWidth = pieceWidth / 4;
  for(j = 0; j < pieceWidth; j += squareWidth) {
    for(i = 0; i < pieceWidth; i += squareWidth) {
      if(projPiece[j][i]) {
        if(projY + j < topBorder || projY + j > botEnd || projX + i < leftBorder || projX + i > rightEnd) {
          return 1;
        }
        else if(ledArray[projY + j][projX + i]) {
          if(!spawning //When spawning, curPiece may be placed illegally so this exception is ignored.
             && i + projX - curX < pieceWidth
             && i + projX - curX >= 0
             && j + projY - curY < pieceWidth
             && j + projY - curY >= 0
             && curPiece[j + projY - curY][i + projX - curX]) {
              continue;
          }
          return 1;
        }
      }
    }
  }
  return 0;
}
static int checkLines(bool** ledArray, int leftBound, int rightBound, int botBound, int topBound, int curX, int pieceWidth, int player) {
  //Checks for complete lines and clears them. Returns the number of lines cleared.
  int i, j, a, b, linesCleared = 0, squareWidth = pieceWidth / 4;
  bool fullLine;
  if(player == 1){
    for(i = curX + pieceWidth - 1; i >= leftBound && i >= curX; i -= squareWidth) {
      fullLine = true;
      for(j = topBound; j <= botBound; j += squareWidth) {
        if(!ledArray[j][i]) {
          fullLine = false;
          break;
        }
      }
      if(fullLine) {
        for(a =  i - squareWidth + 1; a <= rightBound - squareWidth; a++) {
          for(b = topBound; b <= botBound; b++) {
            ledArray[b][a] = ledArray[b][a + squareWidth];
          }
        }
        for(a = rightBound - squareWidth + 1; a <= rightBound; a++) {
          for(b = topBound; b <= botBound; b++) {
            ledArray[b][a] = false;
          }
        }
        linesCleared++;
      }
    }
  }
  else{
    for(i = curX + squareWidth - 1; i <= rightBound && i <= curX + pieceWidth - 1; i += squareWidth) {
      fullLine = true;
      for(j = topBound; j <= botBound; j += squareWidth) {
        if(!ledArray[j][i]) {
          fullLine = false;
          break;
        }
      }
      if(fullLine) {
        for(a =  i; a >= leftBound + squareWidth; a--) {
          for(b = topBound; b <= botBound; b++) {
            ledArray[b][a] = ledArray[b][a - squareWidth];
          }
        }
        for(a = leftBound + squareWidth - 1; a >= leftBound; a--) {
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
static int addGarbage(bool** ledArray, int leftBound, int rightBound, int botBound, int topBound, int squareWidth, int player) {
  //Adds an unclearable, semi-solid line of blocks to the bottom of the play area.
  //Returns 1 if this would end the game with a "top out".
  int i, j;
  bool topOut = false;
  if(player == 1) {
    i = rightBound;
    for(j = topBound; j <= botBound; j += squareWidth) {
      if(ledArray[j][i]) {
        topOut = true;
        break;
      }
    }
    for(i = rightBound - squareWidth; i >= leftBound; i--) {
      for(j = topBound; j <= botBound; j++) {
        ledArray[j][i + squareWidth] = ledArray[j][i];
      }
    }
    drawCheckerboard(ledArray, topBound, leftBound, botBound + 1 - topBound, squareWidth);
    if(topOut) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    i = leftBound;
    for(j = topBound; j <= botBound; j += squareWidth) {
      if(ledArray[j][i]) {
        topOut = true;
        break;
      }
    }
    for(i = leftBound + squareWidth; i <= rightBound; i++) {
      for(j = topBound; j <= botBound; j++) {
        ledArray[j][i - squareWidth] = ledArray[j][i];
      }
    }
    drawCheckerboard(ledArray, topBound, rightBound + 1 - squareWidth, botBound + 1 - topBound, squareWidth);
    if(topOut) {
      return 1;
    }
    else {
      return 0;
    }
  }
}
static int changePieceOrien(int pieceOrien, int modifier) {
  //Returns pieceOrien, turned 90 degrees clockwise (modifier == + 1) or counterclockwise (modifier == - 1)
  if(modifier == 1) {
    pieceOrien++;
    if(pieceOrien > 4) {
      pieceOrien -= 4;
    }
  }
  else {
    pieceOrien--;
    if(pieceOrien < 1) {
      pieceOrien += 4;
    }
  }
  return pieceOrien;
}

void tetrisTwoPlayer(bool** ledArray) {

  //Universal constants:
  const float arrayHeight = 38.0;
  const float arrayWidth = 76.0;

  const float topMargin = 0.0; //The margins bound the area onto which the game draws
  const float botMargin = 0.0;
  const float leftMargin = 0.0;
  const float rightMargin = 0.0;
  const float botEnd = arrayHeight - botMargin - 1.0;
  const float rightEnd = arrayWidth - rightMargin - 1.0;

  const int pieceWidth = 8; //Must be evenly divisible by 4
  const int squareWidth = pieceWidth / 4;

  srand(time(NULL));

  int dropTime = 15; //Decreasing reduces responsiveness but increases speed/difficulty without processor cost. Should be > 1

  //Drawing checkboard border base to the large array:
  drawCheckerboard(ledArray, 0, 0, botEnd + 1 - topMargin, rightEnd + 1 - leftMargin);

  //Player 1: Playfield constraints:
  const int leftBorder = 0; //Borders should be multiples of squareWidth
  const int rightBorder = 38;
  const int topBorder = 2;
  const int botBorder = 16;
  const int leftBound = leftMargin + leftBorder;
  const int rightBound = rightEnd - rightBorder;
  const int topBound = topMargin + topBorder;
  const int botBound = botEnd - botBorder;

  //Player 1: Arrays of upcoming piece types:
  int* nextTypes = make1DArray(14);
  generateNextTypes(nextTypes);

  //Player 1: Object constants:
  const int initPieceX = rightBound + 1 - squareWidth * 4;
  const int initPieceY = topBorder + (botBound + 1 - topBorder) / 2 - squareWidth * 2;

  //Player 1: Object variables:
  float curX = initPieceX;
  float curY = initPieceY;
  float projX = curX;
  float projY = curY;
  float shadX = curX;
  int curType = takeNextType(nextTypes);
  int pieceOrien = 2; //1-4 corresponds to north, east, south, west
  int input = 0;
  int timer = 1;
  int linesCleared = 0;
  int garbageLines = 0;
  bool pieceShifted = false;
  bool pieceImported = true;
  bool pieceDropped = false;

  //Player 1: Initializing held piece and next pieces:
  const int heldPieceY = botEnd + 1 - squareWidth * 5 - 1;
  const int nextPiecesY = botEnd + 1 - squareWidth * 6 - 1;
  const int heldPieceX = squareWidth * 13;
  const int firstNextPieceX = heldPieceX - squareWidth * 6;
  const int secondNextPieceX = firstNextPieceX - squareWidth * 6;

  int heldPieceType = - 1;
  bool holdAllowed = true;

  bool** heldPiece = make2DArray(pieceWidth, pieceWidth);
  fill2DArray(heldPiece, pieceWidth, pieceWidth, false);
  bool** firstNextPiece = make2DArray(pieceWidth, pieceWidth);
  importPiece(firstNextPiece, nextTypes[0], 2, pieceWidth, 1);
  bool** secondNextPiece = make2DArray(pieceWidth, pieceWidth);
  importPiece(secondNextPiece, nextTypes[1], 2, pieceWidth, 1);

  //Player 1: Erasing parts of the checkerboard pattern on the array:
  drawRectangle(ledArray, false, topBorder, leftBorder, botBound + 1 - topBorder, rightBound + 1 - leftBorder);
  drawRectangle(ledArray, false, heldPieceY - 1, heldPieceX, squareWidth * 5, squareWidth * 5);
  drawRectangle(ledArray, false, nextPiecesY - 1, firstNextPieceX, squareWidth * 5, squareWidth * 5);
  drawRectangle(ledArray, false, nextPiecesY - 1, secondNextPieceX, squareWidth * 5, squareWidth * 5);

  //Player 1: Drawing next pieces:
  drawPiece(ledArray, firstNextPiece, nextTypes[0], true, nextPiecesY, firstNextPieceX, pieceWidth, 1);
  drawPiece(ledArray, secondNextPiece, nextTypes[1], true, nextPiecesY, secondNextPieceX, pieceWidth, 1);

  //Player 1: Current state and projected state of active pieces:
  bool** curPiece = make2DArray(pieceWidth, pieceWidth);
  importPiece(curPiece, curType, pieceOrien, pieceWidth, 1);
  bool** projPiece = make2DArray(pieceWidth, pieceWidth);
  copyPiece(projPiece, curPiece, pieceWidth);
  while(checkOverlap(ledArray, projPiece, curPiece, curY, shadX - squareWidth, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
    shadX -= squareWidth;
  }

  //Player 2: Playfield constraints:
  const int leftBorder2 = 38; //Should be a multiple of squareWidth
  const int rightBorder2 = 0;
  const int topBorder2 = 16;
  const int botBorder2 = 2;
  const int leftBound2 = leftMargin + leftBorder2;
  const int rightBound2 = rightEnd - rightBorder2;
  const int topBound2 = topMargin + topBorder2;
  const int botBound2 = botEnd - botBorder2;

  //Player 2: Arrays of upcoming piece types:
  int* nextTypes2 = make1DArray(14);
  generateNextTypes(nextTypes2);

  //Player 2: Object constants:
  const int initPieceX2 = leftBound2;
  const int initPieceY2 = topBorder2 + (botBound2 + 1 - topBorder2) / 2 - squareWidth * 2;

  //Player 2: Object variables:
  float curX2 = initPieceX2;
  float curY2 = initPieceY2;
  float projX2 = curX2;
  float projY2 = curY2;
  float shadX2 = curX2;
  int curType2 = takeNextType(nextTypes2);
  int pieceOrien2 = 4; //1-4 corresponds to north, east, south, west
  int input2 = 0;
  int timer2 = 1;
  int linesCleared2 = 0;
  int garbageLines2 = 0;
  bool pieceShifted2 = false;
  bool pieceImported2 = true;
  bool pieceDropped2 = false;

  //Player 2: Initializing held piece and next pieces:
  const int heldPieceY2 = topMargin + squareWidth + 1;
  const int nextPiecesY2 = topMargin + squareWidth * 2 + 1;
  const int heldPieceX2 = leftBorder2 + squareWidth * 2;
  const int firstNextPieceX2 = heldPieceX2 + squareWidth * 6;
  const int secondNextPieceX2 = firstNextPieceX2 + squareWidth * 6;

  int heldPieceType2 = - 1;
  bool holdAllowed2 = true;

  bool** heldPiece2 = make2DArray(pieceWidth, pieceWidth);
  fill2DArray(heldPiece2, pieceWidth, pieceWidth, false);
  bool** firstNextPiece2 = make2DArray(pieceWidth, pieceWidth);
  importPiece(firstNextPiece2, nextTypes2[0], 4, pieceWidth, 2);
  bool** secondNextPiece2 = make2DArray(pieceWidth, pieceWidth);
  importPiece(secondNextPiece2, nextTypes2[1], 4, pieceWidth, 2);

  //Player 2: Erasing parts of the checkerboard pattern on the array:
  drawRectangle(ledArray, false, topBorder2, leftBorder2, botBound2 + 1 - topBorder2, rightBound2 + 1 - leftBorder2);
  drawRectangle(ledArray, false, heldPieceY2 - 1, heldPieceX2 - squareWidth, squareWidth * 5, squareWidth * 5);
  drawRectangle(ledArray, false, nextPiecesY2 - 1, firstNextPieceX2 - squareWidth, squareWidth * 5, squareWidth * 5);
  drawRectangle(ledArray, false, nextPiecesY2 - 1, secondNextPieceX2 - squareWidth, squareWidth * 5, squareWidth * 5);

  //Player 2: Drawing next pieces:
  drawPiece(ledArray, firstNextPiece2, nextTypes2[0], true, nextPiecesY2, firstNextPieceX2, pieceWidth, 2);
  drawPiece(ledArray, secondNextPiece2, nextTypes2[1], true, nextPiecesY2, secondNextPieceX2, pieceWidth, 2);

  //Player 2: Current state and projected state of active pieces:
  bool** curPiece2 = make2DArray(pieceWidth, pieceWidth);
  importPiece(curPiece2, curType2, pieceOrien2, pieceWidth, 2);
  bool** projPiece2 = make2DArray(pieceWidth, pieceWidth);
  copyPiece(projPiece2, curPiece2, pieceWidth);
  while(checkOverlap(ledArray, projPiece2, curPiece2, curY2, shadX2 + squareWidth, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
    shadX2 += squareWidth;
  }

  while(1) {
    //Redrawing for player 1:
    if(pieceShifted) { // Includes left or right translations of the active piece
      drawPiece(ledArray, curPiece, curType, false, curY, curX, pieceWidth, 1);
      drawShadow(ledArray, curPiece, curType, false, curY, shadX, pieceWidth, 1);
      drawPiece(ledArray, projPiece, curType, true, projY, projX, pieceWidth, 1);
      curY = projY;
      shadX = curX;
      while(checkOverlap(ledArray, projPiece, curPiece, curY, shadX - squareWidth, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
        shadX -= squareWidth;
      }
      drawShadow(ledArray, curPiece, curType, true, curY, shadX, pieceWidth, 1);
      pieceShifted = false;
    }
    else if(pieceImported) { //Includes holding or rotating the active piece
      drawPiece(ledArray, curPiece, curType, false, curY, curX, pieceWidth, 1);
      drawShadow(ledArray, curPiece, curType, false, curY, shadX, pieceWidth, 1);
      drawPiece(ledArray, projPiece, curType, true, projY, projX, pieceWidth, 1);
      copyPiece(curPiece, projPiece, pieceWidth);
      curY = projY;
      shadX = curX;
      while(checkOverlap(ledArray, projPiece, curPiece, curY, shadX - squareWidth, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
        shadX -= squareWidth;
      }
      drawShadow(ledArray, curPiece, curType, true, curY, shadX, pieceWidth, 1);
      pieceImported = false;
    }
    else if(pieceDropped) { //Includes a "soft drop" action only
      drawPiece(ledArray, curPiece, curType, false, curY, curX, pieceWidth, 1);
      drawPiece(ledArray, projPiece, curType, true, projY, projX, pieceWidth, 1);
      curX = projX;
      pieceDropped = false;
    }

    //Redrawing for player 2:
    if(pieceShifted2) { //Includes holding or rotating the active piece
      drawPiece(ledArray, curPiece2, curType2, false, curY2, curX2, pieceWidth, 2);
      drawShadow(ledArray, curPiece2, curType2, false, curY2, shadX2, pieceWidth, 2);
      drawPiece(ledArray, projPiece2, curType2, true, projY2, projX2, pieceWidth, 2);
      curY2 = projY2;
      shadX2 = curX2;
      while(checkOverlap(ledArray, projPiece2, curPiece2, curY2, shadX2 + squareWidth, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
        shadX2 += squareWidth;
      }
      drawShadow(ledArray, curPiece2, curType2, true, curY2, shadX2, pieceWidth, 2);
      pieceShifted2 = false;
    }
    else if(pieceImported2) { //Includes holding or rotating the active piece
      drawPiece(ledArray, curPiece2, curType2, false, curY2, curX2, pieceWidth, 2);
      drawShadow(ledArray, curPiece2, curType2, false, curY2, shadX2, pieceWidth, 2);
      drawPiece(ledArray, projPiece2, curType2, true, projY2, projX2, pieceWidth, 2);
      copyPiece(curPiece2, projPiece2, pieceWidth);
      curY2 = projY2;
      shadX2 = curX2;
      while(checkOverlap(ledArray, projPiece2, curPiece2, curY2, shadX2 + squareWidth, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
        shadX2 += squareWidth;
      }
      drawShadow(ledArray, curPiece2, curType2, true, curY2, shadX2, pieceWidth, 2);
      pieceImported2 = false;
    }
    else if(pieceDropped2) { //Includes a "soft drop" action only
      drawPiece(ledArray, curPiece2, curType2, false, curY2, curX2, pieceWidth, 2);
      drawPiece(ledArray, projPiece2, curType2, true, projY2, projX2, pieceWidth, 2);
      curX2 = projX2;
      pieceDropped2 = false;
    }

    frameTest(ledArray, topMargin, leftMargin, botEnd, rightEnd);

    //Player 1 input handling:
    input = getLeftInput();
    if (input == 1) { //Hard drop
      while(checkOverlap(ledArray, projPiece, curPiece, projY, projX - squareWidth, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
        projX -= squareWidth;
        drawPiece(ledArray, curPiece, curType, false, curY, curX, pieceWidth, 1);
        drawPiece(ledArray, curPiece, curType, true, curY, projX, pieceWidth, 1);
        curX = projX;
        frameTest(ledArray, topMargin, leftMargin, botEnd, rightEnd);
      }
      linesCleared += checkLines(ledArray, leftBound, rightBound, botBound, topBound, curX, pieceWidth, 1);
      while(linesCleared2 > 3){
        linesCleared2 -= 4;
        if(addGarbage(ledArray, leftBound, rightBound, botBound, topBound, squareWidth, 1)){
          break;
        }
      }
      drawPiece(ledArray, firstNextPiece, nextTypes[0], false, nextPiecesY, firstNextPieceX, pieceWidth, 1);
      drawPiece(ledArray, secondNextPiece, nextTypes[1], false, nextPiecesY, secondNextPieceX, pieceWidth, 1);
      curType = takeNextType(nextTypes);
      copyPiece(curPiece, firstNextPiece, pieceWidth);
      copyPiece(firstNextPiece, secondNextPiece, pieceWidth);
      importPiece(secondNextPiece, nextTypes[1], 2, pieceWidth, 1);
      drawPiece(ledArray, firstNextPiece, nextTypes[0], true, nextPiecesY, firstNextPieceX, pieceWidth, 1);
      drawPiece(ledArray, secondNextPiece, nextTypes[1], true, nextPiecesY, secondNextPieceX, pieceWidth, 1);
      projX = initPieceX;
      projY = initPieceY;
      curX = projX;
      curY = projY;
      shadX = curX;
      pieceOrien = 2;
      copyPiece(projPiece, curPiece, pieceWidth);
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, true)) {
        break; //Game loss
      }
      holdAllowed = true;
      timer = 1;
      pieceImported = true;
    }
    else if(input == 5 || timer++ % dropTime == 0) { //Soft drop
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX - squareWidth, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
        linesCleared += checkLines(ledArray, leftBound, rightBound, botBound, topBound, curX, pieceWidth, 1);
        while(linesCleared2 > 3){
          linesCleared2 -= 4;
          if(addGarbage(ledArray, leftBound, rightBound, botBound, topBound, squareWidth, 1)){
            break;
          }
        }
        drawPiece(ledArray, firstNextPiece, nextTypes[0], false, nextPiecesY, firstNextPieceX, pieceWidth, 1);
        drawPiece(ledArray, secondNextPiece, nextTypes[1], false, nextPiecesY, secondNextPieceX, pieceWidth, 1);
        curType = takeNextType(nextTypes);
        copyPiece(curPiece, firstNextPiece, pieceWidth);
        copyPiece(firstNextPiece, secondNextPiece, pieceWidth);
        importPiece(secondNextPiece, nextTypes[1], 2, pieceWidth, 1);
        drawPiece(ledArray, firstNextPiece, nextTypes[0], true, nextPiecesY, firstNextPieceX, pieceWidth, 1);
        drawPiece(ledArray, secondNextPiece, nextTypes[1], true, nextPiecesY, secondNextPieceX, pieceWidth, 1);
        projX = initPieceX;
        projY = initPieceY;
        curX = projX;
        curY = projY;
        shadX = curX;
        pieceOrien = 2;
        copyPiece(projPiece, curPiece, pieceWidth);
        if(checkOverlap(ledArray, projPiece, curPiece, projY, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, true)) {
          break; //Game loss
        }
        holdAllowed = true;
        pieceImported = true;
      }
      else {
        projX -= squareWidth;
        pieceDropped = true;
      }
      timer = 1;
    }
    else if(input == 9) { //Spin clockwise
      pieceOrien = changePieceOrien(pieceOrien, + 1);
      importPiece(projPiece, curType, pieceOrien, pieceWidth, 1);
      pieceImported = true;
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
        if(!checkOverlap(ledArray, projPiece, curPiece, projY + squareWidth, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY += squareWidth;
        }
        else if(curType == 0
                && !checkOverlap(ledArray, projPiece, curPiece, projY + 2 * squareWidth, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY += 2 * squareWidth;
        }
        else if(!checkOverlap(ledArray, projPiece, curPiece, projY - squareWidth, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY -= squareWidth;
        }
        else if(curType == 0
                && !checkOverlap(ledArray, projPiece, curPiece, projY - 2 * squareWidth, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY -= 2 * squareWidth;
        }
        else {
          pieceOrien = changePieceOrien(pieceOrien, - 1);
          importPiece(projPiece, curType, pieceOrien, pieceWidth, 1);
          pieceImported = false;
        }
      }
    }
    else if(input == 10) { //Spin counterclockwise
      pieceOrien = changePieceOrien(pieceOrien, - 1);
      importPiece(projPiece, curType, pieceOrien, pieceWidth, 1);
      pieceImported = true;
      if(checkOverlap(ledArray, projPiece, curPiece, projY, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
        if(!checkOverlap(ledArray, projPiece, curPiece, projY + squareWidth, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth,  false)) {
          projY += squareWidth;
        }
        else if(curType == 0
                && !checkOverlap(ledArray, projPiece, curPiece, projY + 2 * squareWidth, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY += 2 * squareWidth;
        }
        else if(!checkOverlap(ledArray, projPiece, curPiece, projY - squareWidth, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY -= squareWidth;
        }
        else if(curType == 0
                && !checkOverlap(ledArray, projPiece, curPiece, projY - 2 * squareWidth, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY -= 2 * squareWidth;
        }
        else {
          pieceOrien = changePieceOrien(pieceOrien, + 1);
          importPiece(projPiece, curType, pieceOrien, pieceWidth, 1);
          pieceImported = false;
        }
      }
    }
    else if(input == 3) { //Move right
      if(checkOverlap(ledArray, projPiece, curPiece, projY + squareWidth, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
        projY += squareWidth;
        pieceShifted = true;
      }
    }
    else if(input == 7) { //Move left
      if(checkOverlap(ledArray, projPiece, curPiece, projY - squareWidth, projX, curY, curX, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
        projY -= squareWidth;
        pieceShifted = true;
      }
    }
    else if(input == 11 && holdAllowed) {
      if(heldPieceType == -1) {
        drawPiece(ledArray, curPiece, curType, false, curY, curX, pieceWidth, 1);
        drawShadow(ledArray, curPiece, curType, false, curY, shadX, pieceWidth, 1);
        projX = initPieceX;
        projY = initPieceY;
        curX = projX;
        curY = projY;
        shadX = curX;
        drawPiece(ledArray, firstNextPiece, nextTypes[0], false, nextPiecesY, firstNextPieceX, pieceWidth, 1);
        drawPiece(ledArray, secondNextPiece, nextTypes[1], false, nextPiecesY, secondNextPieceX, pieceWidth, 1);
        heldPieceType = curType;
        importPiece(heldPiece, curType, 2, pieceWidth, 1);
        drawPiece(ledArray, heldPiece, heldPieceType, true, heldPieceY, heldPieceX, pieceWidth, 1);
        curType = takeNextType(nextTypes);
        copyPiece(projPiece, firstNextPiece, pieceWidth);
        copyPiece(firstNextPiece, secondNextPiece, pieceWidth);
        importPiece(secondNextPiece, nextTypes[1], 2, pieceWidth, 1);
        drawPiece(ledArray, firstNextPiece, nextTypes[0], true, nextPiecesY, firstNextPieceX, pieceWidth, 1);
        drawPiece(ledArray, secondNextPiece, nextTypes[1], true, nextPiecesY, secondNextPieceX, pieceWidth, 1);
      }
      else{
        drawPiece(ledArray, heldPiece, heldPieceType, false, heldPieceY, heldPieceX, pieceWidth, 1);
        drawPiece(ledArray, curPiece, curType, false, curY, curX, pieceWidth, 1);
        drawShadow(ledArray, curPiece, curType, false, curY, shadX, pieceWidth, 1);
        projX = initPieceX;
        projY = initPieceY;
        curX = projX;
        curY = projY;
        shadX = curX;
        copyPiece(projPiece, heldPiece, pieceWidth);
        importPiece(heldPiece, curType, 2, pieceWidth, 1);
        int tempType = heldPieceType;
        heldPieceType = curType;
        curType = tempType;
        drawPiece(ledArray, heldPiece, heldPieceType, true, heldPieceY, heldPieceX, pieceWidth, 1);
      }
      pieceOrien = 2;
      timer = 1;
      holdAllowed = false;
      pieceImported = true;
    }

    //Player 2 input handling:
    input2 = getRightInput();
    if (input2 == 1) { //Hard drop
      while(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2 + squareWidth, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
        projX2 += squareWidth;
        drawPiece(ledArray, curPiece2, curType2, false, curY2, curX2, pieceWidth, 2);
        drawPiece(ledArray, curPiece2, curType2, true, curY2, projX2, pieceWidth, 2);
        curX2 = projX2;
        frameTest(ledArray, topMargin, leftMargin, botEnd, rightEnd);
      }
      linesCleared2 += checkLines(ledArray, leftBound2, rightBound2, botBound2, topBound2, curX2, pieceWidth,  2);
      while(linesCleared > 3){
        linesCleared -= 4;
        if(addGarbage(ledArray, leftBound2, rightBound2, botBound2, topBound2, squareWidth, 2)) {
          break;
        }
      }
      drawPiece(ledArray, firstNextPiece2, nextTypes2[0], false, nextPiecesY2, firstNextPieceX2, pieceWidth, 2);
      drawPiece(ledArray, secondNextPiece2, nextTypes2[1], false, nextPiecesY2, secondNextPieceX2, pieceWidth, 2);
      curType2 = takeNextType(nextTypes2);
      copyPiece(curPiece2, firstNextPiece2, pieceWidth);
      copyPiece(firstNextPiece2, secondNextPiece2, pieceWidth);
      importPiece(secondNextPiece2, nextTypes2[1], 4, pieceWidth, 2);
      drawPiece(ledArray, firstNextPiece2, nextTypes2[0], true, nextPiecesY2, firstNextPieceX2, pieceWidth, 2);
      drawPiece(ledArray, secondNextPiece2, nextTypes2[1], true, nextPiecesY2, secondNextPieceX2, pieceWidth, 2);
      projX2 = initPieceX2;
      projY2 = initPieceY2;
      curX2 = projX2;
      curY2 = projY2;
      shadX2 = curX2;
      pieceOrien2 = 4;
      copyPiece(projPiece2, curPiece2, pieceWidth);
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, true)) {
        break; //Game loss
      }
      holdAllowed2 = true;
      timer2 = 1;
      pieceImported2 = true;
    }
    else if(input2 == 5 || timer2++ % dropTime == 0) { //Soft drop
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2 + squareWidth, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
        linesCleared2 += checkLines(ledArray, leftBound2, rightBound2, botBound2, topBound2, curX2, pieceWidth, 2);
        while(linesCleared > 3){
          linesCleared -= 4;
          if(addGarbage(ledArray, leftBound2, rightBound2, botBound2, topBound2, squareWidth, 2)) {
            break;
          }
        }
        drawPiece(ledArray, firstNextPiece2, nextTypes2[0], false, nextPiecesY2, firstNextPieceX2, pieceWidth, 2);
        drawPiece(ledArray, secondNextPiece2, nextTypes2[1], false, nextPiecesY2, secondNextPieceX2, pieceWidth, 2);
        curType2 = takeNextType(nextTypes2);
        copyPiece(curPiece2, firstNextPiece2, pieceWidth);
        copyPiece(firstNextPiece2, secondNextPiece2, pieceWidth);
        importPiece(secondNextPiece2, nextTypes2[1], 4, pieceWidth, 2);
        drawPiece(ledArray, firstNextPiece2, nextTypes2[0], true, nextPiecesY2, firstNextPieceX2, pieceWidth, 2);
        drawPiece(ledArray, secondNextPiece2, nextTypes2[1], true, nextPiecesY2, secondNextPieceX2, pieceWidth, 2);
        projX2 = initPieceX2;
        projY2 = initPieceY2;
        curX2 = projX2;
        curY2 = projY2;
        shadX2 = curX2;
        pieceOrien2 = 4;
        copyPiece(projPiece2, curPiece2, pieceWidth);
        if(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, true)) {
          break; //Game loss
        }
        holdAllowed2 = true;
        pieceImported2 = true;
      }
      else {
        projX2 += squareWidth;
        pieceDropped2 = true;
      }
      timer2 = 1;
    }
    else if(input2 == 9) { //Spin clockwise
      pieceOrien2 = changePieceOrien(pieceOrien2, + 1);
      importPiece(projPiece2, curType2, pieceOrien2, pieceWidth, 2);
      pieceImported2 = true;
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
        if(!checkOverlap(ledArray, projPiece2, curPiece2, projY2 - squareWidth, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY2 -= squareWidth;
        }
        else if(curType2 == 0
                && !checkOverlap(ledArray, projPiece2, curPiece2, projY2 - 2 * squareWidth, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY2 -= 2 * squareWidth;
        }
        else if(!checkOverlap(ledArray, projPiece2, curPiece2, projY2 + squareWidth, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY2 += squareWidth;
        }
        else if(curType2 == 0
                && !checkOverlap(ledArray, projPiece2, curPiece2, projY2 + 2 * squareWidth, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY2 += 2 * squareWidth;
        }
        else {
          pieceOrien2 = changePieceOrien(pieceOrien2, - 1);
          importPiece(projPiece2, curType2, pieceOrien2, pieceWidth, 2);
          pieceImported2 = false;
        }
      }
    }
    else if(input2 == 10) { //Spin counterclockwise
      pieceOrien2 = changePieceOrien(pieceOrien2, - 1);
      importPiece(projPiece2, curType2, pieceOrien2, pieceWidth, 2);
      pieceImported2 = true;
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
        if(!checkOverlap(ledArray, projPiece2, curPiece2, projY2 - squareWidth, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY2 -= squareWidth;
        }
        else if(curType2 == 0
                && !checkOverlap(ledArray, projPiece2, curPiece2, projY2 - 2 * squareWidth, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY2 -= 2 * squareWidth;
        }
        else if(!checkOverlap(ledArray, projPiece2, curPiece2, projY2 + squareWidth, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY2 += squareWidth;
        }
        else if(curType2 == 0
                && !checkOverlap(ledArray, projPiece2, curPiece2, projY2 + 2 * squareWidth, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false)) {
          projY2 += 2 * squareWidth;
        }
        else {
          pieceOrien2 = changePieceOrien(pieceOrien2, + 1);
          importPiece(projPiece2, curType2, pieceOrien2, pieceWidth, 2);
          pieceImported2 = false;
        }
      }
    }
    else if(input2 == 3) { //Move right
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2 - squareWidth, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
        projY2 -= squareWidth;
        pieceShifted2 = true;
      }
    }
    else if(input2 == 7) { //Move left
      if(checkOverlap(ledArray, projPiece2, curPiece2, projY2 + squareWidth, projX2, curY2, curX2, leftBorder, rightEnd, topBorder, rightEnd, pieceWidth, false) == 0) {
        projY2 += squareWidth;
        pieceShifted2 = true;
      }
    }
    else if(input2 == 11 && holdAllowed2) {
      if(heldPieceType2 == -1) {
        drawPiece(ledArray, curPiece2, curType2, false, curY2, curX2, pieceWidth, 2);
        drawShadow(ledArray, curPiece2, curType2, false, curY2, shadX2, pieceWidth, 2);
        projX2 = initPieceX2;
        projY2 = initPieceY2;
        curX2 = projX2;
        curY2 = projY2;
        shadX2 = curX2;
        drawPiece(ledArray, firstNextPiece2, nextTypes2[0], false, nextPiecesY2, firstNextPieceX2, pieceWidth, 2);
        drawPiece(ledArray, secondNextPiece2, nextTypes2[1], false, nextPiecesY2, secondNextPieceX2, pieceWidth, 2);
        heldPieceType2 = curType2;
        importPiece(heldPiece2, curType2, 4, pieceWidth, 2);
        drawPiece(ledArray, heldPiece2, heldPieceType2, true, heldPieceY2, heldPieceX2, pieceWidth, 2);
        curType2 = takeNextType(nextTypes2);
        copyPiece(projPiece2, firstNextPiece2, pieceWidth);
        copyPiece(firstNextPiece2, secondNextPiece2, pieceWidth);
        importPiece(secondNextPiece2, nextTypes2[1], 4, pieceWidth, 2);
        drawPiece(ledArray, firstNextPiece2, nextTypes2[0], true, nextPiecesY2, firstNextPieceX2, pieceWidth, 2);
        drawPiece(ledArray, secondNextPiece2, nextTypes2[1], true, nextPiecesY2, secondNextPieceX2, pieceWidth, 2);
      }
      else{
        drawPiece(ledArray, heldPiece2, heldPieceType2, false, heldPieceY2, heldPieceX2, pieceWidth, 2);
        drawPiece(ledArray, curPiece2, curType2, false, curY2, curX2, pieceWidth, 2);
        drawShadow(ledArray, curPiece2, curType2, false, curY2, shadX2, pieceWidth, 2);
        projX2 = initPieceX2;
        projY2 = initPieceY2;
        curX2 = projX2;
        curY2 = projY2;
        shadX2 = curX2;
        copyPiece(projPiece2, heldPiece2, pieceWidth);
        importPiece(heldPiece2, curType2, 4, pieceWidth, 2);
        int tempType2 = heldPieceType2;
        heldPieceType2 = curType2;
        curType2 = tempType2;
        drawPiece(ledArray, heldPiece2, heldPieceType2, true, heldPieceY2, heldPieceX2, pieceWidth, 2);
      }
      pieceOrien2 = 4;
      timer2 = 1;
      holdAllowed2 = false;
      pieceImported2 = true;
    }
  }
  drawPiece(ledArray, curPiece, curType, true, curY, curX, pieceWidth, 1);
  drawPiece(ledArray, curPiece2, curType2, true, curY2, curX2, pieceWidth, 2);
  frameTest(ledArray, topMargin, leftMargin, botEnd, rightEnd);
  free(nextTypes);
  free2DArray(curPiece, pieceWidth);
  free2DArray(projPiece, pieceWidth);
  free(nextTypes2);
  free2DArray(curPiece2, pieceWidth);
  free2DArray(projPiece2, pieceWidth);
  return;
}

/*int main (void) {
  //This function's contents are throwaway. They mimic a lodev menu.
  const float ARRAY_HEIGHT = 38.0;
  const float ARRAY_WIDTH = 76.0;
  bool** ledArray;
  ledArray = make2DArray(ARRAY_HEIGHT, ARRAY_WIDTH);
  fill2DArray(ledArray, ARRAY_HEIGHT, ARRAY_WIDTH, false);
  tetrisTwoPlayer(ledArray);
  free2DArray(ledArray, ARRAY_HEIGHT);
  return 0;
}*/
