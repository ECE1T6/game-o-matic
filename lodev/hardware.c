
/*
  NOTE: Rough draft - does not follow spec yet.
*/

//getControl.c -- spawns a thread and checks current state of game controllers.

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

//#### PIN and CONST. DECLARATIONS ####
#define RESET 8
//player 1
#define JOY1_UP 5
#define	JOY1_RIGHT 6
#define JOY1_DOWN 7
#define JOY1_LEFT 10 
#define BUTTON1 17
#define BUTTON2 18

//player 2
#define JOY2_UP 11
#define	JOY2_RIGHT 12
#define JOY2_DOWN 13
#define JOY2_LEFT 14
#define BUTTON3 19
#define BUTTON4 20


struct ctlData {
		int joy1;
		int joy2;
		bool button1;
		bool button2;
		bool button3;
		bool button4;
		bool reset;
	};

void getControl(struct ctlData *ctl){
 //code for grabbing positions from GPIO ports

	//Joystick 1
	if (digitalRead(JOY1_UP)){
		if (digitalRead(JOY1_LEFT)) 
			ctl->joy1 = 8;
		else if (digitalRead(JOY1_RIGHT))
			ctl->joy1 = 2;
		else 
			ctl->joy1 = 1;
	}
	else if (digitalRead(JOY1_RIGHT)){
		if (digitalRead(JOY1_DOWN)) 
			ctl->joy1 = 4;
		else
			ctl->joy1 = 3;
	}
	else if (digitalRead(JOY1_DOWN)){
		if (digitalRead(JOY1_LEFT)) 
			ctl->joy1 = 6;
		else
			ctl->joy1 = 5;
	}
	else if (digitalRead(JOY1_LEFT)){ 
			ctl->joy1 = 7;
	}

	//Joystick 2
	if (digitalRead(JOY2_UP)){
		if (digitalRead(JOY2_LEFT)) 
			ctl->joy2 = 8;
		else if (digitalRead(JOY2_RIGHT))
			ctl->joy2 = 2;
		else 
			ctl->joy2 = 1;
	}
	else if (digitalRead(JOY2_RIGHT)){
		if (digitalRead(JOY2_DOWN)) 
			ctl->joy2 = 4;
		else
			ctl->joy2 = 3;
	}
	else if (digitalRead(JOY2_DOWN)){
		if (digitalRead(JOY2_LEFT)) 
			ctl->joy2 = 6;
		else
			ctl->joy2 = 5;
	}
	else if (digitalRead(JOY2_LEFT)){ 
			ctl->joy2 = 7;
	}

	//Buttons
	if (digitalRead(BUTTON1))
		ctl->button1 = true;
	if (digitalRead(BUTTON2))
		ctl->button2 = true;
	if (digitalRead(BUTTON3))
		ctl->button3 = true;
	if (digitalRead(BUTTON4))
		ctl->button4 = true;
	if (digitalRead(RESET))
		ctl->reset = true;
	
	return;
}

void *controllerImplement(void *vptr_value) {
	wiringPiSetup();
	for (int i = 5; i<=20; i++){
	pinMode(i, INPUT);
	}
	struct ctlData *ctl = (struct ctlData*) vptr_value;
	while(1) {
		getControl(ctl);
		usleep(2000); //sleep a bit before next poll -- 2000 = 500Hz
	}
}

int main(struct ctlData *ctl, pthread_t *tid){//struct of controller parameters, tid pointer
	int ran = pthread_create(tid, NULL, controllerImplement, (void *) ctl);
	return ran; // if value is <0, pthread_create failed.
}
