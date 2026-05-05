#ifndef GLYF_HPP
#define GLYF_HPP

#include "ttfTable.hpp"
#include "loca.hpp"

enum SimpleGlyphFlags
{
    ON_CURVE_POINT = 0x01,
    X_SHORT_VECTOR = 0x02,
    Y_SHORT_VECTOR = 0x04,
    REPEAT_FLAG    = 0x08,
    X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR = 0x10,
    Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR = 0x20,
    OVERLAP_SIMPLE = 0x40,
    SIMPLE_RESERVED = 0x80
};

struct SimpleGlyf
{
    //--------------------------Only Use if Glyph is a Simple Glyph---------------------------------
    // Array of point indices for the last point of each contour, in increasing numeric order
    std::vector<uint16_t> endPtsOfContours;

    // Total number of bytes for instructions. If instructionLength is zero, no instructions are present for this
    // glyph, and this field is followed directly by the flags field
    uint16_t instructionLength;
    std::vector<uint8_t> instructions; // Array of instruction byte code for the glyph

    // Array of flag elements. See below for details regarding the number of flag array elements
    std::vector<uint8_t> flags;

    // Contour point x-coordinates. See below for details regarding the number of coordinate array elements. Coordinate
    // for the first point is relative to (0,0); others are relative to previous point
    std::vector<int16_t> xCoordinates;

    // Contour point y-coordinates. See below for details regarding the number of coordinate array elements. Coordinate
    // for the first point is relative to (0,0); others are relative to previous point
    std::vector<int16_t> yCoordinates;
};

struct CompoundGlyf
{
    uint16_t flags;
    uint16_t glyphIndex;
    uint16_t arg1;
    uint16_t arg2;
};

struct GlyfHeader
{
    // If the number of contours is greater than or equal to zero, this is a simple glyph. If negative, this is a
    // composite glyph — the value -1 should be used for composite glyphs
    int16_t numberofContours;
    int16_t xMin; // Minimum x for coordinate data
    int16_t yMin; // Minimum y for coordinate data
    int16_t xMax; // Maximum x for coordinate data
    int16_t yMax; // Maximum y for coordinate data

    std::shared_ptr<SimpleGlyf> SimpleGlyfData;
    std::shared_ptr<CompoundGlyf> CompoundGlyfData;
};

class Glyf : public TrueTypeTable
{
    public:
        Glyf(){}
        ~Glyf(){}
        Glyf(const uint16_t cNumGlyphs, const std::shared_ptr<Loca>& crpLoca);
        int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
        std::vector<GlyfHeader> getGlyphOutlines();
        GlyfHeader getSpecifcCharacterOutline(const uint16_t cIndex);
    private:
        void readCompoundGlyph(uint8_t *pPtr, GlyfHeader& rGlyph);
        int8_t readSimpleGlyph(uint8_t *pPtr, GlyfHeader& rGlyph);
        int8_t readSimpleGlyphCoords(uint8_t *pPtr, std::shared_ptr<SimpleGlyf>& rGlyph);
        int8_t readCoord(uint8_t* &pPtr, int16_t& rCoord, const uint8_t cFlag);
        std::vector<GlyfHeader> mGlyfHeader;
        std::shared_ptr<Loca> mLoca;
};

#endif