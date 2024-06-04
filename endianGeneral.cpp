#include "endianGeneral.hpp"
#include <iostream>
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

uint8_t readBytePtr(uint8_t* &rpPtr)
{
    uint8_t result = *rpPtr;
    rpPtr ++;
    return result;
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

float f2Dot14(const uint16_t cVal)
{
    const uint16_t first_2_bits = 0xC000;
    const uint16_t neg_sign = 0x8000;
    const uint16_t last_14_bits = 0x3fff;
    const float divisor = 16384.0;
    int8_t int_val = 0;
    float fraction = 0;

    if (neg_sign & cVal)
    {
        int_val = ((~cVal & first_2_bits) >> F2DOT14_SHIFT) + 1;
        int_val *= -1;
    }
    else
    {
        int_val = (cVal & first_2_bits) >> F2DOT14_SHIFT;
    }

    fraction = cVal & last_14_bits;
    fraction = (float)int_val + (fraction / divisor);

    return fraction;
}