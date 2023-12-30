#ifndef VHEA_H
#define VHEA_H

#include "ttfTable.h"

class Vhea : public TrueTypeTable
{
  public:
    Vhea();
    ~Vhea();
    int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
  private:

};

#endif