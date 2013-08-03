#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <time.h>

#include "register.h"

/*
  Pinout map on Raspberry Pi
*/
#define COL 0
#define ROW 1
#define LATCH 2
#define COLCLK 3
#define ROWCLK 4

/*
  nanosleep parameter spec
*/
typedef struct timespec Timespec;

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
  Initialize all I/O pins.
*/
void initializePins(void) {
  wiringPiSetup();
  pinMode(ROW, OUTPUT);
  pinMode(COL, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(ROWCLK, OUTPUT);
  pinMode(COLCLK, OUTPUT);
  
  return;
}

