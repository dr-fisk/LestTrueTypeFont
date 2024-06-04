#include "prep.hpp"

Prep::Prep()
{
}

int8_t Prep::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
  if (0 >= cNumBytes)
  {
      std::cout << "Error: cnumbytes less than 0." << std::endl;
      return -1;
  }

  mPrep.controlValueProgram.resize(cNumBytes);

  memcpy(mPrep.controlValueProgram.data(), crBuffer.data() + cOffset, mPrep.controlValueProgram.size());

  return 0;
}

Prep::~Prep()
{
}