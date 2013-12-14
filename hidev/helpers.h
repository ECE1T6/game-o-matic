// Consts and Definitions
#ifndef HELPERS_GUARD
#define HELPERS_GUARD

/*Global variables used by all games:*/
#define ARRAY_HEIGHT 38.0;
#define ARRAY_WIDTH 76.0;

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

void freezeFrame(unsigned int frames);

void printScore(int score, char *position);

bool** make2DArray(int height, int width);

void fill2DArray(bool** ledArray, int height, int width, bool lightsOn);

void free2DArray(bool** ledArray, int height);

void drawRectangle(bool** ledArray, bool lightsOn, int topY, int leftX, int HEIGHT, int WIDTH);

int getJoystick(unsigned char player);

bool getLeftButton(unsigned char player);

bool getRightButton(unsigned char player);

int* make1DArray(int LENGTH);

void drawCheckerboard(bool** ledArray, int topY, int leftX, int HEIGHT, int WIDTH);

void printTest(bool** ledArray, float topMargin, float leftMargin, float botEnd, float rightEnd);

void frameTest(bool** ledArray, float topMargin, float leftMargin, float botEnd, float rightEnd);
#endif
