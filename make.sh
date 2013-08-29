#!/bin/bash
# Make certain that this file is placed IN the HIDEV directory.

echo "Starting Compilation..."
#hidev libraries
cd hidev
gcc -o pong pong.c -lm
echo "Pong Compiled."

#lodev libraries
cd ../lodev

echo "Compiling low-level libraries..."
gcc -Wall -pthread displayTest.c init.c register.c render.c control.c -o displayTest -lwiringPi
echo "Compiled lodev libraries; executable as 'run'."
echo "...Finished."
exit 0
