//printScreen.c -- prints all required pixels onto a 64x48 screen. Scans in cols from left/top to right/bottom.

#include <stdio.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <unistd.h>
#include <pthread.h>

void xClock(void) {
	digitalWrite(3, HIGH); //3 = "pin three" on RasPi --> x-"clock" pin
	digitalWrite(3, LOW);
	return;
}

void yClock(void) {
	digitalWrite(4, HIGH); //4 = "pin four" on RasPi --> y-"clock" pin
	digitalWrite(4, LOW);
	return;
}

void outputToScreen(void) {
	digitalWrite(2, HIGH); //2 = "pin two" on RasPi --> x- & y-"latch" pin set
	digitalWrite(2, LOW);
}

void flushAllRegisters(void) { //clears all data from shift registers (but doesn't show this on screen)
	int y = 0, x = 0; //x=cols,y=rows
	digitalWrite(0, LOW); //0 = "pin zero" on RasPi --> x-"data" pin
	digitalWrite(1, LOW); //1 = "pin one" on Raspi --> y-"data" pin
	for(x = 0; x <= 64; x++) {
		yClock();
		xClock();
	}
	return;
}

void flushRowRegisters(void) { //empties row registers (doesn't show on screen) -- typically not needed
	int y = 0; //x=cols,y=rows
	digitalWrite(1, LOW); //1 = "pin one" on Raspi --> y-"data" pin
	for(y = 0; y <= 64; y++){
		yClock();
	}
	return;
}


void printScreen(bool matrixPtr[48][64]){//scans downward, across screen ONE FULL TIME.
	for(int x = 47; x >= 0; x-8) { //making assumption of matrix form matrixPtr[x][y]
		for(int y = 63;y >= 0; y--){
			digitalWrite(1, matrixPtr[x][y]); //1 = "pin one" on Raspi --> y-"data" pin
			yClock();
		}
		if(x==0){ 
			for (int i = 0; i<2; i++){
				digitalWrite(0, HIGH); //0 = "pin zero" on RasPi --> x-"data" pin
				xClock();
				digitalWrite(0, LOW);
				for (int z=0;z<8;z++) xClock();
			}
			digitalWrite(0, HIGH);
			xClock();
			digitalWrite(0, LOW);
		}
		else xClock(); //shifts the data over to make sure the proper column is lit
		outputToScreen();
		usleep(520); // leaves the screen on for a while before the next line is lit: 520us = ~120fps for 48x64 matrix 
	}
return;
}

void *printScreenImplement(void *vptr_value){//matrixPtr points to a bool 8x8 2-d array.
	wiringPiSetup();
	for (int i = 0; i<=4; i++){
	pinMode(i, OUTPUT);
	}
	bool matrixPtr[48][64];
	matrixPtr[0][0] = (bool*) vptr_value;
	flushAllRegisters(); 
	while(1) {
		printScreen(matrixPtr);
	}
}

int main(bool matrixPtr){//matrixPtr points to a bool 64x48 2-d array. Points containing true interpreted on, false is off.
	pthread_t tid;
	pthread_create(&tid, NULL, printScreenImplement, (void *) matrixPtr);
	return tid;
}
