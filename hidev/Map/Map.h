

#ifndef __MAP_H__
#define __MAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "Font.h"
#include "Bmp.h"
#include "Drawing.h"
#include "Shape.h"





#define MAP_ALLOC_ERROR (-1)
#define MAP_NULLPTR_ERROR (-2)
#define MAP_RANGE_ERROR (-3)


#define LEVEL(x) ((byte)((double)((((x)>=0.0&&(x)<=1.0)?(x):(((x)<0.0)?0.0:1.0))*255.0)))
typedef struct __Map {
	int width;
	int height;
	byte** pixels;
} Map;

// For use when Map structure is instantiated on the stack
int Map_Init(Map* map, int width, int height);
// For cleanup with the corresponding Map_Init
void Map_Term(Map* map);

// For use when Map structure is not instantiated
Map* Map_Initialize(int width, int height);
// For cleanup with the corresponding Map_Initialize
void Map_Terminate(Map** map);




//
// Fill the board with 0s
//
// --PARAMS--
// Map* map : pointer to the map structure
//
int Map_Clear(Map* map);
//
// Set the pixel at <x, y> with corresponding brightness level
//
// --PARAMS--
// Map* map : pointer to the map structure
// int x : location
// int y : location
// double brightness : brightness level 0.0 being black and 1.0 being brightest
//
int Map_SetPixel(Map* map, int x, int y, double brightness);
//
// Print the map to standard io
// for debugging only
//
// --PARAMS--
// Map* map : pointer to the map structure
//
int Map_Print(Map* map);




//
// Draws a horizontal line starting from <x_pos, y_pos> to <x_pos + length, y_pos>
// 
// --PARAMS--
// Map* map : pointer to the map structure
// int x_pos : location
// int y_pos : location
// int length : size of line
// double outline :  brightness level 0.0 being black and 1.0 being brightest
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
int Map_DrawHrznLn(Map* map, int x_pos, int y_pos, int length, double outline, bool clip);
//
// Draws a vertical line starting from <x_pos, y_pos> to <x_pos, y_pos + length>
// 
// --PARAMS--
// Map* map : pointer to the map structure
// int x_pos : location
// int y_pos : location
// int length : size of line
// double outline :  brightness level 0.0 being black and 1.0 being brightest
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
int Map_DrawVertLn(Map* map, int x_pos, int y_pos, int length, double outline, bool clip);



//
// Draws an arbitrary line starting from <x0, y0> to <x1, y1>
// 
// --PARAMS--
// Map* map : pointer to the map structure
// int x0 : initial location
// int y0 : initial location
// int x1 : final location
// int y1 : final location
// int length : size of line
// double outline :  brightness level 0.0 being black and 1.0 being brightest
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
int Map_DrawLn(Map* map, int x0, int y0, int x1, int y1,
	double outline, bool clip);
//
// Draws an arc of an ellipse that is centred at <x_centre, y_centre> with x_rad and y_rad as it's two radii
// 
// --PARAMS--
// Map* map : pointer to the map structure
// int x_centre : location
// int y_centre : location
// int x_rad : radius
// int y_rad : radius
// double start_angle : start angle in radians
// double end_angle : end angle in radians
// double outline :  brightness level 0.0 being black and 1.0 being brightest
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
//
// WARNING: make sure the start_angle is smaller than end angle, 
// and make sure they differ by at most 2pi otherwise you may run into weird rendering issues
int Map_DrawArc(Map* map, int x_centre, int y_centre, int x_rad, int y_rad, 
	double start_angle, double end_angle,
	double outline, bool clip);

//
// Draws an aligned rectangle from the vertices <x0, y0>, <x0, y1>, <x1, y1>, and <x1, y0> 
// 
// --PARAMS--
// Map* map : pointer to the map structure
// int x0 : location
// int y0 : location
// int x1 : location
// int y1 : location
// double outline :  brightness level 0.0 being black and 1.0 being brightest
// double fill :  brightness level for inside the object
// bool transparent_fill: when true, the object is not filled, and it is see through. enable if performance is an issue
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
int Map_DrawRect(Map* map, int x0, int y0, int x1, int y1, 
	double outline, double fill, bool transparent_fill, bool clip);
//
// Draws an aligned ellipse with the centre <x_centre, y_centre> and radii x_rad and y_rad
// 
// --PARAMS
// Map* map : pointer to the map structure
// int x_centre : location
// int y_centre : location
// int x_rad : radius
// int y_rad : radius
// double outline :  brightness level 0.0 being black and 1.0 being brightest
// double fill :  brightness level for inside the object
// bool transparent_fill: when true, the object is not filled, and it is see through. enable if performance is an issue
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
int Map_DrawElps(Map* map, int x_centre, int y_centre, int x_rad, int y_rad, 
	double outline, double fill, bool transparent_fill, bool clip);
//
// Draws a polygon with vertices SUM <x_vertex_i, y_vertex_i>
// 
// --PARAMS
// Map* map : pointer to the map structure
// int* x_vertex : list of locations
// int* y_vertex : list of locations
// int Num_vertex : number of vertices in the list
// double outline :  brightness level 0.0 being black and 1.0 being brightest
// double fill :  brightness level for inside the object
// bool transparent_fill: when true, the object is not filled, and it is see through. enable if performance is an issue
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
int Map_DrawPlgn(Map* map, int* x_vertex, int* y_vertex, int Num_vertex, 
	double outline, double fill, bool transparent_fill, bool clip);

//
// Draws a 2D Drawing object centred at <x, y>
// 
// --PARAMS
// Map* map : pointer to the map structure
// Drawing* drawing : pointer to the drawing
// int x : location
// int y : location
// double ratio : what a 1.0 in the drawing represents. e.g. an object with vertex <1.0, 0.5> represents <6, 3> when ratio is 6
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
int Map_AddDrawing(Map* map, Drawing* drawing, int x, int y, double ratio, bool clip);
//
// Draws a Bitmap
// 
// --PARAMS
// Map* map : pointer to the map structure
// Bmp* bmp : pointer to the bitmap
// int x_topLeft : location
// int y_topLeft : location
// double outline :  brightness level 0.0 being black and 1.0 being brightest
// double fill :  brightness level for inside the object
// bool transparent_fill: when true, the object is not filled, and it is see through
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
int Map_AddBmp(Map* map, Bmp* bmp, int x_topLeft, int y_topLeft,
	double outline, double background, bool transparent_background, bool clip);
//
// Draws a string starting from location <x_topLeft, y_topLeft> 
// 
// --PARAMS
// Map* map : pointer to the map structure
// Font* font : pointer to the font
// char* str : the string
// int x_topLeft : location
// int y_topLeft : location
// double outline :  brightness level 0.0 being black and 1.0 being brightest
// double fill :  brightness level for the background
// bool background: when true, the background is not filled, and it is see through
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
// WARNING: note newline, tabs and other control character, or characters that are not ASCII valid are represented as question marks
int Map_WriteLn(Map* map, Font* font, char* str, int x_topLeft, int y_topLeft, 
	double outline, double background, bool transparent_background, bool clip);
//
// Draws a single character starting from location <x_topLeft, y_topLeft> 
// 
// --PARAMS
// Map* map : pointer to the map structure
// Font* font : pointer to the font
// char c : the character
// int x_topLeft : location
// int y_topLeft : location
// double outline :  brightness level 0.0 being black and 1.0 being brightest
// double fill :  brightness level for the background
// bool background: when true, the background is not filled, and it is see through
// bool clip : when true, if the item goes out of bounds, it is trimmed. disable if performance is an issue
// WARNING: note newline, tabs and other control character, or characters that are not ASCII valid are represented as question marks
int Map_WriteChar(Map* map, Font* font, char c, int x_topLeft, int y_topLeft, 
	double outline, double background, bool transparent_background, bool clip);




#endif



