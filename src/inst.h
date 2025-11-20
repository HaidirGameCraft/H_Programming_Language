/**
 * inst.c
 * Created By Haidir
 */
#pragma once

/**
 * Instruction Set
 * Created By: Haidir
 */

#include <stdint.h>
#include <cpu.h>
#include <stdlib.h>
#include <asrt.h>
#include <string.h>

/**
 * Instruction Set
*/

#define PREFIX_MEMORY       1 << 1

#define PREFIX_16BITS       1 << 2
#define PREFIX_32BITS       1 << 3


#define PREFIX_RM_INC       1 << 4
#define PREFIX_OFF_INC      1 << 5
#define PREFIX_VAL_INC      1 << 6
#define PREFIX_EXT_INC      1 << 7

#define EXT_PREFIX_ADDRSIZE 1 << 0

typedef struct {
    uint8_t _0: 1;
    uint8_t m: 1;   // When set, It will set the memory in RM includer ( if set )
    uint8_t b16: 1; // When set, It entering 16 bits
    uint8_t b32: 1; // When set, It entering 32 bits
    uint8_t regmem: 1;  // When set, RM include
    uint8_t o: 1;   // When set, Offset include
    uint8_t v: 1;   // When set, Value include
    uint8_t ext: 1; // When set, Prefix Extended included ( it has 2 bytes of prefix )
} ins_prefix_t;

typedef struct {
    uint8_t addr_size: 1; // When set the addr is 32 bits, otherwise 16 bits
} ins_ext_prefix_t;

typedef struct {
    uint8_t dreg: 3;     // 3 bits represent destination register
    /**
     * FLAGS - 2 bits represent the flags
     * 0| -> when set the Extended RegMem will required ( providing the type of bits of each source and destination register )
     * 1| -> reserved;
     */
    uint8_t flags: 2;
    uint8_t sreg: 3;     // 3 bits represent source register
} ins_regmem_t;

typedef struct {
    uint8_t dbit: 2;
    uint8_t sbit: 2;
    uint8_t _0: 4;
} ins_ext_regmem_t;

uint8_t read8(uint8_t* memory, uint32_t* pc);
uint16_t read16(uint8_t* memory, uint32_t* pc);
uint32_t read32(uint8_t* memory, uint32_t* pc);
uint8_t gread8(uint8_t* memory, uint32_t pc);
uint16_t gread16(uint8_t* memory, uint32_t pc);
uint32_t gread32(uint8_t* memory, uint32_t pc);

uint32_t write8(uint8_t* memory, uint32_t pc, uint8_t data);
uint32_t write16(uint8_t* memory, uint32_t pc, uint16_t data);
uint32_t write32( uint8_t* memory, uint32_t pc, uint32_t data);

uint8_t* reg8(cpu_register_t* reg, uint8_t r);
uint16_t* reg16(cpu_register_t* reg, uint8_t r);
uint32_t* reg32(cpu_register_t* reg, uint8_t r);

char* str_lowercase(const char* str);
char* str_uppercase(const char* str);

uint8_t regbytext(const char* reg_text);
uint8_t regmem_set(uint8_t bit, uint8_t sreg, uint8_t dreg);
uint32_t create_instruction(uint8_t* memory, uint32_t pc, const char* instruction );

#define INSTRUCTION_SET_ARGS uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t* memory, cpu_register_t* reg, uint32_t* pc
#define SIGN_INSTRUCTION_SET(name) void name##_instruction_set(INSTRUCTION_SET_ARGS)
#define PUSH_INSTRUCTION_MEM(name) uint32_t name##_push_instruction(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t regmem, uint32_t disp, uint32_t value)

PUSH_INSTRUCTION_MEM(mov);
PUSH_INSTRUCTION_MEM(add);
PUSH_INSTRUCTION_MEM(sub);
PUSH_INSTRUCTION_MEM(mul);
PUSH_INSTRUCTION_MEM(div);
PUSH_INSTRUCTION_MEM(push);
PUSH_INSTRUCTION_MEM(pop);


#define CREATE_INSTRUCTION_ARGS char* opcode, char* lReg, char* rReg, uint32_t* offset, uint32_t* value, uint8_t* memory, uint32_t pc
#define VARIABLE_INSTRUCTION    ins_prefix_t pre; \
                                memset(&pre, prefix, sizeof( ins_prefix_t ) ); \
                                \
                                ins_regmem_t __regmem; \
                                ins_ext_regmem_t __ext_regmem; \
                                uint8_t* r8 = NULL; \
                                uint16_t* r16 = NULL; \
                                uint32_t* r32 = NULL; \
                                \
                                uint8_t* dr8 = NULL; \
                                uint16_t* dr16 = NULL; \
                                uint32_t* dr32 = NULL; \
                                \
                                uint32_t offset = 0; \
                                uint32_t value = 0;
#define REGMEM_DEFINED(m, p)    if( prefix & PREFIX_RM_INC ) \
                                    memset(&__regmem, read8(m, p), 1); \
                                if( (prefix & PREFIX_RM_INC) && __regmem.flags & (1 << 0) ) \
                                    memset(&__ext_regmem, read8(m, p), 1);

#define OPCODE_ADD_1    0x30    /* ADD REG -> REG */
#define OPCODE_ADD_2    0x31    /* ADD VALUE -> REG */
                                /* ADD $LABEL -> REG */

#define OPCODE_SUB_1    0x33
#define OPCODE_SUB_2    0x34

#define OPCODE_MUL_1    0x36
#define OPCODE_MUL_2    0x37

#define OPCODE_DIV_1    0x39
#define OPCODE_DIV_2    0x3A
#define OPCODE_PUSH_1   0x3B
#define OPCODE_PUSH_2   0x3C
#define OPCODE_POP_1    0x3D

#define OPCODE_LOD_1    0x3E    /* LOD REG(n) -> REG */
#define OPCODE_LOD_2    0x3F    /* LOD ADDR(n) -> REG*/
                                /* LOD $LABEL(n) -> REG */

#define OPCODE_MOV_1    0x40
#define OPCODE_MOV_2    0x41

#define OPCODE_PUSH_RM32    0x44
#define OPCODE_PUSH_VALUE   0x45
#define OPCODE_POP_REG      0x46

#define OPCODE_AND_1        0x47
#define OPCODE_AND_2        0x48
#define OPCODE_OR_1         0x49
#define OPCODE_OR_2         0x4A
#define OPCODE_NOT          0x4B
#define OPCODE_NAND_1       0x4C
#define OPCODE_NAND_2       0x4D
#define OPCODE_NOR_1        0x4E
#define OPCODE_NOR_2        0x4F
#define OPCODE_XOR_1        0x50
#define OPCODE_XOR_2        0x51
#define OPCODE_XNOR_1       0x52
#define OPCODE_XNOR_2       0x53

#define OPCODE_CND_1        0x54    /* CND REGMEM (SYM) REG */
#define OPCODE_CND_2        0x55    /* CND VALUE  (SYM) REGMEM */

#define OPCODE_CALL_1       0x58
#define OPCODE_CALL_2       0x59
#define OPCODE_RETURN       0x5A
#define OPCODE_GO_1         0x5B
#define OPCODE_GO_2         0x5C
#define OPCODE_GOC_1        0x5D
#define OPCODE_GOC_2        0x5E
#define OPCODE_STR_1        0x5F    /* STR REG -> REG */
#define OPCODE_STR_2        0x60    /* STR ADDR -> REG */
                                    /* STR $LABEL -> REG */

#define OPCODE_STP          0x90


SIGN_INSTRUCTION_SET(lod);

SIGN_INSTRUCTION_SET(str);

/**
 * Instruction Set Details
 * - regmem8/16/32 -> using REGISTER or MEMORY ( required OFFSET ) of 8, 16 and 32 bits
 * - reg8/16/32 -> using REGISTER only of 8, 16 and 32 bits
 * - mem8/16/32 -> using MEMORY only with requiring OFFSET of 8, 16 and 32 bits
 * - value8/16/32 -> using the VALUE with size 1, 2 or 4 bytes
 * - (SYM)          -> including 1 bytes slot for symbols
 */

/**
 * MOV - instruction set ( moving data from source to destination)
 * - "mov regmem8/16/32 -> reg8/16/32" -> the bits must be same
 * - "mov reg8/16/32 -> regmem8/16/32" -> the bits must be same
 * - "mov value8/16/32 -> regmem8/16/32" -> the bits must be same
 */
SIGN_INSTRUCTION_SET(mov);

/**
 * ADD - instruction set (add destination by source )
 * - "add regmem8/16/32 -> reg8/16/32" -> the bits must be same
 * - "add reg8/16/32 -> regmem8/16/32" -> the biist must be same
 * - "add value8/16/32 -> regmem8/16/32" -> the bits must be same
 */
SIGN_INSTRUCTION_SET(add);

/**
 * SUB - instruction set (sub destination by source)
 * - "sub regmem8/16/32 -> reg8/16/32" -> the bits must be same
 * - "sub reg8/16/32 -> regmem8/16/32" -> the biist must be same
 * - "sub value8/16/32 -> regmem8/16/32" -> the bits must be same
 */
SIGN_INSTRUCTION_SET(sub);

/**
 * MUL - instruction set (muliply destination by source)
 * - "mul regmem8/16/32 -> reg8/16/32" -> the bits must be same
 * - "mul value8/16/32 -> reg8/16/32" -> the bits must be same
 */
SIGN_INSTRUCTION_SET(mul);

/**
 * DIV - instruction set (divide destination by source)
 * - Quotent will set on ( destination register ), while Reminder will set on (R8E)
 * - "div regmem8/16/32 -> reg8/16/32" -> the bits must be same
 * - "div value8/16/32 -> reg8/16/32" -> the bits must be same
 */
SIGN_INSTRUCTION_SET(div);

/**
 * PUSH - instruction set (pushing the data into stack)
 * - "push value8/16/32"
 * - "push regmem8/16/32"
 */
SIGN_INSTRUCTION_SET(push);

/**
 * POP - instruction set (poping the data from stack)
 * - "pop reg8/16/32"
 */
SIGN_INSTRUCTION_SET(pop);

/**
 * AND - instruction set ( Logic Gate )
 * - "and regmem -> reg"
 * - "and value -> reg"
 */
SIGN_INSTRUCTION_SET(and);

/**
 * OR - instruction set ( Logic Gate )
 * - "or regmem -> reg"
 * - "or value -> reg"
 */
SIGN_INSTRUCTION_SET(or);

/**
 * NOT - instruction set ( Logic Gate )
 * - "not reg"
 */
SIGN_INSTRUCTION_SET(not);

/**
 * NAND - instruction set ( Logic Gate )
 * - "nand regmem -> reg"
 * - "nand value -> reg"
 */
SIGN_INSTRUCTION_SET(nand);

/**
 * NOR - instruction set ( Logic Gate )
 * - "nor regmem -> reg"
 * - "nor value -> reg"
 */
SIGN_INSTRUCTION_SET(nor);

/**
 * XOR - instruction set ( Logic Gate )
 * - "xor regmem -> reg"
 * - "xor value -> reg"
 */
SIGN_INSTRUCTION_SET(xor);

/**
 * STP - instruction set (stopping the cpu)
 */
SIGN_INSTRUCTION_SET(stp);

/**
 * CALL - instruction set ( calling the function by pushing current address to stack and go to the address function )
 * - "call reg"
 * - "call regmem"
 * - "call label"
 * - "call value"
 */
SIGN_INSTRUCTION_SET(call);

/**
 * GO - instruction set ( go to destination address )
 * - "go regmem"
 * - "go value"
 * - "go label"
 */
SIGN_INSTRUCTION_SET(go);

/**
 * RET - instruction set ( return to the previous address by popping the stack )
 */
SIGN_INSTRUCTION_SET(ret);

/**
 * CND - instruction set
 */
SIGN_INSTRUCTION_SET(cnd);