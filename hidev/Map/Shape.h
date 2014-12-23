

#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


enum {
	//S_LINE_LIST,
	//S_LINE_STRIP,
	S_LINE_LOOP,
	//S_TRIANGLE_LIST,
	S_TRIANGLE_STRIP,
	//S_TRIANGLE_FAN,

};


typedef struct __Shape{
	int N;
	int type;
	double outline;
	double fill;
	bool transparent;
	float* x;
	float* y;
	float* z;

} Shape;

// TODO: add support for 3D shapes



#endif


