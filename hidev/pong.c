#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

//Every other size or position value is based on the following eight. We might add or remove some as we write the program.
#define ARRAY_HEIGHT  48.0
#define ARRAY_WIDTH  64.0
#define VERTICAL_MARGIN  0.0
#define HORIZONTAL_MARGIN  0.0
#define PADDLE_HEIGHT  8.0
#define PADDLE_WIDTH  2.0
#define REFRESH_DELAY 10
#define DISTANCE_PER_REFRESH 1 //not implemented
#define BALL_RADIUS 1.0

#define VERTICAL_END  ARRAY_HEIGHT-VERTICAL_MARGIN-1
#define HORIZONTAL_END  ARRAY_WIDTH-HORIZONTAL_MARGIN-1
/*A few of these things are plus or minus one, pending testing or later scrutiny. I'd rather focus on the logic right now.*/

struct ball{
	float x_coord; /*The coordinates would be rounded for output, but floats let us keep track of movement more accurately.*/
	float y_coord;
	float slope;
	float delta_x;
	float delta_y;
	int radius; /*A 3x3 square has radius 1, 4x4 has radius 2, etc.*/
};
struct paddle{
	int x_coord; /*x_coord is constant. The coordinates refer to the innermost pixel on the top of the paddle.*/
	int y_coord; /*Integers might be better than floats here -- the player should know exactly what the paddle's state is. Digital joysticks, limited visual resolution, and (to a smaller extent) limited refresh rates for reading input make that precision less beneficial anyway. However, if we have a fast refresh rate and want to slow down paddle movement then we can either use floats or ignore movement on some refreshes -- the former sounds better.*/
	int height; /*Height and width are #define'd but it might be preferable to refer to them here.*/
	int width;
};
struct score{
	int left_score;
	int right_score;
};

void draw_ball(struct ball *ball_ptr, bool array[ARRAY_HEIGHT][ARRAY_WIDTH], bool boolean){
	int i, j;
	for(i = 2*ball_ptr->radius; i>=0; i--){
		for(j = 2*ball_ptr->radius; j>=0; j--){
			array[(int)(ball_ptr->y_coord+0.5)-ball_ptr->radius+i][(int)(ball_ptr->x_coord+0.5)-ball_ptr->radius+j] = boolean;
		}
	}
}
void draw_left_paddle(struct ball *left_paddle_ptr, bool array[ARRAY_HEIGHT][ARRAY_WIDTH], bool boolean){
	int i, j;
	for(i = left_paddle_ptr->width-1; i>=0; i--){
		for(j = left_paddle_ptr->height-1; j>=0; j--){
			array[ball_ptr->y_coord+j][ball_ptr->x_coord-i] = boolean;
		}
	}
}
void draw_right_paddle(struct ball *right_paddle_ptr, bool array[ARRAY_HEIGHT][ARRAY_WIDTH], bool boolean){
	int i, j;
	for(i = right_paddle_ptr->width-1; i>=0; i--){
		for(j = right_paddle_ptr->height-1; j>=0; j--){
			array[ball_ptr->y_coord+j][ball_ptr->x_coord+i] = boolean;
		}
	}
}
void check_wall_deflection(struct ball *ball_ptr){
	if(ball_ptr->y_coord - ball_ptr->radius < VERTICAL_MARGIN){ /*Deflection off of the top side*/
		ball_ptr->y_coord = VERTICAL_MARGIN + (VERTICAL_MARGIN - (ball_ptr->y_coord - ball_ptr->radius));
		ball_ptr->delta_y *= -1;
		ball_ptr->slope *= -1;
	}
	else if(ball_ptr->y_coord + ball_ptr->radius > VERTICAL_END){ /*Deflection off of the bottom side*/
		ball_ptr->y_coord = VERTICAL_END - ((ball_ptr->y_coord + ball_ptr->radius) - VERTICAL_END);
		ball_ptr->delta_y *= -1;
		ball_ptr->slope *= -1;
	}
}
void check_left_paddle_impact(struct ball *ball_ptr, struct paddle *left_paddle_ptr){
	/*Checking if the ball hit the inner side of left_paddle: (two conditions for vertical position, one checking if it struck the plane of the paddle this turn)*/
	if(ball_ptr->y_coord + (left_paddle_ptr->x_coord - ball_ptr->x_coord) * ball_ptr->slope >= left_paddle_ptr->y_coord - ball_ptr->radius && ball_ptr->y_coord + (left_paddle_ptr->x_coord - ball_ptr->x_coord) * ball_ptr->slope <= left_paddle_ptr->y_coord + ball_ptr->radius && ball_ptr->x_coord - ball_ptr->delta_x - ball_ptr->radius >= left_paddle_ptr->x_coord){ 
		/*These ignore imparted paddle momentum right now:*/
		ball_ptr->x_coord = left_paddle_ptr->x_coord + (left_paddle_ptr->x_coord - ball_ptr->x_coord);
		ball_ptr->delta_x *= -1;
		ball_ptr->slope *= -1;
	}
}
void check_right_paddle_impact(struct ball *ball_ptr, struct paddle *right_paddle_ptr){
	/*Checking if the ball hit the inner side of right_paddle: (two conditions for vertical position, one checking if it struck the plane of the paddle this turn)*/
	if(ball_ptr->y_coord - (ball_ptr->x_coord - right_paddle_ptr->x_coord) * ball_ptr->slope >= right_paddle_ptr->y_coord - ball_ptr->radius && ball_ptr->y_coord - (ball_ptr->x_coord - right_paddle_ptr->x_coord) * ball_ptr->slope <= right_paddle_ptr->y_coord + ball_ptr->radius && ball_ptr->x_coord - ball_ptr->delta_x + ball_ptr->radius <= right_paddle_ptr->x_coord){
		/*These ignore imparted paddle momentum right now:*/
		ball_ptr->x_coord = right_paddle_ptr->x_coord - (ball_ptr->x_coord - right_paddle_ptr->x_coord);
		ball_ptr->delta_x *= -1;
		ball_ptr->slope *= -1;
	}
}
void reset_ball(struct ball *ball_ptr){
	ball_ptr->x_coord = ARRAY_WIDTH/2;
	ball_ptr->y_coord = rand()%(ARRAY_HEIGHT-2*VERTICAL_MARGIN-2*ball_ptr->radius)+VERTICAL_MARGIN+ball_ptr->radius;
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
/*initialization start*/
	srand(time(NULL));
	int i,j;

	struct ball ball = {ARRAY_WIDTH/2, rand()%(ARRAY_HEIGHT-2*VERTICAL_MARGIN-2*BALL_RADIUS)+VERTICAL_MARGIN+BALL_RADIUS, -1.0, sqrt(0.5), -sqrt(0.5), BALL_RADIUS};
	struct ball *ball_ptr = &ball;

	struct paddle left_paddle = {HORIZONTAL_MARGIN+PADDLE_WIDTH+3, ARRAY_HEIGHT/2-PADDLE_HEIGHT/2, PADDLE_HEIGHT, PADDLE_WIDTH};
	struct paddle *left_paddle_ptr = &left_paddle;

	struct paddle right_paddle = {HORIZONTAL_END-PADDLE_WIDTH-3, ARRAY_HEIGHT/2-PADDLE_HEIGHT/2, PADDLE_HEIGHT, PADDLE_WIDTH};
	struct paddle *right_paddle_ptr = &right_paddle;

	struct score score = {0, 0};
	struct score *score_ptr = &score;

	bool array[ARRAY_HEIGHT][ARRAY_WIDTH] = {false};
/*initialization end*/
/*game start*/
	while (1){
		/*sleep function using REFRESH_DELAY*/
		
		/*Erase objects from array:*/
		draw_ball(ball_ptr, array, false);
		draw_left_paddle(left_paddle_ptr, array, false);
		draw_right_paddle(right_paddle_ptr, array, false);
		
		/*gather input and move paddles*/

		/*Moving the ball, accounting for deflections: (should detect which surface is hit first in a corner)*/
		ball_ptr->y_coord += ball_ptr->delta_y;
		ball_ptr->x_coord += ball_ptr->delta_x;
		
		check_wall_deflection(ball_ptr);
		
		if(ball_ptr->x_coord - ball_ptr->radius < left_paddle_ptr->x_coord){ /*The ball has pierced the plane of left_paddle*/
			
			check_left_paddle_side_deflect(ball_ptr, left_paddle_ptr);
			
			/*Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)*/
			
			/*Checking if the ball hit the left edge of the screen:*/			
			if((int) (ball_ptr->x_coord + 0.5) - ball_ptr->radius < HORIZONTAL_MARGIN){
				score_ptr->left_score++; //Also change displayed score
				if(score_ptr->left_score == 5){
					break; /*Goes to display_winner()*/
				}
				reset_ball(ball_ptr);
			}
		}
		else if(ball_ptr->x_coord + ball_ptr->radius >= right_paddle_ptr->x_coord){ /*The ball has pierced the plane of right_paddle*/
			
			check_right_paddle_side_deflect(ball_ptr, right_paddle_ptr);

			/*Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)*/

			/*Checking if the ball hit the left edge of the screen:*/
			if((int) (ball_ptr->x_coord + 0.5) + ball_ptr->radius > HORIZONTAL_END){
				score_ptr->right_score++; //Also change displayed score
				if(score_ptr->right_score == 5){
					break; /*Goes to display_winner()*/
				}
				reset_ball(ball_ptr);
			}
		}

		draw_ball(ball_ptr, array, true);
		draw_left_paddle(left_paddle_ptr, array, true);
		draw_right_paddle(right_paddle_ptr, array, true);

        	/*pass array to higher level function*/

		/*detect if the ball went off the screen and, if it did, increment score. if the incremented score == 5, break. if someone scored but both scores are < 5, reset the ball and initiate a short countdown before it starts moving again.*/
	}
	display_winner(score_ptr);
/*game end*/
	return 0;
}
