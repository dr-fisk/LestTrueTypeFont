#ifndef KERN_HPP
#define KERN_HPP

#include "ttfTable.hpp"

struct KernHeader
{
  uint32_t version; // Table version number—set to 0 for windows.
  uint32_t nTables; // Number of subtables in the kerning table.
};

struct KernSubTableHeader
{
  uint32_t length;     // Kern subtable version number—set to 0.
  uint16_t coverage;   // Length of the subtable, in bytes (including this header).
  uint16_t tupleIndex; // What type of information is contained in this table.
};

struct KernPair
{
  uint16_t left;  // The glyph index for the left-hand glyph in the kerning pair.
  uint16_t right; // The glyph index for the right-hand glyph in the kerning pair.

  // The kerning value for the above pair, in font design units. If this value is greater than zero, the characters
  //  will be moved apart. If this value is less than zero, the character will be moved closer together.
  int16_t value;
};

struct KernSubtableFormat0
{
  uint16_t nPairs;                 // This gives the number of kerning pairs in the table.

  // The largest power of two less than or equal to the value of nPairs, multiplied by the size in bytes of an entry
  //  in the table.
  uint16_t searchRange;
  
  // This is calculated as log2 of the largest power of two less than or equal to the value of nPairs. This value 
  // indicates how many iterations of the search loop will have to be made.
  // (For example, in a list of eight items, there would have to be three iterations of the loop).
  uint16_t entrySelector;

  // The value of nPairs minus the largest power of two less than or equal to nPairs, and then multiplied by the size
  // in bytes of an entry in the table.
  uint16_t rangeShift;
  std::vector<KernPair> kernPairs; // Array of KernPair records.
};

class Kern : public TrueTypeTable
{
    public:
      Kern() = default;
      ~Kern() = default;
      int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
    private:
      KernHeader mKernHeader;
      KernSubTableHeader mKernSubTableHeader;
      KernSubtableFormat0 mFormatTable;
};

#endif