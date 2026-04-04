#pragma once

#include <inst.h>
#include <cpu.h>
#include <interrupt.h>

void bios_interrupt_init();
void bios_interrupt_text( int index, cpu_register_t* reg);