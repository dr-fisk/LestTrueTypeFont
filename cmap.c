#include "cmap.h"

enum FormatType
{
    FORMAT_UNICODE,
    FORMAT_MAC,
    FORMAT_RESERVED,
    FORMAT_WINDOWS
};

Cmap::Cmap()
{
    mCmapHeader.version = 0;
    mCmapHeader.numTables = 0;

    mFormat.format = 0;
    mFormat.length = 0;
    mFormat.language = 0;
    mFormat.segCountX2 = 0;
    mFormat.searchRange = 0;
    mFormat.entrySelectory = 0;
    mFormat.rangeShift = 0;
    mFormat.reservedPad = 0;
}

Cmap::~Cmap()
{
}

/* Function:    readTable
   Description: Reads in the data from the ttf buffer and begins constructing a Cmap Table
   Parameters:  std::vector<uint8_t> - ttf buffer data
                uint32_t             - Offset from the start of file where Cmap data starts
                uint32_t             - Length of table not used for all cases only when padding is introduced to structs
   Returns:     None
 */
int8_t Cmap::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
    if (0 >= cNumBytes)
    {
        std::cout << "Error: cnumbytes less than 0." << std::endl;
        return -1;
    }

    memcpy((void *)&mCmapHeader, crBuffer.data() + cOffset, CMAP_HEADER_SIZE);

    mCmapHeader.version = lesthtons(mCmapHeader.version);
    mCmapHeader.numTables = lesthtons(mCmapHeader.numTables);

    #ifdef DEBUG
        std::cout << "Cmap version: " << mCmapHeader.version << std::endl;
        std::cout << "Cmap numTables: " << mCmapHeader.numTables << std::endl;
    #endif

    if (0 >= mCmapHeader.numTables)
    {
        std::cout << "Error: Num Tables read from CMAP is: " << mCmapHeader.numTables << std::endl;
        return -1;
    }
    
    mCmapHeader.encodingRecords.resize(mCmapHeader.numTables);

    memcpy(mCmapHeader.encodingRecords.data(), crBuffer.data() + cOffset + CMAP_HEADER_SIZE,
           sizeof(EncodingRecord) * mCmapHeader.encodingRecords.size());

    #ifdef DEBUG
        std::cout << "pId\tpsId\toffset\t" << std::endl;
    #endif

    for (auto& curr_cmap : mCmapHeader.encodingRecords)
    {
        curr_cmap.platformID = lesthtons(curr_cmap.platformID);
        curr_cmap.encodingID = lesthtons(curr_cmap.encodingID);
        curr_cmap.subTableOffset = lesthtonl(curr_cmap.subTableOffset);

        #ifdef DEBUG
            std::cout << curr_cmap.platformID << "\t" << curr_cmap.encodingID << "\t" << curr_cmap.subTableOffset << std::endl;
        #endif

        // Currently only unicode will be implemented as it's widely supported over multiple OS
        // TODO: Add MAC and Windows format support
        if (FORMAT_UNICODE == curr_cmap.platformID)
        {
            readFormat4(crBuffer, cOffset + curr_cmap.subTableOffset);
        }
    }

    return 1;
}

/* Function:    readFormat4
   Description: Reads in the data from the ttf buffer and begins constructing a the Format4 structure
                Format4 is the structure used for unicode
   Parameters:  std::vector<uint8_t> - ttf buffer data
                uint32_t             - Offset from the start of Cmap data where Format4 data starts
   Returns:     None
 */
void Cmap::readFormat4(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset)
{
    uint32_t curr_offset = cOffset;

    memcpy((void *)&mFormat, crBuffer.data() + cOffset, sizeof(mFormat.format) + sizeof(mFormat.length) +
           sizeof(mFormat.language) + sizeof(mFormat.segCountX2) + sizeof(mFormat.searchRange) +
           sizeof(mFormat.entrySelectory) + sizeof(mFormat.rangeShift));

    curr_offset += sizeof(mFormat.format) + sizeof(mFormat.length) + sizeof(mFormat.language) +
                   sizeof(mFormat.segCountX2) + sizeof(mFormat.searchRange) + sizeof(mFormat.entrySelectory) +
                   sizeof(mFormat.rangeShift);

    mFormat.format = lesthtons(mFormat.format);
    mFormat.length = lesthtons(mFormat.length);
    mFormat.language = lesthtons(mFormat.language);
    mFormat.segCountX2 = lesthtons(mFormat.segCountX2);
    mFormat.searchRange = lesthtons(mFormat.searchRange);
    mFormat.entrySelectory = lesthtons(mFormat.entrySelectory);
    mFormat.rangeShift = lesthtons(mFormat.rangeShift);

    #ifdef DEBUG
        std::cout << "Format: " << mFormat.format << ", Length: " << mFormat.length << ", Language: " << mFormat.language <<
        ", Segment Count: " << mFormat.segCountX2 / 2 << ", Search Range: " << mFormat.searchRange << ", Entry selector: " <<
        mFormat.entrySelectory << ", Range Shift: " << mFormat.rangeShift << std::endl;
    #endif

    // Now that we have segCountX2 start resizing arrays and copying data
    mFormat.endCode.resize(mFormat.segCountX2 / sizeof(uint16_t));
    mFormat.startCode.resize(mFormat.segCountX2 / sizeof(uint16_t));
    mFormat.idDelta.resize(mFormat.segCountX2 / sizeof(uint16_t));
    mFormat.idRangeOffsets.resize(mFormat.segCountX2 / sizeof(uint16_t));

    // EndCode
    memcpy(mFormat.endCode.data(), crBuffer.data() + curr_offset, mFormat.endCode.size() * sizeof(uint16_t));
    curr_offset += mFormat.endCode.size() * sizeof(uint16_t) + sizeof(mFormat.reservedPad);

    // Start Code
    memcpy(mFormat.startCode.data(), crBuffer.data() + curr_offset, mFormat.startCode.size() * sizeof(uint16_t));
    curr_offset += mFormat.startCode.size() * sizeof(uint16_t);

    // Id Delta
    memcpy(mFormat.idDelta.data(), crBuffer.data() + curr_offset, mFormat.idDelta.size() * sizeof(uint16_t));
    curr_offset += mFormat.idDelta.size() * sizeof(uint16_t);
    
    // ID Range Offsets
    memcpy(mFormat.idRangeOffsets.data(), crBuffer.data() + curr_offset,
           mFormat.idRangeOffsets.size() * sizeof(uint16_t));
    curr_offset += mFormat.idRangeOffsets.size() * sizeof(uint16_t);

    #ifdef DEBUG
        std::cout << "Segment Ranges:\tstartCode\tendCode\tidDelta\tidRangeOffset" << std::endl;
    #endif

    for (uint16_t i = 0; i < mFormat.segCountX2 / sizeof(uint16_t); i ++)
    {
        mFormat.endCode[i] = lesthtons(mFormat.endCode[i]);
        mFormat.startCode[i] = lesthtons(mFormat.startCode[i]);
        mFormat.idDelta[i] = lesthtons(mFormat.idDelta[i]);
        mFormat.idRangeOffsets[i] = lesthtons(mFormat.idRangeOffsets[i]);

        #ifdef DEBUG
            std::cout << "--------------:\t" << (int)mFormat.startCode[i] << "\t\t" << (int)mFormat.endCode[i] << "\t" << (int)mFormat.idDelta[i] << "\t" <<
            mFormat.idRangeOffsets[i] << std::endl;
        #endif
    }

    mFormat.glyphIdArray.resize((mFormat.length - (curr_offset - cOffset)) / sizeof(uint16_t));
    memcpy(mFormat.glyphIdArray.data(), crBuffer.data() + curr_offset, mFormat.glyphIdArray.size() * sizeof(uint16_t));

    for (auto& glyph : mFormat.glyphIdArray)
    {
        glyph = lesthtons(glyph);
    }
}

/* Function:    getGlyphIndex
   Description: Calculates the index and grabs the value in the glyphIDArray
   Parameters:  uint16_t - The two byte character code to find
   Returns:     uint16_t - The index for the character in the glyphIDArray
 */
uint16_t Cmap::getGlyphIndex(const uint16_t cCodePoint)
{
    int32_t glyph_index = -1;

    uint32_t end_code_index = 0;
    // Find the end code for the Codepoint which is the first end code that is greater than or equal to cCodePoint
    for (const auto& end_code : mFormat.endCode)
    {
        if (end_code >= cCodePoint)
        {
            glyph_index = end_code_index;
            break;
        }

        end_code_index ++;
    }

    // Valid end code for given codepoint not found
    if (-1 == glyph_index)
    {
        return 0;
    }

    // Check the corresponding start code, if it is less than or equal to the codepoint move to nex step
    if (mFormat.startCode[glyph_index] <= cCodePoint)
    {
        // Check if ID range offset is 0 for given index
        if (mFormat.idRangeOffsets[glyph_index] == 0)
        {
            // If ID range offset is 0, add codepoint to the current ID Delta and that is your index
            return (uint16_t)(mFormat.idDelta[glyph_index] + cCodePoint);
        }
        // if not 0 then take ID Range Offsets Value and add it to the address of the value
        else
        {
            uint16_t offset = glyph_index + mFormat.idRangeOffsets[glyph_index] / 2 +
                              cCodePoint - mFormat.startCode[glyph_index] - mFormat.idRangeOffsets.size();

            if (0 != offset)
            {
                return (mFormat.glyphIdArray[mFormat.idDelta[glyph_index] + offset]);
            }
        }
    }

    return 0;
}