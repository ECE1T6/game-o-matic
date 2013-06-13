//printScreen.c -- prints/mux's all required pixels on an LED screen. Scans in rows from top to bottom.

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

//#### PIN and CONST. DECLARATIONS ####
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

//clears all data from shift registers (but doesn't show this on screen)
void flushAllRegisters(void) { 	
  int y = 0, x = 0; //x=#cols,y=#rows
	digitalWrite(COL, LOW); 
	digitalWrite(ROW, LOW);
  //assuming array is strictly as wide or wider than tall
	for(x = 0; x <= ARRAY_WIDTH; x++) { 
		yClock();
		xClock();
	}
	return;
}

//clears all data from shift registers (but doesn't show this on screen)
void flushRowRegisters(void) { 
	int x = 0; //x=#cols,y=#rows
	digitalWrite(ROW, LOW); //1 = "pin one" on Raspi --> x-"data" pin
	for(x = 0; x <= ARRAY_WIDTH; x++){
		xClock();
	}
	return;
}

//scans downward, across screen ONE FULL TIME.
void printScreen(bool (**matrix)){	
  //making assumption of matrix form matrix[x][y]
  for(int y = ARRAY_HEIGHT-1; y >= 0; y--) { 
		for(int x = ARRAY_WIDTH-1;x >= 0; x--){
			digitalWrite(ROW, (matrix[y][x])); //1 = "pin one" on Raspi --> x-"data" pin
			xClock();
		}
		if(y==0){ 
			for (int i = 0; i<2; i++){
				digitalWrite(COL, HIGH); //0 = "pin zero" on RasPi --> y-"data" pin
				yClock();
				for (int z=0;z<8;z++) yClock();
			}
			digitalWrite(COL, HIGH);
			yClock();
				digitalWrite(COL, LOW);
		}
		else yClock(); //shifts the data over to make sure the proper column is lit
		outputToScreen();
		// leaves the screen on for a while before the next line is lit = ~60fps
    usleep(520); 
  }
	return;
}

//matrix points to a bool 2-d array.
void *printScreenImplement(void *vptr_value) {
	wiringPiSetup();
	for (int i = 0; i<=4; i++){
	pinMode(i, OUTPUT);
	}
	bool (**matrix) = (bool (**)) vptr_value;
	flushAllRegisters(); 
	while(1) {
		printScreen(matrix);
	}
}

int main(bool (**matrix)){//matrixPtr points to a bool 2-d array. Points containing true interpreted on, false is off.
	pthread_t tid;
	pthread_create(&tid, NULL, printScreenImplement, (void *) matrix);
	return tid;
}
