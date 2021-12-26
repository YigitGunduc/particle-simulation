#!/bin/bash

#set -xe

CC="gcc"
OUTPUT="sim.out"
FLAGS="-Wall -Wextra -Werror -o $OUTPUT -lSDL2"

mkdir ./build/

if [[ $1 == "--water" ]]
  then
  echo "[INFO] compiling the water simulation"
  $CC water.c $FLAGS
  mv ./$OUTPUT ./build/
else
  echo "[INFO] compiling the sand simulation"
  $CC sand.c $FLAGS
  mv ./$OUTPUT ./build/
fi
