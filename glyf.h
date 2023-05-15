#ifndef GLYF_H
#define GLYF_H

#include "ttfTable.h"

struct GlyfHeader
{
    int16_t numberofContours;
    int16_t xMin;
    int16_t yMin;
    int16_t xMax;
    int16_t yMax;
};

class Glyf : public TrueTypeTable
{
    public:
        Glyf();
        ~Glyf(){}
        void readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
    private:
        GlyfHeader mGlyfHeader;
};

#endif