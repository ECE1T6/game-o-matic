//printScreen.c -- prints/mux's all required pixels on an LED screen. Scans in rows from top to bottom.

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

//#### PIN and CONST. DECLARATIONS ####

struct ctlData {
		int vert1;
		int hor1;
		int vert2;
		int hor2;
		bool buttonHit1;
		bool buttonHit2;
	};

void getControl(struct ctlData *ctl){
 //code for grabbing positions from USB ports
	return;
}

void *controllerImplement(void *vptr_value) {
	//wiringPiSetup();
	//for (int i = 0; i<=4; i++){
	//pinMode(i, OUTPUT);
	//}
	struct ctlData *ctl = (struct ctlData*) vptr_value;
	while(1) {
		getControl(ctl);
		usleep(250); //sleep a bit before next poll
	}
}

int main(struct ctlData *ctl, pthread_t *tid){//struct of controller parameters, tid pointer
	int ran = pthread_create(tid, NULL, controllerImplement, (void *) ctl);
	return ran; // if value is <0, pthread_create failed.
}
