#!/bin/bash
# Make certain that this file is placed IN the HIDEV directory.

echo "Starting Compilation..."
#hidev libraries
cd hidev
gcc -o pong pong.c -lm
echo "Pong Compiled."
gcc -o displayTestLarge displayTestLarge.c -lwiringPi -std=gnu99
echo "displayTestLarge Compiled."
#lowdev libraries

cd ../lodev

gcc -o flushScreen flushScreen.c -lwiringPi
echo "flushScreen Compiled."
gcc -o printScreen printScreen.c -lwiringPi -std=gnu99
echo "printScreen Compiled."
echo "...Finished."
exit 1

