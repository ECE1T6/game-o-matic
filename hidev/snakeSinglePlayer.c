#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
/*Windows: (for testing only)*/
//#include <windows.h>
//#include <conio.h>
/*Linux:*/
#include <unistd.h>

/*Global variables used by all games:*/
const float ARRAY_HEIGHT = 38.0;
const float ARRAY_WIDTH = 76.0;
float TOP_MARGIN; /*The margins bound the area controlled by the game.*/
float BOT_MARGIN;
float LEFT_MARGIN;
float RIGHT_MARGIN;
float BOT_END; /*= ARRAY_HEIGHT - BOT_MARGIN - 1.0*/
float RIGHT_END; /*= ARRAY_WIDTH - RIGHT_MARGIN - 1.0*/

/*Structures:*/
struct segment {
  int x;
  int y;
  struct segment *next;
};

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
struct segment *initializeList(int INITIAL_X, int INITIAL_Y, int INITIAL_SIZE, bool **ledArray) {
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
int checkOutBounds(int headX, int headY) {
  if(headY < TOP_MARGIN || headY > BOT_END) {
    return 1;
  }
  if(headX < LEFT_MARGIN || headX > RIGHT_END) {
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
int moveHeadCoords(int moveDir, int visibleMoveDir, float *headX, float *headY, float moveDistance) {
  int previousMoveDir = moveDir;
  moveDir = getLeftInput(); /*Accepting integer test input.*/
  if (moveDir != 1 && moveDir != 3 && moveDir != 5 && moveDir != 7) { /*Enforcing cardinal directions*/
    moveDir = previousMoveDir;
  }
  else if(moveDir - visibleMoveDir == 4 || visibleMoveDir - moveDir == 4) { /*Moving into previous segments is disallowed*/
    moveDir = previousMoveDir;
  }
  if (moveDir == 1) {
    *headY -= moveDistance;
  }
  else if (moveDir == 3) {
    *headX += moveDistance;
  }
  else if (moveDir == 5) {
    *headY += moveDistance;
  }
  else if (moveDir == 7) {
    *headX -= moveDistance;
  }
  return moveDir;
}
struct segment *truncateList(struct segment *first, bool **ledArray) {
  struct segment *current, *previous;
  current = first;
  previous = NULL;
  while (current->next != NULL) {
    previous = current;
    current = current->next;
  }
  if (previous == NULL) { /*i.e. if the last segment is being deleted.*/
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
void endGame(bool **ledArray, int foodY, int foodX, struct segment *first) {
  int endGameDelay;
  for(endGameDelay = 0; endGameDelay < 8; endGameDelay++) {
    frameTest(ledArray);
  }
  ledArray[foodY][foodX] = false;
  int counter = 0;
  while(first != NULL) {
    first = truncateList(first, ledArray);
    frameTest(ledArray);
  }
}
void shiftList(struct segment *first, int headX, int headY, bool **ledArray) {
  int oldX, oldY, newX, newY;
  struct segment *place = first;
  newX = place->x;
  newY = place->y;
  place->x = headX;
  place->y = headY;
  ledArray[headY][headX] = true;
  while(place->next != NULL) {
    place = place->next;
    oldX = place->x;
    oldY = place->y;
    place->x = newX;
    place->y = newY;
    newX = oldX;
    newY = oldY;
  }
  ledArray[oldY][oldX] = false;
  return;
}

void snake(bool** ledArray) {
  /*Setting values of the global variables:*/
  TOP_MARGIN = 10.0;
  BOT_MARGIN = 10.0;
  LEFT_MARGIN = 10.0;
  RIGHT_MARGIN = 10.0;
  BOT_END = ARRAY_HEIGHT - BOT_MARGIN - 1.0;
  RIGHT_END = ARRAY_WIDTH - RIGHT_MARGIN - 1.0;

  /*Local constants:*/
  const int INITIAL_X = LEFT_MARGIN + (RIGHT_END - LEFT_MARGIN) / 2;
  const int INITIAL_Y = TOP_MARGIN + (BOT_END - TOP_MARGIN) / 2;
  const int INITIAL_SIZE = (int) ((RIGHT_END - LEFT_MARGIN) * 0.1);
  srand(time(NULL));

  /*Local variables:*/
  float moveDistance = 0.9; /*Adjustable for difficulty. Must be <= 1.*/
  struct segment *first = initializeList(INITIAL_X, INITIAL_Y, INITIAL_SIZE, ledArray);
  float headX = first->x;
  float headY = first->y;
  int moveDir = 3; /*no move == 0, up == 1, right == 3, down == 5, left == 7*/
  int visibleMoveDir = 3;
  int foodX;
  int foodY;
  placeFood(&foodX, &foodY, first, BOT_END - TOP_MARGIN, RIGHT_END - LEFT_MARGIN, TOP_MARGIN, LEFT_MARGIN, ledArray);
  int score = 0;

  while(1) {
    frameTest(ledArray);
    moveDir = moveHeadCoords(moveDir, visibleMoveDir, &headX, &headY, moveDistance);
    if(checkOutBounds(headX + 0.5, headY + 0.5)) {
      break;
    }
    if((int) (headX + 0.5) == foodX && (int) (headY + 0.5) == foodY) {
      first = addToList(first, headX + 0.5, headY + 0.5, ledArray);
      placeFood(&foodX, &foodY, first, BOT_END - TOP_MARGIN, RIGHT_END - LEFT_MARGIN, TOP_MARGIN, LEFT_MARGIN, ledArray);
      score += 10; /*Also change displayed score, if it exists.*/
    }
    else if((int) (headX + 0.5) != first->x || (int) (headY + 0.5) != first->y) {
      shiftList(first, headX + 0.5, headY + 0.5, ledArray);
      visibleMoveDir = moveDir;
    }
    if(checkCollision(first->x, first->y, first->next)) {
      break;
    }
  }
  endGame(ledArray, foodY, foodX, first);
  return;
}

int main (void) {
  /*This function's contents are placeholders for a lodev menu.*/
  bool** ledArray;
  ledArray = make2DArray(ARRAY_HEIGHT, ARRAY_WIDTH);
  fill2DArray(ledArray, ARRAY_HEIGHT, ARRAY_WIDTH, false);
  int input = 0;
  do {
    printf("Welcome to Snake (official bootleg edition).\nEnter \"1\" to play Snake.\nEnter \"0\" to exit.");
    scanf("%d", &input);
  } while(input < 0 || input > 1);
  while (input == 1) {
      snake(ledArray);
      do {
        printf("Enter \"1\" to play again.\nEnter \"0\" to exit.\n");
        scanf("%d", &input);
      } while (input < 0 || input > 1);
  }
  free2DArray(ledArray, ARRAY_HEIGHT);
  return 0;
}
