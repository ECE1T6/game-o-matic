#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

/*Lodev Libraries*/
#include "screen.h"
#include "lodevInit.h"

/*Hidev Libraries*/
#include "snake.h"
#include "pong.h"
#include "tetris.h"
#include "helpers.h"

int main (void) {
  const int arrayHeight = 38;
  const int arrayWidth = 76;
  bool** ledArray;
  ledArray = make2DArray(arrayHeight, arrayWidth);
  fill2DArray(ledArray, arrayHeight, arrayWidth, false);

  /*Here we will assign the lodev display function to the array and decorate it*/

  int input = 0;
  do {
    printf("Welcome to the game-o-matic!\nEnter \"1\" to play single player Snake!\nEnter \"2\" to play two player Snake!\nEnter \"3\" to play single player Tetris!\nEnter \"4\" to play two player Tetris!\nEnter \"5\" to play Pong!\nEnter \"0\" to exit.");
    scanf("%d", &input);
  } while(input < 0 || input > 5);
  if(input == 1) {
    snakeSinglePlayer(ledArray);
  }
  if(input == 2) {
    snakeTwoPlayer(ledArray);
  }
  if(input == 3) {
    tetrisSinglePlayer(ledArray);
  }
  if(input == 4) {
    tetrisTwoPlayer(ledArray);
  }
  if(input == 5) {
    pong(ledArray);
  }
  free2DArray(ledArray, arrayWidth);
  return 0;
}
