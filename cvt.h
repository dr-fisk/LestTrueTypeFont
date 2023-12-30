#ifndef CVT_H
#define CVT_H

#include <cstdint>
#include <vector>

#include "ttfTable.h"

struct CvtTable
{
  std::vector<uint16_t> instructions;
};

class Cvt : public TrueTypeTable
{
  public:
    Cvt();
    ~Cvt();
    int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
  private:
    CvtTable mCvt;
};

#endif