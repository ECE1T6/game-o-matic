#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
//#include <windows.h>
/*Every other size or position value is based on the following. We might add or remove some as we write the program.*/
#define ARRAY_HEIGHT  48
#define ARRAY_WIDTH  64
#define TOP_MARGIN  0.0 /*The margins bound the playable space in the array -- they might hold things like score*/
#define BOTTOM_MARGIN  0.0
#define LEFT_MARGIN  0.0
#define RIGHT_MARGIN  0.0
#define PADDLE_HEIGHT  8.0
#define PADDLE_WIDTH  2.0
#define BALL_RADIUS  1.0
#define REFRESH_DELAY  10
#define DISTANCE_PER_REFRESH  1.0

#define BOTTOM_END  ARRAY_HEIGHT-BOTTOM_MARGIN-1
#define RIGHT_END  ARRAY_WIDTH-RIGHT_MARGIN-1

struct ball{
	float x_coord; /*The coordinates would be rounded for output, but floats let us keep track of movement more accurately.*/
	float y_coord;
	float slope;
	float delta_x;
	float delta_y;
	float radius; /*A 3x3 square has radius 1, 5x5 has radius 2, etc.*/
};
struct paddle{
	float x_coord; /*x_coord is constant. The coordinates refer to the innermost pixel on the top of the paddle.*/
	float y_coord;
	float height; /*Height and width are #define'd but it might be preferable to refer to them here.*/
	float width;
};
struct score{
	int left_score;
	int right_score;
};

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
void draw_ball(struct ball *ball_ptr, bool array[ARRAY_HEIGHT][ARRAY_WIDTH], bool boolean){
	int i, j;
	for(j = 2*ball_ptr->radius; j>=0; j--){
		for(i = 2*ball_ptr->radius; i>=0; i--){
			if((int) (ball_ptr->y_coord + 0.5 - ball_ptr->radius) + i <= BOTTOM_END /*This might not occur*/
			&& (int) (ball_ptr->y_coord + 0.5 - ball_ptr->radius) + i >= TOP_MARGIN /*This might not occur*/
			&& (int) (ball_ptr->x_coord + 0.5 - ball_ptr->radius) + j <= RIGHT_END
			&& (int) (ball_ptr->x_coord + 0.5 - ball_ptr->radius) + j >= LEFT_MARGIN){
				array[(int)(ball_ptr->y_coord + 0.5 - ball_ptr->radius) + i]
				     [(int)(ball_ptr->x_coord + 0.5 - ball_ptr->radius) + j] = boolean;
			}
		}
	}
}
void draw_left_paddle(struct paddle *left_paddle_ptr, bool array[ARRAY_HEIGHT][ARRAY_WIDTH], bool boolean){
	int i, j;
	for(j = left_paddle_ptr->height-1; j>=0; j--){
		for(i = left_paddle_ptr->width-1; i>=0; i--){
			array[(int) (left_paddle_ptr->y_coord + 0.5) + j][(int) (left_paddle_ptr->x_coord + 0.5) - i] = boolean;
		}
	}
}
void draw_right_paddle(struct paddle *right_paddle_ptr, bool array[ARRAY_HEIGHT][ARRAY_WIDTH], bool boolean){
	int i, j;
	for(j = right_paddle_ptr->height-1; j>=0; j--){
		for(i = right_paddle_ptr->width-1; i>=0; i--){
			array[(int) (right_paddle_ptr->y_coord + 0.5) + j][(int) (right_paddle_ptr->x_coord + 0.5) + i] = boolean;
		}
	}
}
void check_wall_deflection(struct ball *ball_ptr){
	if(ball_ptr->y_coord - ball_ptr->radius < TOP_MARGIN){ /*Deflection off of the top side*/
		ball_ptr->y_coord = TOP_MARGIN + (TOP_MARGIN - (ball_ptr->y_coord - ball_ptr->radius));
		ball_ptr->delta_y *= -1;
		ball_ptr->slope *= -1;
	}
	else if(ball_ptr->y_coord + ball_ptr->radius > BOTTOM_END){ /*Deflection off of the bottom side*/
		ball_ptr->y_coord = BOTTOM_END - ((ball_ptr->y_coord + ball_ptr->radius) - BOTTOM_END);
		ball_ptr->delta_y *= -1;
		ball_ptr->slope *= -1;
	}
}
void check_left_paddle_impact(struct ball *ball_ptr, struct paddle *left_paddle_ptr){
	int ball_impact_y_coord = ball_ptr->y_coord + (left_paddle_ptr->x_coord - ball_ptr->x_coord) * ball_ptr->slope;
	/*Checking if the ball hit the inner side of left_paddle: (two conditions for vertical position, one checking if impact occurred this cycle*/
	if(ball_impact_y_coord + ball_ptr->radius >= left_paddle_ptr->y_coord
	&& ball_impact_y_coord - ball_ptr->radius <= left_paddle_ptr->y_coord + left_paddle_ptr->height - 1
	&& ball_ptr->x_coord - ball_ptr->delta_x - ball_ptr->radius >= left_paddle_ptr->x_coord){
		/*These ignore imparted paddle momentum right now:*/
		ball_ptr->x_coord = left_paddle_ptr->x_coord + (left_paddle_ptr->x_coord - ball_ptr->x_coord);
		ball_ptr->delta_x *= -1;
		ball_ptr->slope *= -1;
	}
}
void check_right_paddle_impact(struct ball *ball_ptr, struct paddle *right_paddle_ptr){
	int ball_impact_y_coord = ball_ptr->y_coord - (ball_ptr->x_coord - right_paddle_ptr->x_coord) * ball_ptr->slope;
	/*Checking if the ball hit the inner side of right_paddle: (two conditions for vertical position, one checking if impact occurred this cycle)*/
	if(ball_impact_y_coord + ball_ptr->radius >= right_paddle_ptr->y_coord
	&& ball_impact_y_coord - ball_ptr->radius <= right_paddle_ptr->y_coord + right_paddle_ptr->height - 1
	&& ball_ptr->x_coord - ball_ptr->delta_x + ball_ptr->radius <= right_paddle_ptr->x_coord){
		/*These ignore imparted paddle momentum right now:*/
		ball_ptr->x_coord = right_paddle_ptr->x_coord - (ball_ptr->x_coord - right_paddle_ptr->x_coord);
		ball_ptr->delta_x *= -1;
		ball_ptr->slope *= -1;
	}
}
void reset_ball(struct ball *ball_ptr){
	ball_ptr->x_coord = LEFT_MARGIN + (RIGHT_END - LEFT_MARGIN) / 2;
	ball_ptr->y_coord = TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2;
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
	int score_delay = 10; /*Deactivates ball movement for some time after someone scores*/

	struct ball ball = {LEFT_MARGIN + (RIGHT_END - LEFT_MARGIN) / 2, TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2, 1.0, DISTANCE_PER_REFRESH * sqrt(0.5), -DISTANCE_PER_REFRESH * sqrt(0.5), BALL_RADIUS};
	struct ball *ball_ptr = &ball;

	struct paddle left_paddle = {LEFT_MARGIN + PADDLE_WIDTH + 3, TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2 - PADDLE_HEIGHT / 2, PADDLE_HEIGHT, PADDLE_WIDTH};
	struct paddle *left_paddle_ptr = &left_paddle;

	struct paddle right_paddle = {RIGHT_END - PADDLE_WIDTH - 3, TOP_MARGIN + (BOTTOM_END - TOP_MARGIN) / 2 - PADDLE_HEIGHT / 2, PADDLE_HEIGHT, PADDLE_WIDTH};
	struct paddle *right_paddle_ptr = &right_paddle;

	struct score score = {0, 0};
	struct score *score_ptr = &score;

	bool array[ARRAY_HEIGHT][ARRAY_WIDTH] = {false};

/*initialization end*/
/*game start*/
	while (1){
		/*sleep function using REFRESH_DELAY*/

		/*Erase paddles from array:*/
		draw_left_paddle(left_paddle_ptr, array, false);
		draw_right_paddle(right_paddle_ptr, array, false);

		/*gather input and move paddles*/

		if (score_delay > 0){
			score_delay--;
		}
		else if(score_delay == 0){
			draw_ball(ball_ptr, array, false); /*Erase ball from array*/
			ball_ptr->y_coord += ball_ptr->delta_y;
			ball_ptr->x_coord += ball_ptr->delta_x;
			check_wall_deflection(ball_ptr);

			if(ball_ptr->x_coord - ball_ptr->radius < left_paddle_ptr->x_coord){ /*The ball has pierced the plane of left_paddle*/

				check_left_paddle_impact(ball_ptr, left_paddle_ptr);

				/*Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)*/

				/*Checking if the ball hit the left edge of the screen:*/
				if((int)(ball_ptr->x_coord + 0.5 - ball_ptr->radius) < LEFT_MARGIN){
					score_ptr->left_score++; /*Also change displayed score*/
					if(score_ptr->left_score == 5){
						break; /*Goes to display_winner()*/
					}
					reset_ball(ball_ptr);
					score_delay = 10;
				}
			}
			else if(ball_ptr->x_coord + ball_ptr->radius >= right_paddle_ptr->x_coord){ /*The ball has pierced the plane of right_paddle*/
                check_right_paddle_impact(ball_ptr, right_paddle_ptr);

				/*Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)*/

				/*Checking if the ball hit the left edge of the screen:*/
				if((int) (ball_ptr->x_coord + 0.5 + ball_ptr->radius) > RIGHT_END){
					score_ptr->right_score++; /*Also change displayed score*/
					if(score_ptr->right_score == 5){
						break; /*Goes to display_winner()*/
					}
					reset_ball(ball_ptr);
					score_delay = 10;
				}
			}
			draw_ball(ball_ptr, array, true);
		}

		draw_left_paddle(left_paddle_ptr, array, true);
		draw_right_paddle(right_paddle_ptr, array, true);

        print_test(array);
	usleep(35000);
	system("clear");
        //Sleep(35);
        //system("cls");

        /*pass array to higher level function*/

	}
	display_winner(score_ptr);
/*game end*/
	return 0;
}
