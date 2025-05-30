#ifndef OS2_HPP
#define OS2_HPP

#include "ttfTable.hpp"

struct OS2Table
{
  uint16_t version;
  int16_t  xAvgCharWidth;
  uint16_t usWeightClass;
  uint16_t usWidthClass;
  uint16_t fsType;
  int16_t  ySubscriptXSize;
  int16_t  ySubscriptYSize;
  int16_t  ySubscriptXOffset;
  int16_t  ySubscriptYOffset;
  int16_t  ySuperscriptXSize;
  int16_t  ySuperscriptYSize;
  int16_t  ySuperscriptXOffset;
  int16_t  ySuperscriptYOffset;
  int16_t  yStrikeoutSize;
  int16_t  yStrikeoutPosition;
  int16_t  sFamilyClass;
  uint8_t  panose[10];
  uint32_t ulUnicodeRange1;
  uint32_t ulUnicodeRange2;
  uint32_t ulUnicodeRange3;
  uint32_t ulUnicodeRange4;
  uint32_t achVendId;
  uint16_t fsSelection;
  uint16_t usFirstCharIndex;
  uint16_t usLastCharIndex;
  int16_t  sTypoAscender;
  int16_t  sTypoDescender;
  int16_t  sTypoLineGap;
  uint16_t usWinAscent;
  uint16_t usWinDescent;
  uint32_t ulCodePageRange1;
  uint32_t ulCodePageRange2;
  int16_t  sxHeight;
  int16_t  sCapHeight;
  uint16_t usDefaultChar;
  uint16_t usBreakChar;
  uint16_t usMaxContext;
  uint16_t usLowerOpticalPointSize;
  uint16_t usUpperOpticalPointSize;
};

class OS2 : public TrueTypeTable
{
  public:
    OS2();
    ~OS2();
    int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
    OS2Table getOS2Table() const;
  private:
    OS2Table mOsTable;
};

#endif