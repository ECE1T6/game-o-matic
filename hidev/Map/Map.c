

#include "Map.h"


#define CHECK_NULLPTR_ERROR(x) \
	do { \
		if (!x){ \
			return MAP_NULLPTR_ERROR; \
		} \
	} while(0)

#define CHECK_RANGE_ERROR(x, y) \
	do { \
		if (x >= 0 && x < map->width && \
			y >= 0 && y < map->height){ \
		} else { \
			return MAP_RANGE_ERROR; \
		} \
	} while(0)


#define SET_PIXEL(x, y, c) \
	do { \
		if (clip){ \
			if ( (x) >= 0 && (x) < map->width && (y) >= 0 && (y) < map->height){ \
				map->pixels[x][y] = c; \
			} \
		} else { \
			map->pixels[x][y] = c; \
		} \
	} while(0)

#define LINE_STEP (60.0)


int Map_Init(Map* map, int width, int height){
	map->width = width;
	map->height = height;


	// Initialize 2D array
	map->pixels = (byte**) malloc(map->width * sizeof(byte*));
	if (!map->pixels){
		return MAP_ALLOC_ERROR;
	}

	int i;	
	for (i = 0; i < map->width; i++){
		map->pixels[i] = (byte*) malloc(map->height * sizeof(byte));
		if (!map->pixels[i]){
			return MAP_ALLOC_ERROR;
		}
	}
	// End of array initialization

	Map_Clear(map);

	return 0;

}

Map* Map_Initialize(int width, int height){

	Map* map = (Map*) malloc(sizeof(Map));
	if (!map){
		return NULL;
	}

	map->width = width;
	map->height = height;


	// Initialize 2D array
	map->pixels = (byte**) malloc(map->width * sizeof(byte*));
	if (!map->pixels){
		return NULL;
	}

	int i;	
	for (i = 0; i < map->width; i++){
		map->pixels[i] = (byte*) malloc(map->height * sizeof(byte));
		if (!map->pixels[i]){
			return NULL;
		}
	}
	// End of array initialization

	Map_Clear(map);

	return map;
}

void Map_Term(Map* map){

	if (map == NULL){
		return;
	}
	
	// Free 2D array
	
	int i;
	for (i = 0; i < map->width; i++){
		free(map->pixels[i]);
	}
	free(map->pixels);
	map->pixels = NULL;
	
}

void Map_Terminate(Map** map){

	// Free 2D array
	if (map == NULL || (*map) == NULL){
		return;
	}

	int i;
	for (i = 0; i < (*map)->width; i++){
		free((*map)->pixels[i]);
	}
	free((*map)->pixels);

	// Null the pointer for safety
	free(*map);
	*map = NULL;
}

int Map_Clear(Map* map){

	CHECK_NULLPTR_ERROR(map);
	int x, y;
	for (y = 0; y < map->height; y++){
		for (x = 0; x < map->width; x++){
			map->pixels[x][y] = LEVEL(0.0);
		}
	}

	return 0;
}

int Map_SetPixel(Map* map, int x, int y, double level){
	CHECK_NULLPTR_ERROR(map);
	CHECK_RANGE_ERROR(x, y);

	map->pixels[x][y] = LEVEL(level);

	return 0;
}

int Map_Print(Map* map){
	
	CHECK_NULLPTR_ERROR(map);

	int x, y;
	/////////////////////////////////
	printf(" ");
	for (x = 0; x < map->width; x++){
		printf("_");
	}
	printf(" \n");
	//////////////////////////////////
	for (y = 0; y < map->height; y++){
		printf("|");
		for (x = 0; x < map->width; x++){
			if (map->pixels[x][y] == LEVEL(1.0)){
				printf("#");
			} else if (map->pixels[x][y] == LEVEL(0.0)){
				printf(".");
			} else if (map->pixels[x][y] == LEVEL(0.42)){
				printf("A");	
			} else {
				printf("?");
			}
		}
		printf("|\n");
	}

	/////////////////////////////////
	printf(" ");
	for (x = 0; x < map->width; x++){
		printf("_");
	}
	printf(" \n");

	return 0;


}

int Map_DrawHrznLn(Map* map, int x, int y, int length, double outline, bool clip){
	CHECK_NULLPTR_ERROR(map);
	if (!clip){
		CHECK_RANGE_ERROR(x, y);
		CHECK_RANGE_ERROR(x+length, y);
	}


	length = ABS(length);


	int i;
	for (i = x; i <= x + length; i++){
		SET_PIXEL(i, y, LEVEL(outline));
	}
	

	return 0;
}

int Map_DrawVertLn(Map* map, int x, int y, int length, double outline, bool clip){
	CHECK_NULLPTR_ERROR(map);
	if (!clip){
		CHECK_RANGE_ERROR(x, y);
		CHECK_RANGE_ERROR(x, y+length);
	}
	
	length = ABS(length);

	int i;
	for (i = y; i <= y + length; i++){
		SET_PIXEL(x, i, LEVEL(outline));
	}

	return 0;
}



int Map_DrawLn(Map* map, 
	int x0, int y0, int x1, int y1,
	double outline, bool clip){
	CHECK_NULLPTR_ERROR(map);
	if (!clip){
		CHECK_RANGE_ERROR(x0, y0);
		CHECK_RANGE_ERROR(x1, y1);
	}

 	bool steep = (ABS(y1-y0) > ABS(x1-x0));
	if (steep){
		SWAP(x0, y0);
		SWAP(x1, y1);
	}

	if (x0 > x1){
		SWAP(x0, x1);
		SWAP(y0, y1);
	}

	float dx = x1 - x0;
	float dy = ABS(y1 - y0);
	float error = dx / 2.0f;
	
	int ystep = (y0 < y1) ? 1 : -1;
	int y = lround(y0);
	int maxX = lround(x1);

	int x;
	for (x = lround(x0); x < maxX; x++){
		if (steep){
			SET_PIXEL(y, x, LEVEL(outline));
		} else {
			SET_PIXEL(x, y, LEVEL(outline));
		}

		error -= dy;
		if (error < 0.5){
			y += ystep;
			error += dx;
		}

	}
	
	return 0;

}

int Map_DrawRect(Map* map,
	int x0, int y0, int x1, int y1, 
	double outline, double fill, bool transparent_fill, bool clip){
	CHECK_NULLPTR_ERROR(map);
	if (!clip){
		CHECK_RANGE_ERROR(x0, y0);
		CHECK_RANGE_ERROR(x1, y1);
	}

	if (x0 > x1){
		SWAP(x0, x1);
	}
	if (y0 > y1){
		SWAP(y0, y1);
	}

	Map_DrawHrznLn(map, x0, y0, (x1 - x0), outline, clip);
	Map_DrawHrznLn(map, x0, y1, (x1 - x0), outline, clip);

	Map_DrawVertLn(map, x0, y0, (y1 - y0), outline, clip);
	Map_DrawVertLn(map, x1, y0, (y1 - y0), outline, clip);

	if (!transparent_fill){
		int x, y;
		for (x = x0 + 1; x < x1; x++){
			for (y = y0 + 1; y < y1; y++){
				SET_PIXEL(x, y, LEVEL(fill));
			}
		}
	}

	return 0;

}

int Map_DrawArc(Map* map, int cx, int cy, int width, int height, 
	double theta_i, double theta_f,
	double outline, bool clip){

	if (!clip){
		CHECK_RANGE_ERROR(cx - width, cy - height);
		CHECK_RANGE_ERROR(cx + width, cy + height);
	}

	if (theta_i > theta_f){
		double temp = theta_i;
		theta_i = theta_f;
		theta_f = temp;
	}

	double step = (theta_f - theta_i) / LINE_STEP;
	double theta;
	for (theta = theta_i; theta < theta_f; theta += step){
		int x = cx + lround(width * cos(theta));
		int y = cy + lround(height * sin(theta));
		int x_next = cx + lround(width * cos(theta + step));
		int y_next = cy + lround(height * sin(theta + step));
		Map_DrawLn(map, x, y, x_next, y_next, outline, clip);
	}


}

int Map_DrawElps(Map* map, int cx, int cy, int width, int height, 
	double outline, double fill, bool transparent_fill, bool clip){

	CHECK_NULLPTR_ERROR(map);

	if (!clip){
		CHECK_RANGE_ERROR(cx - width, cy - height);
		CHECK_RANGE_ERROR(cx + width, cy + height);
	}

	int hh = height * height;
	int ww = width * width;
	int hhww = hh * ww;
	int x0 = width;
	int dx = 0;

	
	if (!transparent_fill){
		// do the horizontal diameter
		int x;
		for (x = -width; x <= width; x++){
			SET_PIXEL(cx+x, cy, LEVEL(fill));
		}

		// now do both halves at the same time, away from the diameter
		int y;
		for (y = 1; y <= height; y++) {
	
			int x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more
			for ( ; x1 > 0; x1--){
				if (x1*x1*hh + y*y*ww <= hhww) { break;}
			}
			dx = x0 - x1;  // current approximation of the slope
			x0 = x1;


			int x;
			for (x = -x0; x <= x0; x++) {
				SET_PIXEL(cx+x, cy-y, LEVEL(fill));
				SET_PIXEL(cx+x, cy+y, LEVEL(fill));
			}
		}
	}

	Map_DrawArc(map, cx, cy, width, height, 0.0, 2.0 * M_PI, outline, clip);

	return 0;
}

int Map_DrawPlgn(Map* map, int* x, int* y, int N, 
	double outline, double fill, bool transparent_fill, bool clip){

	CHECK_NULLPTR_ERROR(map);
	if (N < 3){
		return MAP_RANGE_ERROR;
	}

	int i;
	if (!clip){
		for (i = 0; i < N; i++){
			CHECK_RANGE_ERROR(x[i], y[i]);
		}
	}


	if (!transparent_fill){

		int image_top = y[0], image_bot = y[0], image_left = x[0], image_right = x[0];
		for (i = 0; i < N; i++){
			image_top = MIN(image_top, y[i]);
			image_bot = MAX(image_bot, y[i]);
			image_left  = MIN(image_left, x[i]);
			image_right = MAX(image_right, x[i]);
		}


		// fill polygon
		int  nodes, pixelX, pixelY, swap ;
		int* nodeX = (int*) malloc(N * sizeof(int));

		//  Loop through the rows of the image.
		for (pixelY=image_top; pixelY<image_bot; pixelY++) {
			int j;
			//  Build a list of nodes.
			nodes=0; j=N-1;
			for (i=0; i<N; i++) {
				if ((float)y[i] < (float) pixelY && (float)y[j]>=(float) pixelY ||
					(float)y[j] < (float) pixelY && (float)y[i]>=(float) pixelY) {
	
					nodeX[nodes++] =
						lround((float)x[i]+(pixelY-(float)y[i])/
						((float)y[j]-(float)y[i]) *((float)x[j]-(float)x[i])); 
				}
		    		j = i; 
			}
			//  Sort the nodes, via a simple “Bubble” sort.
			i = 0;
			while (i < nodes-1) {
				if (nodeX[i] > nodeX[i+1]) {
					SWAP(nodeX[i], nodeX[i+1]);
					if (i) {
						i--;
					} 
				} else {
					i++; 
				}
			}
	
			//  Fill the pixels between node pairs.
			for (i=0; i<nodes; i+=2) {
				if (nodeX[i] >= image_right) {
					break;
				}
				if (nodeX[i+1] > image_left) {
					if (nodeX[i] < image_left) { 
						nodeX[i] = image_left;
					}					
					if (nodeX[i+1] > image_right) { 
						nodeX[i+1] = image_right;
					}
					for (j=nodeX[i]; j < nodeX[i+1]; j++){
						SET_PIXEL(j,pixelY, LEVEL(fill)); 
					}
				}
			}
		}
		free(nodeX);
	}
	

	// draw outlines
	for (i = 0; i < N - 1; i++){
		Map_DrawLn(map, x[i], y[i], x[i+1], y[i+1], outline, clip);
	}
	Map_DrawLn(map, x[N-1], y[N-1], x[0], y[0], outline, clip);

	return 0;

}


int Map_AddDrawing(Map* map, Drawing* drawing, int x, int y, double ratio, bool clip){
	CHECK_NULLPTR_ERROR(map);
	CHECK_NULLPTR_ERROR(drawing);
	

	int* x_list = (int*) malloc(drawing->N * sizeof(int));
	int* y_list = (int*) malloc(drawing->N * sizeof(int));

	Drawing_GetVertices(drawing, x, y, ratio, x_list, y_list);
	
	

	int ret;
	switch(drawing->type){
	case (D_LINE_LOOP):
		ret = Map_DrawPlgn(map, x_list, y_list, drawing->N, drawing->outline, drawing->fill, drawing->transparent, clip);
		break;
	case (D_TRIANGLE_STRIP):
		int i;
		if (!clip){
			for (i = 0; i < drawing->N; i++){
				CHECK_RANGE_ERROR(x_list[i], y_list[i]);
			}
		}
		for (i = 0; i < drawing->N - 2; i++){
			Map_DrawPlgn(map, &x_list[i], &y_list[i], 3, drawing->outline, drawing->fill, drawing->transparent, clip);
		}
		break;
	}
	
	free(x_list);
	free(y_list);

	return ret;

}

int Map_AddBmp(Map* map, Bmp* bmp, int x, int y,
	double outline, double background, bool transparent_background, bool clip){
	
	CHECK_NULLPTR_ERROR(map);
	CHECK_NULLPTR_ERROR(bmp);

	if (!clip){
		CHECK_RANGE_ERROR(x, y);
		CHECK_RANGE_ERROR(x + bmp->width - 1, y + bmp->height - 1);
	}

	int i, j;
	for (i = 0; i < bmp->width; i++){
		for (j = 0; j < bmp->height; j++){
			if (bmp->pixels[i][j] == 1){
				SET_PIXEL(i + x, j + y, LEVEL(outline));
			} else if (!transparent_background){
				SET_PIXEL(i + x, j + y, LEVEL(background));
			}
		}

	}


}

int Map_WriteLn(Map* map, Font* font, char* str, int x_topLeft, int y_topLeft, 
	double outline, double background, bool transparent_background, bool clip){

	CHECK_NULLPTR_ERROR(map);
	CHECK_NULLPTR_ERROR(str);
	CHECK_NULLPTR_ERROR(font);

	int size = strlen(str);


	if (!clip){
		CHECK_RANGE_ERROR(x_topLeft, y_topLeft);
		CHECK_RANGE_ERROR(x_topLeft + size * font->width - 1, y_topLeft - 1);
	}
	
	int i;
	for (i = 0; i < size; i++){
		Map_WriteChar(map, font, str[i], x_topLeft + i*font->width, y_topLeft,
			outline, background, transparent_background, clip);
	}

	return 0;
}


int Map_WriteChar(Map* map, Font* font, char c, int x_topLeft, int y_topLeft, 
	double outline, double background, bool transparent_background, bool clip){



	if (!clip){
		CHECK_RANGE_ERROR(x_topLeft, y_topLeft);
		CHECK_RANGE_ERROR(x_topLeft + font->width - 1, y_topLeft + font->height - 1);
	}	

	if ( c < ' ' || c > '~'){
		c = '?';
	}	

	int x, y;
	for (x = 0; x < font->width; x++){
		for (y = 0; y < font->height; y++){
			if (font->bits[c-' '][x][y] == 1){
				SET_PIXEL(x_topLeft+x, y_topLeft+y, LEVEL(outline));
			} else {
				if (!transparent_background){
					SET_PIXEL(x_topLeft+x, y_topLeft+y, LEVEL(background));
				}
			}
		}
	}

	return 0;
	
}

