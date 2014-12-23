

#include "Font.h"


#define NUM_ASCII_READABLE (127 - ' ')


#define CHECK_NULLPTR_ERROR(x) \
	do { \
		if (!x){ \
			return FNT_NULLPTR_ERROR; \
		} \
	} while(0)


int Font_Init(Font* font, char* filename){
	FILE* fp = fopen(filename, "rb");

	font->width = ReadInt(fp);
	font->height = ReadInt(fp);

	font->bits = (bool***) malloc(NUM_ASCII_READABLE * sizeof(bool**));

	int i, j, k;
	for (i = 0; i < NUM_ASCII_READABLE; i++){
		font->bits[i] = (bool**) malloc(font->width * sizeof(bool*));
		for (j = 0; j < font->width; j++){
			font->bits[i][j] = (bool*) malloc(font->height * sizeof(bool));
		}
	}

	int count = 0;
	char c;
	for (i = 0; i < NUM_ASCII_READABLE; i++){
		for (j = 0; j < font->height; j++){
			for (k = 0; k < font->width; k++){
				if (count == 0){
					c = fgetc(fp);
				}
				font->bits[i][k][j] = ((c >> (8 - count - 1)) & 0x1);
				count++;
				if (count == 8){
					count = 0;
				}
			}
		}
		count = 0;
	}
	
	fclose(fp);

	return 0;
}



Font* Font_Initialize(char* filename){
	Font* font = (Font*) malloc(sizeof(Font));

	FILE* fp = fopen(filename, "rb");

	font->width = ReadInt(fp);
	font->height = ReadInt(fp);

	font->bits = (bool***) malloc(NUM_ASCII_READABLE * sizeof(bool**));

	int i, j, k;
	for (i = 0; i < NUM_ASCII_READABLE; i++){
		font->bits[i] = (bool**) malloc(font->width * sizeof(bool*));
		for (j = 0; j < font->width; j++){
			font->bits[i][j] = (bool*) malloc(font->height * sizeof(bool));
		}
	}

	int count = 0;
	char c;
	for (i = 0; i < NUM_ASCII_READABLE; i++){
		for (j = 0; j < font->height; j++){
			for (k = 0; k < font->width; k++){
				if (count == 0){
					c = fgetc(fp);
				}
				font->bits[i][k][j] = ((c >> (8 - count - 1)) & 0x1);
				count++;
				if (count == 8){
					count = 0;
				}
			}
		}
		count = 0;
	}

	fclose(fp);
	
	return font;
}

void Font_Term(Font* font){

	int i, j;
	for (i = 0; i < NUM_ASCII_READABLE; i++){
		for (j = 0; j < font->width; j++){
			free(font->bits[i][j]);
		}
		free(font->bits[i]);
	}
	free(font->bits);

}

void Font_Terminate(Font** font){

	int i, j;
	for (i = 0; i < NUM_ASCII_READABLE; i++){
		for (j = 0; j < (*font)->width; j++){
			free((*font)->bits[i][j]);
		}
		free((*font)->bits[i]);
	}
	free((*font)->bits);

	free(*font);
	*font = NULL;
}


int Font_Resize(Font* font, int factor){

	CHECK_NULLPTR_ERROR(font);

	if (factor < 1){
		return FNT_RANGE_ERROR;
	}
	
	int old_width = font->width;
	int old_height = font->height;
	bool*** old_bits = font->bits;

	font->width *= factor;
	font->height *= factor;

	int i, j, k;
	font->bits = (bool***) malloc(NUM_ASCII_READABLE * sizeof(bool**));
	for (i = 0; i < NUM_ASCII_READABLE; i++){
		font->bits[i] = (bool**) malloc(font->width * sizeof(bool*));
		for (j = 0; j < font->width; j++){
			font->bits[i][j] = (bool*) malloc(font->height * sizeof(bool));
		}
	}

	for (i = 0; i < NUM_ASCII_READABLE; i++){
		for (j = 0; j < font->height; j++){
			for (k = 0; k < font->width; k++){
				font->bits[i][k][j] = old_bits[i][k/factor][j/factor];
			}
		}
	}


	for (i = 0; i < NUM_ASCII_READABLE; i++){
		for (j = 0; j < old_width; j++){
			free(old_bits[i][j]);
		}
		free(old_bits[i]);
	}
	free(old_bits);



}




