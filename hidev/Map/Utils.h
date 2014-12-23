

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>


typedef unsigned char byte;

#define ABS(x) (((x) == 0.0)?(0.0):(((x)>0.0)?(x):(-(x))))
#define SWAP(x, y) \
	do { \
		(x) = (x)^(y); \
		(y) = (x)^(y); \
		(x) = (x)^(y); \
	} while(0)
#define MAX(x, y) (((x)>(y))?(x):(y))
#define MIN(x, y) (((x)<(y))?(x):(y))

typedef union {
	struct{
		unsigned char c0;
		unsigned char c1;
	};
	short i;
} ShortRead;


typedef union {
	struct{
		unsigned char c0;
		unsigned char c1;
		unsigned char c2;
		unsigned char c3;
	};
	int i;
} IntRead;


// Big endians
inline short ReadShort(FILE* fp){
	int i;
	ShortRead sr;
	sr.c1 = fgetc(fp);
	sr.c0 = fgetc(fp);

	return sr.i;
	
}

inline int ReadInt(FILE* fp){
	int i;
	IntRead ir;
	ir.c3 = fgetc(fp);
	ir.c2 = fgetc(fp);
	ir.c1 = fgetc(fp);
	ir.c0 = fgetc(fp);

	return ir.i;
	
}

// Little endians

inline short ReadShortLE(FILE* fp){
	int i;
	ShortRead sr;
	sr.c0 = fgetc(fp);
	sr.c1 = fgetc(fp);

	return sr.i;
	
}

inline int ReadIntLE(FILE* fp){
	int i;
	IntRead ir;
	ir.c0 = fgetc(fp);
	ir.c1 = fgetc(fp);
	ir.c2 = fgetc(fp);
	ir.c3 = fgetc(fp);

	return ir.i;
	
}



#endif


