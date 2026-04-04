#include "emulator.h"
#include <virt/bios/BIOS.h>
#include <virt/bios/text.h>
#include <virt/bios/interrupt.h>
#include <inst.h>
#include <cpu.h>
#include <interrupt.h>

cpu_register_t creg;
EmulatorWindow::EmulatorWindow( const std::string& title, int width, int height )
{
    this->title = title;
    this->width = width;
    this->height = height;
}

void EmulatorWindow::Init() {
    InitWindow( this->width, this->height, this->title.c_str() );
    SetTargetFPS( 60 );
    this->isRun = true;

    
    BIOS::Init( this->width, this->height );
    bios_text_init();

    cpu_initialize();
    cpu_set_memory(BIOS::memory, 0x100000);

    bios_interrupt_init();
}

void EmulatorWindow::Run() {

    bios_text_print("Virtual Machine H\n\n");
    Disk* disk = nullptr;

    bios_text_print("Scanning Primary Disk from BIOS...\n");
    if( BIOS::primary_disk.disk == nullptr )
        bios_text_print("Cannot read because the Primary Disk is not attached\n");
    else
        disk = &BIOS::primary_disk;

    if( disk == nullptr )
    {
        bios_text_print("Scanning Secondary Disk from BIOS...\n");
        if( BIOS::secondary_disk.disk == nullptr )
            bios_text_print("Cannot read because the Secondary Disk is not attached\n");
        else
            disk = &BIOS::secondary_disk;
    }
    

    int idx = 0;
    uint8_t textColor = 0b1111;
    while( this->isRun )
    {
        BeginDrawing();
        ClearBackground( BLACK );

        cpu_execute( 1 );
        BIOS::BIOSBasicInterface();

        int key = GetKeyPressed();
        if( key != 0 ) {

            if( key == KEY_ESCAPE )
                break;

            // if( key == KEY_ENTER )
            //     key = '\n';
            // else if( key == KEY_BACKSPACE )
            //     key = '\x08';

            // bios_text_putc( (char) key );
        }

        EndDrawing();
    }
}