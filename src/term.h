#pragma once

/**
 * Terminal
 */


#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif


#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

EXTERN void enableRawMode();
EXTERN void disableRawMode();
EXTERN void InitTerminal();
EXTERN void GetPosCursorTerminal(int *rows, int* cols);
EXTERN void SetPosCursorTerminal(int rows, int cols);
EXTERN void GetSizeTerminal(int *width, int *height);
