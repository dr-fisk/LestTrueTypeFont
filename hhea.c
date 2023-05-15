#include "hhea.h"

 void Hhea::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
 {
    memcpy(&mHheaHeader, crBuffer.data() + cOffset, sizeof(mHheaHeader));

    mHheaHeader.version = lesthtonl(mHheaHeader.version);  
    mHheaHeader.ascent = lesthtons(mHheaHeader.ascent);   
    mHheaHeader.descent = lesthtons(mHheaHeader.descent);
    mHheaHeader.lineGap = lesthtons(mHheaHeader.lineGap);
    mHheaHeader.advancedWidthMax = lesthtons(mHheaHeader.advancedWidthMax);
    mHheaHeader.minLeftSideBearing = lesthtons(mHheaHeader.minLeftSideBearing);
    mHheaHeader.minRightSideBearing = lesthtons(mHheaHeader.minRightSideBearing);
    mHheaHeader.xMaxExtent = lesthtons(mHheaHeader.xMaxExtent);
    mHheaHeader.caretSlopeRise = lesthtons(mHheaHeader.caretSlopeRise);
    mHheaHeader.caretSlopeRun = lesthtons(mHheaHeader.caretSlopeRun);
    mHheaHeader.caretOffset = lesthtons(mHheaHeader.caretOffset);
    mHheaHeader.metricDataFormat = lesthtons(mHheaHeader.metricDataFormat);
    mHheaHeader.numOfLongHorMetrics = lesthtons(mHheaHeader.numOfLongHorMetrics);

    std::cout << "Version: " << mHheaHeader.version << std::endl;
    std::cout << "Ascent: " << mHheaHeader.ascent << std::endl;  
    std::cout << "Descent: " << mHheaHeader.descent << std::endl;
    std::cout << "Line Gap: " << mHheaHeader.lineGap << std::endl;
    std::cout << "Advanced Width Max: " << mHheaHeader.advancedWidthMax << std::endl;
    std::cout << "Min Left Side Bearing: " << mHheaHeader.minLeftSideBearing << std::endl;
    std::cout << "Min Right Side Bearing: " << mHheaHeader.minRightSideBearing << std::endl;
    std::cout << "xMaxExtent: " << mHheaHeader.xMaxExtent << std::endl;
    std::cout << "Caret Slope Rise: " << mHheaHeader.caretSlopeRise << std::endl;
    std::cout << "Caret Slope Run: " << mHheaHeader.caretSlopeRun << std::endl;
    std::cout << "Caret Offset: " << mHheaHeader.caretOffset << std::endl;
    std::cout << "Metric Data Format: " << mHheaHeader.metricDataFormat << std::endl;
    std::cout << "Num Of Long Hor Metrics: " << mHheaHeader.numOfLongHorMetrics << std::endl;
 }

 uint16_t Hhea::getNumOfLongHorMetrics() const
 {
    return mHheaHeader.numOfLongHorMetrics;
 }