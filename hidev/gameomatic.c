#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

/*Lodev Libraries*/
#include "screen.h"
#include "lodevInit.h"

/*Hidev Libraries*/
#include "snakeSinglePlayer.h"
#include "snakeTwoPlayer.h"
#include "pong.h"
#include "tetris.h"
#include "helpers.h"

/*Variables used by all games:*/
const float ARRAY_HEIGHT = 38.0;
const float ARRAY_WIDTH = 76.0;
float TOP_MARGIN; /*The margins bound the playable area of the game.*/
float BOTTOM_MARGIN;
float LEFT_MARGIN;
float RIGHT_MARGIN;
float BOTTOM_END; /*= ARRAY_HEIGHT - BOTTOM_MARGIN - 1.0*/
float RIGHT_END; /*= ARRAY_WIDTH - RIGHT_MARGIN - 1.0*/

int main (void) {
  bool** ledArray;
  ledArray = makeArray(ARRAY_HEIGHT, ARRAY_WIDTH);
  fillArray(ledArray, ARRAY_HEIGHT, ARRAY_WIDTH, false);

  /*Here we will assign the lodev display function to the array and decorate it*/

  int input = 0;
  do {
    printf("Welcome to the game-o-matic!\nEnter \"1\" to play pong!\nEnter \"2\" to play snake!\nEnter \"0\" to exit.");
    scanf("%d", &input);
  } while(input < 0 || input > 2);
  if(input == 1) {
    pong(ledArray);
  }
  if(input == 2) {
    snake(ledArray);
  }
  freeArray(ledArray);
  return 0;
}
