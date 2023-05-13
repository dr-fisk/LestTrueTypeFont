#include <fstream>
#include <iostream>
#include <cstring>

#include "lestTtf.h"
#include "endianGeneral.h"

enum TTFRequiredTablesBitMask
{
    TTF_CMAP = 0x0001,
    TTF_GLYF = 0x0002,
    TTF_HEAD = 0x0004,
    TTF_HHEA = 0x0008,
    TTF_HMTX = 0x0010,
    TTF_LOCA = 0x0020,
    TTF_MAXP = 0x0040,
    TTF_NAME = 0x0080,
    TTF_POST = 0x0100,
    TTF_OS2 = 0x0200,
    TTF_REQUIRED = 0x03FF,
    TTF_NOTREQ = 0x0000
};

enum TTFTableType
{
    TABLE_CMAP,
    TABLE_GLYF,
    TABLE_HEAD,
    TABLE_HHEA,
    TABLE_HMTX,
    TABLE_LOCA,
    TABLE_MAXP,
    TABLE_NAME,
    TABLE_POST,
    TABLE_OS2,
    TABLE_CVT,
    TABLE_FPGM,
    TABLE_HDMX,
    TABLE_KERN,
    TABLE_PREP,
    TABLE_VDMX,
    TABLE_GASP,
    TABLE_DSIG,
    TABLE_GDEF,
    TABLE_GSUB,
    TABLE_JSTF,
    TABLE_LTSH,
    TABLE_PCLT
};

static const std::map<std::string, std::pair<TTFRequiredTablesBitMask, TTFTableType>> sTableTags = {
                                                                            // Required Tables in ttf files
                                                                            {"cmap", {TTF_CMAP, TABLE_CMAP}},
                                                                            {"glyf", {TTF_GLYF, TABLE_GLYF}},
                                                                            {"head", {TTF_HEAD, TABLE_HEAD}},
                                                                            {"hhea", {TTF_HHEA, TABLE_HHEA}},
                                                                            {"hmtx", {TTF_HMTX, TABLE_HMTX}},
                                                                            {"loca", {TTF_LOCA, TABLE_LOCA}},
                                                                            {"maxp", {TTF_MAXP, TABLE_MAXP}},
                                                                            {"name", {TTF_NAME, TABLE_NAME}},
                                                                            {"post", {TTF_POST, TABLE_POST}},
                                                                            {"OS/2", {TTF_OS2, TABLE_OS2}},
                                                                            // Option Tables in ttf files
                                                                            {"cvt ", {TTF_NOTREQ, TABLE_CVT}},
                                                                            {"fpgm", {TTF_NOTREQ, TABLE_FPGM}},
                                                                            {"hdmx", {TTF_NOTREQ, TABLE_HDMX}},
                                                                            {"kern", {TTF_NOTREQ, TABLE_KERN}},
                                                                            {"prep", {TTF_NOTREQ, TABLE_PREP}},
                                                                            {"VDMX", {TTF_NOTREQ, TABLE_VDMX}},
                                                                            {"gasp", {TTF_NOTREQ, TABLE_GASP}},
                                                                            {"DSIG", {TTF_NOTREQ, TABLE_DSIG}},
                                                                            {"GDEF", {TTF_NOTREQ, TABLE_GDEF}},
                                                                            {"GSUB", {TTF_NOTREQ, TABLE_GSUB}},
                                                                            {"JSTF", {TTF_NOTREQ, TABLE_JSTF}},
                                                                            {"LTSH", {TTF_NOTREQ, TABLE_LTSH}},
                                                                            {"PCLT", {TTF_NOTREQ, TABLE_PCLT}} };

/* Function:    read
   Description: Reads in the data from the ttf buffer and begins constructing all data
   Parameters:  std::string - File Path to ttf
   Returns:     int8_t      - success or error encountered
 */
int8_t LestTrueType::read(std::string& rTtfPath)
{
    std::ifstream ttfFile(rTtfPath, std::ios::binary);

    if (!ttfFile.is_open())
    {
        std::cout << "Failed to open ttf file: " << rTtfPath << std::endl;
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

    std::cout << "scalar type: " << mFontDirectory.offSub.scalarType << std::endl;
    std::cout << "numTables type: " << mFontDirectory.offSub.numTables << std::endl;
    std::cout << "search range type: " << mFontDirectory.offSub.searchRange << std::endl;
    std::cout << "entry selector type: " << mFontDirectory.offSub.entrySelector << std::endl;
    std::cout << "range shift type: " << mFontDirectory.offSub.rangeShift << std::endl;

    mBufferOffset += sizeof(mFontDirectory.offSub);
    std::cout << "offset: " << mBufferOffset << std::endl;
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

    std::cout << "#)\ttag\tlen\toffset" << std::endl;
    std::string tag;
    std::shared_ptr<TrueTypeTable> table;

    uint32_t num = 1;
    for(auto& curr_table : mFontDirectory.tableDir)
    {
        curr_table.checkSum = lesthtonl(curr_table.checkSum);
        curr_table.offset = lesthtonl(curr_table.offset);
        curr_table.length = lesthtonl(curr_table.length);
        std::cout << num << ")\t" << curr_table.tag[0] << curr_table.tag[1] << curr_table.tag[2] << curr_table.tag[3] << "\t" << curr_table.length << "\t" << curr_table.offset << std::endl;
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
            return -1;
        }

        table = tableFactory(tag);
        // At this point all tables can be assumed to exist
        if (nullptr == table)
        {
            std::cout << "Table is either invalid or currently not implemented." << std::endl;
        }
        else
        {
            table->readTable(crBuffer, curr_table.offset);
            mTables[tag] = table;
        }
    }

    std::cout << "Offset: " << (int) mBufferOffset << std::endl;

    if (TTF_REQUIRED != required_tables_enc)
    {
        //TODO: Get which required table is missing
        std::cout << "Missing a required Table." << std::endl;
        return -1;
    }

    return 0;
}

/* Function:    copyTableBitMask
   Description: Constructs a bit mask of required tables in ttf files
   Parameters:  std::string - Table Tag name
                uint16_t    - Current bit mask
   Returns:     int8_t      - success or error encountered
 */
int8_t LestTrueType::copyTableBitMask(const std::string& crTag, uint16_t& rBitMask)
{
    if(sTableTags.find(crTag) == sTableTags.end())
    {
        std::cout << "Unknown Table Tag encountered " << crTag << "." << std::endl;
        return -1;
    }

    if (TTF_NOTREQ == sTableTags.at(crTag).first)
    {
        return 1;
    }

    rBitMask |= sTableTags.at(crTag).first;

    return 1;
}

std::shared_ptr<TrueTypeTable> LestTrueType::tableFactory(const std::string& crTag)
{
    switch(sTableTags.at(crTag).second)
    {
        case TABLE_CMAP:
            return std::make_shared<Cmap>(Cmap());
        case TABLE_GLYF:
            return nullptr;
        case TABLE_HEAD:
            return nullptr;
        case TABLE_HHEA:
            return nullptr;
        case TABLE_HMTX:
            return nullptr;
        case TABLE_LOCA:
            return nullptr;
        case TABLE_MAXP:
            return nullptr;
        case TABLE_NAME:
            return nullptr;
        case TABLE_POST:
            return nullptr;
        case TABLE_OS2:
            return nullptr;
        case TABLE_CVT:
            return nullptr;
        case TABLE_FPGM:
            return nullptr;
        case TABLE_HDMX:
            return nullptr;
        case TABLE_KERN:
            return nullptr;
        case TABLE_PREP:
            return nullptr;
        case TABLE_VDMX:
            return nullptr;
        case TABLE_GASP:
            return nullptr;
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