#include "kern.hpp"

int8_t Kern::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
  if (0 >= cNumBytes)
  {
      std::cout << "Error: cnumbytes less than 0." << std::endl;
      return -1;
  }

  uint8_t* ptr = (uint8_t *) crBuffer.data() + cOffset;

  mKernHeader.version = read2BytesPtr(ptr);
  mKernHeader.nTables = read2BytesPtr(ptr);

  mKernSubTableHeader.length = read4BytesPtr(ptr);
  mKernSubTableHeader.coverage = read2BytesPtr(ptr);
  mKernSubTableHeader.tupleIndex = read2BytesPtr(ptr);

  mFormatTable.nPairs = read2BytesPtr(ptr);
  mFormatTable.searchRange = read2BytesPtr(ptr);
  mFormatTable.entrySelector = read2BytesPtr(ptr);
  mFormatTable.rangeShift = read2BytesPtr(ptr);
  mFormatTable.kernPairs.resize(mFormatTable.nPairs);

  for(auto& kerns : mFormatTable.kernPairs)
  {
    kerns.left = read2BytesPtr(ptr);
    kerns.right = read2BytesPtr(ptr);
    kerns.value = read4BytesPtr(ptr);
  }

  return 0;
}