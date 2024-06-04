#ifndef HEAD_HPP
#define HEAD_HPP

#include "ttfTable.hpp"

struct HeadHeader
{
    uint16_t majorVersion;       //	Major version number of the font header table — set to 1.
    uint16_t minorVersion;       //	Minor version number of the font header table — set to 0.
    uint32_t fontRevision;       //	Minor version number of the font header table — set to 0.
    // To compute: set it to 0, sum the entire font as uint32, then store 0xB1B0AFBA - sum. If the font is used as a
    // component in a font collection file, the value of this field will be invalidated by changes to the file structure and font table directory, and must be ignored.
    uint32_t checksumAdjustment;
    uint32_t magicNumber;        // Set to 0x5F0F3CF5.
    uint16_t flags;              // See https://learn.microsoft.com/en-us/typography/opentype/spec/head
    // Set to a value from 16 to 16384. Any value in this range is valid. In fonts that have TrueType outlines, a
    // power of 2 is recommended as this allows performance optimizations in some rasterizers.
    uint16_t unitsPerEm;
    // Number of seconds since 12:00 midnight that started January 1st 1904 in GMT/UTC time zone.
    uint64_t created;
    // Number of seconds since 12:00 midnight that started January 1st 1904 in GMT/UTC time zone.
    uint64_t modified;
    int16_t  xMin;               // Minimum x coordinate across all glyph bounding boxes.
    int16_t  yMin;               //	Minimum y coordinate across all glyph bounding boxes.
    int16_t  xMax;               //	Maximum x coordinate across all glyph bounding boxes.
    int16_t  yMax;               //	Maximum y coordinate across all glyph bounding boxes.
    uint16_t macStyle;           // See https://learn.microsoft.com/en-us/typography/opentype/spec/head
    uint16_t lowestRecPPEM;      // Smallest readable size in pixels.
    int16_t  fontDirectionHint;  // See https://learn.microsoft.com/en-us/typography/opentype/spec/head
    int16_t  indexToLocFormat;   // 0 for short offsets (Offset16), 1 for long (Offset32).
    int16_t  glyphDataFormat;    // 0 for current format.
};

class HeadTable : public TrueTypeTable
{
    public:
        HeadTable();
        ~HeadTable(){}
        int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
        int16_t getLocaFormat() const;
        HeadHeader getHeadHeaderTable() const;
    private:
        HeadHeader mHeadHeader;
};

#endif