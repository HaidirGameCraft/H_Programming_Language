echo "[Building]: H Programming Language for Linux"
rm -rf ./build-linux
cmake -B ./build-linux
cmake --build ./build-linux

cp -rf ./build-linux/bin ./
