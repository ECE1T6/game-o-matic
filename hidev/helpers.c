/*Functions usable by all games in gameomatic libary*/

bool** makeArray(float HEIGHT, float WIDTH) {
  int i;
  bool** ledArray = (bool**) malloc(HEIGHT*sizeof(bool*));
  for (i = 0; i < HEIGHT; i++) {
    ledArray[i] = (bool*) malloc(WIDTH*sizeof(bool));
  }
  return ledArray;
}

void fillArray(bool** ledArray, float HEIGHT, float WIDTH, bool lightsOn) {
  int i, j;
  for(i = 0; i < HEIGHT; i++) {
    for(j = 0; j < WIDTH; j++) {
      ledArray[i][j] = lightsOn;
    }
  }
  return;
}

freeArray(bool** ledArray) {
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

int getLeftButton(unsigned char player){
	int state = 0;
	if (player & (1 << 1)){
		state = 1;
	}
	return state;
}
int getRightButton(unsigned char player){
	if (player & (1 << 0)){
		state = 1;
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


