


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include <iostream>
using namespace std;


int main(int argc, char* argv[]){

	char str[1024];

	FILE* fp = fopen("default.font", "r");
	FILE* out = fopen("default.bcm", "wb");
	int width, height;
	width = atoi(fgets(str, 1024, fp));
	height = atoi(fgets(str, 1024, fp));

	cout << width << endl;
	cout << height << endl;

	//TODO: fix for more than one digit
	fputc(0, out);
	fputc(0, out);
	fputc(0, out);
	fputc(width, out);
	
	fputc(0, out);
	fputc(0, out);
	fputc(0, out);
	fputc(height, out);
	

	for (int i = 0; i < 127 - ' '; i++){
		fgets(str, 1024, fp);
		char** letter = new char* [width];
		for (int j = 0; j < width; j++){
			letter[j] = new char [height];
		}

		for (int j = 0; j < height; j++){
			for (int k = 0; k < width; k++){
				letter[k][j] = fgetc(fp); 
			}
			fgetc(fp);
		}

		char c = 0;
		int count = 0;
		for (int j = 0; j < height; j++){
			for (int k = 0; k < width; k++){
				if (letter[k][j] == '1'){
					c |= (1 << (8-count-1));
				} 
				
				count++;
				if (count == 8){
					fputc(c, out);
					count = 0;
					c = 0;
				}
			}
		}


		for (int j = 0; j < 8; j++){
				
			count++;
			if (count == 8){
				fputc(c, out);
				count = 0;
				c = 0;
				break;
			}
		}


		
		for (int j = 0; j < width; j++){
			delete[] letter[j];
		}
		delete[] letter;
	}
	
	return 0;
}

