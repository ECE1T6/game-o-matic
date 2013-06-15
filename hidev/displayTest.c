#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <wiringPi.h>
#include <pthread.h>

#define ARRAY_HEIGHT  48 //change these to the matrix size
#define ARRAY_WIDTH  64

//#### PIN DECLARATIONS ####
#define	COL 0
#define ROW 1
#define LATCH 2
#define COLCLK 3
#define ROWCLK 4

//#### BEGINNING OF printScreen LIBRARY ####
//Scans in rows from top-to-bottom.

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
	digitalWrite(ROW, LOW); //push in x-data
	for(x = 0; x <= ARRAY_WIDTH; x++){
		xClock();
	}
	return;
}

//scans downward, across screen ONE FULL TIME.
void printScreen(bool (**matrix)){	
   for(int y = ARRAY_HEIGHT-1; y >= 0; y--) { 
		for(int x = ARRAY_WIDTH-1;x >= 0; x--){
			digitalWrite(ROW, (matrix[y][x])); //push in x-data
			xClock();
		}
		if(y==0){ 
			for (int i = 1; i<((ARRAY_HEIGHT+7)/8); i++){//used for daisychaining row registers
				digitalWrite(COL, HIGH); //push in y-data
				yClock();
				for (int z=0;z<8;z++) yClock();
			}
			digitalWrite(COL, HIGH);
			yClock();
			digitalWrite(COL, LOW);
		}
		else yClock(); //shifts the data over to make sure the proper column is lit
		outputToScreen();
		// leave the screen on for a while before the next line is lit; 1040uS = ~120fps 
    usleep(520); 
  }
	return;
}

void print_test(bool (**array)){
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

//matrix points to a bool 8x8 2-d array.
void *printScreenImplement(void *vptr_value) {
	wiringPiSetup();
	for (int i = 0; i<=4; i++){
	pinMode(i, OUTPUT);
	}
	bool (**matrix) = (bool (**)) vptr_value;
	flushAllRegisters(); 
	while(1) {
		//print_test(matrix);
		printScreen(matrix);
		//system("clear");
	}
}

//matrix points to a bool 2-d array. 
//Points containing true interpreted on, false is off.

int MainScreen(bool (**matrix)){
  pthread_t tid;
	pthread_create(&tid, NULL, printScreenImplement, (void *) matrix);
	return tid;
}

//##### END OF printScreen LIBRARY ####

void cleanArray(int y, int x, bool (**array)){
	array[y][x] = false;
	return;
}

void updateArray(int y, int x, bool (**array)){
	array[y][x] = true;
	return;
}

int main (void){
  /*initialization start*/
	wiringPiSetup();	//
	bool **array;
	array = (bool**) malloc(ARRAY_HEIGHT*sizeof(bool*));
	for  (int i=0; i<ARRAY_HEIGHT; i++){
		array[i] = (bool*) malloc (ARRAY_WIDTH*sizeof(bool));
	}
	for (int i=0; i<ARRAY_HEIGHT; i++){
		for (int j=0; j<ARRAY_WIDTH; j++){
			array[i][j] = false;
		}
	}
	int x=0, y=0;
  /*initialization end*/
	int tid = MainScreen(array);
	while (1){
    cleanArray(x,y,array);
    if (y==ARRAY_HEIGHT-1 && x==ARRAY_WIDTH-1){x=0;y=0;}
    else if (y == ARRAY_HEIGHT-1 && x < ARRAY_WIDTH-1){y=0;x++;}
    else if (y<ARRAY_HEIGHT-1) y++;
    updateArray(x,y,array);
   // print_test(array);
    usleep(35000);
   // system("clear");		
}
	return 0;
}
