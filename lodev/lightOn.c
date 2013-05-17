//lightOn.c -- turns on a pixel on a 64x48 screen. Scans from left/top to right/bottom.

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

int lightOn(int col, int row){
	int y=0,x=0; //x=cols,y=rows
	for(x=0;x<=64;x++&&y++){
		if(x==col)digitalWrite(0,HIGH); //0 = "pin zero" on RasPi --> x-"data" pin
		else digitalWrite(0,LOW);
		if(y==row)digitalWrite(1,HIGH); //1 = "pin one" on Raspi --> y-"data" pin
		else digitalWrite(1,LOW);
		clock();
	} 
	outputToScreen();
	digitalWrite(0,LOW); //reset to base state
	digitalWrite(1,LOW);
return;
}


int main(int col, int row){
wiringPiSetup();
lightOn(col,row);
return;
}
