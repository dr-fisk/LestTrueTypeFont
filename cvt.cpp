#include "cvt.hpp"

Cvt::Cvt()
{
}

int8_t Cvt::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
  if (0 >= cNumBytes)
  {
      std::cout << "Error: cnumbytes less than 0." << std::endl;
      return -1;
  }

  mCvt.instructions.resize(cNumBytes / sizeof(uint16_t));

  memcpy(mCvt.instructions.data(), crBuffer.data() + cOffset, mCvt.instructions.size());

  return 0;
}

Cvt::~Cvt()
{
}