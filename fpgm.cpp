#include "fpgm.hpp"

Fpgm::Fpgm()
{
}

int8_t Fpgm::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
  if (0 >= cNumBytes)
  {
      std::cout << "Error: cnumbytes less than 0." << std::endl;
      return -1;
  }

  mFpgm.instructions.resize(cNumBytes);

  memcpy(mFpgm.instructions.data(), crBuffer.data() + cOffset, mFpgm.instructions.size());

  return 0;
}

Fpgm::~Fpgm()
{
}