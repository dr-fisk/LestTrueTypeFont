#include "loca.hpp"

int8_t Loca::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
    if (0 == cNumBytes)
    {
        std::cout << "Error: Cannot create loca array of 0 bytes." << std::endl;
        return -1;
    }

    mByteArray.insert(mByteArray.end(), &crBuffer[cOffset], &crBuffer[cOffset + cNumBytes]);
    
    #ifdef DEBUG
        for (int i = 0; i < 647; i ++)
        {
            std::cout << "Offset at idx " << i << " for glyph is: " << getGlyphOffset(i) << std::endl;
        }
    #endif

    return 1;
}

uint32_t Loca::getGlyphOffset(const uint32_t cGlyphIndex)
{
    uint8_t *ptr = mByteArray.data();

    // If loca is 1 then it is a 4 byte value
    if (mLocaFormat)
    {
        ptr += cGlyphIndex * sizeof(uint32_t);
        return read4BytesPtr(ptr);
    }
    // otherwise use 2 byte value
    else
    {
        ptr += cGlyphIndex * sizeof(uint16_t);
        return read2BytesPtr(ptr) * 2;
    }
}