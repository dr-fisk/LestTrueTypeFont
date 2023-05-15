#include "loca.h"

void Loca::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
    if (0 == cNumBytes)
    {
        std::cout << "Error: Cannot create loca array of 0 bytes." << std::endl;
        return;
    }

    // mByteArray.resize(cNumBytes);
    std::cout << "offset for loca: " << cOffset << std::endl;
    // memcpy(mByteArray.data(), crBuffer.data() + cOffset, cNumBytes);
    mByteArray.insert(mByteArray.end(), &crBuffer[cOffset], &crBuffer[cOffset + cNumBytes]);
    std::cout << "Size of Loca: " << mByteArray.size() << std::endl;
    
    for (int i = 0; i < 647; i ++)
    {
        std::cout << "Offset at idx " << i << " for glyph is: " << getGlyphOffset(0, i) << std::endl;
    }
}

uint32_t Loca::getGlyphOffset(const int16_t cLocaType, const uint32_t cGlyphIndex)
{
    uint8_t *ptr = mByteArray.data();

    // If loca is 1 then it is a 4 byte value
    if (cLocaType)
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