#include "hdebug.h"
#include <term.h>

#include <unistd.h>
#include <inst.h>
#include <string.h>
#include <vector>
#include <cpu.h>
#include <tools/memory.h>

#include "instruction_decode.h"

typedef struct {
    uint32_t address;
} code_breakpoint_t;

typedef struct {
    uint32_t address;
    char* text;
    uint8_t breakpoint_flags;
} instruction_text_t;

extern "C" cpu_register_t reg;
extern "C" uint8_t is_program_running;

FILE* debug_file = NULL;

int text_lines = 0;
int text_cols = 0;
#ifdef WIN32
HANDLE hstdin = NULL;
HANDLE hstdout = NULL;
DWORD in_mode = 0;
DWORD out_mode = 0;
#else
struct termios original_termios;
#endif

int id_window = 0; // (Default) Disassembly Program Windows

static uint8_t* memory = NULL;
uint32_t max_size_memory = 0;
std::vector<uint8_t> instruction_size_list;
static std::vector<instruction_text_t> instruction_text;

static std::vector<code_breakpoint_t> breakpoints;
static int length_memory = 0;

char* command_terminal();
void display_memory(int x, int y, int width, int height, char key);
void display_register(int start_x, int width);
uint32_t read_instruction( uint32_t pc );
void get_terminal_size(int* width, int* height);

void debug_open_file(const char* filename) {
    debug_file = fopen(filename, "rb+");
    if( !debug_file )
        ASSERT("The File is not defined");
}

void debug_read_file(uint32_t size_memory) {

    fseek(debug_file, 0, SEEK_END );
    long size = ftell( debug_file );
    fseek(debug_file, 0, SEEK_SET );

    memory = (uint8_t*) malloc( size_memory );
    fread( memory, size, 1, debug_file ); 
    length_memory = size;

    for(uint32_t index = 0; index < length_memory;)
    {
        uint32_t tmp_index = index;
        index = read_instruction( index );
        uint8_t size_instruction = index - tmp_index;

        char* inst_text = instruction_decode( memory, tmp_index, size_instruction );
        if( inst_text != NULL ) {
            instruction_text.push_back( { tmp_index, inst_text, 0 } );
        }
        instruction_size_list.push_back( size_instruction );
    }

    cpu_initialize();
    cpu_set_memory( memory, size_memory );
    draw_terminal();

    free( memory );
}

void debug_close_file() {
    fclose( debug_file );

    for(int i = 0; i < instruction_text.size(); i++ )
        __free( instruction_text[i].text );
}

uint32_t read_instruction( uint32_t pc ) {
    uint8_t prefix = read8( memory, &pc );
    uint8_t ext_prefix = 0;
    if( prefix & PREFIX_EXT_INC )
        ext_prefix = read8( memory, &pc );
    uint8_t opcode = read8( memory, &pc );
    uint8_t reg = 0;
    uint8_t sym = 0;
    uint16_t offset = 0;
    uint32_t value = 0;

    if( prefix == 0x0F )
        return pc;

    if( prefix & PREFIX_REG_INC ) {
        reg = read8( memory, &pc );
    }

    if( prefix & PREFIX_OFF_INC ) {
        offset = read16( memory, &pc );
    }
    else if ( prefix & PREFIX_SYM_INC ) {
        sym = read8( memory, &pc );
    }

    if( prefix & PREFIX_VAL_INC )
        value = read32( memory, &pc );

    return pc;
}

char* command_terminal() {
    int width = 0;
    int height = 0;
    get_terminal_size(&width, &height);
    int half_height = (int)( height / 2 );

    int start_y = half_height;

    char* buffer = (char*) __malloc( 1024 );
    int index_buffer = 0;
    int length_buffer = 1024;
    memset( buffer, 0, 1024 );

    int rows = 1;
    int columns = 1;

    while( true ) {
        GetSizeTerminal(&width, &height);
        half_height = (int)( height / 2 );

        printf("\x1b[%i;%iH\x1b[0J", height, 0);
        printf("-> %s", buffer);
        char key = getchar();

        if( key == 27 )
            break;

        if( key == 0x0a )   // Enter Key
        {

            // breakpoint code command
            if( strncmp( buffer, "bp", 2) == 0 )
            {
                uint32_t bp_addr = 0;
                if( sscanf(buffer, "bp 0x%x", &bp_addr) != 0 )
                {
                   for(auto& ins_text : instruction_text )
                   {
                        if( ins_text.address == bp_addr )
                        {
                            ins_text.breakpoint_flags = 1;
                        }
                   }
                }
            } else if ( strncmp( buffer, "go", 2) == 0 )
            {
                uint32_t addr = 0;
                if( sscanf( buffer, "go 0x%x", &addr) != 0 )
                {

                    for(int i = 0; i < instruction_text.size(); i++ )
                    {
                        uint32_t range = addr - instruction_text[i].address;
                        if( instruction_text[i].address == addr )
                        {
                            text_lines = i;
                            break;
                        }
                    }
                }
            }
            else {
                printf("\x1b[%i;%iH\x1b[0J", height, 0);
                printf("Unknown Debug Command Line");
            }


            // clearing the buffer user input
            memset( buffer, index_buffer, 0 );
            break;
        }

        if( key >= ' ' && key < 127 && index_buffer < length_buffer )
        {
            printf("%c", key);
            buffer[ index_buffer ] = key;
            index_buffer++;
            if( columns >= width )
            {
                rows++;
                columns = 0;
            }
            else
                columns++;
        }


        fflush( stdout );
        usleep(1000);
    }

    __free( buffer );
    return NULL;
}


void draw_terminal() {
    enableRawMode();
    int width = 0;
    int height = 0;
    GetSizeTerminal(&width, &height);
    printf("\x1b[?1049h");

    int cursor_rows = 1;
    int cursor_cols = 1;

    int third_width = (int)( width / 3 );
    int half_height = (int)( height / 2 );
    
    int lines_instruction_pointer = 0;

    int total_size_outer_instruction = 0;
    while( true ) {
        GetSizeTerminal(&width, &height);
        third_width = (int)( width / 3 );
        half_height = (int)( height / 2 );


        if( id_window != 0 )
        {
            char key = getchar();
            if( key == 'q' )
                break;

            if( key == 'w' )
            {
                char skey = getchar();
                if( skey == 'w' )
                {
                    id_window++;
                    if( id_window >= 2 )
                        id_window = 0;
                }
            }

            if( id_window == 1 )
                display_memory( third_width * 2, 1, third_width, half_height, key);
            continue;
        }

        printf("\x1b[%i;%iH\x1b[1J", half_height, width);

        printf("\x1b[H");
        for(int i = 0; i < width; i++)
            printf("=");
        printf("\x1b[1000D\x1b[1B");

        if ( cursor_rows <= 0 )
        {
            cursor_rows = 1;
            text_lines--;
            if( text_lines < 0 )
            {
                text_lines = 0;
                total_size_outer_instruction = 0;
            }

        } else if ( cursor_rows >= half_height ) {
            cursor_rows = half_height;
            text_lines++;
        }

        for(int _i = text_lines; _i < instruction_text.size(); _i++) {
            int __t = _i - text_lines;

            if( __t >= half_height )
                break;

            if( _i == instruction_text.size() - 1 )
                break;

            if( _i < instruction_text.size() )
            {
                if( instruction_text[_i].breakpoint_flags == 1 || lines_instruction_pointer == _i )
                    printf(" ->");
                else
                    printf("   ");
                printf("%s", instruction_text[_i].text );
            }
            printf("\x1b[1000D\x1b[1B");
        }

        // display the register
        display_register( third_width, third_width );
        display_memory( third_width * 2, 1, third_width, half_height, 0 );

        // set the cursor position
        printf("\x1b[%i;%iH", cursor_rows, cursor_cols );

        char key = getchar();
        if( key == 27 ) // ESCAPE Key
        {
            char key_1 = getchar();
            char key_2 = getchar();

            // TODO
        }

        if( key == ';' )
        {
            command_terminal();
        }
        else if ( key == 'w' )
        {
            char second_key = getchar();
            if( second_key == 'w' )
            {
                id_window++;
                // The value will be change
                if( id_window >= 2 )
                    id_window = 0;
            }
        }
        else if ( key == 'n' )  // Next 1 step
        {
            cpu_execute( 1 );
            if( is_program_running == false )
                break;
            int index_ins_text = 0;
            for(const auto& ins_text : instruction_text )
            {
                if( ins_text.address == reg.pc )
                {
                    if( index_ins_text <= half_height )
                        text_lines = 0;
                    else 
                        text_lines = index_ins_text;
                    
                    lines_instruction_pointer = index_ins_text;
                    break;
                }
                index_ins_text++;
            }
            
        } else if ( key == 'c' ) // Continue until breakpoint or stp instruction
        {
            
            while( true )
            {
                cpu_execute( 1 );
                if( !is_program_running )
                    break;

                int index_ins_text = 0;
                bool __breakpoint_break = false;    
                for(const auto& ins_text : instruction_text )
                {
                    if( ins_text.address == reg.pc )
                    {
                        if( index_ins_text <= half_height )
                            text_lines = 0;
                        else 
                            text_lines = index_ins_text;
                    
                        lines_instruction_pointer = index_ins_text;
                        if( ins_text.breakpoint_flags == 1 )
                            __breakpoint_break = true;
                        break;
                    }
                    index_ins_text++;
                }

                if( __breakpoint_break )
                    break;
            }

            if( !is_program_running )
                break;
        }

        if( key == 'q' )
            break;

        if( key == 'i' || key == 'I' )
            cursor_rows--;
        else if ( key == 'k' || key == 'K' )
            cursor_rows++;

        fflush(stdout);
        usleep(1000);
        
    }
    printf("\x1b[?1049l");

    if( is_program_running )
        printf("The Program has been Stopped\n");
}

int main(int argc, const char* argv[]) {
    const char* file = NULL;
    uint32_t memory_size = 0x100000; // 1MiB Memory Size ( Default )
    for(int i = 0; i < argc; i++) {
        if( strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0 )
        {
            printf("usage: %s [OPTIONS] <binary_file.ho>; -> debug the binary file\n", argv[0] );
            printf("help: %s -h|--help\n", argv[0] );
            printf("===============================\n");
            printf("[OPTIONS]:\n");
            printf("-h      --help          : help | details \n");
            return EXIT_SUCCESS;
        } else if( strcmp("-rsize", argv[i]) == 0 ) {
            if( i + 1 >= argc )
            {
                printf("failed: the value is not insert or non");
                return EXIT_FAILURE;
            }

            sscanf(argv[i + 1], "%i", &memory_size);
        }
        else {
            file = argv[i];
        }
    }


    if( file == NULL )
    {
        printf("Error: No such file");
        __memcheck();
        __memclear();
        return EXIT_FAILURE;
    }

    debug_open_file( file );
    debug_read_file( memory_size );
    debug_close_file();

    __memcheck();
    __memclear();
    return EXIT_SUCCESS;
}

void get_terminal_size(int* width, int* height) {
#ifdef WIN32
    // The Get Terminal Size based on: https://stackoverflow.com/questions/6812224/getting-terminal-size-in-c-for-windows
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    *height = rows;
    *width = columns;
#else
    struct winsize ws;
    ioctl( 0 , TIOCGWINSZ, &ws);
    *width = ws.ws_col;
    *height = ws.ws_row;
#endif
}

void display_register(int start_x, int width) {
    printf("\x1b[H\x1b[1B");
    printf("\x1b[%iG RA: 0x%.08x\x1b[1B", start_x, reg.ra);
    printf("\x1b[%iG RB: 0x%.08x\x1b[1B", start_x, reg.rb);
    printf("\x1b[%iG RC: 0x%.08x\x1b[1B", start_x, reg.rc);
    printf("\x1b[%iG RD: 0x%.08x\x1b[1B", start_x, reg.rd);
    printf("\x1b[%iG RE: 0x%.08x\x1b[1B", start_x, reg.re);
    printf("\x1b[%iG RF: 0x%.08x\x1b[1B", start_x, reg.rf);
    printf("\x1b[%iG RG: 0x%.08x\x1b[1B", start_x, reg.rg);
    printf("\x1b[%iG RH: 0x%.08x\x1b[1B", start_x, reg.rh);
    printf("\x1b[%iG RI: 0x%.08x\x1b[1B", start_x, reg.ri);
    printf("\x1b[%iG RJ: 0x%.08x\x1b[1B", start_x, reg.rj);
    printf("\x1b[%iG RK: 0x%.08x\x1b[1B", start_x, reg.rk);
    printf("\x1b[%iG RL: 0x%.08x\x1b[1B", start_x, reg.rl);
    printf("\x1b[%iG RM: 0x%.08x\x1b[1B", start_x, reg.rm);
    printf("\x1b[%iG RN: 0x%.08x\x1b[1B", start_x, reg.rn);
    printf("\x1b[%iG RS: 0x%.08x\x1b[1B", start_x, reg.rs);
    printf("\x1b[%iG RP: 0x%.08x\x1b[1B", start_x, reg.rp);
    printf("\x1b[%iG Flags: %.016b\x1b[1B", start_x, reg.flags);
    printf("\x1b[%iG PC: 0x%.08x", start_x, reg.pc);
}

void display_memory(int x, int y, int width, int height, char key) {
    static uint32_t memory_lines = 0;
    static int rows_cursor = 0;
    static int cols_cursor = 0;
    static int text_lines = 0;
    static int text_cols = 0;


    if( key == 'i' )
    {
        rows_cursor--;
        if( rows_cursor < 0 ) {
            rows_cursor = 0;
            text_lines--;
            if( text_lines < 0 )
                text_lines = 0;
        }

    } else if ( key == 'k' )
    {
        rows_cursor++;
        
        if( rows_cursor >= height ) {
            rows_cursor = height - 1;
            text_lines++;
        }
    }
    else if ( key == 'j' )
    {
        cols_cursor--;
        if( cols_cursor < 0 )
            cols_cursor = 0;
    } else if ( key == 'l' )
    {
        cols_cursor++;
        if( cols_cursor > width )
            cols_cursor = width;
    }

    printf("\x1b[%i;%iH", y + 1, x + 1);
    y = y + 1;
    printf("Memory Display:\x1b[%i;%iH", y + 1, x + 1);
    for(int i = 0; i < height; i++) {
        // ESC[0K - Erase line from cursor to end of line
        printf("\x1b[0K");

        uint32_t address = (i + text_lines) * 16;

        printf("%.08x:", address);
        for(int j = 0; j < 16; j++) {
          printf(" %.02x", gread8( memory, address + j ) );  
        }

        // ESC[#:#H - Move the cursor to position
        printf("\x1b[%i;%iH", y + i + 2, x + 1);
    }
    // set the position cursor
    printf("\x1b[%i;%iH", y + rows_cursor + 1, x + 1 + cols_cursor );

}


