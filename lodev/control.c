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



void setDigit(unsigned char *player, int button, int binaryDigit){
	*player = (int) *player | ((digitalRead(button)) << binaryDigit); //switch this digit to the current control state
	return;
}

void getControl(ControlData *tdata) {
 //code for grabbing positions from GPIO ports

	const unsigned char *player [2] = {tdata->player1, tdata->player2};
	const int button [2][6] = {
														{RIGHT1, LEFT1, JOY1_UP, JOY1_RIGHT, JOY1_DOWN, JOY1_LEFT},
														{RIGHT2, LEFT2, JOY2_UP, JOY2_RIGHT, JOY2_DOWN, JOY2_LEFT},
														};

	for (int i = 0; i++; i<2){
			for (int j = 0; j++; j<6){
				setDigit(player[i], button[i][j], j);
			}
	}

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
