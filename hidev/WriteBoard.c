
//#include "stdafx.h" //have this for C++
//#include "targetver.h" //have this for C++
#include "characters.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdbool.h>//have this for C


const int CHRCX = 5;
const int CHRCY = 7;
const int BOARDX = 64;
const int BOARDY = 48;
bool board[BOARDY][BOARDX] = {false};


typedef struct {
	bool** canvas;

	int x;
	int y;

	int width;
	int height;

}drawing;

bool** makeArray(int width, int height) { 
	width*=30;// I'm sick of this shit
	height*=30;

	bool** a;  
	a = (bool**) malloc(width*sizeof(bool*));  
	for (int i = 0; i < width; i++)  
		a[i] = (bool*) malloc(height*sizeof(bool));  
	return a;  
}
drawing* addDrawing(drawing* d1, drawing* d2){
	drawing* finalD = (drawing*)malloc(sizeof(drawing));

	finalD->x = d1->x + d2->x;
	finalD->y = d1->y;

	finalD->width = d1->width + d2->width;
	finalD->height = d1->height;

	finalD->canvas = makeArray(finalD->width, finalD->height);


	for(int i = 0; i < d1->height; i++){
		for(int j = 0; j < d1->width; j++){
			finalD->canvas[i][j]= d1->canvas[i][j];
		}			
	}
	for(int i = 0; i < d1->height; i++){
		for(int j = d1->width; j < finalD->width; j++){
			finalD->canvas[i][j]= d2->canvas[i][j-d1->width];
		}			
	}


	return finalD;
}
void insertDrawing(drawing* d){
	for(int i = 0; i < d->height; i++){
		for(int j = 0; j < d->width; j++){
			board[i+d->y][j+d->x]= d->canvas[i][j];
		}			
	}
}

void translateHrz(drawing* d, int factor){
	d->x += factor;
}
void translateVrt(drawing* d, int factor){
	d->y += factor;
}

void mirrorHrz(drawing* d){
	bool temp;
	for(int i = 0; i < (d->width/2); i++){
		for(int j = 0; j < d->height; j++){
			temp = d->canvas[j][i];
			d->canvas[j][i] = d->canvas[j][d->width-i-1];
			d->canvas[j][d->width-i-1] = temp;
		}
	}
}
void mirrorVrt(drawing* d){//top goes to bottom, bottom goes to top
	bool temp;
	for(int i = 0; i <= (d->height/2); i++){
		for(int j = 0; j<d->width; j++){
			temp = d->canvas[i][j];
			d->canvas[i][j] = d->canvas[d->height-i-1][j];
			d->canvas[d->height-i-1][j] = temp;
		}
	}
}
void rotateCw(drawing* d){
	int max = (d->height>d->width ? d->height:d->width);

	bool** temp = makeArray(d->height, d->width);//switches width and height when mallocing

	for(int i = 0; i < max; i++){
		for(int j = 0; j < max; j++){
			temp[j][i]=d->canvas[i][j];
		}
	}

	bool** clone = d->canvas;
	d->canvas = temp;
	free(clone);

	int t = d->width;
	d->width = d->height;
	d->height = t;

	mirrorHrz(d);
}
void rotateCcw(drawing* d){
	int max = (d->height>d->width ? d->height:d->width);

	bool** temp = makeArray(d->height, d->width);//switches width and height when mallocing

	for(int i = 0; i < max; i++){
		for(int j = 0; j < max; j++){
			temp[j][i]=d->canvas[i][j];
		}
	}

	bool** clone = d->canvas;
	d->canvas = temp;
	free(clone);

	int t = d->width;
	d->width = d->height;
	d->height = t;

	mirrorVrt(d);
}
void scaleHrz(drawing* d, int factor){
	d->width *= factor;

	bool** temp = makeArray(d->width,d->height );

	for(int i=0;i<d->height;i++){
		for(int j=0;j<d->width;j++){
			temp[i][factor*j]=d->canvas[i][j];
		}
	}

	for(int i=0;i<d->height;i++){
		for(int j=0;j<d->width*factor;j++){
			temp[i][j]=temp[i][(j/factor)*factor];
		}
	}

	bool** clone = d->canvas;
	d->canvas = temp;
	free(clone);
}
void scaleVrt(drawing* d, int factor){
	d->height *= factor;

	bool** temp = makeArray(d->width, d->height);

	for(int i=0;i<d->height;i++){
		for(int j=0;j<d->width;j++){
			temp[factor*i][j] = d->canvas[i][j];
		}
	}

	for(int i=0;i<d->height*factor;i++){
		for(int j=0;j<d->width;j++){
			temp[i][j]=temp[(i/factor)*factor][j];
		}
	}

	bool** clone = d->canvas;
	d->canvas = temp;
	free(clone);
}
void shearHrz(drawing* d, int factor){
	

	bool** temp = makeArray(d->width + abs(factor)*(d->height-1), d->height);

	if (factor > 0){
		for(int i = 0; i < d->height; i++){
			for(int j = 0; j < d->width; j++){
				temp[i][j+((d->height-i-1)*factor)] = d->canvas[i][j];
			}
		}
	}
	else{
		factor *= -1;
		for(int i = 0; i < d->height; i++){
			for(int j = 0; j < d->width; j++){
				temp[i][j+(i*factor)] = d->canvas[i][j];
			}
		}
	}
	bool** clone = d->canvas;
	d->canvas = temp;
	free(clone);

	d->width += factor*(d->height-1);
}
void shearVrt(drawing* d, int factor){
	bool** temp = makeArray(d->width , d->height+ abs(factor)*(d->width-1));

	if (factor > 0){
		for(int i = 0; i < d->width; i++){
			for(int j = 0; j < d->height; j++){
				temp[j+((d->width-i-1)*factor)][i] = d->canvas[j][i];
			}
		}
	}
	else{
		factor *= -1;
		for(int i = 0; i < d->width; i++){
			for(int j = 0; j < d->height; j++){
				temp[j+(i*factor)][i] = d->canvas[j][i];
			}
		}
	}
	bool** clone = d->canvas;
	d->canvas = temp;
	free(clone);

	d->height += factor*(d->width-1);
}

void drawChrc_numb(drawing* d, char c,int blank){
	for(int i = 0; i < CHRCY; i++){
		for(int j = 0; j < CHRCX; j++){
			d-> canvas[i][j+blank]= CHAR_NUM[c][i][j];
		}			
	}
}
void drawChrc_ucas(drawing* d, char c,int blank){
	for(int i = 0; i < CHRCY; i++){
		for(int j = 0; j < CHRCX; j++){
			d-> canvas[i][j+blank]= CHAR_UPPER[c][i][j];
		}			
	}
}
void drawChrc_lcas(drawing* d, char c,int blank){
	for(int i = 0; i < CHRCY; i++){
		for(int j = 0; j < CHRCX; j++){
			d-> canvas[i][j+blank]= CHAR_LOWER[c][i][j];
		}			
	}
}
void drawChrc_spce(drawing* d, int blank){
	for(int i = 0; i < CHRCY; i++){
		for(int j = 0; j < CHRCX; j++){
			d-> canvas[i][j+blank]= false;
		}			
	}
}

void drawChrc(drawing* d, char c, int boardX, int boardY){
	d->canvas = makeArray(CHRCX, CHRCY);

	d->x = boardX;
	d->y = boardY;

	d->width = CHRCX;
	d->height = CHRCY;

	if(c>='0' && c<='9'){
		drawChrc_numb(d, c-48, 0);
	}
	else if(c>='A' && c<='Z'){
		drawChrc_ucas(d, c-65, 0);
	}
	else if(c>='a' && c<='z'){
		drawChrc_lcas(d, c-97, 0);
	}
	else if(c==' '){
		drawChrc_spce(d, 0);
	}
}
void drawString(drawing* d, char* s, int boardX, int boardY){
	int length = strlen(s);
	char c;

	d->canvas = makeArray(CHRCX,(CHRCY+1)*length);

	d->x = boardX;
	d->y = boardY;

	d->width = (CHRCX+1)*length;
	d->height = CHRCY;
	for(int i=0; i<length; i++){
		c = s[i];

		if(c>='0' && c<='9'){
			drawChrc_numb(d, c-48, i*(CHRCX+1));
		}
		else if(c>='A' && c<='Z'){
			drawChrc_ucas(d, c-65, i*(CHRCX+1));
		}
		else if(c>='a' && c<='z'){
			drawChrc_lcas(d, c-97, i*(CHRCX+1));
		}
		else if(c==' '){
			drawChrc_spce(d, i*(CHRCX+1));
		}
	}
}

void testPrint(drawing* d){
	for(int i = 0; i < d->height; i++){
		for(int j = 0; j < d->width; j++){
			if(d->canvas[i][j] == true){
				printf("#");
			}
			else{
				printf(".");
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("height:%d\n",d->height);
	printf("width :%d\n",d->width);
	printf("x     :%d\n",d->x);
	printf("y     :%d\n",d->y);
}

void testPrintBoard(){
	for(int i = 0; i < BOARDY; i++){
		for(int j = 0; j < BOARDX; j++){
			if(board[i][j] == true){
				printf("#");
			}
			else{
				printf(".");
			}
		}
		printf("\n");
	}
}

int main()
{
	printf("I'm alive!\n");
	drawing* d = (drawing*)malloc(sizeof(drawing));

	d->canvas = makeArray(CHRCX, CHRCY);
	drawString(d,"ABC",3,5);

	rotateCw(d);

	testPrint(d);

	printf("\n\n\n");
	testPrintBoard();

	insertDrawing(d);

	printf("\n\n\n");
	testPrintBoard();


	//system("Pause");//needed for C++
	return 0;
}
