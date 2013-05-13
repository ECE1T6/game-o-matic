//flushScreen.c -- blanks all pixels on a 64x48 screen. Scans from left/top to right/bottom.

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

int clock(void){
  digitalWrite(3,HIGH); //3 = "pin three" on RasPi --> "clock" pin
	digitalWrite(3,LOW);
return;
}

int outputToScreen(void){
	digitalWrite(2,HIGH); //2 = "pin two" on RasPi --> "latch" pin set
	digitalWrite(2,LOW);
}

int flushScreen(void){
	int y=0,x=0; //x=cols,y=rows
	digitalWrite(0,LOW); //0 = "pin zero" on RasPi --> x-"data" pin
	digitalWrite(1,LOW); //1 = "pin one" on Raspi --> y-"data" pin
	for(x=0;x<=64;x++){
		clock();
	}
	outputToScreen();
return;
}


int main(void){
flushScreen(col,row);
return;
}
