#ifndef LOCA_H
#define LOCA_H

#include "ttfTable.h"

class Loca : public TrueTypeTable
{
    public:
        Loca(){}
        ~Loca(){}
        void readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
        uint32_t getGlyphOffset(const int16_t cLocaType, const uint32_t cGlyphIndex);
    private:
        std::vector<uint8_t> mByteArray;
};

#endif