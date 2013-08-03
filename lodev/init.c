#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "init.h"
#include "render.h"
#include "screen.h"

/*
  Initialize all threads and call them here.
*/
void init(bool **screen) {  
  pthread_t render_tid;
  ScreenData *tdata = (ScreenData*) malloc(sizeof(ScreenData));
  tdata -> screen = screen;
  pthread_create(&render_tid, NULL, render, tdata);
  //halt execution until called thread terminates.
  pthread_join(render_tid, NULL);
  free(tdata);
  return;
}
