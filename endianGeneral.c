#include "endianGeneral.h"

// Swaps endianness for short
uint16_t lesthtons(uint16_t val)
{
    uint16_t result = ((val & 0xff00) >> BYTE_SIZE );
    result |= ((val & 0x00ff) << BYTE_SIZE);
    return result;
}

// Swaps endianness for long
uint32_t lesthtonl(uint32_t val)
{
    uint32_t result = ((val & 0xff000000) >> (WORD_SIZE + BYTE_SIZE));
    result |= ((val & 0x00ff0000) >> BYTE_SIZE);
    result |= ((val & 0x0000ff00) << BYTE_SIZE);
    result |= ((val & 0x000000ff) << (WORD_SIZE + BYTE_SIZE));
    return result;
}

//! Swaps endianness for long long
uint64_t lesthtonll(uint64_t val)
{
    uint32_t result1 = lesthtonl((val & 0x00000000ffffffff));
    uint32_t result2 = lesthtonl((val & 0xffffffff00000000) >> (WORD_SIZE + WORD_SIZE));
    
    uint64_t finalResult = result1;
    finalResult <<= WORD_SIZE + WORD_SIZE;
    return finalResult | result2;
}

// 01 02 03 04 05 06 07 08
// 08 07 06 05 04 03 02 01