#ifndef POST_H
#define POST_H

#include "ttfTable.h"

struct PostHeader
{
    // 0x00010000 for version 1.0
    // 0x00020000 for version 2.0
    // 0x00025000 for version 2.5 (deprecated)
    // 0x00030000 for version 3.0
    int32_t version;
    // Italic angle in counter-clockwise degrees from the vertical. Zero for upright text, negative for text that leans
    // to the right (forward)
    int32_t italicAngle;
    int16_t underlinePosition;  // See https://learn.microsoft.com/en-us/typography/opentype/spec/post
    int16_t underlineThickness; // See https://learn.microsoft.com/en-us/typography/opentype/spec/post
    // Set to 0 if the font is proportionally spaced, non-zero if the font is not proportionally spaced
    // (i.e. monospaced).
    uint32_t isFixedPitch;
    uint32_t minMemType42;      // Minimum memory usage when an OpenType font is downloaded
    uint32_t maxMemType42;      // Maximum memory usage when an OpenType font is downloaded
    uint32_t minMemType1;       // Minimum memory usage when an OpenType font is downloaded as a Type 1 font
    uint32_t maxMemType1;       // Maximum memory usage when an OpenType font is downloaded as a Type 1 font
    uint16_t numGlyphs;         // Number of glyphs (this should be the same as numGlyphs in 'maxp' table)
    std::vector<uint16_t> glyphNameIndex; // Array of indices into the string data. See below for details
    std::vector<uint8_t> stringData;      // Storage for the string data
};

class Post : public TrueTypeTable
{
    public:
        Post(){}
        // Post(const uint16_t cNumGlyphs);
        ~Post(){}
        void readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
    private:
        PostHeader mPostHeader;
};

#endif