//flushScreen.c -- blanks all pixels on a 64x48 screen. Scans from left/top to right/bottom.

#include <stdio.h>
#include <wiringPi.h>

int xClock(void){
	digitalWrite(3,HIGH); //3 = "pin three" on RasPi --> x-"clock" pin
	digitalWrite(3,LOW);
return;
}

int yClock(void){
	digitalWrite(4,HIGH); //4 = "pin four" on RasPi --> y-"clock" pin
	digitalWrite(4,LOW);
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
		xClock();
		yClock();
	}
	outputToScreen();
return;
}


int main(void){
wiringPiSetup();
for (int i = 0; i<=4; i++){
	pinMode(i, OUTPUT);
	}
flushScreen();
return;
}
