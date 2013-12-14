/*Functions usable by all games in gameomatic libary*/
#include "helpers.h"

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

bool** make2DArray(int height, int width) {
  int i;
  bool** ledArray = (bool**) malloc(height*sizeof(bool*));
  for (i = 0; i < height; i++) {
    ledArray[i] = (bool*) malloc(width*sizeof(bool));
  }
  return ledArray;
}

void fill2DArray(bool** ledArray, int height, int width, bool lightsOn) {
  int i, j;
  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      ledArray[i][j] = lightsOn;
    }
  }
  return;
}

void free2DArray(bool** ledArray, int height) {
  int i;
  for (i = 0; i < height; i++) {
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

int* make1DArray(int LENGTH) {
  int* longArray = (int*) malloc(LENGTH*sizeof(int));
  int i;
  for(i = 0; i < LENGTH; i++) {
    longArray[i] = -1;
  }
  return longArray;
}

void drawCheckerboard(bool** ledArray, int topY, int leftX, int HEIGHT, int WIDTH) {
  int i, j;
  for(j = HEIGHT - 1; j >= 0; j--) {
    for(i = WIDTH - 1; i >= 0; i--) {
      if((j + i) % 2 == 0) {
        ledArray[topY + j][leftX + i] = true;
      }
      else {
        ledArray[topY + j][leftX + i] = false;
      }
    }
  }
  return;
}

void printTest(bool** ledArray, float topMargin, float leftMargin, float botEnd, float rightEnd) {
  int i, j;
  for(i = topMargin; i <= botEnd; i++) {
    for(j = leftMargin; j <= rightEnd; j++) {
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

void frameTest(bool** ledArray, float topMargin, float leftMargin, float botEnd, float rightEnd) {
  //Windows:
  //Sleep(5);
  //system("cls");
  //Linux:
  usleep(50000);
  system("clear");

  printTest(ledArray, topMargin, leftMargin, botEnd, rightEnd);
}

