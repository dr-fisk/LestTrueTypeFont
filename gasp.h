#ifndef GASP_H
#define GASP_H

#include <cstdint>

#include "ttfTable.h"

struct GaspRange
{
  uint16_t rangeMaxPPEM;
  uint16_t rangeGaspBehavior;
};

struct GaspTable
{
  uint16_t version;
  uint16_t numRanges;
  std::vector<GaspRange> gaspRange;
};

class Gasp : public TrueTypeTable
{
  public:
    Gasp();
    ~Gasp();
    int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
  private:
    GaspTable mGasp;
};

#endif