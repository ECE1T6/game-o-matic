#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <wiringPi.h>
#include <pthread.h>
#include <time.h>

/*
  Screen size spec
*/
#define DISPLAY_WIDTH  38 
#define DISPLAY_LENGTH  76
#define UNIT_WIDTH 8
#define UNIT_LENGTH 76

/*
  Pinout map on Raspberry Pi
*/
#define COL 0
#define ROW 1
#define LATCH 2
#define COLCLK 3
#define ROWCLK 4

/*
  Screen wrapper for arguments passed to asynchronous thread functions.
*/
typedef struct {
  bool **screen;
} ScreenData;

/*
  nanosleep parameter spec
*/
typedef struct timespec {
  time_t tv_sec;
  long tv_nsec;
} Timespec;

const Timespec nanospec = {
  0, 100L
};

/*
  Clock in data to the column (sinking) registers.
  Hardware: TPIC6B595N acting as sink.
  100ns delay should be safe; minimum delays can be found on datasheets.
*/
void colclk(void) {
  digitalWrite(COLCLK, HIGH);
  nanosleep(&nanospec, NULL); 
  digitalWrite(COLCLK, LOW);
  return;
}

/*
  Clock in data to the row (sourcing) registers
  Hardware: TPIC6B595N in combination with transistors to source current.
*/
void rowclk(void) {
  digitalWrite(ROWCLK, HIGH);
  nanosleep(&nanospec, NULL);
  digitalWrite(ROWCLK, LOW);
  return;
}

/*
  Latches both row and column registers.
*/
void latch(void) {
  digitalWrite(LATCH, HIGH); 
  nanosleep(&nanospec, NULL);
  digitalWrite(LATCH, LOW);
  return;
}

/*
  Serial data in setters for row and column registers.
*/
void rowInHigh(void) {
  digitalWrite(ROW, HIGH);
  return;
}

void rowInLow(void) {
  digitalWrite(ROW, LOW);
  return;
}

void colInHigh(void) {
  digitalWrite(COL, HIGH);
  return;
}

void colInLow (void) {
  digitalWrite(COL, LOW);
  return;
}

/*
  Clears the data of both column and row registers to 0 (low).
*/
void flush(void) {
  int row, col;
  rowInLow();
  colInLow();

  for (row = 0; row < UNIT_WIDTH; row++) {
    rowclk();
  }

  for (col = 0; col < UNIT_LENGTH; col++) {
    colclk();
  }
  
  return;
}

/*
  Sets the column data pins to low, with the exception of
  the leading data pin, which is set to high.
*/
void resetRows(void) {
  int row;
  //Set each of the rows to low except the first row.
  rowInLow();
  for (int row = UNIT_WIDTH - 1; row > 0; row--) {
    rowclk();
  }
  
  //Finally, set the first row to high.
  rowInHigh();
  rowclk();
  
  //Reset serial in to low (as it should be by default).
  rowInLow();

  return;
}

/*
  Updates the screen with latest data. 
*/
void refresh(bool **screen) { 
  int col, row;
  resetCols();
  for (row = UNIT_WIDTH - 1; row >= 0; row--) { 
    for (col = UNIT_LENGTH - 1; col >= 0; col--) {
      if (screen[row][col] == true) {
        colInHigh();
      } else {
        colInLow();
      }
      colclk();
    }
    
    latch();
    //shift to next row the next time the loop executes.
    rowclk();
    //muxing delay (100 us)
    usleep(100);
  }
  
  return;
}

/*
  Thread function calls refresh indefinitely.
*/
void *render(void *args) {
  ScreenData *arguments = (ScreenData*) arguments;
  bool **screen = arguments -> screen;
  while (1) {
    refresh(screen);
  }

  return NULL;
}

/*
  Initialize all I/O pins.
*/
void initializePins(void) {
  pinMode(ROW, OUTPUT);
  pinMode(COL, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(ROWCLK, OUTPUT);
  pinMode(COLCLK, OUTPUT);
  
  return;
}

/*
  Initialize all threads and call them here.
*/
void init(bool **screen) {  
  pthread_t render_tid;
  ScreenData *tdata = (ScreenData*) malloc(sizeof(ScreenData));
  tdata -> screen = screen;
  pthread_create(&render_tid, NULL, render, tdata);
  //halt execution until called thread terminates.
  pthread_join(render_tid);

  return;
}

/*
  Turn on/off pixel on screen with its location specified by the 
  col and row paramters.
*/
void off(int row, int col, bool **screen) {
  screen[row][col] = false;
  return;
}

void on(int row, int col, bool **screen) {
  screen[row][col] = true;
  return;
}

/*
  Print screen data on console for debugging purposes.
*/
void printTest(bool **screen) {
  int col, row, pixel;
  for (row = 0; row < UNIT_WIDTH; row++) {
    for (col = 0; col < UNIT_LENGTH; col++) {
      pixel = screen[row][col];
      if (pixel == true){
        printf(" #");
      } else {
        printf("  ");
      }
    }
    printf("\n");
  }
  return;
}

int main(void) {
  wiringPiSetup();
  initializePins();
  bool **screen;
  int col, row;
  screen = (bool**) malloc(UNIT_WIDTH * sizeof(bool*));
  for (row = 0; row < UNIT_WIDTH; row++) {
    screen[row] = (bool*) malloc(UNIT_LENGTH * sizeof(bool));
    for (col = 0; col < UNIT_LENGTH; col++) {
      screen[row][col] = false;
    }
  }
  
  init(screen);
  free(screen);
  return 0;
}
