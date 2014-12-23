

#ifndef __BMP_H__
#define __BMP_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "Utils.h"


typedef struct __Bmp{
	int width;
	int height;
	bool** pixels;
} Bmp;


int Bmp_Init(Bmp* bmp, char* filename);
Bmp* Bmp_Initialize(char* filename);

void Bmp_Term(Bmp* bmp);
void Bmp_Terminate(Bmp** bmp);



#endif




