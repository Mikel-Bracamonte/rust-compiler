#!/bin/bash

if [ "$1" == "-b" ]; then
    g++ main.cpp exp.cpp parser.cpp scanner.cpp token.cpp visitor.cpp -o compiler || {
        exit 1
    }
    shift
fi
if [ -z "$1" ]; then
    echo "input file not defined"
    exit 1
fi
./compiler "$1" || exit 1
gcc output.s -o exec
./exec