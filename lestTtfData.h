#ifndef LEST_TTF_DATA_H
#define LEST_TTF_DATA_H

#include <cstdint>
#include <vector>

static const uint8_t TAG_SIZE = 4;

struct TableDirectory
{
    char tag[TAG_SIZE];
    uint32_t checkSum;
    uint32_t offset; // Offset from the start of file
    uint32_t length;
};

struct OffsetSubtable
{
    uint32_t scalarType;
    uint16_t numTables;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;
};

struct FontDirectory
{
    OffsetSubtable offSub;
    std::vector<TableDirectory> tableDir;
};

#endif