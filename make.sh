#!/bin/bash
# Make certain that this file is placed IN the MAIN directory.

echo "Starting Compilation..."
#hidev libraries
cd hidev
gcc gameomatic.c helpers.c pong.c tetrisGame.c snakeTwoPlayer.c snakeSinglePlayer.c -o gameomatic -lm -std=gnu99
GCC=$?
if [ $GCC == 2 ]; then #compilation failed for some reason
  echo "Hidev Compilation FAILED"
  exit 1;
elif [ $GCC != 0 ]; then #compilation had warnings
  echo "...Hidev Compiled, with warnings"
else 
  echo "...Hidev Compiled Succesfully."
fi

#lodev libraries
cd ../lodev

echo "Compiling low-level libraries..."
gcc -Wall -pthread displayTest.c lodevInit.c register.c render.c control.c -o displayTest -lwiringPi -std=gnu99
GCC=$?
if [ $GCC == 2 ]; then #sucessful compilation
  echo "Lodev Compilation FAILED"
  exit 1;
elif [ $GCC != 0 ]; then #compilation had warnings
  echo "...Lodev Compiled, with warnings"
else 
  echo "Compiled lodev libraries; executable as 'run'."
  echo "...Finished Succesfully."
fi
exit 0;
