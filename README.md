game-o-matic
============

RaspberryPi gaming glory: Software to run a 64x48 single-colour LED matrix/display and some basic, classic games. This project will be divided up into 2 software sections:
- High-level programming: will include games (starting with Pong!) and whatever else we want.
- Low-level programming: writing a library that will provide the key functions + hardware interface

A lodev libraries <a href="https://docs.google.com/document/d/19enZnzPExQ6_HpwOZU4J00wAjfWi3ineDXp8A0TtiM0/edit">spec. sheet</a>: https://docs.google.com/document/d/19enZnzPExQ6_HpwOZU4J00wAjfWi3ineDXp8A0TtiM0/edit

IN THE WORKS:

void lightOn (int col, int row);

void printScreen(bool** matrixPtr);

void flushScreen (void);

void write (int col, int row, char* str); //writes str to the matrix in preperation for output

In addition, we need to write interface functions to sample the input from the joystick and provide this information to the high-level programmers conveniently.
