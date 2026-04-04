#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

class Disk {
public:
    FILE *disk;
    void OpenDisk( const std::string& filename );
    void ReadSector( uint32_t sector_cout, uint32_t buffer_addr, uint32_t lba_start );
};