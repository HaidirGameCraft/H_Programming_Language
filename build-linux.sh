#!/bin/sh

echo "[Building]: H Programming Language for Linux"

if [ ! -d "./build-linux" ]; then
    set debug=0
    while getopts debug: option
    do
        case "${option}" in
            debug) debug=1
        esac
    done
    echo "Compiling use Debug=$debug"
    cmake -B ./build-linux -DUSEDEBUG=$debug
    unset debug
fi

cmake --build ./build-linux
cp -rf ./build-linux/bin ./
