#include "./instruction_decode.h"
#include <ins/inst.h>
#include <tools/tools.h>
#include <tools/memory.h>
#include <string>

const char* instruction_opcode_name( uint8_t opcode );
const char* instruction_symbols_name( uint8_t symbols );
char* decode_instruction( uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t* memory, uint32_t pc );

char* instruction_decode( uint8_t* memory, uint32_t pc, uint8_t size ) {
    uint32_t _tpc = pc;
    uint8_t prefix = read8( memory, &pc );
    uint8_t ext_prefix = 0;
    if( prefix & PREFIX_EXT_INC )
        ext_prefix = read8( memory, &pc );

    uint8_t opcode = read8( memory, &pc );
    const char* opcode_name = instruction_opcode_name( opcode );
    if( opcode_name == NULL )
        return NULL;

    char* __tmp = (char*) __malloc( 256 );
    memset( __tmp, 0, 256 );
    sprintf( __tmp, "%.08x: %s", _tpc, opcode_name );
    char* __inside = decode_instruction( prefix, ext_prefix, opcode, memory, pc );
    if( __inside != NULL )
        sprintf( __tmp, "%.08x: %s %s", _tpc, opcode_name, __inside);

    __free( __inside );
    char* __instruction = (char*) strcpy( __tmp,strlen( __tmp) );
    __free( __tmp );
    
    return __instruction;

}

const char* instruction_opcode_name( uint8_t opcode ) {
    int i = 0;
#define INSTRUCTION_OPCODE_NAME(x) \
    i = 0; \
    for(i = 0; i < sizeof( x ); i++ ) { \
        if( x[i].opcode == opcode ) \
        {   \
            return x[i].opcode_name; \
        } \
    }

    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_reg_reg)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_regoff_reg)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_reg_regoff)

    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_value_reg)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_reg_value)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_valueoff_reg)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_reg_valueoff)

    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_label_reg)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_reg_label)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_labeloff_reg)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_reg_labeloff)

    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_reg)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_value)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode_label)
    INSTRUCTION_OPCODE_NAME(instruction_maker_opcode)

    return NULL;
}

char* decode_instruction( uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t* memory, uint32_t pc ) {
    int i = 0;
#define INSTRUCTION_DECODER(x) \
    for( i = 0; i < sizeof( instruction_maker_##x ) / sizeof( h_text_compile ) ; i++ ) { \
        if( instruction_maker_##x[i].opcode == opcode && instruction_maker_##x[i].prefix == prefix ) { \
            return decode_instruction_##x(prefix, ext_prefix, opcode, memory, pc ); \
        } \
    }

    INSTRUCTION_DECODER(opcode_reg_reg);
    INSTRUCTION_DECODER(opcode_regoff_reg);
    INSTRUCTION_DECODER(opcode_reg_regoff);
    INSTRUCTION_DECODER(opcode_value_reg);
    INSTRUCTION_DECODER(opcode_reg_value);
    INSTRUCTION_DECODER(opcode_valueoff_reg);
    INSTRUCTION_DECODER(opcode_reg_valueoff);
    INSTRUCTION_DECODER(opcode_label_reg);
    INSTRUCTION_DECODER(opcode_reg_label);
    INSTRUCTION_DECODER(opcode_labeloff_reg);
    INSTRUCTION_DECODER(opcode_reg_labeloff);
    INSTRUCTION_DECODER(opcode_reg);
    INSTRUCTION_DECODER(opcode_value);
    INSTRUCTION_DECODER(opcode_label);
    INSTRUCTION_DECODER(opcode);

    return NULL;
}

const char* decode_register( uint8_t reg ) {
    if      ( reg ==  0) return "ra";
    else if ( reg ==  1) return "rb";
    else if ( reg ==  2) return "rc";
    else if ( reg ==  3) return "rd";
    else if ( reg ==  4) return "re";
    else if ( reg ==  5) return "rf";
    else if ( reg ==  6) return "rg";
    else if ( reg ==  7) return "rh";
    else if ( reg ==  8) return "ri";
    else if ( reg ==  9) return "rj";
    else if ( reg == 10) return "rk";
    else if ( reg == 11) return "rl";
    else if ( reg == 12) return "rm";
    else if ( reg == 13) return "rn";
    else if ( reg == 14) return "rs";
    else if ( reg == 15) return "rp";

    return NULL;
}

const char* instruction_symbols_name( uint8_t symbols ) {
    if( symbols == SYM_EQUAL )
        return "==";
    else if ( symbols == SYM_LESS )
        return "<";
    else if ( symbols == SYM_GREATER )
        return ">";
    else if ( symbols == SYM_LESS_EQUAL )
        return "<=";
    else if ( symbols == SYM_GREATER_EQUAL )
        return ">=";

    return "->";
}


#define VARIABLES__     ins_register_t reg = { 0 }; \
                        uint8_t sym = 0; \
                        uint16_t offset = 0; \
                        uint32_t value = 0; \
                        if ( prefix & PREFIX_REG_INC ) \
                            memset(&reg, read8( memory, &pc ), 1); \
                        if ( prefix & PREFIX_OFF_INC ) \
                            offset = read16( memory, &pc ); \
                        else if ( prefix & PREFIX_SYM_INC ) \
                            sym = read8( memory, &pc ); \
                        if( prefix & PREFIX_VAL_INC ) \
                            value = read32( memory, &pc );

char* decode_instruction_opcode_reg_reg      (ARGUMENTS_DECODE_INSTRUCTION) {
    VARIABLES__;

   const char* sreg = decode_register( reg.sreg );
   const char* dreg = decode_register( reg.dreg );
   const char* symbols = instruction_symbols_name( sym );
   char* text = (char*) __malloc( 128 );
   memset(text, 0, 128);

   sprintf( text, "%s %s %s", sreg, symbols, dreg);
   char* ret = (char*) strcpy( text, strlen( text ) );
   __free( text );
   return ret;
}

char* decode_instruction_opcode_regoff_reg   (ARGUMENTS_DECODE_INSTRUCTION){
    VARIABLES__
    const char* sreg = decode_register( reg.sreg );
    const char* dreg = decode_register( reg.dreg );
    char* text = (char*) __malloc( 128 );
    memset( text, 0, 128 );

    sprintf(text, "%s(0x%x) -> %s", sreg, offset, dreg);

    char* ret = (char*) strcpy( text, strlen( text) );
    __free( text );
    return ret;
}

char* decode_instruction_opcode_reg_regoff   (ARGUMENTS_DECODE_INSTRUCTION){
    VARIABLES__
    const char* sreg = decode_register( reg.sreg );
    const char* dreg = decode_register( reg.dreg );

    char* text = (char*) __malloc( 128 );
    memset( text, 0, 128 );

    sprintf(text, "%s -> %s(0x%x)", sreg, dreg, offset);

    char* ret = (char*) strcpy( text, strlen( text) );
    __free( text );
    return ret;
}
char* decode_instruction_opcode_value_reg    (ARGUMENTS_DECODE_INSTRUCTION){
    VARIABLES__
    const char* dreg = decode_register( reg.dreg );
    const char* symbols = instruction_symbols_name( sym );
    char* text = (char*) __malloc( 128 );
    memset( text, 0, 128 );
    sprintf( text, "0x%x %s %s", value, symbols, dreg);

    char* ret = strcpy( text, strlen( text ) );
    __free( text );

    return ret;
}
char* decode_instruction_opcode_reg_value    (ARGUMENTS_DECODE_INSTRUCTION){
    VARIABLES__
    const char* sreg = decode_register( reg.sreg );
    const char* symbols = instruction_symbols_name( sym );
    char* text = (char*) __malloc( 128 );
    memset( text, 0, 128 );
    sprintf( text, "%s %s 0x%x", sreg, symbols, value);

    char* ret = strcpy( text, strlen( text ) );
    __free( text );

    return ret;
}
char* decode_instruction_opcode_valueoff_reg (ARGUMENTS_DECODE_INSTRUCTION){
    VARIABLES__
    const char* dreg = decode_register( reg.dreg );
    
    char* text = (char*) __malloc( 128 );
    memset( text, 0, 128 );
    sprintf( text, "0x%x(0x%x) -> %s", value, offset, dreg);

    char* ret = strcpy( text, strlen( text ) );
    __free( text );

    return ret;
}
char* decode_instruction_opcode_reg_valueoff (ARGUMENTS_DECODE_INSTRUCTION){
    VARIABLES__
    const char* sreg = decode_register( reg.sreg );
    
    char* text = (char*) __malloc( 128 );
    memset( text, 0, 128 );
    sprintf( text, "%s -> 0x%x(0x%x)", sreg, value, offset);

    char* ret = strcpy( text, strlen( text ) );
    __free( text );

    return ret;
}
char* decode_instruction_opcode_label_reg    (ARGUMENTS_DECODE_INSTRUCTION){
    return decode_instruction_opcode_value_reg(prefix, ext_prefix, opcode, memory, pc);
}
char* decode_instruction_opcode_reg_label    (ARGUMENTS_DECODE_INSTRUCTION){
    return decode_instruction_opcode_reg_value( prefix, ext_prefix, opcode, memory, pc );
}
char* decode_instruction_opcode_labeloff_reg (ARGUMENTS_DECODE_INSTRUCTION){
    return decode_instruction_opcode_valueoff_reg( prefix, ext_prefix, opcode, memory, pc );
}
char* decode_instruction_opcode_reg_labeloff (ARGUMENTS_DECODE_INSTRUCTION){
    return decode_instruction_opcode_reg_valueoff( prefix, ext_prefix, opcode, memory, pc );
}
char* decode_instruction_opcode_reg          (ARGUMENTS_DECODE_INSTRUCTION){
    VARIABLES__
    const char* sreg = decode_register( reg.sreg );
    char* text = strcpy(sreg, strlen( sreg ) );
    return text;
}
char* decode_instruction_opcode_value        (ARGUMENTS_DECODE_INSTRUCTION){
    VARIABLES__
    char* text = (char*) __malloc( 128 );
    memset( text, 0, 128 );
    sprintf(text, "0x%x", value);

    char* ret = strcpy( text, strlen( text ) );
    __free( text );
    return ret;
}
char* decode_instruction_opcode_label        (ARGUMENTS_DECODE_INSTRUCTION){
    VARIABLES__

    char* text = (char*) __malloc( 128 );
    memset( text, 0, 128 );
    sprintf(text, "0x%x", value);

    char* ret = strcpy( text, strlen( text ) );
    __free( text );
    return NULL;
}
char* decode_instruction_opcode              (ARGUMENTS_DECODE_INSTRUCTION)
{
    VARIABLES__

    return NULL;
}










