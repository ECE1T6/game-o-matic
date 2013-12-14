#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "lodevInit.h"
#include "render.h"
#include "screen.h"
#include "control.h"

/*
  Initialize all threads and call them here.
*/
void init(bool **screen, unsigned char *player1, unsigned char *player2) { //Don't forget to add in the ControlData 
   pthread_t render_tid, control_tid;
   ScreenData *scrnData = (ScreenData*) malloc(sizeof(ScreenData));
	ControlData *ctlData = (ControlData*) malloc(sizeof(ControlData));
  	scrnData -> screen = screen;
	ctlData -> player1 = player1;
	ctlData -> player2 = player2;
   pthread_create(&render_tid, NULL, render, scrnData);
	pthread_create(&control_tid, NULL, control, ctlData);
   //halt execution until called thread terminates.
   pthread_join(render_tid, NULL);
	pthread_join(control_tid, NULL);
	free(scrnData);
	free(ctlData);

  return;
}
