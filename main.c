#include "lestTtf.h"
#include <iostream>

void printHeaderTable(const LestTrueType cFont)
{
  HeadHeader head = cFont.getHeadHeaderTable();

  std::cout << "Head Header Table Specs" << std::endl;
  std::cout << "------------------------------------------------------------------" << std::endl;
  std::cout << "xMin: " << head.xMin << std::endl;
  std::cout << "xMax: " << head.xMax << std::endl;
  std::cout << "yMin: " << head.yMin << std::endl;
  std::cout << "yMax: " << head.yMax << std::endl;
  std::cout << "------------------------------------------------------------------" << std::endl;
}

void printGlyph(const LestTrueType cFont, const int16_t cChar)
{
  GlyfHeader glyf;
  cFont.getSpecifcCharacterOutline(cChar, glyf);

  std::cout << (char)cChar << " Specifications" << std::endl;
  std::cout << "------------------------------------------------------------------" << std::endl;
  std::cout << "Num contours: " << glyf.numberofContours << std::endl;
  std::cout << "xMin: " << glyf.xMin << std::endl;
  std::cout << "xMax: " << glyf.xMax << std::endl;
  std::cout << "yMin: " << glyf.yMin << std::endl;
  std::cout << "yMax: " << glyf.yMax << std::endl;
  std::cout << "------------------------------------------------------------------" << std::endl;
}

void printOs2(const LestTrueType cFont)
{
  OS2Table os = cFont.getOS2Table();

  std::cout << "OS2 Table Specs" << std::endl;
  std::cout << "------------------------------------------------------------------" << std::endl;
  std::cout << "OS2 Version: " << os.version << std::endl;
  std::cout << "OS2 xAvgCharWidth: " << os.xAvgCharWidth << std::endl;
  std::cout << "OS2 usWeightClass: " << os.usWeightClass << std::endl;
  std::cout << "OS2 usWidthClass: " << os.usWidthClass << std::endl;
  std::cout << "OS2 fsType:" << os.fsType << std::endl;
  std::cout << "OS2 ySubscriptXSize:" << os.ySubscriptXSize << std::endl;
  std::cout << "OS2 ySubscriptYSize:" << os.ySubscriptYSize << std::endl;
  std::cout << "OS2 ySubscriptXOffset: " << os.ySubscriptXOffset << std::endl;
  std::cout << "OS2 ySubscriptYOffset: " << os.ySubscriptYOffset << std::endl;
  std::cout << "OS2 ySuperscriptXSize: " << os.ySuperscriptXSize << std::endl;
  std::cout << "OS2 ySuperscriptYSize: " << os.ySuperscriptYSize << std::endl;
  std::cout << "OS2 ySuperscriptXOffset: " << os.ySuperscriptXOffset << std::endl;
  std::cout << "OS2 ySuperscriptYOffset: " << os.ySuperscriptYOffset << std::endl;
  std::cout << "OS2 yStrikeoutSize: " << os.yStrikeoutSize << std::endl;
  std::cout << "OS2 yStrikeoutPosition: " << os.yStrikeoutPosition << std::endl;
  std::cout << "OS2 sFamilyClass: " << os.sFamilyClass << std::endl;
  std::cout << "OS2 ulUnicodeRange1: " << os.ulUnicodeRange1 << std::endl;
  std::cout << "OS2 ulUnicodeRange2: " << os.ulUnicodeRange2 << std::endl;
  std::cout << "OS2 ulUnicodeRange3: " << os.ulUnicodeRange3 << std::endl;
  std::cout << "OS2 ulUnicodeRange4: " << os.ulUnicodeRange4 << std::endl;
  std::cout << "OS2 achVendId: " << os.achVendId << std::endl;
  std::cout << "OS2 fsSelection: " << os.fsSelection << std::endl;
  std::cout << "OS2 usFirstCharIndex: " << os.usFirstCharIndex << std::endl;
  std::cout << "OS2 usLastCharIndex: " << os.usLastCharIndex << std::endl;
  std::cout << "OS2 sTypoAscender: " << os.sTypoAscender << std::endl;
  std::cout << "OS2 sTypoDescender: " << os.sTypoDescender << std::endl;
  std::cout << "OS2 sTypoLineGap: " << os.sTypoLineGap << std::endl;
  std::cout << "OS2 usWinAscent: " << os.usWinAscent << std::endl;
  std::cout << "OS2 usWinDescent: " << os.usWinDescent << std::endl;
  std::cout << "OS2 ulCodePageRange1: " << os.ulCodePageRange1 << std::endl;
  std::cout << "OS2 ulCodePageRange2: " << os.ulCodePageRange2 << std::endl;
  std::cout << "OS2 sxHeight: " << os.sxHeight << std::endl;
  std::cout << "OS2 sCapHeight: " << os.sCapHeight << std::endl;
  std::cout << "OS2 usDefaultChar: " << os.usDefaultChar << std::endl;
  std::cout << "OS2 usBreakChar: " << os.usBreakChar << std::endl;
  std::cout << "OS2 usMaxContext: " << os.usMaxContext << std::endl;
  std::cout << "OS2 usLowerOpticalPointSize: " << os.usLowerOpticalPointSize << std::endl;
  std::cout << "OS2 usUpperOpticalPointSize: " << os.usUpperOpticalPointSize << std::endl;
  std::cout << "------------------------------------------------------------------" << std::endl;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "Error: Wrong number of args provided." << std::endl;
    std::cout << "Usage: ./main <path to ttf> <-h|-help|-head|-glyph <char|char decimal>|-os2>" << std::endl;
    return -1;
  }

  if (2 == argc)
  {
    std::string argument = argv[1];

    if("-h" == argument || "-help" == argument)
    {
    std::cout << "Usage: ./main <path to ttf> <-h|-help|-head|-glyph <char|char decimal>|-os2>" << std::endl;
      return 0;
    }
    else
    {
      std::cout << "Invalid arg supplied." << std::endl;
      std::cout << "Usage: ./main <path to ttf> <-h|-help|-head|-glyph <char|char decimal>|-os2>" << std::endl;
      return -1;
    }
  }

  std::string path = argv[1];
  LestTrueType font;

  if(-1 == font.read(path))
  {
    std::cout << "Failed to read ttf file " << argv[1] << "." << " Terminating process." << std::endl;
    return -1;
  }

  std::vector<std::string> args;

  for(int i = 2; i < argc; i++)
  {
    args.push_back(argv[i]);
  }
  
  size_t currIdx = 0;
  while( currIdx < args.size())
  {
    if("-head" == args[currIdx])
    {
      printHeaderTable(font);
    }
    else if("-glyph" == args[currIdx])
    {
      currIdx ++;
      int charDec = 0;

      try
      {
        if(std::isdigit(args[currIdx][0]))
        {
          charDec = stoi(args[currIdx]);
        }
        else
        {
          charDec = (int)args[currIdx][0];
        }
      }
      catch(const std::exception& crException)
      {
        std::cout << "Failed to interpret glyph provided. Useage of command is -glyph <char|char decimal>." <<
        std::endl;
        return -1;
      }

      printGlyph(font, charDec);
    }
    else if("-os2" == args[currIdx])
    {
      printOs2(font);
    }
    else
    {
      std::cout << "Unknown Command encountered. Terminating process." << std::endl;
      return -1;
    }

    currIdx ++;
  }

  return 0;
}