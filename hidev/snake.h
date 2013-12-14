#ifndef SNAKE_GUARD
#define SNAKE_GUARD

#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
//Windows (for testing only):
//#include <windows.h>
//#include <conio.h>
//Linux:
#include <unistd.h>

void snakeTwoPlayer(bool** ledArray);
void snakeSinglePlayer(bool** ledArray);
#endif
