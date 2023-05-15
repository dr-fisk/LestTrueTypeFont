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

uint16_t read2BytesPtr(uint8_t* &rpPtr)
{
    uint16_t result = *rpPtr;
    result <<= BYTE_SIZE;
    rpPtr ++;
    result |= (*rpPtr & 0x00ff );
    rpPtr ++;
    return result;
}

uint32_t read4BytesPtr(uint8_t* &rpPtr)
{
    uint32_t result = *rpPtr;
    result <<= (WORD_SIZE + BYTE_SIZE);
    const uint32_t hex_val = 0x000000ff;
    rpPtr ++;
    result |= (*rpPtr & hex_val) << WORD_SIZE;
    rpPtr ++;
    result |= (*rpPtr & hex_val) << BYTE_SIZE;
    rpPtr ++;
    result |= (*rpPtr & hex_val);
    rpPtr ++;
    return result;
}

uint64_t read8BytesPtr(uint8_t* &rpPtr)
{
    uint32_t result1 = read4BytesPtr(rpPtr);
    uint32_t result2 = read4BytesPtr(rpPtr);
    uint64_t finalResult = result2;
    finalResult <<= WORD_SIZE + WORD_SIZE;
    return (finalResult | result1);
}