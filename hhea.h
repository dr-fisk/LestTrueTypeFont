#ifndef HHEA_H
#define HHEA_H

#include "ttfTable.h"

struct HheaHeader
{
    int32_t  version;             // 0x00010000 (1.0)
    int16_t  ascent;              // Distance from baseline of highest ascender
    int16_t  descent;             // Distance from baseline of lowest descender
    int16_t  lineGap;             // typographic line gap
    uint16_t advancedWidthMax;    // must be consistent with horizontal metrics
    int16_t  minLeftSideBearing;  // must be consistent with horizontal metrics
    int16_t  minRightSideBearing; // must be consistent with horizontal metrics
    int16_t  xMaxExtent;          // max(lsb + (xMax-xMin))
    int16_t  caretSlopeRise;      // used to calculate the slope of the caret (rise/run) set to 1 for vertical caret
    int16_t  caretSlopeRun;       // 0 for vertical
    int16_t  caretOffset;         // set value to 0 for non-slanted fonts
    int16_t  reserved;
    int16_t  reserved2;
    int16_t  reserved3;
    int16_t  reserved4;
    int16_t  metricDataFormat;    // 0 for current format
    uint16_t numOfLongHorMetrics; // 0 for current format
};

class Hhea : public TrueTypeTable
{
    public:
        Hhea(){}
        ~Hhea(){}
        void readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
        uint16_t getNumOfLongHorMetrics() const;
    private:
        HheaHeader mHheaHeader;
};

#endif