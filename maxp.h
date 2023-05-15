#ifndef MAXP_H
#define MAXP_H

#include "ttfTable.h"

struct MaxpV1
{
    int32_t version;                // Version 1.0
    uint16_t numGlyphs;             // the number of glyphs in the font
    uint16_t maxPoints;             // points in non-compound glyph
    uint16_t maxContours;           // contours in non-compound glyph
    uint16_t maxComponentPoints;    // points in compound glyph
    uint16_t maxComponentContours;  // contours in compound glyph
    uint16_t maxZones;              // set to 2
    uint16_t maxTwilightPoints;     // points used in Twilight Zone (Z0)
    uint16_t maxStorage;            // number of Storage Area locations
    uint16_t maxFunctionDefs;       // number of FDEFs
    uint16_t maxInstructionDefs;    // number of IDEFs
    uint16_t maxStackElements;      // maximum stack depth
    uint16_t maxSizeOfInstructions; // byte count for glyph instructions
    uint16_t maxComponentElements;  // number of glyphs referenced at top level
    uint16_t maxComponentDepth;     // levels of recursion, set to 0 if font has only simple glyphs
};

class Maxp : public TrueTypeTable
{
    public:
        Maxp(){}
        ~Maxp(){}
        void readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
        uint16_t getNumGlyphs() const;
    private:
        MaxpV1 mMaxpV1;
};

#endif