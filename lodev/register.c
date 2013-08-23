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

#define PAUSE 25

void pause() {
  int bogomips = 500; //approx bogomips of the processor
  volatile unsigned long int loops = PAUSE * bogomips * 0.001;
  for (loops; loops > 0; loops--);
  return;
}

/*
  Clock in data to the column (sinking) registers.
  Hardware: TPIC6B595N acting as sink.
  25ns delay should be safe; GPIO timeout + 25ns = ~100ns. 
	Minimum delays can be found on datasheets.
*/
void colclk(void) {
  digitalWrite(COLCLK, HIGH);
  pause(); 
  digitalWrite(COLCLK, LOW);
  return;
}

/*
  Clock in data to the row (sourcing) registers
  Hardware: TPIC6B595N in combination with transistors to source current.
*/
void rowclk(void) {
  digitalWrite(ROWCLK, HIGH);
  pause();
  digitalWrite(ROWCLK, LOW);
  return;
}

/*
  Latches both row and column registers.
*/
void latch(void) {
  digitalWrite(LATCH, HIGH); 
	//nanosleep(&nanospec, NULL);
  pause();
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

