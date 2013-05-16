//#include "stdafx.h" //have this for C++
//#include "targetver.h" //have this for C++
#include <stdio.h>
#include <iostream>
#include <stdbool.h>//have this for C


//add period, comma, semicolon, colon, apostrophe, quote, exclamation, question, asterix, hashtag, etc.

void replacePixels_number(bool a[48][64], char c, int x, int y){
  for(int i = 0; i < 7; i++){
		for(int j = 0; j < 5; j++){
			a[i+y][j+x]= CHAR_NUM[c][i][j];
		}			
	}//replaces necessary pixels one by one.
}
void replacePixels_upperCase(bool a[48][64], char c, int x, int y){
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 5; j++){
			a[i+y][j+x]= CHAR_UPPER[c][i][j];
		}			
	}//replaces necessary pixels one by one.
}
void replacePixels_lowerCase(bool a[48][64], char c, int x,int y){
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 5; j++){
			a[i+y][j+x]= CHAR_LOWER[c][i][j];
		}			
	}//replaces necessary pixels one by one.
}
/*
NOTE: currently only works for numbers and capital letters, if need be we can add more characters to it.

array: should be a global variable... IMPORTANT IMPORTANT IMPORTANT
char c : the character to be drawn
x, y: the location of the upper left corner of the 5x7 array inside the big 64x48 array
note x and y start from 0
*/

bool drawChar(bool array[48][64], char c, int x, int y){//replace 48 and 64 with a CONST when you feel like it


	if(x>59 || x<0 || y>41 || y<0){
		return 0;// false if out of bounds
	}

	else if(c>='0' && c<='9'){ //Call replacePixels function for numbers only
			replacePixels_number(array, c-48, x, y);
	}
	else if(c>='A' && c<='Z'){ //Call replacePixels function for capital letters only
			replacePixels_upperCase(array, c-65 , x, y);
	}
	else if(c>='a' && c<='z'){ //Call replacePixels function for capital letters only
			replacePixels_lowerCase(array, c-97 , x, y);
	}

	return 1;
}

void printTest(bool array[48][64]){
	int i, j;
	for(i = 0; i < 48; i++){
		for(j = 0; j < 64; j++){
			if(array[i][j] == true){
				printf("O", array[i][j]);
			}
			else{
				printf(" ", array[i][j]);
			}
		}
		printf("\n");
	}
}

int main()//For testing Purposes only
{
	bool board[48][64] = {false};
	bool test;
	test = drawChar(board,'j',59,41);

	printTest(board);
	//system("Pause");//needed for C++
	return 0;
}

