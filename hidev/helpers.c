/*Functions usable by all games in gameomatic libary*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

void freezeFrame(unsigned int frames) {
  //Call a sleep function roughly the length of a frame, About 15000us = 1 frame.
  const unsigned int usecs = 15000;
  usleep((usecs*frames));
}

void printScore(int score, char *position) { //This is a placeholder for a lodev function
  /*Valid positions are: MID, MID_BOT, MID_TOP, MID_LEFT, MID_RIGHT,
 			TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT.*/
  if (strcmp (position, "MID") == 0){
  	
  }
  else if(strcmp (position, "MID_BOT") == 0){
  	
  }
  else if(strcmp (position, "MID_TOP") == 0){
  	
  }
  else if(strcmp (position, "MID_LEFT") == 0){
  	
  }
  else if(strcmp (position, "MID_RIGHT") == 0){
  	
  }
  else if(strcmp (position, "TOP_LEFT") == 0){
  	
  }
  else if(strcmp (position, "TOP_RIGHT") == 0){
  	
  }
  else if(strcmp (position, "BOT_LEFT") == 0){
  	
  }
  else if(strcmp (position, "BOT_RIGHT") == 0){
  	
  }
  return;
}

bool** make2DArray(float HEIGHT, float WIDTH) {
  int i;
  bool** ledArray = (bool**) malloc(HEIGHT*sizeof(bool*));
  for (i = 0; i < HEIGHT; i++) {
    ledArray[i] = (bool*) malloc(WIDTH*sizeof(bool));
  }
  return ledArray;
}

void fill2DArray(bool** ledArray, float HEIGHT, float WIDTH, bool lightsOn) {
  int i, j;
  for(i = 0; i < HEIGHT; i++) {
    for(j = 0; j < WIDTH; j++) {
      ledArray[i][j] = lightsOn;
    }
  }
  return;
}

free2DArray(bool** ledArray) {
  int i;
  for (i = 0; i < ARRAY_HEIGHT; i++) {
    free(ledArray[i]);
  }
  free(ledArray);
  return;
}

void drawRectangle(bool** ledArray, bool lightsOn, int topY, int leftX, int HEIGHT, int WIDTH) {
  int i, j;
  for(j = HEIGHT - 1; j >= 0; j--) {
    for(i = WIDTH - 1; i >= 0; i--) {
      ledArray[topY + j][leftX + i] = lightsOn;
    }
  }
  return;
}

int getJoystick(unsigned char player) { 
	if (player & (1 << 2)){
			if (player & (1 << 5)){
				return 8;
				}
			if (player & (1 << 1)){
				return 2;
				}
			return 1;
	}
	if (player & (1 << 4)){
			if (player & (1 << 5)){
				return 6;
				}
			if (player & (1 << 1)){
				return 4;
				}
			return 5;
	}
	if (player & (1 << 1)){
		return 3;
	}
	if (player & (1 << 5)){
		return 7;
	} 
	return 0;
}

bool getLeftButton(unsigned char player){
	bool state = false;
	if (player & (1 << 1)){
		state = true;
	}
	return state;
}
bool getRightButton(unsigned char player){
	bool state = false;
	if (player & (1 << 0)){
		state = true;
	}
	return state;
}

void printTest(bool** ledArray) {
  int i, j;
  for(i = TOP_MARGIN; i <= BOTTOM_END; i++) {
    for(j = LEFT_MARGIN; j <= RIGHT_END; j++) {
      if(ledArray[i][j] == true) {
        printf("O", ledArray[i][j]);
      }
      else {
        printf("*", ledArray[i][j]);
      }
    }
    printf("\n");
  }
  return;
}

void frameTest(bool** ledArray, float TOP_MARGIN, float LEFT_MARGIN, float BOT_END, float RIGHT_END) {
  //Windows:
  //Sleep(5);
  //system("cls");
  //Linux:
  usleep(50000);
  system("clear");

  printTest(ledArray, TOP_MARGIN, LEFT_MARGIN, BOT_END, RIGHT_END);
}

