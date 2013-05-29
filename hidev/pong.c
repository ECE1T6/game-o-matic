#include <stdio.h>
#include <math.h>
#include <stdbool.h>
//#include <windows.h>
#include <unistd.h>
//#include "stdafx.h"
//#include "targetver.h"

#define ARRAY_HEIGHT  48.0
#define ARRAY_WIDTH  64.0
#define TOP_MARGIN  0.0 /*The margins bound the area where the centre of the ball can be.*/
#define LEFT_MARGIN  0.0 /*The ball is supposed to go off the left and right edges.*/
#define RIGHT_MARGIN  0.0
#define BOTTOM_MARGIN 0.0

#define BALL_DIAMETER  2.0 /*Let this equal the intended diameter minus 1.*/

#define PADDLE_HEIGHT  8.0
#define PADDLE_WIDTH  2.0

#define REFRESH_DELAY  10
#define DISTANCE_PER_REFRESH  1.0

const float BOTTOM_END = (ARRAY_HEIGHT - BOTTOM_MARGIN - BALL_DIAMETER - 1.0);
const float RIGHT_END = (ARRAY_WIDTH - RIGHT_MARGIN - 1.0);

const float LEFT_PADDLE_X_COORD = (LEFT_MARGIN + PADDLE_WIDTH + 3.0 + 1.0); /*Refers to the innermost pixel on the top of the paddle.*/
const float RIGHT_PADDLE_X_COORD = (ARRAY_WIDTH - RIGHT_MARGIN - 1.0 - PADDLE_WIDTH - 3.0 + 1.0);

const float LEFT_IMPACT_X_COORD = (LEFT_MARGIN + PADDLE_WIDTH + 3.0 + 2.0);
const float RIGHT_IMPACT_X_COORD = (ARRAY_WIDTH - RIGHT_MARGIN - 1.0 - PADDLE_WIDTH - 3.0 - BALL_DIAMETER);

float ball_x_coord; /*The coordinates are rounded for output, but floats let us keep track of movement more accurately.*/
float ball_y_coord; /*note that "-19" is only here for testing*/
float ball_delta_x;
float ball_delta_y;

float left_paddle_y_coord; /*Refers to the innermost top pixel.*/
float right_paddle_y_coord;

int left_score = 0;
int right_score = 0;

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
	int displayed_x_coord = (int) (ball_x_coord + 0.5);
	int displayed_y_coord = (int) (ball_y_coord + 0.5);
	for(j = 0; j <= BALL_DIAMETER; j++){
		for(i = 0; i <= BALL_DIAMETER; i++){
			if(displayed_x_coord + j <= RIGHT_END && displayed_x_coord + j >= LEFT_MARGIN){
				array[displayed_y_coord + i][displayed_x_coord + j] = boolean;
			}
		}
	}
}
void draw_left_paddle(bool array[(int)ARRAY_HEIGHT][(int)ARRAY_WIDTH], bool boolean){
	int i, j;
	int displayed_y_coord = (int) (left_paddle_y_coord + 0.5);
	for(j = PADDLE_HEIGHT - 1; j >= 0; j--){
		for(i = PADDLE_WIDTH - 1; i >= 0; i--){
			array[displayed_y_coord + j][(int) LEFT_PADDLE_X_COORD - i] = boolean;
		}
	}
}
void draw_right_paddle(bool array[(int)ARRAY_HEIGHT][(int)ARRAY_WIDTH], bool boolean){
	int i, j;
	int displayed_y_coord = (int) (right_paddle_y_coord + 0.5);
	for(j = PADDLE_HEIGHT - 1; j >= 0; j--){
		for(i = PADDLE_WIDTH - 1; i >= 0; i--){
			array[displayed_y_coord + j][(int) RIGHT_PADDLE_X_COORD + i] = boolean;
		}
	}
}
void check_wall_deflection(){
	if(ball_y_coord < TOP_MARGIN){ /*Deflection off of the top side*/
        ball_y_coord = TOP_MARGIN + (TOP_MARGIN -  ball_y_coord);
        ball_delta_y *= - 1;
	}
	else if(ball_y_coord > BOTTOM_END){ /*Deflection off of the bottom side*/
        ball_y_coord = BOTTOM_END - (ball_y_coord - BOTTOM_END);
        ball_delta_y *= - 1;
	}
}
void check_left_paddle_impact(){
	int ball_impact_y_coord =  ball_y_coord + (LEFT_IMPACT_X_COORD -  ball_x_coord) * -  ball_delta_y /  ball_delta_x;
	/*Checking if the ball hit the inner side of left_paddle: (two conditions for vertical position, one checking if impact occurred this cycle*/
	if(ball_impact_y_coord + (LEFT_IMPACT_X_COORD - ball_x_coord) * - ball_delta_y / ball_delta_x + BALL_DIAMETER >= left_paddle_y_coord
		&& ball_impact_y_coord + (LEFT_IMPACT_X_COORD - ball_x_coord) * - ball_delta_y / ball_delta_x <= left_paddle_y_coord + PADDLE_HEIGHT - 1
		&& ball_x_coord - ball_delta_x >= LEFT_IMPACT_X_COORD){
			/*These ignore imparted paddle momentum right now:*/
			ball_x_coord = LEFT_IMPACT_X_COORD + (LEFT_IMPACT_X_COORD - ball_x_coord);
			ball_delta_x *= - 1;
	}
}
void check_right_paddle_impact(){
	int ball_impact_y_coord =  ball_y_coord - (ball_x_coord - RIGHT_IMPACT_X_COORD) * ball_delta_y / ball_delta_x;
	/*Checking if the ball hit the inner side of right_paddle: (two conditions for vertical position, one checking if impact occurred this cycle)*/
	if(ball_impact_y_coord + (ball_x_coord - RIGHT_IMPACT_X_COORD) * ball_delta_y / ball_delta_x + BALL_DIAMETER >= right_paddle_y_coord
		&& ball_impact_y_coord + (ball_x_coord - RIGHT_IMPACT_X_COORD) * ball_delta_y / ball_delta_x - BALL_DIAMETER <= right_paddle_y_coord + PADDLE_HEIGHT - 1
		&& ball_x_coord - ball_delta_x <= RIGHT_IMPACT_X_COORD){
			/*These ignore imparted paddle momentum right now:*/
            ball_x_coord = RIGHT_IMPACT_X_COORD - (ball_x_coord - RIGHT_IMPACT_X_COORD);
            ball_delta_x *= - 1;
	}
}
void reset_ball(){
    ball_x_coord = LEFT_MARGIN + (RIGHT_END - LEFT_MARGIN) / 2;
    ball_y_coord = TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2;
    printf("SCORE TEST\n");
}
void display_winner(){
	if(left_score == 5){
		/*print to screen that the left player won, or do some visual effect*/
	}
	else{
		/*print to screen that the right player won, or do some visual effect*/
	}
}


int main (void){

    ball_x_coord = LEFT_MARGIN + (RIGHT_END - LEFT_MARGIN) / 2; /*The coordinates are rounded for output, but floats let us keep track of movement more accurately.*/
    ball_y_coord = TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2 -19; /*note that "-19" is only here for testing*/
    ball_delta_x = - DISTANCE_PER_REFRESH * sqrt(0.5);
    ball_delta_y = - DISTANCE_PER_REFRESH * sqrt(0.5);

    left_paddle_y_coord = TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2 - PADDLE_HEIGHT / 2; /*Refers to the innermost top pixel.*/
    right_paddle_y_coord = TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2 - PADDLE_HEIGHT / 2;

	bool array[(int)ARRAY_HEIGHT][(int)ARRAY_WIDTH] = {false};

	int score_delay = 10; /*Deactivates ball movement for some time after someone scores*/

	/*game start*/
	while (1){
		/*sleep function using REFRESH_DELAY*/

		/*Erase paddles from array:*/
		draw_left_paddle(array, false);
		draw_right_paddle(array, false);

		/*gather input and move paddles*/

		if (score_delay > 0){
			score_delay--;
		}
		else if(score_delay == 0){
			draw_ball(array, false); /*Erase ball from array*/
            ball_y_coord += ball_delta_y;
            ball_x_coord += ball_delta_x;
			check_wall_deflection();

			if(ball_x_coord <= LEFT_IMPACT_X_COORD){ /*The ball has pierced the plane of left_paddle*/
				check_left_paddle_impact(left_paddle_y_coord);

				/*Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)*/

				/*Checking if the ball hit the left edge of the screen:*/
				if(ball_x_coord < LEFT_MARGIN){
					left_score++; /*Also change displayed score*/
					if(left_score == 25){
						break; /*Goes to display_winner()*/
					}
					reset_ball();
					score_delay = 10;
				}
			}
			else if(ball_x_coord >= RIGHT_IMPACT_X_COORD){ /*The ball has pierced the plane of right_paddle*/

				check_right_paddle_impact(right_paddle_y_coord);

				/*Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)*/

				/*Checking if the ball hit the left edge of the screen:*/
				if(ball_x_coord > RIGHT_END){
                    right_score++; /*Also change displayed score*/
					if(right_score == 25){
						break; /*Goes to display_winner()*/
					}
					reset_ball();
					score_delay = 10;
				}
			}
			draw_ball(array, true);
		}

		draw_left_paddle(array, true);
		draw_right_paddle(array, true);

		//print_test(array);
		usleep(35000);
		//system("clear");
		//Sleep(25);
		//system("cls");
		/*pass array to higher level function*/

	}
	display_winner();
	/*game end*/
	return 0;
}
