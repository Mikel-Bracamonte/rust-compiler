#!/bin/bash

if [ "$1" == "-b" ]; then
    # el g++ debe compilar todos los .cpp
    g++ *.cpp -o compiler || {
        exit 1
    }
    shift
fi
if [ -z "$1" ]; then
    echo "input file not defined"
    exit 1
fi
./compiler "$1" || exit 1
gcc input.s -o output
./output