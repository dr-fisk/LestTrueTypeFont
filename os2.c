#include "os2.h"

OS2::OS2()
{
}

OS2::~OS2()
{
}

int8_t OS2::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
  if (0 == cNumBytes)
  {
      std::cout << "Error: Cannot copy 0 bytes." << std::endl;
      return -1;
  }

  uint8_t *temp_ptr = (uint8_t *)crBuffer.data();
  temp_ptr = temp_ptr + cOffset;

  mOsTable.version = read2BytesPtr(temp_ptr);
  mOsTable.xAvgCharWidth = read2BytesPtr(temp_ptr);
  mOsTable.usWeightClass = read2BytesPtr(temp_ptr);
  mOsTable.usWidthClass = read2BytesPtr(temp_ptr);
  mOsTable.fsType = read2BytesPtr(temp_ptr);
  mOsTable.ySubscriptXSize = read2BytesPtr(temp_ptr);
  mOsTable.ySubscriptYSize = read2BytesPtr(temp_ptr);
  mOsTable.ySubscriptXOffset = read2BytesPtr(temp_ptr);
  mOsTable.ySubscriptYOffset = read2BytesPtr(temp_ptr);
  mOsTable.ySuperscriptXSize = read2BytesPtr(temp_ptr);
  mOsTable.ySuperscriptYSize = read2BytesPtr(temp_ptr);
  mOsTable.ySuperscriptXOffset = read2BytesPtr(temp_ptr);
  mOsTable.ySuperscriptYOffset = read2BytesPtr(temp_ptr);
  mOsTable.yStrikeoutSize = read2BytesPtr(temp_ptr);
  mOsTable.yStrikeoutPosition = read2BytesPtr(temp_ptr);
  mOsTable.sFamilyClass = read2BytesPtr(temp_ptr);
  memcpy(mOsTable.panose, temp_ptr, sizeof(mOsTable.panose));
  temp_ptr = temp_ptr + sizeof(mOsTable.panose);
  mOsTable.ulUnicodeRange1 = read4BytesPtr(temp_ptr);
  mOsTable.ulUnicodeRange2 = read4BytesPtr(temp_ptr);
  mOsTable.ulUnicodeRange3 = read4BytesPtr(temp_ptr);
  mOsTable.ulUnicodeRange4 = read4BytesPtr(temp_ptr);
  mOsTable.achVendId = read4BytesPtr(temp_ptr);
  mOsTable.fsSelection = read2BytesPtr(temp_ptr);
  mOsTable.usFirstCharIndex = read2BytesPtr(temp_ptr);
  mOsTable.usLastCharIndex = read2BytesPtr(temp_ptr);
  mOsTable.sTypoAscender = read2BytesPtr(temp_ptr);
  mOsTable.sTypoDescender = read2BytesPtr(temp_ptr);
  mOsTable.sTypoLineGap = read2BytesPtr(temp_ptr);
  mOsTable.usWinAscent = read2BytesPtr(temp_ptr);
  mOsTable.usWinDescent = read2BytesPtr(temp_ptr);
  mOsTable.ulCodePageRange1 = read4BytesPtr(temp_ptr);
  mOsTable.ulCodePageRange2 = read4BytesPtr(temp_ptr);
  mOsTable.sxHeight = read2BytesPtr(temp_ptr);
  mOsTable.sCapHeight = read2BytesPtr(temp_ptr);
  mOsTable.usDefaultChar = read2BytesPtr(temp_ptr);
  mOsTable.usBreakChar = read2BytesPtr(temp_ptr);
  mOsTable.usMaxContext = read2BytesPtr(temp_ptr);
  mOsTable.usLowerOpticalPointSize = read2BytesPtr(temp_ptr);
  mOsTable.usUpperOpticalPointSize = read2BytesPtr(temp_ptr);
  return 0;
}

OS2Table OS2::getOS2Table() const
{
  return mOsTable;
}