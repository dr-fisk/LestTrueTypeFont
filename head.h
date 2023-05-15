#ifndef HEAD_H
#define HEAD_H

#include "ttfTable.h"

struct HeadHeader
{
    uint16_t majorVersion;
    uint16_t minorVersion;
    uint32_t fontRevision;
    uint32_t checksumAdjustment;
    uint32_t magicNumber;
    uint16_t flags;
    uint16_t unitsPerEm;
    uint64_t created;
    uint64_t modified;
    int16_t  xMin;
    int16_t  xMax;
    int16_t  yMin;
    int16_t  yMax;
    uint16_t macStyle;
    uint16_t lowestRecPPEM;
    int16_t  fontDirectionHint;
    int16_t  indexToLocFormat;
    int16_t  glyphDataFormat;
};

class HeadTable : public TrueTypeTable
{
    public:
        HeadTable();
        ~HeadTable(){}
        void readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
    private:
        HeadHeader mHeadHeader;
};

#endif