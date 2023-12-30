#ifndef VDMX_H
#define VDMX_H

#include "ttfTable.h"

struct RatioRange
{
  uint8_t bCharSet;    // Character set (see below)
  uint8_t xRatio;      // Value to use for x-Ratio
  uint8_t yStartRatio; // Starting y-Ratio value
  uint8_t yEndRatio;   // Ending y-Ratio value
};

struct VdmxHeader
{
  uint16_t version;                                 // Version number (0 or 1).
  uint16_t numRecs;                                 //Number of VDMX groups present
  uint16_t numRatios;                               // Number of aspect ratio groupings
  std::vector<RatioRange> ratRange;                 // Ratio record array
  //Offset from start of this table to the VDMXGroup table for a corresponding RatioRange record
  std::vector<uint16_t> vdmxGroupOffsets;
};

class Vdmx : public TrueTypeTable
{
  public:
    Vdmx();
    ~Vdmx();
    int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
    VdmxHeader getVdmxHeader() const;
  private:
    VdmxHeader mVdmxHeader;
};

#endif