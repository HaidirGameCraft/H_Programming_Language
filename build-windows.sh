#!/bin/sh
echo "[Building] H Programming Language for Windows"
if [ ! -d "./build-win" ]; then
    cmake -B ./build-win -DCMAKE_TOOLCHAIN_FILE=../toolchain/toolchain-mingw.cmake
fi

cmake --build ./build-win
cp -rf ./build-win/bin ./
