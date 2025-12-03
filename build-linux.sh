#!/bin/sh

echo "[Building]: H Programming Language for Linux"

if [ ! -d "./build-linux" ]; then
    cmake -B ./build-linux
fi

cmake --build ./build-linux
cp -rf ./build-linux/bin ./
