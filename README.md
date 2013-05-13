game-o-matic
============

RaspberryPi gaming glory: Software to run a 64x48 single-colour LED matrix/display and some basic, classic games. This project will be divided up into 2 software sections:
- High-level programming: will include games (starting with Pong!) and whatever else we want.
- Low-level programming: writing a library that will provide the key functions + hardware interface

IN THE WORKS:

void lightOn (int col, int row);

void lightOff (int col, int row);

void rowOn (int row);

void colOn (int col);

void flushScreen (void);

void write (char* str); //writes str to the screen

In addition, we need to write interface functions to sample the input from the joystick and provide this information to the high-level programmers conveniently.
