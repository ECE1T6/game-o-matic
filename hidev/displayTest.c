#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <wiringPi.h>
#include <pthread.h>
//#include <printScreen.h> //custom lodev library
/*Every other size or position value is based on the following. We might add or remove some as we write the program.*/
#define ARRAY_HEIGHT  8
#define ARRAY_WIDTH  64
#define TOP_MARGIN  0.0 /*The margins bound the playable space in the array -- they might hold things like score*/
#define BOTTOM_MARGIN  0.0
#define LEFT_MARGIN  0.0
#define RIGHT_MARGIN  0.0
#define DISTANCE_PER_REFRESH  1.0

#define BOTTOM_END  ARRAY_HEIGHT-BOTTOM_MARGIN-1
#define RIGHT_END  ARRAY_WIDTH-RIGHT_MARGIN-1

//### BEGINNING OF printScreen LIBRARY ####

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
	for(x = 0; x <= 16; x++) {
		yClock();
		xClock();
	}
	return;
}

int flushRowRegisters(void) { //clears all data from shift registers (but doesn't show this on screen)
	int y = 0; //x=cols,y=rows
	digitalWrite(1, LOW); //1 = "pin one" on Raspi --> y-"data" pin
	for(y = 0; y <= 8; y++){
		yClock();
	}
	return;
}


void printScreen(bool matrixPtr[ARRAY_HEIGHT][ARRAY_WIDTH]){//scans across screen ONE FULL TIME.
	int x, y;
	//printf("poop"); usleep(260); //uncomment this line to verify if the pthread is calling this function correctly
	for(x = 0; x < 8; x++) { //making assumption of matrix form matrixPtr[x][y]
		flushRowRegisters();
		for(y = 0;y < 8; y++) {
			if (matrixPtr[x][y] == true){
				digitalWrite(1, HIGH); //1 = "pin one" on Raspi --> y-"data" pin
			}
			else digitalWrite(1, LOW);
			yClock();
		}
		digitalWrite(0, HIGH); //0 = "pin zero" on RasPi --> x-"data" pin
		if (x != 0) xClock(); //shifts the data over to make sure the proper column is lit
		outputToScreen();
		usleep(260); // leaves the screen on for a while before the next line is lit = 60fps
	}
	
	return;
}
//#### IMPLEMENTATION of printScreen : ###
void *printScreenImplement(void *vptr_value){//matrixPtr points to a bool 8x8 2-d array.
	wiringPiSetup();
	bool matrixPtr[ARRAY_HEIGHT][ARRAY_WIDTH];
	matrixPtr[0][0] = (bool*) vptr_value;
	flushAllRegisters(); 
	while(1) {
		//printf("poops");//uncomment this to verify if the pthread is being created
		printScreen(matrixPtr);
	}
	return;
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
}


int main (void){
/*initialization start*/
	pthread_t tid;
	wiringPiSetup();
	bool array[ARRAY_HEIGHT][ARRAY_WIDTH] = {false};
	int x=0, y=0;
/*initialization end*/
	pthread_create(&tid, NULL, printScreenImplement, (void *) array);
	//while (1){
	x=1;y=1;
	//cleanArray(y,x,array);
	//if (x==48 && y==7){x=0;y=0;}
	//else if (x==48 && y<7){x=0;y++;}
	//else if (x<48)x++;
	updateArray(y,x,array);
       // print_test(array);
	while (1){usleep(35000);
	//system("clear");

	}
	return 0;
}
