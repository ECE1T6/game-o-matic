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
  Clock in data to the column SN74 registers.
  100ns delay should be safe; minimum delays can be found on datasheets.
*/
void colclk(void) {
  digitalWrite(COLCLK, HIGH);
  nanosleep(100); 
  digitalWrite(COLCLK, LOW);
  return;
}

/*
  Clock in data to the row TPIC registers
*/
void rowclk(void) {
  digitalWrite(ROWCLK, HIGH);
  nanosleep(100);
  digitalWrite(ROWCLK, LOW);
  return;
}

/*
  Latches both row and column registers.
*/
void latch(void) {
  digitalWrite(LATCH, HIGH); 
  nanosleep(100);
  digitalWrite(LATCH, LOW);
  return;
}

/*
  Setters for serial data in of row and column registers.
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

  for (row = 0; row < UNIT_LENGTH; row++) {
    rowclk();
  }

  for (col = 0; col < UNIT_WIDTH; col++) {
    colclk();
  }
  
  return;
}

/*
  Sets the data of the column data pins to low, with the exception of
  the leading data pin, which is set to high.
*/
void resetCols(void) {
  int col
  colInLow();
  for (int col = UNIT_WIDTH -1; col > 0; col--) {
    colclk();
  }
  colInHigh();
  colclk();
  colInLow();

  return;
}

/*
  Updates the screen with latest data. 
*/
void refresh(bool **screen) { 
  int col, row;
  resetCols();
  for (col = UNIT_WIDTH - 1; col >= 0; col--) { 
    for (row = UNIT_LENGTH - 1; row >= 0; row--) {
      if (screen[col][row] == true) {
        rowInHigh();
      } else {
        rowInLow();
      }
      rowclk();
    }
    
    latch();
    colclk(); //shifts the data over to make sure the proper column is lit
    usleep(100);  //delay
  }
  return;
}

/*
  Thread function calls refresh indefinitely.
*/
void *render(void *args) {
  ScreenData *args = (ScreenData*) args;
  bool **screen = args -> screen;
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
  pthread_join(render_tid);

  return;
}

/*
  Turn on/off pixel on screen with its location specified by the 
  col and row paramters.
*/
void off(int col, int row, bool **screen) {
  screen[col][row] = false;
  return;
}

void on(int col, int row, bool **screen) {
  screen[col][row] = true;
  return;
}

/*
  Print screen data on console for debugging purposes.
*/
void printTest(bool **screen) {
  int col, row, pixel;
  for (col = 0; col < UNIT_WIDTH; col++) {
    for (row = 0; row < UNIT_LENGTH; row++) {
      pixel = screen[col][row];
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
  for (col = 0; col < UNIT_WIDTH; col++) {
    screen[col] = (bool*) malloc(UNIT_LENGTH * sizeof(bool));
    for (row = 0; row < UNIT_LENGTH; row++) {
      screen[col][row] = false;
    }
  }
  /*initialization end*/
  init(screen);
  free(screen);
  return 0;
}
