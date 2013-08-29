//control.c -- spawns a thread and checks current state of game controllers.

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <stdbool.h>

#include "control.h"

//#### PIN and CONST. DECLARATIONS ####
//Poll sleep. 4000us = 250Hz.
#define SLEEP 4000

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

void getControl(ControlData *tdata) {
 //code for grabbing positions from GPIO ports

	//Player 1
	tdata->player1 = digitalRead(JOY1_LEFT); //assign to value, to reset player
	tdata->player1 = tdata->player1 << 1;
	tdata->player1 += digitalRead(JOY1_DOWN) << 0;
	tdata->player1 = tdata->player1 << 1;
	tdata->player1 += digitalRead(JOY1_RIGHT) << 0;
	tdata->player1 = tdata->player1 << 1;
	tdata->player1 += digitalRead(JOY1_UP) << 0;
	tdata->player1 = tdata->player1 << 1;
	tdata->player1 += digitalRead(LEFT1) << 0;
	tdata->player1 = tdata->player1 << 1;
	tdata->player1 += digitalRead(RIGHT1);
	
	//Joystick 2
	tdata->player2 = digitalRead(JOY1_LEFT); //assign to value, to reset player
	tdata->player2 = tdata->player2 << 1;
	tdata->player2 += digitalRead(JOY1_DOWN) << 0;
	tdata->player2 = tdata->player2 << 1;
	tdata->player2 += digitalRead(JOY1_RIGHT) << 0;
	tdata->player2 = tdata->player2 << 1;
	tdata->player2 += digitalRead(JOY1_UP) << 0;
	tdata->player2 = tdata->player2 << 1;
	tdata->player2 += digitalRead(LEFT1) << 0;
	tdata->player2 = tdata->player2 << 1;
	tdata->player2 += digitalRead(RIGHT1);

	return;
}

void *control(void *args) {
	wiringPiSetup();
	for (int i = 5; i<=20; i++){
	pinMode(i, INPUT);
	}
	ControlData *tdata = (ControlData*) args;
	while(1) {
		getControl(tdata);
		usleep(SLEEP); //sleep a bit before next poll
	}

	return NULL;
}
