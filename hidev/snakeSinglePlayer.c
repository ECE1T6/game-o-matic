#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
//Windows (for testing only):
//#include <windows.h>
//#include <conio.h>
//Linux:
#include <unistd.h>

//Structures:
struct segment {
  int x;
  int y;
  struct segment *next;
};

//Game-agnostic functions (require lodev support):
void freezeFrame(void) {
  //Call a sleep function roughly the length of a frame. Adjust for aesthetics.
  usleep(30000);
}
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
void printScore(int score) { //This is a placeholder for a lodev function
  return;
}

//Test functions (remove in final version):
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
  usleep(50000);
  system("clear");

  printTest(ledArray, TOP_MARGIN, LEFT_MARGIN, BOT_END, RIGHT_END);
}

//Snake-specific functions:
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
  //Returns 1 if (newX, newY) coincides with any coordinate pair described in a segment list
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
int checkOutBounds(int headX, int headY, float TOP_MARGIN, float LEFT_MARGIN, float BOT_END, float RIGHT_END) {
  if(headY < TOP_MARGIN || headY > BOT_END || headX < LEFT_MARGIN || headX > RIGHT_END) {
    return 1;
  }
  return 0;
}
void placeFood(int *foodX, int *foodY, struct segment *first, int HEIGHT, int WIDTH, int FIRST_TOP, int FIRST_LEFT, bool **ledArray) {
  do {
    *foodY = rand() % HEIGHT + FIRST_TOP;
    *foodX = rand() % WIDTH + FIRST_LEFT;
  } while (ledArray[*foodY][*foodX] == true);
  ledArray[*foodY][*foodX] = true;
  return;
}
int moveHeadCoords(int moveDir, int visibleMoveDir, float *headX, float *headY, float moveDistance) {
  //Moves the head coordinates but does not modify any list.
  int previousMoveDir = moveDir;
  moveDir = getLeftInput();
  if (moveDir != 1 && moveDir != 3 && moveDir != 5 && moveDir != 7) { //Enforcing cardinal directions
    moveDir = previousMoveDir;
  }
  else if(moveDir - visibleMoveDir == 4 || visibleMoveDir - moveDir == 4) { //Moving into previous segments is disallowed
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
  //Removes the last structure of any list.
  struct segment *current, *previous;
  current = first;
  previous = NULL;
  while (current->next != NULL) {
    previous = current;
    current = current->next;
  }
  if (previous == NULL) { //i.e. if the last segment is being deleted.
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
    freezeFrame();
  }
  ledArray[foodY][foodX] = false;
  while(first != NULL) {
    first = truncateList(first, ledArray);
    freezeFrame();
  }
  return;
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

  //Playfield constants:
  const float ARRAY_HEIGHT = 38.0;
  const float ARRAY_WIDTH = 76.0;
  const float TOP_MARGIN = 5.0; //The margins bound the area controlled by the game.
  const float BOT_MARGIN = 5.0;
  const float LEFT_MARGIN = 10.0;
  const float RIGHT_MARGIN = 10.0;
  const float BOT_END = ARRAY_HEIGHT - BOT_MARGIN - 1.0;
  const float RIGHT_END = ARRAY_WIDTH - RIGHT_MARGIN - 1.0;

  //Snake-initializing constants:
  const int INITIAL_X = (RIGHT_END - LEFT_MARGIN) / 2;
  const int INITIAL_Y = (BOT_END - TOP_MARGIN) / 2;
  const int INITIAL_SIZE = (int) ((RIGHT_END - LEFT_MARGIN) * 0.1);

  //Miscellaneous initialization:
  float moveDistance = 0.9; //Increase for speed and difficulty. Must be <= 1.
  srand(time(NULL));
  int score = 0;

  //Snake variable initialization:
  int moveDir = 3; //no move == 0, up == 1, right == 3, down == 5, left == 7
  int visibleMoveDir = 3;
  struct segment *first = initializeList(INITIAL_X, INITIAL_Y, INITIAL_SIZE, ledArray);
  float headX = first->x;
  float headY = first->y;

  //Food spawns:
  int foodX, foodY;
  placeFood(&foodX, &foodY, first, BOT_END - TOP_MARGIN, RIGHT_END - LEFT_MARGIN, TOP_MARGIN, LEFT_MARGIN, ledArray);

  while(1) {
    frameTest(ledArray, TOP_MARGIN, LEFT_MARGIN, BOT_END, RIGHT_END);
    moveDir = moveHeadCoords(moveDir, visibleMoveDir, &headX, &headY, moveDistance);
    if(checkOutBounds(headX + 0.5, headY + 0.5, TOP_MARGIN, LEFT_MARGIN, BOT_END, RIGHT_END)) {
      break;
    }
    if((int) (headX + 0.5) == foodX && (int) (headY + 0.5) == foodY) {
      first = addToList(first, headX + 0.5, headY + 0.5, ledArray);
      placeFood(&foodX, &foodY, first, BOT_END - TOP_MARGIN, RIGHT_END - LEFT_MARGIN, TOP_MARGIN, LEFT_MARGIN, ledArray);
      score += 10; //Also change displayed score, if it exists.
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
  //This function's contents are throwaway. They mimic a lodev menu.
  const float ARRAY_HEIGHT = 38.0;
  const float ARRAY_WIDTH = 76.0;
  bool** ledArray;
  ledArray = make2DArray(ARRAY_HEIGHT, ARRAY_WIDTH);
  fill2DArray(ledArray, ARRAY_HEIGHT, ARRAY_WIDTH, false);
  snake(ledArray);
  free2DArray(ledArray, ARRAY_HEIGHT);
  return 0;
}
