
#ifdef WIN32
#else
#include <sys/ioctl.h>
#endif

#include <term.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef WIN32
HANDLE hstdin = NULL;
HANDLE hstdout = NULL;
DWORD  modein_backup = 0;
DWORD  modeout_backup = 0;
CONSOLE_SCREEN_BUFFER_INFO csbf;
#else
struct termios backup_term;
struct winsize ws;
#endif

int cursor_rows = 0; // Indicate from UP to DOWN 
int cursor_cols = 0; // Indicate from LEFT to RIGHT

void enableRawMode() {

#ifdef WIN32
    hstdin = GetStdHandle( STD_INPUT_HANDLE );
    hstdout = GetStdHandle( STD_OUTPUT_HANDLE );

    DWORD newmodein, newmodeout;
    GetConsoleMode( hstdin, &modein_backup );
    GetConsoleMode( hstdout, &modeout_backup );

    newmodein = modein_backup;
    newmodeout = modeout_backup;

    newmodein  = newmodein & ~( ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT );
    newmodeout = newmodeout & ( ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT );
    SetConsoleMode( hstdin, newmodein );
    SetConsoleMode( hstdout, newmodeout );

#else
// Linux
    struct termios new_term;
    tcgetattr( STDIN_FILENO, &backup_term );
    new_term = backup_term;

    // disable ECHO and ICANON which meant disable the useful of echo
    // ICANON mean when user press the key on keyboard it give single char ( not using enter )
    new_term.c_lflag = new_term.c_lflag & ~( ECHO | ICANON );

    // set the new termios to attribute and change it immediately
    tcsetattr( STDIN_FILENO, TCSANOW, &new_term);
#endif

    // When the program is kill or terminate, the disableRawMode will run
    atexit( disableRawMode );
    
}

void disableRawMode() {
#ifdef WIN32
    SetConsoleMode(hstdin, modein_backup);
    SetConsoleMode(hstdout, modeout_backup);
#else
    // set the backup termios immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &backup_term);
#endif
}

void InitTerminal() {

}

void GetPosCursorTerminal(int *rows, int* cols) {
    *rows = cursor_rows;
    *cols = cursor_cols;
}

void SetPosCursorTerminal(int rows, int cols) {
    cursor_rows = rows;
    cursor_cols = cols;
}

void GetSizeTerminal(int *width, int *height) {
#ifdef WIN32
    GetConsoleScreenBufferInfo(hstdout, &csbf);
    *width = csbf.dwSize.X;
    *height = csbf.dwSize.Y;
#else
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    *width = ws.ws_col;
    *height = ws.ws_row;
#endif
}













