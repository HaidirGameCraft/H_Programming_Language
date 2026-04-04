#pragma once

#include <raylib.h>
#include <string>

class EmulatorWindow {
public:
    int width;
    int height;
    std::string title;
    bool isRun = false;
    EmulatorWindow( const std::string& title, int width, int height );
    void Init();
    void Run();
};