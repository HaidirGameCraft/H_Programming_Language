#include "Disk.h"
#include <asrt.h>
#include <virt/bios/BIOS.h>

void Disk::OpenDisk( const std::string& filename ) {
    this->disk = fopen( filename.c_str(), "rwb+" );
    EXITFAIL( !this->disk, printf("Error: Disk is not found\n") );
}

void Disk::ReadSector( uint32_t sector_cout, uint32_t buffer_addr, uint32_t lba_start ) {
    fseek( this->disk, lba_start * 512, SEEK_SET );
    fread( &BIOS::memory[buffer_addr], 512, 1, this->disk);
}