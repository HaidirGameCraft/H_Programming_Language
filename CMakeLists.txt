cmake_minimum_required(VERSION 3.22)

project(h C CXX)
set(COMPILER_NAME hc)
set(HBYTE_NAME hbyte)
set(LIBRARY_H "h_linker")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
#if( WIN32 )
    #    set(CMAKE_SYSTEM_NAME Windows)
    #    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    #    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    #    set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
#endif()

# if ( USEDEBUG=1 )
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__DEBUG__=1")
# endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

file(GLOB C_SOURCES "./src/*.c" 
                    "./src/ins/*.c"
                    "./src/data/*.c"
                    "./src/libc/*.c"
                    "./src/tools/*.c"
    )

file(GLOB HC_CXX_SOURCES    "./src/compiler/*.cpp"
                            "./src/tools/*.cpp"
                            "./src/hbyte/*.cpp"
                            "./src/compiler/objects/*.cpp"
                        )
file(GLOB HBYTE_CXX_SOURCES "./src/hbyte/*.cpp"
                            "./src/tools/*.cpp")
file(GLOB DBG_CXX_SOURCES   "./src/debug/*.cpp")

add_executable(${PROJECT_NAME} main.c)
add_executable(${COMPILER_NAME} hcompiler.cpp ${HC_CXX_SOURCES})
add_executable(${HBYTE_NAME} hbyte_main.cpp ${HBYTE_CXX_SOURCES})
add_executable(debug ${DBG_CXX_SOURCES} ${HBYTE_CXX_SOURCES})

add_library(${LIBRARY_H} STATIC ${C_SOURCES})
target_include_directories(${LIBRARY_H} PUBLIC "src/")

include_directories(${PROJECT_NAME} PRIVATE "src/")
include_directories(${COMPILER_NAME} PRIVATE "src/")
include_directories(${HBYTE_NAME} PRIVATE "src/")
include_directories(debug PRIVATE "src/")

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARY_H})
target_link_libraries(debug PRIVATE ${LIBRARY_H})

if( WIN32 )
    message("[H Programming Language] Compiling With Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
    target_link_libraries(${COMPILER_NAME} PRIVATE ws2_32)
    target_link_libraries(${HBYTE_NAME} PRIVATE ws2_32)
    target_link_libraries(debug PRIVATE ws2_32)
endif()
