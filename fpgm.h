#ifndef FPGM_H
#define FPGM_H

#include <cstdint>

#include "ttfTable.h"

struct FpgmTable
{
  std::vector<uint8_t> instructions;
};

class Fpgm : public TrueTypeTable
{
  public:
    Fpgm();
    ~Fpgm();
    int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
  private:
    FpgmTable mFpgm;
};

#endif