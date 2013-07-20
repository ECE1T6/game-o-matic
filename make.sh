#!/bin/bash
# Make certain that this file is placed IN the HIDEV directory.

echo "Starting Compilation..."
#hidev libraries
cd hidev
gcc -o pong pong.c -lm
echo "Pong Compiled."
gcc -o displayTest displayTest.c -lwiringPi -std=gnu99
echo "displayTest Compiled."
#lowdev libraries

cd ../lodev

gcc -o flushScreen flushScreen.c -lwiringPi
echo "flushScreen Compiled."
gcc -o printScreen printScreen.c -lwiringPi -std=gnu99
echo "printScreen Compiled."
gcc -o getControl getControl.c -lwiringPi -std=gnu99
echo "...Finished."
exit 1

