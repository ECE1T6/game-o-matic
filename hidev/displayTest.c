#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <wiringPi.h>
#include <pthread.h>
//#include <printScreen.h> //custom lodev library
/*Every other size or position value is based on the following. We might add or remove some as we write the program.*/
#define ARRAY_HEIGHT  48
#define ARRAY_WIDTH  64
#define TOP_MARGIN  0.0 /*The margins bound the playable space in the array -- they might hold things like score*/
#define BOTTOM_MARGIN  0.0
#define LEFT_MARGIN  0.0
#define RIGHT_MARGIN  0.0
#define DISTANCE_PER_REFRESH  1.0

#define BOTTOM_END  ARRAY_HEIGHT-BOTTOM_MARGIN-1
#define RIGHT_END  ARRAY_WIDTH-RIGHT_MARGIN-1

//### BEGINNING OF printScreen LIBRARY ####
//printScreen.c -- prints all required pixels onto a 64x48 screen. Scans in cols from left/top to right/bottom.

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
	return;
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

void flushRowRegisters(void) { //clears all data from shift registers (but doesn't show this on screen)
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
		usleep(520); // leaves the screen on for a while before the next line is lit = 60fps
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

int MainScreen(bool matrixPtr){//matrixPtr points to a bool 64x48 2-d array. Points containing true interpreted on, false is off.
	pthread_t tid;
	pthread_create(&tid, NULL, printScreenImplement, (void *) matrixPtr);
	return tid;
}

//##### END OF printScreen LIBRARY ####

void cleanArray(int x, int y, bool array[ARRAY_HEIGHT][ARRAY_WIDTH]){
	array[x][y] = false;
	return;
}

void updateArray(int x, int y, bool array[ARRAY_HEIGHT][ARRAY_WIDTH]){
	array[x][y] = true;
	return;
}

void print_test(bool array[ARRAY_HEIGHT][ARRAY_WIDTH]){
    int i, j;
    for(i = 0; i < ARRAY_HEIGHT; i++){
        for(j = 0; j < ARRAY_WIDTH; j++){
            if(array[i][j] == true){
                printf(" #", array[i][j]);
            }
            else{
                printf("  ", array[i][j]);
            }
        }
        printf("\n");
    }
 return;
}


int main (void){
/*initialization start*/
	wiringPiSetup();	//
	bool array[ARRAY_HEIGHT][ARRAY_WIDTH] = {false};
	int x=0, y=0;
/*initialization end*/
	int tid=MainScreen(array);
	while (1){
	cleanArray(y,x,array);
	if (x==63 && y==47){x=0;y=0;}
	else if (x==63 && y<47){x=0;y++;}
	else if (x<63)x++;
	updateArray(y,x,array);
        //print_test(array);
	usleep(35000);
	//system("clear");
	}
	return 0;
}
