#ifndef PREP_HPP
#define PREP_HPP

#include <cstdint>

#include "ttfTable.hpp"

struct PrepTable
{
  std::vector<uint8_t> controlValueProgram;
};

class Prep : public TrueTypeTable
{
  public:
    Prep();
    ~Prep();
    int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
  private:
    PrepTable mPrep;
};

#endif