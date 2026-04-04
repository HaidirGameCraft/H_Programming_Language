
#include "text.h"
#include <virt/bios/interrupt.h>
#include <virt/bios/BIOS.h>

void bios_interrupt_disk( int index, cpu_register_t* reg );

void bios_interrupt_init() {
    AddInterrupt( 0x13, (uint64_t) bios_interrupt_text );
    AddInterrupt( 0x10, (uint64_t) bios_interrupt_disk );
}

void bios_interrupt_text( int index, cpu_register_t* reg) {
    uint8_t sl = reg->ra >> 8 & 0xFF;
    if( sl == 0x01 )
        bios_text_putc( (char)(reg->ra & 0xFF) );
}

void bios_interrupt_disk( int index, cpu_register_t* reg ) {
    uint8_t sl = reg->ra >> 8 & 0xFF;
    if( sl == 0x01 )    // read sector from disk
    {
        uint8_t sector_count = reg->ra & 0xff;
        uint32_t buffer_addr = reg->rb;
        uint32_t lba_start = reg->rd;
        uint8_t  drive_number = reg->re & 0xFF;

        Disk* disk = nullptr;
        // Primary Disk
        if( drive_number == 0x80 )
        {
            disk = &BIOS::primary_disk;
        }
        // Secondary Disk
        else if ( drive_number == 0xE0 )
        {
            disk = &BIOS::secondary_disk;
        }

        disk->ReadSector( sector_count, buffer_addr, lba_start );
        reg->ra = 0x00;
    } else {
        reg->ra = 0xFF;
    }
}