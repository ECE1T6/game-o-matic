#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
//#include <printScreen.h> //custom lodev library
/*Every other size or position value is based on the following. We might add or remove some as we write the program.*/
#define ARRAY_HEIGHT  8
#define ARRAY_WIDTH  8
#define TOP_MARGIN  0.0 /*The margins bound the playable space in the array -- they might hold things like score*/
#define BOTTOM_MARGIN  0.0
#define LEFT_MARGIN  0.0
#define RIGHT_MARGIN  0.0
#define DISTANCE_PER_REFRESH  1.0

#define BOTTOM_END  ARRAY_HEIGHT-BOTTOM_MARGIN-1
#define RIGHT_END  ARRAY_WIDTH-RIGHT_MARGIN-1

void cleanArray(int x, int y, bool array[ARRAY_HEIGHT][ARRAY_WIDTH]){
	array[x][y] = false;
return;
}

void updateArray(int x, int y, bool array[ARRAY_HEIGHT][ARRAY_WIDTH]){
	array[x][y] = true;
return;
}

void print_test(bool array[ARRAY_HEIGHT][ARRAY_WIDTH]){
    int i, j;
    for(i = 0; i < ARRAY_HEIGHT; i++){
        for(j = 0; j < ARRAY_WIDTH; j++){
            if(array[i][j] == true){
                printf(" #", array[i][j]);
            }
            else{
                printf("  ", array[i][j]);
            }
        }
        printf("\n");
    }
}


int main (void){
/*initialization start*/

	bool array[ARRAY_HEIGHT][ARRAY_WIDTH] = {false};
	int x=0, y=0;
/*initialization end*/

	while (1){
	cleanArray(y,x,array);
	if (x<8){
		x++;
		if(y<8)y++;
		else y=0;
	}
	if (y==8)y=0;
	updateArray(y,x,array);
	//printScreen(array); //prints to LED matrix
        print_test(array);
	usleep(35000);
	system("clear");

	}
	return 0;
}
