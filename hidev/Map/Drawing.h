

#ifndef __DRAWING_H__
#define __DRAWING_H__


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define DRW_ALLOC_ERROR (-1)
#define DRW_NULLPTR_ERROR (-2)
#define DRW_RANGE_ERROR (-3)


enum {
	//D_LINE_LIST,
	//D_LINE_STRIP,
	D_LINE_LOOP,
	//D_TRIANGLE_LIST,
	D_TRIANGLE_STRIP,
	//D_TRIANGLE_FAN,

};

typedef struct __Drawing {
	int N;
	int type;
	double outline;
	double fill;
	bool transparent;
	float* x;
	float* y;

} Drawing;


int Drawing_Init(Drawing* drawing, float* x, float* y, int N, int type, double outline, double fill, bool transparent_fill);
Drawing* Drawing_Initialize(float* x, float* y, int N, int type, double outline, double fill, bool transparent_fill);

void Drawing_Term(Drawing* drawing);
void Drawing_Terminate(Drawing** drawing);


int Drawing_GetVertices(Drawing* drawing, int x, int y, float ratio, int* x_list, int* y_list);


// counter clockwise
int Drawing_Rotate(Drawing* drawing, float angle);
int Drawing_Scale(Drawing* drawing, float x_factor, float y_factor);
int Drawing_ShearHrzn(Drawing* drawing, float factor);
int Drawing_ShearVert(Drawing* drawing, float factor);
// WARNING: make sure <x_vector, y_vector> is normalized
int Drawing_Reflect(Drawing* drawing, float x_vector, float y_vector);


#endif



