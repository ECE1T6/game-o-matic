#ifndef TETRIS_GUARD
#define TETRIS_GUARD

#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
//#include <tetrominoes.h>
/*Windows: (for testing only)*/
//#include <windows.h>
//#include <conio.h>
/*Linux:*/
#include <unistd.h>
extern const bool allTetrominoes[7][8][8];

void tetrisSinglePlayer(bool** ledArray);
void tetrisTwoPlayer(bool** ledArray); 

#endif
