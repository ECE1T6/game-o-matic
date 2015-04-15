#!/bin/bash
# Make certain that this file is placed IN the MAIN directory.

echo "Starting Compilation..."
#hidev libraries
cd hidev
gcc gameomatic.c helpers.c pong.c tetrominoes.c snakeTwoPlayer.c snakeSinglePlayer.c tetrisSinglePlayer.c tetrisTwoPlayer.c -o gameomatic -lm -I . -I ../lodev -std=gnu99
GCC=$?
if [ $GCC != 0 ]; then #compilation failed for some reason
  echo "Hidev Compilation FAILED"
  exit 1;
else 
  echo "...Hidev Compiled Succesfully."
fi

#lodev libraries
cd ../lodev

echo "Compiling low-level libraries..."
gcc -Wall -pthread max72.c displayTest.c lodevInit.c register.c render.c control.c -o displayTest -lwiringPi -std=gnu99
GCC=$?
if [ $GCC != 0 ]; then #sucessful compilation
  echo "Lodev Compilation FAILED"
  exit 1;
else 
  echo "Compiled lodev libraries; executable as 'run'."
  echo "...Finished Succesfully."
fi
exit 0;
