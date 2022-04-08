#! /usr/bin/env bash

FILE1=./research/tmp.so
FILE2=./src/mpi
DIR1=./build
DIR2=./research/__pycache__
DIR3=./.vscode

if test -f "$FILE1"; then
    echo "$FILE1 exists"
    rm "$FILE1"
fi

if test -f "$FILE2"; then
    echo "$FILE2 exists"
    rm "$FILE2"
fi

if test -d "$DIR1"; then
    echo "directory $DIR1 exits"
    rm -r "$DIR1"
fi

if test -d "$DIR2"; then
    echo "directory $DIR2 exits"
    rm -r "$DIR2"
fi

if test -d "$DIR3"; then
    echo "directory $DIR3 exits"
    rm -r "$DIR3"
fi