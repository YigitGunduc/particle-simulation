#!/bin/bash

CC="gcc"
RUN=true
LOG=false
STRICT=false
LIBS="-lSDL2"
OUTPUT="sim.out"
BUILD_DIR="./build/"
CFLAGS="-Wall -Wextra -o $OUTPUT"

if [ "$LOG" == true ]
  then  
  echo "[INFO] logger is active"
  set -xe
fi

if [ "$STRICT" == true ]
  then  
  echo "[INFO] will compile on the strict mode(-Werror)"
  CFLAGS="${CFLAGS} -Werror"
fi

if [ ! -d "$BUILD_DIR" ];
then
  mkdir ./$BUILD_DIR/
fi

if [[ $1 == "--water" ]]
  then
  echo "[INFO] compiling the water simulation"
  SOURCE="src/water.c"
else
  echo "[INFO] compiling the sand simulation"
  SOURCE="src/sand.c"
fi

$CC $SOURCE $CFLAGS $LIBS
mv ./$OUTPUT ./$BUILD_DIR/

if [ "$RUN" == true ]
  then  
  echo "[INFO] running the executable"
  ./$BUILD_DIR/$OUTPUT
fi
