#!/bin/sh

set -xe

CC="${CXX:-cc}"

gcc ldr.c -o ldr -Wall -ggdb -O3 -std=c11 -pedantic -lraylib
