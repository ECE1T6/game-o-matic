// writeBoard.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h" //have this for C++
//#include "targetver.h" //have this for C++
#include "characters.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdbool.h>//have this for C


//add period, comma, semicolon, colon, apostrophe, quote, exclamation, question, asterix, hashtag, etc.
bool board[48][64] = {false};

/*
x1,y1: top left pixel
x2,y2: bottom right pixel
xNew,yNew: new top left pixel location to be translated
*/
void translateBox(int x1, int y1, int x2, int y2, int xNew, int yNew){
}
/*
rotates 90degrees clockwise
*/
void rotateClockwise(int x1,int y1, int x2, int y2){
}
void skewHorizontal(int x1, int y1, int x2, int y2, int factor){
}
void skewVertical(int x1, int y1, int x2, int y2, int factor){
}
void stretchHorizontal(int x1, int y1, int x2, int y2, int scale){
	bool** temp;
	temp = (bool**) malloc((x2-x1)*scale*sizeof(bool*));
	for (int i = 0; i < (x2-x1)*scale; i++){
		temp[i] = (bool*) malloc((y2-y1)*scale*sizeof(bool));
	}


	for(int i=0;i<(y2-y1);i++){//Stretch
		for(int j=0;j<(x2-x1);j++){
			temp[i][scale*j]=board[y1+i][x1+j];
		}
	}

	for(int i=0;i<(y2-y1);i++){//fill
		for(int j=0;j<(x2-x1)*scale;j++){
			temp[i][j]=temp[i][(j/scale)*scale];
		}
	}
		
	for(int i = 0; i < (x2-x1)*scale ; i++){//place back into board
		for(int j = 0; j < (y2-y1); j++){
			board[y1+j][x1+i]= temp [j][i];
		}			
	}
	free(temp);
}
void stretchVertical(int x1, int y1, int x2, int y2, int scale){
	bool** temp;
	temp = (bool**) malloc((x2-x1)*scale*sizeof(bool*));
	for (int i = 0; i < (x2-x1)*scale; i++){
		temp[i] = (bool*) malloc((y2-y1)*scale*sizeof(bool));
	}


	for(int i=0;i<(x2-x1);i++){
		for(int j=0;j<(y2-y1);j++){
			temp[scale*j][i]=board[y1+j][x1+i];
		}
	}

	for(int i=0;i<(x2-x1);i++){
		for(int j=0;j<(y2-y1)*scale;j++){
			temp[j][i]=temp[(j/scale)*scale][i];
		}
	}

	for(int i = 0; i < (x2-x1) ; i++){
		for(int j = 0; j < (y2-y1)*scale; j++){
			board[y1+j][x1+i]= temp [j][i];
		}			
	}
	free(temp);
}

void replacePixels_number( char c, int x, int y){
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 5; j++){
			board[i+y][j+x]= CHAR_NUM[c][i][j];
		}			
	}//replaces necessary pixels one by one.
}
void replacePixels_upperCase( char c, int x, int y){
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 5; j++){
			board[i+y][j+x]= CHAR_UPPER[c][i][j];
		}			
	}//replaces necessary pixels one by one.
}
void replacePixels_lowerCase( char c, int x,int y){
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 5; j++){
			board[i+y][j+x]= CHAR_LOWER[c][i][j];
		}			
	}//replaces necessary pixels one by one.
}
void replacePixels_space(int x, int y){
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 5; j++){
			board[i+y][j+x]= 0;
		}			
	}//replaces necessary pixels one by one.
}
/*
NOTE: currently only works for numbers and capital letters, if need be we can add more characters to it.

char c : the character to be drawn
x, y: the location of the upper left corner of the 5x7 array inside the big 64x48 array
note x and y start from 0
*/

bool drawChar( char c, int x, int y,int scale){//replace 48 and 64 with a CONST when you feel like it


	if(x>(64-5*scale) || x<0 || y>(48-7*scale) || y<0){
		return 0;// false if out of bounds
	}

	else if(c>='0' && c<='9'){ //Call replacePixels function for numbers only
		replacePixels_number( c-48, x, y);
	}
	else if(c>='A' && c<='Z'){ //Call replacePixels function for capital letters only
		replacePixels_upperCase( c-65 , x, y);
	}
	else if(c>='a' && c<='z'){ //Call replacePixels function for capital letters only
		replacePixels_lowerCase( c-97 , x, y);
	}
	else if(c==' '){
		replacePixels_space(x, y);
	}
	stretchHorizontal(x,y,x+6,y+8,scale);
	stretchVertical(x,y,x+6*scale,y+8,scale);


	return 1;
}
bool drawString(char* s,int length, int x, int y, int scale){//length is length of string (DEAL WITH IT!!!)
	//Do I need to malloc it? naw...
	for(int i=0; i<length; i++){//for loop is... I'm not gonna comment it's kinda obvious actually
		if(drawChar(s[i],x+6*i*scale,y,scale)){
			/*IMPORTANT NOTE: the problem with this implementation is it
			returns the zero after the program has started executing, could be a problem or could be an asset
			*/
		}
		else{
			return 0;
		}
	}
	return 1;
}


void printTest(){
	for(int i = 0; i < 48; i++){
		for(int j = 0; j < 64; j++){
			if(board[i][j] == true){
				printf("#", board[i][j]);
			}
			else{
				printf(".", board[i][j]);
			}
		}
		printf("\n");
	}
}

int main()//For testing Purposes only
{
	bool test;

	test = drawString("ABC",3,0,0,3);
	printf("%d\n",test);

	/*for(int i='A';i<='Z';i++){
	test = drawChar(i,(i-'A')*6 %60 ,(i-'A')/10 *8,1);
	}*/

	printTest();
	//system("Pause");//needed for C++
	return 0;
}
