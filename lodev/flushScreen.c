//flushScreen.c -- blanks all pixels on a 64x48 screen. Scans from left/top to right/bottom.

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#define	COL 0
#define ROW 1
#define LATCH 2
#define COLCLK 3
#define ROWCLK 4
#define ARRAY_HEIGHT  48 //change these to the matrix size
#define ARRAY_WIDTH  64

void yClock(void) {
	digitalWrite(COLCLK, HIGH);
	digitalWrite(COLCLK, LOW);
	return;
}

void xClock(void) {
	digitalWrite(ROWCLK, HIGH);
	digitalWrite(ROWCLK, LOW);
	return;
}

void outputToScreen(void) {
	digitalWrite(LATCH, HIGH); 
	digitalWrite(LATCH, LOW);
	return;
}

void flushScreen(void){
	int y=0,x=0; //x=cols,y=rows
	digitalWrite(ROW,LOW); //0 = "pin zero" on RasPi --> x-"data" pin
	digitalWrite(COL,LOW); //1 = "pin one" on Raspi --> y-"data" pin
	for(x=0;x<ARRAY_WIDTH;x++){ //assumes array is strictly wider or as wide as tall
		xClock();
		yClock();
	}
	outputToScreen();
return;
}

void main(void){
wiringPiSetup();
flushScreen();
return;
}
