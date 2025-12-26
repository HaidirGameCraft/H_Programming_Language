#!/bin/sh

rm -rf ./bin
sh ./build-windows.sh
sh ./build-linux.sh
echo "[Build Complete] H Programming Language for Windows and Linux has beed complete"
