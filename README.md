game-o-matic
============

RaspberryPi gaming glory: Software to run a 64x48 single-colour LED matrix/display and some basic, classic games. This project will be divided up into 2 software sections:
- High-level programming: will include games (starting with Pong!) and whatever else we want.
- Low-level programming: writing a library that will provide the key functions + hardware interface

IN THE WORKS:

-int printScreen(bool** matrixPtr, pthread_t *tid) -- prints the malloc'd array to the LED display

-void flushScreen (void); -- blanks the screen for shutdown / game end / exit

-void WriteBoard(int col, int row, char* str, size); -- writes string/text and scales to req. size in the matrix for output

-void pong(bool** array, struct ctlData *ctl); -- The pong game

-void snake(bool** array, struct ctlData *ctl); -- The snake game

-void gameOMatic(void); -- wrapper program; calls the required init functions and acts as a menu for games + displaying tweets.


In addition, we need to write interface functions to sample the input from the joystick and provide this information to the high-level programmers conveniently.

eg.,

-int controller(struct ctlData *ctl, pthread_t *tid); -- passes the current position/value of the USB/GPIO controller to the game
