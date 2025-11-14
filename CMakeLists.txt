cmake_minimum_required(VERSION 3.22)

project(emulator)

file(GLOB C_SOURCES "./src/*.c" 
                    "./src/ins/*c"
    )

add_executable(${PROJECT_NAME} main.c ${C_SOURCES})
include_directories(${PROJECT_NAME} PRIVATE "src/")