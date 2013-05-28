//printScreen.c -- prints all required pixels onto a 64x48 screen. Scans in cols from left/top to right/bottom.

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>

int xClock(void) {
	digitalWrite(3, HIGH); //3 = "pin three" on RasPi --> x-"clock" pin
	digitalWrite(3, LOW);
	return;
}

int yClock(void) {
	digitalWrite(4, HIGH); //4 = "pin four" on RasPi --> y-"clock" pin
	digitalWrite(4, LOW);
	return;
}

int outputToScreen(void) {
	digitalWrite(2, HIGH); //2 = "pin two" on RasPi --> x- & y-"latch" pin set
	digitalWrite(2, LOW);
}

int flushAllRegisters(void) { //clears all data from shift registers (but doesn't show this on screen)
	int y = 0, x = 0; //x=cols,y=rows
	digitalWrite(0, LOW); //0 = "pin zero" on RasPi --> x-"data" pin
	digitalWrite(1, LOW); //1 = "pin one" on Raspi --> y-"data" pin
	for(x = 0; x <= 64; x++) {
		yClock();
		xClock();
	}
	return;
}

int flushRowRegisters(void) { //clears all data from shift registers (but doesn't show this on screen)
	int y = 0; //x=cols,y=rows
	digitalWrite(1, LOW); //1 = "pin one" on Raspi --> y-"data" pin
	for(y = 0; y <= 64; y++){
		yClock();
	}
	return;
}


int printScreen(bool matrixPtr){//scans downward, across screen ONE FULL TIME.
	for(int x = 15; x >= 0; x--) { //making assumption of matrix form matrixPtr[x][y]
		flushRowRegisters();
		for(int y = 63;y >= 0; y--) {
			if (matrixPtr[(x+32)][y] == "true") {
				digitalWrite(1, HIGH); //1 = "pin one" on Raspi --> y-"data" pin
			}
			else digitalWrite(1, LOW);
			yClock();
		}
		for(int y = 63;y >= 0; y--) {
			if (matrixPtr[(x+16)][y] == "true") {
				digitalWrite(1, HIGH); 
			}
			else digitalWrite(1, LOW);
			yClock();
		}
		for(int y = 63;y >= 0; y--) {
			if (matrixPtr[x][y] == "true") {
				digitalWrite(1, HIGH);
			}
			else digitalWrite(1, LOW);
			yClock();
		}
		if(x==0){ 
			for (int i = 0; i<2; i++){
				digitalWrite(0, HIGH); //0 = "pin zero" on RasPi --> x-"data" pin
				xClock();
				digitalWrite(0, LOW);
				for (z=0;z<16;z++) xClock();
			}
			digitalWrite(0, HIGH);
			xClock();
			digitalWrite(0, LOW);
		}
		else xClock(); //shifts the data over to make sure the proper column is lit
		outputToScreen();
		usleep(260); // leaves the screen on for a while before the next line is lit = 60fps
	}
	
	return;
}

void *printScreenImplement(void *vptr_value){//matrixPtr points to a bool 8x8 2-d array.
	wiringPiSetup();
	bool matrixPtr[48][64];
	matrixPtr[0][0] = (bool*) vptr_value;
	flushAllRegisters(); 
	while(1) {
		//printf("poops");//uncomment this to verify if the pthread is being created
		printScreen(matrixPtr);
	}
	return;
}

int main(bool matrixPtr){//matrixPtr points to a bool 64x48 2-d array. Points containing true interpreted on, false is off.
	pthread_t tid;
	pthread_create(&tid, NULL, printScreenImplement, (void *) array);
	wiringPiSetup();
	flushAllRegisters(); 
	while(1) {
		printScreen(matrixPtr);
	}
	return;
}
