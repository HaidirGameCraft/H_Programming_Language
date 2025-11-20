cmake_minimum_required(VERSION 3.22)

project(h C CXX)
set(COMPILER_NAME hc)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")


file(GLOB C_SOURCES "./src/*.c" 
                    "./src/ins/*.c"
                    "./src/data/*.c"
    )

file(GLOB CXX_SOURCES   "./src/*.cpp"
                        "./src/data/*.cpp"
                        "./src/cpp/*.cpp")

add_executable(${PROJECT_NAME} main.c ${C_SOURCES})
add_executable(${COMPILER_NAME} hcompiler.cpp ${CXX_SOURCES})
include_directories(${PROJECT_NAME} PRIVATE "src/")
include_directories(${COMPILER_NAME} PRIVATE "src/")