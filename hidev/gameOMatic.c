#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hidev.h" //custom library
#include "lodev.h" //custom library

//###### GLOBAL DEFINITIONS ####
#define _GNU_SOURCE
#define ARRAY_HEIGHT  48 //change these to the matrix size
#define ARRAY_WIDTH  64
#define TWEET_FILE /home/pi/tweets.txt //change this to the file where tweets are being stored

void main(void){

//### BEGINNING OF INITIALIZATION ####
//system call to start the twitter getting program
	system("java getTweets -Xms10M -Xmx40M -jar /home/pi/getTweets.jar file=TWEET_FILE"); 

//malloc the display array
	bool **screen;
		screen = (bool**) malloc(ARRAY_HEIGHT*sizeof(bool*));
		for  (int i=0; i<ARRAY_HEIGHT; i++){
			screen[i] = (bool*) malloc (ARRAY_WIDTH*sizeof(bool));
		}
		for (int i=0; i<ARRAY_HEIGHT; i++){
			for (int j=0; j<ARRAY_WIDTH; j++){
				screen[i][j] = false;
			}
		}

//Declare tids
	pthread_t *screenTid, *ctlTid;

//spawn thread for running the display
	int screenRun = -1;
	for (int i = 0; i < 3 && screenRun < 0; i++)
		screenRun = printScreen(screen, screenTid); //return value of <0 means failure

//define and malloc controller data struct
	struct ctlData {
		int joy1;
		int joy2;
		bool button1;
		bool button2;
		bool button3;
		bool button4;
	};

	struct ctlData *ctl;
	ctl = (struct *ctlData) malloc(sizeof(struct ctlData));

//spawn thread for getting controller data
	int ctlRun = -1;
	for (int i = 0; i < 3 && ctlRun < 0; i++)
		ctlRun = controller(ctl, ctlTid); //return value of <0 means failure
	
//### END INITIALIZATION ###

while(*reset == false){
//display menu for games selection and tweets
		while(ctl.button1 != true && ctl.button2 != true){
			//display dem tweets and dem menus
		}
//when a selection is made: (BEWARE! pseudo-code galore!)
		stopDisplayingTweets();
		stopDisplayingMenu();
		blankArray(screen, ARRAY_HEIGHT, ARRAY_WIDTH);
		if (selection == 1) //start pong
			pong(screen, ctl); 
		else if (selection == 2) //start snake
			snake(screen, ctl);
		blankArray(screen, ARRAY_HEIGHT, ARRAY_WIDTH);
	}
//begin shutdown routine
	//unmalloc all the things that were malloc'd!
	//exit or kill all threads
	pthread_join(ctlTid, NULL);
	pthread_join(screenTid, NULL);
	system("sudo restart"); // or simply just restart the program?
return;
}

