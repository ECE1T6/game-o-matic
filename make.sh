#!/bin/bash
# Make certain that this file is placed IN the HIDEV directory.

echo "Starting Compilation..."
#hidev libraries

gcc -o pong pong.c -lm
gcc -o displayTestLarge displayTestLarge.c -lwiringPi -std=gnu99

#lowdev libraries

cd ..
cd lodev

gcc -o flushScreen flushScreen.c -lwiringPi
gcc -o printScreen printScreen.c -lwiringPi -std=gnu99

echo "...Finished."
exit 1

