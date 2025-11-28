cmake_minimum_required(VERSION 3.22)

project(h C CXX)
set(COMPILER_NAME hc)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
#if( WIN32 )
    #    set(CMAKE_SYSTEM_NAME Windows)
    #    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    #    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    #    set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
#endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__DEBUG__=1")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

file(GLOB C_SOURCES "./src/*.c" 
                    "./src/ins/*.c"
                    "./src/data/*.c"
    )

file(GLOB CXX_SOURCES   "./src/*.cpp"
                        "./src/data/*.cpp"
                        "./src/cpp/*.cpp"
                        "./src/tools/*.cpp")

add_executable(${PROJECT_NAME} main.c ${C_SOURCES})
add_executable(${COMPILER_NAME} hcompiler.cpp ${CXX_SOURCES})
include_directories(${PROJECT_NAME} PRIVATE "src/")
include_directories(${COMPILER_NAME} PRIVATE "src/")

if( WIN32 )
    message("[H Programming Language] Compiling With Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
    target_link_libraries(${COMPILER_NAME} PRIVATE ws2_32)
endif()
