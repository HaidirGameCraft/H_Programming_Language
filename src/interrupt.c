#include "cpu.h"
#include <interrupt.h>
#include <stdio.h>

#include <libc/file.h>

static interrupt_t interrupts[MAX_INTERRUPT_ARRAY] = { 0 };
extern uint8_t* memory;
extern cpu_register_t reg;
extern uint8_t is_program_running;

static const char* message_interrupt[] = {
    "Divide Zero Exception",
    "Unknown Opcode Exception",
    "Out Of Memory Exception",
    "Reserved",
    "Reserved",
    "Reserved",
};

void StubInterrupt( int index, cpu_register_t *reg );
extern void SyscallInterrupt(int index, cpu_register_t *reg );


// Declaration Method
void InitInterrupt() {
    for(int i = 0; i < MAX_INTERRUPT_ARRAY; i++)
        interrupts[i].handle = (uint64_t) StubInterrupt;

    interrupts[0x20].handle = (uint64_t) SyscallInterrupt;
}

void AddInterrupt( int index, uint64_t address ) {


}

void CallInterrupt( int index ) {
    interrupt_t* interrupt = &interrupts[index];
    if( interrupt->handle == 0 )
    {
        printf("SYSTEM ERROR: Interrupt is not implemented");
        is_program_running = 0;
    }

    void (*__interrupt_call)(int, cpu_register_t*) = (void (*)(int, cpu_register_t*)) interrupt->handle;
    __interrupt_call( index, &reg );
}

void StubInterrupt( int index, cpu_register_t *reg) {
    if( index >= (int)( sizeof( message_interrupt ) / sizeof( const char* ) ) ) {    
        printf("Interrupt STUB");
    } else {
        printf( "%s", message_interrupt[index] );
    }
    is_program_running = 0;
}



