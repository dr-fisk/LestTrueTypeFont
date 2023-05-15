#ifndef CMAP_H
#define CMAP_H

#include <cstdint>
#include <vector>
#include "ttfTable.h"

// Only supporting unicode for now
static const uint8_t CMAP_HEADER_SIZE = 4;

struct EncodingRecord
{
    uint16_t platformID;     // Platform ID
    uint16_t encodingID;     // Platform-specific encoding ID
    uint32_t subTableOffset; // From the start of cmap data
};

struct CmapHeader
{
    uint16_t version;                             //	Table version number (0).
    uint16_t numTables;                           // Number of encoding tables that follow.
    std::vector<EncodingRecord> encodingRecords;
};

struct Format4
{
    uint16_t format;                      // Platform-specific encoding ID
    uint16_t length;                      // This is the length in bytes of the subtable.
    // For requirements on use of the language field, see “Use of the language field in 'cmap' subtables” in this
    // document.
    uint16_t language;
    uint16_t segCountX2;                  //  2 × segCount.
    // Maximum power of 2 less than or equal to segCount, times 2 ((2**floor(log2(segCount))) * 2,
    // where “**” is an exponentiation operator)
    uint16_t searchRange;
    // Log2 of the maximum power of 2 less than or equal to segCount (log2(searchRange/2), which is equal to
    // floor(log2(segCount)))
    uint16_t entrySelectory;
    uint16_t rangeShift;                  // segCount times 2, minus searchRange ((segCount * 2) - searchRange)
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
        void readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
        uint16_t getGlyphIndex(const uint16_t cCodePoint);
    private:
        void readFormat4(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset);

        CmapHeader mCmapHeader;
        Format4 mFormat;
};

#endif