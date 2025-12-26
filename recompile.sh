#!/bin/sh
cmake --build ./build-win
cmake --build ./build-linux

rm -rf ./bin
cp -rf ./build-win/bin ./
cp -rf ./build-linux/bin ./
