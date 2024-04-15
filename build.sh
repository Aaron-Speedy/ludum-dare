#!/bin/sh

set -xe

CC="${CXX:-cc}"
CFLAGS="-Wall -ggdb -O3 -std=c11 -pedantic"

$CC -c maze.c $CFLAGS -lraylib
$CC ldr.c $CFLAGS -o ldr -lraylib maze.o
