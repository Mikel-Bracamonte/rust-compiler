#!/bin/bash

USE_DOCKER=false
CONTAINER_NAME="gcc-container"

while [[ "$1" =~ ^- ]]; do
    case $1 in
        -b)
            g++ *.cpp -o compiler || exit 1
            ;;
        -w)
            USE_DOCKER=true
            ;;
        *)
            echo "Unknown flag: $1"
            exit 1
            ;;
    esac
    shift
done

if [ -z "$1" ]; then
    echo "input file not defined"
    exit 1
fi

./compiler "$1" || exit 1

if $USE_DOCKER; then
    docker cp input.s "$CONTAINER_NAME":/tmp/input.s
    docker exec "$CONTAINER_NAME" gcc /tmp/input.s -o /tmp/output || exit 1
    docker exec "$CONTAINER_NAME" /tmp/output
else
    gcc input.s -o output || exit 1
    ./output
fi
