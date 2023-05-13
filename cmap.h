#ifndef CMAP_H
#define CMAP_H

#include <cstdint>
#include <vector>
#include "ttfTable.h"

// Only supporting unicode for now
static const uint8_t CMAP_HEADER_SIZE = 4;

struct EncodingRecord
{
    uint16_t platformID;
    uint16_t encodingID;
    uint32_t subTableOffset; // From the start of cmap data
};

struct CmapHeader
{
    uint16_t version;
    uint16_t numTables;
    std::vector<EncodingRecord> encodingRecords;
};

struct Format4
{
    uint16_t format;
    uint16_t length;
    uint16_t language;
    uint16_t segCountX2;
    uint16_t searchRange;
    uint16_t entrySelectory;
    uint16_t rangeShift;
    std::vector<uint16_t> endCode;        // Size of segCountX2 / 2
    uint16_t reservedPad;
    std::vector<uint16_t> startCode;      // Size of segCountX2 / 2
    std::vector<uint16_t> idDelta;        // Size of segCountX2 / 2
    std::vector<uint16_t> idRangeOffsets; // Size of segCountX2 / 2
    std::vector<uint16_t> glyphIdArray;   // Arbitrary size
};

class Cmap : public TrueTypeTable
{
    public:
        Cmap();
        ~Cmap();
        void readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset);
    private:
        void readFormat4(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset);
        uint16_t getGlyphIndex(const uint16_t cCodePoint);

        CmapHeader mCmapHeader;
        Format4 mFormat;
};

#endif