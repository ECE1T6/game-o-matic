#!/bin/bash
# Make certain that this file is placed IN the MAIN directory.

echo "Starting Compilation..."
#hidev libraries
cd hidev
gcc gameomatic.c helpers.c pong.c tetrisGame.c snakeTwoPlayer.c snakeSinglePlayer.c -o gameomatic -lm -std=gnu99
echo "...Game-O-Matic Compiled."
GCC = $?
if [ $RET1 -ne 0 ]; then #compilation failed for some reason
  echo "Hidev Compilation FAILED"
  exit 1;
fi

#lodev libraries
cd ../lodev

echo "Compiling low-level libraries..."
gcc -Wall -pthread displayTest.c lodevInit.c register.c render.c control.c -o displayTest -lwiringPi -std=gnu99
GCC = $?
if [ $GCC -eq 0 ]; then #sucessful compilation
  echo "Compiled lodev libraries; executable as 'run'."
  echo "...Finished Succesfully."
else #compilation of lodev failed!
  echo "Lodev Compilation FAILED"
fi
exit $GCC
