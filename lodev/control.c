
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
//player 1
#define JOY1_UP 5
#define	JOY1_RIGHT 6
#define JOY1_DOWN 7
#define JOY1_LEFT 10 
#define LEFT1 17
#define RIGHT1 18

//player 2
#define JOY2_UP 11
#define	JOY2_RIGHT 12
#define JOY2_DOWN 13
#define JOY2_LEFT 14
#define LEFT2 19
#define RIGHT2 20


struct ctlData {
		unsigned char player1;
		unsigned char player2;
	};


void getControl(struct ctlData *ctl){
 //code for grabbing positions from GPIO ports

	//Player 1
	ctl->player1 = digitalRead(JOY1_LEFT); //assign to value, to reset player
	ctl->player1 = ctl->player1 << 1;
	ctl->player1 += digitalRead(JOY1_DOWN) << 0;
	ctl->player1 = ctl->player1 << 1;
	ctl->player1 += digitalRead(JOY1_RIGHT) << 0;
	ctl->player1 = ctl->player1 << 1;
	ctl->player1 += digitalRead(JOY1_UP) << 0;
	ctl->player1 = ctl->player1 << 1;
	ctl->player1 += digitalRead(LEFT1) << 0;
	ctl->player1 = ctl->player1 << 1;
	ctl->player1 += digitalRead(RIGHT1);
	
	//Joystick 2
	ctl->player2 = digitalRead(JOY1_LEFT); //assign to value, to reset player
	ctl->player2 = ctl->player2 << 1;
	ctl->player2 += digitalRead(JOY1_DOWN) << 0;
	ctl->player2 = ctl->player2 << 1;
	ctl->player2 += digitalRead(JOY1_RIGHT) << 0;
	ctl->player2 = ctl->player2 << 1;
	ctl->player2 += digitalRead(JOY1_UP) << 0;
	ctl->player2 = ctl->player2 << 1;
	ctl->player2 += digitalRead(LEFT1) << 0;
	ctl->player2 = ctl->player2 << 1;
	ctl->player2 += digitalRead(RIGHT1);

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
