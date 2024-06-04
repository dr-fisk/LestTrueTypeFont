#ifndef CVT_HPP
#define CVT_HPP

#include <cstdint>
#include <vector>

#include "ttfTable.hpp"

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