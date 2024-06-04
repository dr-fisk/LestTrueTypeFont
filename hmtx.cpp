#include "hmtx.hpp"

Hmtx::Hmtx(const uint16_t cNumHorMetrics, const uint16_t cNumGlyphs)
{
    if (0 == cNumHorMetrics || 0 == cNumGlyphs)
    {
        std::cout << "Error: Number of glyphs or hor metrics incorrect." << std::endl;
    }
    else
    {
        mHmtxHeader.hMetrics.resize(cNumHorMetrics);
        mHmtxHeader.leftSideBearing.resize(cNumGlyphs - cNumHorMetrics);
    }
}

int8_t Hmtx::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
    if (0 >= cNumBytes)
    {
        std::cout << "Error: cnumbytes less than 0." << std::endl;
        return -1;
    }

    memcpy(mHmtxHeader.hMetrics.data(), crBuffer.data() + cOffset,
           mHmtxHeader.hMetrics.size() * sizeof(LongHorMetric));
    uint32_t next_offset = cOffset + mHmtxHeader.hMetrics.size() * sizeof(LongHorMetric);
    memcpy(mHmtxHeader.leftSideBearing.data(), crBuffer.data() + next_offset,
           mHmtxHeader.leftSideBearing.size() * sizeof(uint16_t));

    int i = 0;
    for(auto& metrics: mHmtxHeader.hMetrics)
    {
        metrics.advanceWidth = lesthtons(metrics.advanceWidth);
        metrics.leftSideBearing = lesthtons(metrics.leftSideBearing);

        #ifdef DEBUG
            std::cout << i << ". advWid: " << metrics.advanceWidth <<  ", LSBear: " << metrics.leftSideBearing << std::endl;
        #endif
        
        i++;
    }
    
    i = 0;
    for(auto& side_bearing : mHmtxHeader.leftSideBearing)
    {
        side_bearing = lesthtons(side_bearing);

        #ifdef DEBUG
            std::cout << i << ". Left Side Bearing Val: " << side_bearing << std::endl;
        #endif

        i++;
    }

    return 1;
}

LongHorMetric Hmtx::getLongHorMetrics(const uint16_t cCodePoint) const
{
  if(cCodePoint > mHmtxHeader.hMetrics.size())
  {
    return LongHorMetric{};
  }

  return mHmtxHeader.hMetrics[cCodePoint];
}