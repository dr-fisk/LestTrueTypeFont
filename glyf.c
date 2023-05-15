#include "glyf.h"

Glyf::Glyf()
{
    memset(&mGlyfHeader, 0, sizeof(mGlyfHeader));
}

void Glyf::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
    std::cout << sizeof(mGlyfHeader) << std::endl;
    memcpy(&mGlyfHeader, crBuffer.data() + cOffset, sizeof(mGlyfHeader));

    mGlyfHeader.numberofContours = lesthtons(mGlyfHeader.numberofContours);
    mGlyfHeader.xMin = lesthtons(mGlyfHeader.xMin);
    mGlyfHeader.yMin = lesthtons(mGlyfHeader.yMin);
    mGlyfHeader.xMax = lesthtons(mGlyfHeader.xMax);
    mGlyfHeader.yMax = lesthtons(mGlyfHeader.yMax);

    std::cout << "Num Contours: " << mGlyfHeader.numberofContours << " xMin: " << mGlyfHeader.xMin << " yMin: " << mGlyfHeader.yMin << " xMax: " << mGlyfHeader.xMax << " yMax: " << mGlyfHeader.yMax << std::endl;
}