#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "register.h"
#include "render.h"
#include "screen.h"

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
  for (row = UNIT_WIDTH - 1; row > 0; row--) {
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
  resetRows();
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
  ScreenData *arguments = (ScreenData*) args;
  bool **screen = arguments -> screen;
  while (1) {
    refresh(screen);
  }

  return NULL;
}

