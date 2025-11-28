echo "[Building] H Programming Language for Windows"
rm -rf ./build-win
cmake -B ./build-win -DCMAKE_TOOLCHAIN_FILE=../toolchain/toolchain-mingw.cmake
cmake --build ./build-win

cp -rf ./build-win/bin ./
