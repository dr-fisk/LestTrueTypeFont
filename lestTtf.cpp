#include <fstream>
#include <iostream>
#include <cstring>
#include <queue>

#include "lestTtf.hpp"
#include "lestTableData.hpp"
#include "endianGeneral.hpp"
#include "cmap.hpp"
#include "hmtx.hpp"
#include "maxp.hpp"
#include "loca.hpp"
#include "name.hpp"
#include "post.hpp"
#include "cvt.hpp"
#include "fpgm.hpp"
#include "gasp.hpp"
#include "prep.hpp"
#include "kern.hpp"

static const std::map<std::string, TableData> sTableTags = {
                                                            // Required Tables in ttf files
                                                            {sCMAP, {TTF_CMAP, TABLE_CMAP, false}},
                                                            {sGLYF, {TTF_GLYF, TABLE_GLYF, true}},
                                                            {sHEAD, {TTF_HEAD, TABLE_HEAD, false}},
                                                            {sHHEA, {TTF_HHEA, TABLE_HHEA, false}},
                                                            {sHMTX, {TTF_HMTX, TABLE_HMTX, true}},
                                                            {sLOCA, {TTF_LOCA, TABLE_LOCA, false}},
                                                            {sMAXP, {TTF_MAXP, TABLE_MAXP, false}},
                                                            {sNAME, {TTF_NAME, TABLE_NAME, false}},
                                                            {sPOST, {TTF_POST, TABLE_POST, false}},
                                                            {sOS2,  {TTF_OS2, TABLE_OS2,   false}},
                                                            // Option Tables in ttf files
                                                            {sCVT,  {TTF_NOTREQ, TABLE_CVT,  false}},
                                                            {sFPGM, {TTF_NOTREQ, TABLE_FPGM, false}},
                                                            {sHDMX, {TTF_NOTREQ, TABLE_HDMX, false}},
                                                            {sKERN, {TTF_NOTREQ, TABLE_KERN, false}},
                                                            {sPREP, {TTF_NOTREQ, TABLE_PREP, false}},
                                                            {sVDMX, {TTF_NOTREQ, TABLE_VDMX, false}},
                                                            {sGASP, {TTF_NOTREQ, TABLE_GASP, false}},
                                                            {sDSIG, {TTF_NOTREQ, TABLE_DSIG, false}},
                                                            {sGDEF, {TTF_NOTREQ, TABLE_GDEF, false}},
                                                            {sGSUB, {TTF_NOTREQ, TABLE_GSUB, false}},
                                                            {sJSTF, {TTF_NOTREQ, TABLE_JSTF, false}},
                                                            {sLTSH, {TTF_NOTREQ, TABLE_LTSH, false}},
                                                            {sPCLT, {TTF_NOTREQ, TABLE_PCLT, false}},
                                                            {sPCLT, {TTF_NOTREQ, TABLE_PCLT, false}},
                                                            {sPCLT, {TTF_NOTREQ, TABLE_PCLT, false}}};

LestTrueType::LestTrueType() :  mBufferOffset(0)
{
    memset(&mFontDirectory.offSub, 0, sizeof(mFontDirectory.offSub));
}

/* Function:    read
   Description: Reads in the data from the ttf buffer and begins constructing all data
   Parameters:  std::string - File Path to ttf
   Returns:     int8_t      - success or error encountered
 */
int8_t LestTrueType::read(const std::string& crTtfPath)
{
    std::ifstream ttfFile(crTtfPath, std::ios::binary);

    if (!ttfFile.is_open())
    {
        std::cout << "Failed to open ttf file: " << crTtfPath << std::endl;
        return -1;
    }

    // Create a buffer to contain entire file size
    ttfFile.seekg(0, std::ios::end);
    std::vector<uint8_t> ttfBuffer(ttfFile.tellg());
    ttfFile.seekg(0, std::ios::beg);

    ttfFile.read((char *)ttfBuffer.data(), ttfBuffer.size());

    return readFontDirectory(ttfBuffer);
}

/* Function:    readFontDirectory
   Description: Reads in the data from the ttf buffer to construct the font directory
   Parameters:  std::vector<uint8_t> - The ttf buffer data
   Returns:     int8_t               - success or error encountered
 */
int8_t LestTrueType::readFontDirectory(const std::vector<uint8_t>& crBuffer)
{
    readOffsetSubTable(crBuffer);

    mFontDirectory.tableDir.resize(mFontDirectory.offSub.numTables);
    return readTableDirectory(crBuffer);
}

/* Function:    readOffsetSubTable
   Description: Reads in the data from the ttf buffer to construct the offset subtable
   Parameters:  std::vector<uint8_t> - The ttf buffer data
   Returns:     None
 */
void LestTrueType::readOffsetSubTable(const std::vector<uint8_t>& crBuffer)
{
    memcpy(&mFontDirectory.offSub, crBuffer.data(), sizeof(mFontDirectory.offSub));

    mFontDirectory.offSub.scalarType = lesthtonl(mFontDirectory.offSub.scalarType);
    mFontDirectory.offSub.numTables = lesthtons(mFontDirectory.offSub.numTables);
    mFontDirectory.offSub.searchRange = lesthtons(mFontDirectory.offSub.searchRange);
    mFontDirectory.offSub.entrySelector = lesthtons(mFontDirectory.offSub.entrySelector);
    mFontDirectory.offSub.rangeShift = lesthtons(mFontDirectory.offSub.rangeShift);

    #ifdef DEBUG
        std::cout << "scalar type: " << mFontDirectory.offSub.scalarType << std::endl;
        std::cout << "numTables type: " << mFontDirectory.offSub.numTables << std::endl;
        std::cout << "search range type: " << mFontDirectory.offSub.searchRange << std::endl;
        std::cout << "entry selector type: " << mFontDirectory.offSub.entrySelector << std::endl;
        std::cout << "range shift type: " << mFontDirectory.offSub.rangeShift << std::endl;
    #endif

    mBufferOffset += sizeof(mFontDirectory.offSub);
    
    #ifdef DEBUG
        std::cout << "offset: " << mBufferOffset << std::endl;
    #endif
}

/* Function:    readTableDirectory
   Description: Reads in the data from the ttf buffer and begins constructing Table Directory
   Parameters:  std::vector<uint8_t> - ttf buffer data
   Returns:     int8_t               - success or error encountered
 */
int8_t LestTrueType::readTableDirectory(const std::vector<uint8_t>& crBuffer)
{
    uint16_t required_tables_enc = 0;
    memcpy(mFontDirectory.tableDir.data(), crBuffer.data() + mBufferOffset,
           sizeof(TableDirectory) * mFontDirectory.tableDir.size());
    mBufferOffset += sizeof(TableDirectory) * mFontDirectory.tableDir.size();

    #ifdef DEBUG
        std::cout << "#)\ttag\tlen\toffset" << std::endl;
    #endif
    std::string tag;
    std::shared_ptr<TrueTypeTable> table;
    std::queue<std::pair<std::string, TableDirectory>> processing_req;

    // Process tables Note: Some tables come before other tables that give necessary data for initialization
    uint32_t num = 1;
    for (auto& curr_table : mFontDirectory.tableDir)
    {
        curr_table.checkSum = lesthtonl(curr_table.checkSum);
        curr_table.offset = lesthtonl(curr_table.offset);
        curr_table.length = lesthtonl(curr_table.length);

        #ifdef DEBUG
            std::cout << num << ")\t" << curr_table.tag[0] << curr_table.tag[1] << curr_table.tag[2] << curr_table.tag[3] << "\t" << curr_table.length << "\t" << curr_table.offset << std::endl;
        #endif
        
        num ++;
        tag = curr_table.tag[0];
        tag += curr_table.tag[1];
        tag += curr_table.tag[2];
        tag += curr_table.tag[3];

        if (crBuffer.size() < curr_table.length + curr_table.offset)
        {
            std::cout << "Error: Offset + Length of " << tag << " will result in out of bounds." << std::endl;
            return -1;
        }

        // Check to see if table exists in map
        if (-1 == copyTableBitMask(tag, required_tables_enc))
        {
            std::cout << "Bit table mask doesn't exist." << std::endl;
            continue;
            // return -1;
        }

        table = tableFactory(tag);
        // At this point all tables can be assumed to exist
        if (nullptr == table)
        {
            std::cout << tag << " Table is either invalid or currently not implemented." << std::endl;
        }
        else
        {
            if (!sTableTags.at(tag).delayProcessing)
            {
                if (-1 == table->readTable(crBuffer, curr_table.offset, curr_table.length))
                {
                    std::cout << "Read table failed for " << tag << std::endl;
                    return -1;
                }

                mTables[tag] = table;
            }
            else
            {
                // May not need a queue if only HMTX is required for processing
                processing_req.push({tag, curr_table});
            }
        }
    }

    std::pair<std::string, TableDirectory> queue_top;
    // Process Tables that can now be initialized that all tables are done
    while(!processing_req.empty())
    {
        queue_top = processing_req.front();
        processing_req.pop();
        mTables[queue_top.first] = processRemainingTables(queue_top.first);

        // Should not be possible but just in case
        if (nullptr == mTables[queue_top.first])
        {
            #ifdef DEBUG
                std::cout << queue_top.first << " Table is either invalid or currently not implemented." << std::endl;
            #endif
        }
        else
        {
            if (-1 == mTables[queue_top.first]->readTable(crBuffer, queue_top.second.offset, queue_top.second.length))
            {
                std::cout << "Delayed read table failed for " << queue_top.first << std::endl;
                return -1;
            }
        }
    }

    #ifdef DEBUG
        std::cout << "Offset: " << (int) mBufferOffset << std::endl;
    #endif

    if (TTF_REQUIRED != required_tables_enc)
    {
        //TODO: Get which required table is missing
        std::cout << "Missing a required Table." << std::endl;
        return -1;
    }

    return 1;
}

/* Function:    copyTableBitMask
   Description: Constructs a bit mask of required tables in ttf files
   Parameters:  std::string - Table Tag name
                uint16_t    - Current bit mask
   Returns:     int8_t      - success or error encountered
 */
int8_t LestTrueType::copyTableBitMask(const std::string& crTag, uint16_t& rBitMask)
{
    if (sTableTags.find(crTag) == sTableTags.end())
    {
        std::cout << "Unknown Table Tag encountered " << crTag << "." << std::endl;
        return -1;
    }

    if (TTF_NOTREQ == sTableTags.at(crTag).mask)
    {
        return 1;
    }

    rBitMask |= sTableTags.at(crTag).mask;

    return 1;
}

std::shared_ptr<TrueTypeTable> LestTrueType::tableFactory(const std::string& crTag)
{
    switch(sTableTags.at(crTag).type)
    {
        case TABLE_CMAP:
            return std::make_shared<Cmap>();
        case TABLE_GLYF:
            return std::make_shared<Glyf>();
        case TABLE_HEAD:
            return std::make_shared<HeadTable>();
        case TABLE_HHEA:
            return std::make_shared<Hhea>();
        case TABLE_HMTX:
            return std::make_shared<Hmtx>();
        case TABLE_LOCA:
            return std::make_shared<Loca>(Loca(std::dynamic_pointer_cast<HeadTable>(mTables[sHEAD])->getLocaFormat()));
        case TABLE_MAXP:
            return std::make_shared<Maxp>();
        case TABLE_NAME:
            return std::make_shared<Name>();
        case TABLE_POST:
            return std::make_shared<Post>();
        case TABLE_OS2:
            return std::make_shared<OS2>();
        case TABLE_CVT:
            return std::make_shared<Cvt>();
        case TABLE_FPGM:
            return std::make_shared<Fpgm>();
        case TABLE_HDMX:
            return nullptr;
        case TABLE_KERN:
            return std::make_shared<Kern>();
        case TABLE_PREP:
            return std::make_shared<Prep>();
        case TABLE_VDMX:
            return std::make_shared<Vdmx>();
        case TABLE_GASP:
            return std::make_shared<Gasp>();
        case TABLE_DSIG:
            return nullptr;
        case TABLE_GDEF:
            return nullptr;
        case TABLE_GSUB:
            return nullptr;
        case TABLE_JSTF:
            return nullptr;
        case TABLE_LTSH:
            return nullptr;
        case TABLE_PCLT:
            return nullptr;
        default:
            std::cout << "Error: Invalid Table Type code encountered." << std::endl;
            return nullptr;
    }
}

std::shared_ptr<TrueTypeTable> LestTrueType::processRemainingTables(const std::string& crTag)
{
    switch(sTableTags.at(crTag).type)
    {
        case TABLE_HMTX:
            return std::make_shared<Hmtx>(
              Hmtx(std::dynamic_pointer_cast<Hhea>(mTables.at(sHHEA))->getNumOfLongHorMetrics(),
              std::dynamic_pointer_cast<Maxp>(mTables.at(sMAXP))->getNumGlyphs()));
        case TABLE_GLYF:
            return std::make_shared<Glyf>(Glyf(std::dynamic_pointer_cast<Maxp>(mTables.at(sMAXP))->getNumGlyphs(),
                                          std::dynamic_pointer_cast<Loca>(mTables.at(sLOCA))));
        default:
            std::cout << "Error: Table provided can be processed in main loop." << std::endl;
            return nullptr;
    }
}

uint16_t LestTrueType::getGlyphIndex(const uint16_t cCodePoint)
{
    return std::dynamic_pointer_cast<Cmap>(mTables.at(sCMAP))->getGlyphIndex(cCodePoint);
}

std::vector<GlyfHeader> LestTrueType::getGlyphOutlines()
{
    return std::dynamic_pointer_cast<Glyf>(mTables.at(sGLYF))->getGlyphOutlines();
}

int8_t LestTrueType::getSpecifcCharacterOutline(const uint16_t cCodePoint, GlyfHeader& rGlyph) const
{
    const uint16_t IDX = std::dynamic_pointer_cast<Cmap>(mTables.at(sCMAP))->getGlyphIndex(cCodePoint);

    if (IDX > std::dynamic_pointer_cast<Maxp>(mTables.at(sMAXP))->getNumGlyphs())
    {
        std::cout << "Max glyphs is: " << std::dynamic_pointer_cast<Maxp>(mTables.at(sMAXP))->getNumGlyphs() << std::endl;
        std::cout << "Error: index " << IDX << " received out of bounds." << std::endl;
        return -1;
    }

    rGlyph = std::dynamic_pointer_cast<Glyf>(mTables.at(sGLYF))->getSpecifcCharacterOutline(IDX);

    return 1;
}

HheaHeader LestTrueType::getHheaHeader() const
{
  return std::dynamic_pointer_cast<Hhea>(mTables.at(sHHEA))->getHheaHeader();
}

HeadHeader LestTrueType::getHeadHeaderTable() const
{
  return std::dynamic_pointer_cast<HeadTable>(mTables.at(sHEAD))->getHeadHeaderTable();
}

OS2Table LestTrueType::getOS2Table() const
{
  return std::dynamic_pointer_cast<OS2>(mTables.at(sOS2))->getOS2Table(); 
}

bool LestTrueType::hasOS2Table() const
{
  return nullptr != std::dynamic_pointer_cast<OS2>(mTables.at(sOS2));
}

VdmxHeader LestTrueType::getVdmxHeader() const
{
  return std::dynamic_pointer_cast<Vdmx>(mTables.at(sVDMX))->getVdmxHeader();
}

LongHorMetric LestTrueType::getSpecificCharacterHorMetrics(const uint16_t cCodePoint) const
{
  const uint16_t IDX = std::dynamic_pointer_cast<Cmap>(mTables.at(sCMAP))->getGlyphIndex(cCodePoint);

  if (IDX > std::dynamic_pointer_cast<Maxp>(mTables.at(sMAXP))->getNumGlyphs())
  {
      std::cout << "Max glyphs is: " << std::dynamic_pointer_cast<Maxp>(mTables.at(sMAXP))->getNumGlyphs() << std::endl;
      std::cout << "Error: index " << IDX << " received out of bounds." << std::endl;
      return LongHorMetric{};
  }

  if (mTables.find(sHMTX) == mTables.end())
  {
    std::cout << "Font file does not contain an HMTX table." << std::endl;
    return LongHorMetric{};
  }

  return std::dynamic_pointer_cast<Hmtx>(mTables.at(sHMTX))->getLongHorMetrics(IDX);
}