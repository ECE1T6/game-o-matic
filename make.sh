#!/bin/bash
# Make certain that this file is placed IN the MAIN directory.

echo "Starting Compilation..."
#hidev libraries
cd hidev
gcc gameomatic.c helpers.c pong.c tetrisGame.c snakeTwoPlayer.c snakeSinglePlayer.c -o gameomatic -lm -std=gnu99
echo "...Game-O-Matic Compiled."

#lodev libraries
cd ../lodev

echo "Compiling low-level libraries..."
gcc -Wall -pthread displayTest.c lodevInit.c register.c render.c control.c -o displayTest -lwiringPi -std=gnu99
echo "Compiled lodev libraries; executable as 'run'."
echo "...Finished."
exit 0
