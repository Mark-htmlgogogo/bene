#!/bin/bash

if [ ${1:-UNIX} == "WIN" ]; then
    CC="i586-mingw32msvc-gcc"
    EXT=".exe"
else
    CC=g++
    #CC=c99
fi

#CFLAGS="-Wextra -g -ansi -pedantic"
#CFLAGS="-Wextra -g -ansi"
CFLAGS="-Wall -O0 -pg -g3" # for debugging
#CFLAGS="-Wall  -O3"

$CC $CFLAGS -o greedy$EXT greedy.cpp read_files.cpp
