#include "vdmx.h"

Vdmx::Vdmx()
{

}

Vdmx::~Vdmx()
{

}

int8_t Vdmx::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
  if (0 == cNumBytes)
  {
    std::cout << "Error: Cannot copy 0 bytes." << std::endl;
    return -1;
  }

  uint8_t *temp_ptr = (uint8_t *)crBuffer.data();
  temp_ptr = temp_ptr + cOffset;

  mVdmxHeader.version = read2BytesPtr(temp_ptr);
  mVdmxHeader.numRecs = read2BytesPtr(temp_ptr);
  mVdmxHeader.numRatios = read2BytesPtr(temp_ptr);
  mVdmxHeader.ratRange.resize(mVdmxHeader.numRatios);
  mVdmxHeader.vdmxGroupOffsets.resize(mVdmxHeader.numRatios);

  memcpy(mVdmxHeader.ratRange.data(), temp_ptr, sizeof(RatioRange) * mVdmxHeader.ratRange.size());
  temp_ptr = temp_ptr + sizeof(RatioRange) * mVdmxHeader.ratRange.size();
  memcpy(mVdmxHeader.vdmxGroupOffsets.data(), temp_ptr, sizeof(uint16_t) * mVdmxHeader.vdmxGroupOffsets.size());

  return 0;
}

VdmxHeader Vdmx::getVdmxHeader() const
{
  return mVdmxHeader;
}