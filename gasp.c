#include "gasp.h"

Gasp::Gasp()
{
}

int8_t Gasp::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
  if (0 >= cNumBytes)
  {
      std::cout << "Error: cnumbytes less than 0." << std::endl;
      return -1;
  }

  uint8_t *temp_ptr = (uint8_t *)crBuffer.data();
  temp_ptr = temp_ptr + cOffset;

  mGasp.version = read2BytesPtr(temp_ptr);
  mGasp.numRanges = read2BytesPtr(temp_ptr);

  mGasp.gaspRange.resize(mGasp.numRanges);

  for(auto& range : mGasp.gaspRange)
  {
    range.rangeMaxPPEM = read2BytesPtr(temp_ptr);
    range.rangeGaspBehavior = read2BytesPtr(temp_ptr);
  }

  return 0;
}

Gasp::~Gasp()
{
}