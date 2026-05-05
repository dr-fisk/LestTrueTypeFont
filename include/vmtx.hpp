#ifndef VMTX_HPP
#define VMTX_HPP

#include "ttfTable.hpp"

class Vmtx : public TrueTypeTable
{
  public:
    Vmtx();
    ~Vmtx();
    int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
  private:

};

#endif