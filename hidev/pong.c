#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Every other size or position value is based on the following eight. We might add or remove some as we write the program.
#define ARRAY_HEIGHT  48
#define ARRAY_WIDTH  64
#define VERTICAL_MARGIN  0
#define HORIZONTAL_MARGIN  0
#define PADDLE_HEIGHT  8
#define PADDLE_WIDTH  2
#define REFRESH_DELAY 10
#define DISTANCE_PER_REFRESH 1 //not implemented
/*
The margins keep track of the amount of the 64x48 array we decide not to use for the game. We don't actually need an array, since we can just keep track of the location and size of the paddles and ball, but we'll imagine a grid anyway. Of several implementations, I think the most efficient would be that we use a coordinate system as if a 64x48 array existed and use the margins to measure the game boundaries. This way, the code's coordinate system is compatible with the output functions without excess arithmetic. The downside is somewhat unintuitive code.
*/

#define VERTICAL_END  ARRAY_HEIGHT-VERTICAL_MARGIN-1
#define HORIZONTAL_END  ARRAY_WIDTH-HORIZONTAL_MARGIN-1
/*A few of these things are plus or minus one, pending testing or later scrutiny. I'd rather focus on the logic right now. A key point is that we're treating the LED array like any other array, and arrays start at 0.*/

struct ball{
    int previous_x_coord; //These keep track of what is lit up, to be changed if (int)x_coord != previous_x_coord
    int previous_y_coord;
    float x_coord; //The coordinates would be rounded for output, but floats let us keep track of movement more accurately.
    float y_coord;
    float slope; /*The ball moves some distance per refresh (a constant speed). The slope defines how it is split into
                   components, which modify x_coord and y_coord. Right now, slope is not necessary but nice.*/
    float delta_x; //These are components of the distance traveled per unit of time.
    float delta_y;
    int radius; /*For testing purposes at least we'll assume the ball to be square -- changing shape is needless and
                  complex. A 3x3 square has radius 1, 4x4 has radius 2, etc. It's possible but unlikely that this would
                  work better as a float. It's common to allow some visual clipping before interactions occur in games,    
                  but with our visual resolution it probably won't work well.*/
};

struct paddle{
    int x_coord; //x_coord is constant. The coordinates might be the innermost pixel on the top of the paddle.
    int y_coord;
    /*Integers might be better than floats here -- the player should know exactly what the paddle's state is. Digital
    joysticks, limited visual resolution, and (to a smaller extent) limited refresh rates for reading input make that
    precision less beneficial anyway. However, if we have a fast refresh rate and want to slow down paddle movement then
    we can either use floats or ignore movement on some refreshes -- the former sounds better.*/
    int height; //Height and width are #define'd but it might be preferable to refer to them here.
    int width;
};

struct score{
    int left_score;
    int right_score;
};

//print_gamestate is for testing purposes
void print_gamestate(struct ball *ball_ptr, struct paddle *left_paddle_ptr, struct paddle *right_paddle_ptr, struct score *score_ptr){
    printf("ball:\nx_coord = %f\ny_coord = %f\nslope = %f\ndelta_x = %f\ndelta_y = %f\nradius = %d\n\n", ball_ptr->x_coord, ball_ptr->y_coord, ball_ptr->slope, ball_ptr->delta_x, ball_ptr->delta_y, ball_ptr->radius);
    printf("left_paddle:\nx_coord = %d\ny_coord = %d\nheight = %d\nwidth = %d\n\n", left_paddle_ptr->x_coord, left_paddle_ptr->y_coord, left_paddle_ptr->height, left_paddle_ptr->width);
    printf("right_paddle:\nx_coord = %d\ny_coord = %d\nheight = %d\nwidth = %d\n\n", right_paddle_ptr->x_coord, right_paddle_ptr->y_coord, right_paddle_ptr->height, right_paddle_ptr->width);
    printf("score:\nleft_score = %d\nright_score = %d\n\n", score_ptr->left_score, score_ptr->right_score);
}

void display_winner(struct score *score_ptr){
    if(score_ptr->left_score == 5){
        //print to screen that the left player won, or do some visual effect
    }
    else{
        //print to screen that the right player won, or do some visual effect
    }
}

int main (void){
//initialization start
    srand(time(NULL));

    struct ball ball = {ARRAY_WIDTH/2, rand()%(ARRAY_HEIGHT-2*VERTICAL_MARGIN)+VERTICAL_MARGIN, -1.0, 'R', sqrt(0.5), -sqrt(0.5), 1};
    struct ball *ball_ptr = &ball;

    struct paddle left_paddle = {HORIZONTAL_MARGIN+PADDLE_WIDTH+3, ARRAY_HEIGHT/2-PADDLE_HEIGHT/2, PADDLE_HEIGHT, PADDLE_WIDTH};
    struct paddle *left_paddle_ptr = &left_paddle;

    struct paddle right_paddle = {HORIZONTAL_END-PADDLE_WIDTH-3, ARRAY_HEIGHT/2-PADDLE_HEIGHT/2, PADDLE_HEIGHT, PADDLE_WIDTH};
    struct paddle *right_paddle_ptr = &right_paddle;

    struct score score = {0, 0};
    struct score *score_ptr = &score;

    //print_gamestate(ball_ptr, left_paddle_ptr, right_paddle_ptr, score_ptr); //for testing purposes
//initialization end
//game start
    while (1){
        //sleep function using REFRESH_DELAY
        //gather input -- move paddles -- have plan if paddle runs into the ball

        //Moving the ball, accounting for deflections: (should detect which surface is hit first in a corner)
        //y_coord:
        ball_ptr->previous_y_coord = (int)ball_ptr->y_coord;
        ball_ptr->y_coord += ball_ptr->delta_y;
        if(ball_ptr->y_coord - ball_ptr->radius < VERTICAL_MARGIN){ //deflection off of the top side
            ball_ptr->y_coord = VERTICAL_MARGIN + (VERTICAL_MARGIN - ball_ptr->y_coord);
            ball_ptr->delta_y *= -1;
            ball_ptr->slope *= -1;
        }
        else if(ball_ptr->y_coord + ball_ptr->radius > VERTICAL_END){ // deflection off of the bottom side
            ball_ptr->y_coord = VERTICAL_END - (ball_ptr->y_coord - VERTICAL_END);
            ball_ptr->delta_y *= -1;
            ball_ptr->slope *= -1;
        }
        //x_coord:
        ball_ptr->previous_x_coord = (int) ball_ptr->x_coord;
        ball_ptr->x_coord += ball_ptr->delta_x;
        if(ball_ptr->x_coord - ball_ptr->radius <= left_paddle_ptr->x_coord){ //possible interaction with left_paddle
            //Checking if the ball hit the inner side of left_paddle:
            if(ball_ptr->y_coord + (left_paddle_ptr->x_coord - ball_ptr->x_coord) * ball_ptr->slope >= left_paddle_ptr->y_coord - ball_ptr->radius &&
               ball_ptr->y_coord + (left_paddle_ptr->x_coord - ball_ptr->x_coord) * ball_ptr->slope <= left_paddle_ptr->y_coord + ball_ptr->radius){

                //These ignore imparted paddle momentum right now:
                ball_ptr->x_coord = left_paddle_ptr->x_coord + (left_paddle_ptr->x_coord - ball_ptr->x_coord);
                ball_ptr->delta_x *= -1;
                ball_ptr->slope *= -1;
            }
            //Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)
            //BUG: conditions will still be checked if the ball passes the paddle -- if it passes successfully, return must be prevented
        }
        else if(ball_ptr->x_coord + ball_ptr->radius >= left_paddle_ptr->x_coord){ //possible interaction with right_paddle
            //Checking if the ball hit the inner side of right_paddle:
            if(ball_ptr->y_coord - (ball_ptr->x_coord - right_paddle_ptr->x_coord) * ball_ptr->slope >= right_paddle_ptr->y_coord - ball_ptr->radius &&
               ball_ptr->y_coord - (ball_ptr->x_coord - right_paddle_ptr->x_coord) * ball_ptr->slope <= right_paddle_ptr->y_coord + ball_ptr->radius){

                //These ignore imparted paddle momentum right now:
                ball_ptr->x_coord = right_paddle_ptr->x_coord - (ball_ptr->x_coord - right_paddle_ptr->x_coord);
                ball_ptr->delta_x *= -1;
                ball_ptr->slope *= -1;
            }
            //Checking if the ball hit the edge of the paddle: (not yet done) (non-critical functionality)
            //BUG: conditions will still be checked if the ball passes the paddle -- if it passes successfully, return must be prevented
        }

        //give signal to commit outputs to the screen, or not as the case may be
        /*detect if the ball went off the screen and, if it did, increment score. if the incremented score == 5,
        break. if someone scored but both scores are < 5, reset the ball and initiate a short countdown before it
        starts moving again.*/
    }
    display_winner(score_ptr);
//game end
    return 0;
}
