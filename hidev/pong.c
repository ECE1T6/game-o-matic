#include "stdafx.h"
#include "targetver.h"
#include <stdio.h>
#include <math.h>
//#include <stdbool.h>
//#include <unistd.h>
#include <windows.h>

#define ARRAY_HEIGHT 48
#define ARRAY_WIDTH 64

#define BALL_RADIUS 1.0 /*A 3x3 square has radius 1, 5x5 has radius 2, etc.*/

#define TOP_MARGIN (0.0 + BALL_RADIUS) /*The margins bound the area where the centre of the ball can be.*/
#define BOTTOM_MARGIN (0.0 + BALL_RADIUS) /*The first number may be modified.*/
#define LEFT_MARGIN 0.0
#define RIGHT_MARGIN 0.0

#define BOTTOM_END (ARRAY_HEIGHT - BOTTOM_MARGIN - 1)
#define RIGHT_END (ARRAY_WIDTH - RIGHT_MARGIN - 1)

#define PADDLE_HEIGHT 8.0
#define PADDLE_WIDTH 2.0

#define LEFT_PADDLE_X_COORD (LEFT_MARGIN + PADDLE_WIDTH + 3.0 + 1.0) /*Refers to the innermost pixel on the top of the paddle.*/
#define RIGHT_PADDLE_X_COORD (RIGHT_END - PADDLE_WIDTH - 3.0 + 1.0)

#define LEFT_IMPACT_X_COORD (LEFT_MARGIN + PADDLE_WIDTH + 3.0 + 1.0 + BALL_RADIUS)
#define RIGHT_IMPACT_X_COORD (ARRAY_WIDTH - RIGHT_MARGIN - 1.0 - PADDLE_WIDTH - 3.0 - BALL_RADIUS)

#define REFRESH_DELAY 10
#define DISTANCE_PER_REFRESH 1.0

float x_coord = LEFT_MARGIN + (RIGHT_END - LEFT_MARGIN) / 2; /*The coordinates are rounded for output, but floats let us keep track of movement more accurately.*/
float y_coord = TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2 -17;
float delta_x =  DISTANCE_PER_REFRESH * sqrt(0.5);
float delta_y = - DISTANCE_PER_REFRESH * sqrt(0.5);

struct score{ /* This might be worth removing.*/
	int left_score;
	int right_score;
};

void print_test(bool array[(int)ARRAY_HEIGHT][(int)ARRAY_WIDTH]){
	int i, j;
	for(i = 0; i < ARRAY_HEIGHT; i++){
		for(j = 0; j < ARRAY_WIDTH; j++){
			if(array[i][j] == true){
				printf("O", array[i][j]);
			}
			else{
				printf("*", array[i][j]);
			}
		}
		printf("\n");
	}
}
/*This could likely be improved to minimize calculations.*/
void draw_ball(bool array[(int)ARRAY_HEIGHT][(int)ARRAY_WIDTH], bool boolean){
	int i, j;
	int displayed_x_coord = (int) ( x_coord + 0.5);
	int displayed_y_coord = (int) ( y_coord + 0.5);
	for(j = -BALL_RADIUS; j < BALL_RADIUS; j++){
		for(i = -BALL_RADIUS; i < BALL_RADIUS; i++){
			if(displayed_x_coord + j < RIGHT_END && displayed_x_coord + j > LEFT_MARGIN){
				array[displayed_y_coord + i][displayed_x_coord + j] = boolean;
			}
		}
	}
}
void draw_left_paddle(float left_paddle_y_coord, bool array[(int)ARRAY_HEIGHT][(int)ARRAY_WIDTH], bool boolean){
	int i, j;
	int displayed_y_coord = (int) (left_paddle_y_coord + 0.5);
	for(j = PADDLE_HEIGHT - 1; j >= 0; j--){
		for(i = PADDLE_WIDTH ; i >= 1; i--){
			array[displayed_y_coord + j][(int) LEFT_PADDLE_X_COORD - i] = boolean;
		}
	}
}
void draw_right_paddle(float right_paddle_y_coord, bool array[(int)ARRAY_HEIGHT][(int)ARRAY_WIDTH], bool boolean){
	int i, j;
	int displayed_y_coord = (int) (right_paddle_y_coord + 0.5);
	for(j = PADDLE_HEIGHT - 1; j >= 0; j--){
		for(i = PADDLE_WIDTH-2 ; i >= -1; i--){
			array[displayed_y_coord + j][(int) RIGHT_PADDLE_X_COORD + i] = boolean;
		}
	}
}
void check_wall_deflection(){
	if( y_coord < TOP_MARGIN){ /*Deflection off of the top side*/
		 y_coord = TOP_MARGIN + (TOP_MARGIN -  y_coord);
		 delta_y *= - 1;
	}
	else if( y_coord > BOTTOM_END){ /*Deflection off of the bottom side*/
		 y_coord = BOTTOM_END - ( y_coord - BOTTOM_END);
		 delta_y *= - 1;
	}
}
void check_left_paddle_impact( float left_paddle_y_coord){
	int ball_impact_y_coord =  y_coord + (LEFT_IMPACT_X_COORD -  x_coord) * -  delta_y /  delta_x;
	/*Checking if the ball hit the inner side of left_paddle: (two conditions for vertical position, one checking if impact occurred this cycle*/
	if(ball_impact_y_coord + (LEFT_IMPACT_X_COORD -  x_coord) * -  delta_y /  delta_x + BALL_RADIUS >= left_paddle_y_coord
		&& ball_impact_y_coord + (LEFT_IMPACT_X_COORD -  x_coord) * -  delta_y /  delta_x - BALL_RADIUS <= left_paddle_y_coord + PADDLE_HEIGHT - 1
		&&  x_coord -  delta_x >= LEFT_IMPACT_X_COORD){
			/*These ignore imparted paddle momentum right now:*/
			 x_coord = LEFT_IMPACT_X_COORD + (LEFT_IMPACT_X_COORD -  x_coord);
			 delta_x *= - 1;
	}
}
void check_right_paddle_impact( float right_paddle_y_coord){
	int ball_impact_y_coord =  y_coord - ( x_coord - RIGHT_IMPACT_X_COORD) *  delta_y /  delta_x;
	/*Checking if the ball hit the inner side of right_paddle: (two conditions for vertical position, one checking if impact occurred this cycle)*/
	if(ball_impact_y_coord + ( x_coord - RIGHT_IMPACT_X_COORD) *  delta_y /  delta_x + BALL_RADIUS >= right_paddle_y_coord
		&& ball_impact_y_coord + ( x_coord - RIGHT_IMPACT_X_COORD) *  delta_y /  delta_x - BALL_RADIUS <= right_paddle_y_coord + PADDLE_HEIGHT - 1
		&&  x_coord -  delta_x <= RIGHT_IMPACT_X_COORD){
			/*These ignore imparted paddle momentum right now:*/
			 x_coord = RIGHT_IMPACT_X_COORD - ( x_coord - RIGHT_IMPACT_X_COORD);
			 delta_x *= - 1;
	}
}
void reset_ball(){
	 x_coord = LEFT_MARGIN + (RIGHT_END - LEFT_MARGIN) / 2;
	 y_coord = TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2;
}
void display_winner(struct score *score_ptr){
	if(score_ptr->left_score == 5){
		/*print to screen that the left player won, or do some visual effect*/
	}
	else{
		/*print to screen that the right player won, or do some visual effect*/
	}
}


int main (void){

	float left_paddle_y_coord = TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2 - PADDLE_HEIGHT / 2; /*Refers to the innermost top pixel.*/
	float right_paddle_y_coord = TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2 - PADDLE_HEIGHT / 2;


	struct score score = {0, 0};
	struct score *score_ptr = &score;

	bool array[(int)ARRAY_HEIGHT][(int)ARRAY_WIDTH] = {false};

	int score_delay = 10; /*Deactivates ball movement for some time after someone scores*/

	/*game start*/
	while (1){
		/*sleep function using REFRESH_DELAY*/

		/*Erase paddles from array:*/
		draw_left_paddle(left_paddle_y_coord, array, false);
		draw_right_paddle(right_paddle_y_coord, array, false);

		/*gather input and move paddles*/

		if (score_delay > 0){
			score_delay--;
		}
		else if(score_delay == 0){
			draw_ball(array, false); /*Erase ball from array*/
			 y_coord +=  delta_y;
			 x_coord +=  delta_x;
			check_wall_deflection();

			if( x_coord <= LEFT_IMPACT_X_COORD){ /*The ball has pierced the plane of left_paddle*/
				check_left_paddle_impact( left_paddle_y_coord);

				/*Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)*/

				/*Checking if the ball hit the left edge of the screen:*/
				if( x_coord < LEFT_MARGIN){
					score_ptr->left_score++; /*Also change displayed score*/
					if(score_ptr->left_score == 5){
						break; /*Goes to display_winner()*/
					}
					reset_ball( );
					score_delay = 10;
				}
			}
			else if( x_coord >= RIGHT_IMPACT_X_COORD){ /*The ball has pierced the plane of right_paddle*/

				check_right_paddle_impact(right_paddle_y_coord);

				/*Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)*/

				/*Checking if the ball hit the left edge of the screen:*/
				if( x_coord > RIGHT_END){
					score_ptr->right_score++; /*Also change displayed score*/
					if(score_ptr->right_score == 5){
						break; /*Goes to display_winner()*/
					}
					reset_ball( );
					score_delay = 10;
				}
			}
			draw_ball(array, true);
		}

		draw_left_paddle(left_paddle_y_coord, array, true);
		draw_right_paddle(right_paddle_y_coord, array, true);

		print_test(array);
		//usleep(35000);
		//system("clear");
		//Sleep(25);
		system("cls");

		/*pass array to higher level function*/

	}
	display_winner(score_ptr);
	/*game end*/
	return 0;
}
