#ifndef HMTX_H
#define HMTX_H

#include "ttfTable.h"

struct LongHorMetric
{
    // Paired advance width and left side bearing values for each glyph. Records are indexed by glyph ID.
    uint16_t advanceWidth;

    // Left side bearings for glyph IDs greater than or equal to numberOfHMetrics.
    int16_t  leftSideBearing;
};

struct HmtxHeader
{
    // The value numOfLongHorMetrics comes from the 'hhea' table. If the font is monospaced, only one entry need
    // be in the array but that entry is required.
    std::vector<LongHorMetric> hMetrics;

    // Here the advanceWidth is assumed to be the same as the advanceWidth for the last entry above. The number of
    // entries in this array is derived from the total number of glyphs minus numOfLongHorMetrics. This generally is
    // used with a run of monospaced glyphs (e.g. Kanji fonts or Courier fonts). Only one run is allowed and it must be
    // at the end.
    std::vector<int16_t> leftSideBearing;
};

class Hmtx : public TrueTypeTable
{
    public:
        Hmtx(){}
        Hmtx(const uint16_t cNumHorMetrics, const uint16_t cNumGlyphs);
        ~Hmtx(){}
        int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
    private:
        HmtxHeader mHmtxHeader;
};

#endif