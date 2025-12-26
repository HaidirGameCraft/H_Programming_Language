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


#define PREFIX_SYM_INC      1 << 3
#define PREFIX_REG_INC      1 << 4
#define PREFIX_OFF_INC      1 << 5
#define PREFIX_VAL_INC      1 << 6
#define PREFIX_EXT_INC      1 << 7

#define EXT_PREFIX_ADDRSIZE 1 << 0

typedef struct {
    uint8_t _0: 1;
    uint8_t _1: 1;      
    uint8_t _2: 1;      
    uint8_t sym: 1;     // When set, Symbols Include
    uint8_t r: 1;       // When set, Register include
    uint8_t o: 1;       // When set, Offset include
    uint8_t v: 1;       // When set, Value include
    uint8_t ext: 1;     // When set, Prefix Extended included ( it has 2 bytes of prefix )
} ins_prefix_t;

typedef struct {
    uint8_t dreg: 4;
    uint8_t sreg: 4;
} ins_register_t;

typedef struct {
    uint8_t sym: 4;
    uint8_t rsvd: 4;
} ins_symbols_t;

#define SYM_EQUAL           1
#define SYM_LESS            2
#define SYM_GREATER         3
#define SYM_LESS_EQUAL      4
#define SYM_GREATER_EQUAL   5
#define SYM_SHIFT_LEFT      6
#define SYM_SHIFT_RIGHT     7

uint8_t read8(uint8_t* memory, uint32_t* pc);
uint16_t read16(uint8_t* memory, uint32_t* pc);
uint32_t read32(uint8_t* memory, uint32_t* pc);
uint8_t gread8(uint8_t* memory, uint32_t pc);
uint16_t gread16(uint8_t* memory, uint32_t pc);
uint32_t gread32(uint8_t* memory, uint32_t pc);

uint32_t write8(uint8_t* memory, uint32_t pc, uint8_t data);
uint32_t write16(uint8_t* memory, uint32_t pc, uint16_t data);
uint32_t write32( uint8_t* memory, uint32_t pc, uint32_t data);

uint32_t* getreg(cpu_register_t* reg, uint8_t r);

char* str_lowercase(const char* str);
char* str_uppercase(const char* str);

uint8_t regbytext(const char* reg_text);
uint8_t regmem_set(uint8_t bit, uint8_t sreg, uint8_t dreg);
uint32_t create_instruction(uint8_t* memory, uint32_t pc, const char* instruction, int line);

#define INSTRUCTION_SET_ARGS uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t* memory, cpu_register_t* reg, uint32_t* pc
#define SIGN_INSTRUCTION_SET(name) uint8_t name##_instruction_set(INSTRUCTION_SET_ARGS)


#define CREATE_INSTRUCTION_ARGS char* opcode, char* lReg, char* rReg, uint32_t* offset, uint32_t* value, uint8_t* memory, uint32_t pc
#define VARIABLE_INSTRUCTION    ins_prefix_t pre; \
                                memset(&pre, prefix, sizeof( ins_prefix_t ) ); \
                                \
                                ins_register_t __register; \
                                uint32_t* sreg = NULL; \
                                \
                                uint32_t* dreg = NULL; \
                                \
                                uint32_t offset = 0; \
                                uint32_t value = 0;
#define REGMEM_DEFINED(m, p)    if( prefix & PREFIX_REG_INC ) \
                                    memset(&__register, read8(m, p), 1);

#define OPCODE_SH_1     0x20    /* SH REG (<<|>>) REG */
#define OPCODE_SH_2     0x21    /* SH REG (<<|>>) VALUE */

#define OPCODE_ADD_1    0x30    /* ADD REG -> REG */
#define OPCODE_ADD_2    0x31    /* ADD VALUE -> REG */
                                /* ADD $LABEL -> REG */
#define OPCODE_INT      0x32    /* INT REG */
                                /* INT VALUE */

#define OPCODE_SUB_1    0x33
#define OPCODE_SUB_2    0x34

#define OPCODE_MUL_1    0x36
#define OPCODE_MUL_2    0x37

#define OPCODE_DIV_1    0x39
#define OPCODE_DIV_2    0x3A
#define OPCODE_PUSH_1   0x3B
#define OPCODE_PUSH_2   0x3C
#define OPCODE_POP_1    0x3D

#define OPCODE_LOD_1    0x3E    /* LOD REG(+-off) -> REG */
#define OPCODE_LOD_2    0x3F    /* LOD ADDR(+-off) -> REG*/
                                /* LOD $LABEL(+-off) -> REG */

#define OPCODE_MOV_1    0x40    /* MOVS REG -> REG */
#define OPCODE_MOV_2    0x41    /* MOVS VALUE -> REG */
                                /* MOVS $LABEL -> REG */

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

#define OPCODE_CND_1        0x54    /* CND REG (SYM) REG */
#define OPCODE_CND_2        0x55    /* CND VALUE  (SYM) REG */
                                    /* CND $LABEL (SYM) REG */
#define OPCODE_CND_3        0x56    /* CND REG (SYM) VALUE */
                                    /* CND REG (SYM) $LABEL */

#define OPCODE_CALL_1       0x58
#define OPCODE_CALL_2       0x59
#define OPCODE_RETURN       0x5A
#define OPCODE_GO_1         0x5B
#define OPCODE_GO_2         0x5C
#define OPCODE_GOC_1        0x5D
#define OPCODE_GOC_2        0x5E
#define OPCODE_STR_1        0x5F    /* STR REG -> REG(+-off) */
#define OPCODE_STR_2        0x60    /* STR REG -> ADDR(+-off) */
                                    /* STR REG -> $LABEL(+-off) */
#define OPCODE_STRS_1       0x61    /* STRS REG -> REG(+-off) */
#define OPCODE_STRS_2       0x62    /* STRS REG -> ADDR(+-off) */
                                    /* STRS REG -> $LABEL(+-off)*/
#define OPCODE_STRD_1       0x63    /* STRD REG -> REG(+-off) */
#define OPCODE_STRD_2       0x64    /* STRD REG -> ADDR(+-off) */
                                    /* STRD REG -> $LABEL(+-off)*/

#define OPCODE_LODS_1       0x65    /* LODS REG(+-off) -> REG */
#define OPCODE_LODS_2       0x66    /* LODS ADDR(+-off) -> REG */
                                    /* LODS $LABEL(+-off) -> REG*/

#define OPCODE_LODD_1       0x67    /* LODD REG(+-off) -> REG */
#define OPCODE_LODD_2       0x68    /* LODD ADDR(+-off) -> REG */
                                    /* LODD $LABEL(+-off) -> REG*/
#define OPCODE_MOVS_1       0x69   /* MOVS REG -> REG */
#define OPCODE_MOVS_2       0x6A   /* MOVS VALUE -> REG */
                                    /* MOVS $LABEL -> REG */
#define OPCODE_MOVD_1       0x6B   /* MOVD REG -> REG */
#define OPCODE_MOVD_2       0x6C   /* MOVD VALUE -> REG
                                    /* MOVD $LABEL -> REG */
#define OPCODE_PNT_1        0x6D    /* PNT REG */
/**
 * PNT VALUE
 * PNT $LABEL
 */
#define OPCODE_PNT_2        0x6E
/**
 * PCT REG
 */
#define OPCODE_PCT_1        0x6F
/**
 * PCT VALUE
 * PCT $LABEL
 */
#define OPCODE_PCT_2        0x70

/**
 * IN REG -> REG
 * IN VALUE -> REG
*/
#define OPCODE_IN           0x71

/**
 * OUT REG -> REG
 * OUT REG -> VALUE
 */ 
#define OPCODE_OUT          0x72

#define OPCODE_STP          0x90
