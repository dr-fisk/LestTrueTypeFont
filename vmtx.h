#ifndef VMTX_H
#define VMTX_H

#include "ttfTable.h"

class Vmtx : public TrueTypeTable
{
  public:
    Vmtx();
    ~Vmtx();
    int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
  private:

};

#endif