#ifndef LEST_TTF_DATA_H
#define LEST_TTF_DATA_H

#include <cstdint>
#include <vector>

static const uint8_t TAG_SIZE = 4;

struct TableDirectory
{
    char tag[TAG_SIZE]; // 4-byte identifier
    uint32_t checkSum;  // checksum for this table
    uint32_t offset;    // Offset from the start of file
    uint32_t length;    // length of this table in byte (actual length not padded length)
};

struct OffsetSubtable
{
    uint32_t scalarType;    // A tag to indicate the OFA scaler to be used to rasterize this font
    uint16_t numTables;     // number of tables
    uint16_t searchRange;   // (maximum power of 2 <= numTables)*16
    uint16_t entrySelector; // log2(maximum power of 2 <= numTables)
    uint16_t rangeShift;    // numTables*16-searchRange
};

struct FontDirectory
{
    OffsetSubtable offSub;
    std::vector<TableDirectory> tableDir;
};

#endif