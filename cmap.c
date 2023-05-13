#include "cmap.h"
#include "endianGeneral.h"

#include <cstring>
#include <iostream>

enum FormatType
{
    FORMAT_UNICODE,
    FORMAT_MAC,
    FORMAT_RESERVED,
    FORMAT_WINDOWS
};

Cmap::Cmap()
{
}

Cmap::~Cmap()
{
}

void Cmap::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset)
{
    memcpy((void *)&mCmapHeader, crBuffer.data() + cOffset, CMAP_HEADER_SIZE);

    mCmapHeader.version = lesthtons(mCmapHeader.version);
    mCmapHeader.numTables = lesthtons(mCmapHeader.numTables);
    std::cout << "Cmap version: " << mCmapHeader.version << std::endl;
    std::cout << "Cmap numTables: " << mCmapHeader.numTables << std::endl;
    mCmapHeader.encodingRecords.resize(mCmapHeader.numTables);

    memcpy(mCmapHeader.encodingRecords.data(), crBuffer.data() + cOffset + CMAP_HEADER_SIZE,
           sizeof(EncodingRecord) * mCmapHeader.encodingRecords.size());

    std::cout << "pId\tpsId\toffset\t" << std::endl;
    for (auto& curr_cmap : mCmapHeader.encodingRecords)
    {
        curr_cmap.platformID = lesthtons(curr_cmap.platformID);
        curr_cmap.encodingID = lesthtons(curr_cmap.encodingID);
        curr_cmap.subTableOffset = lesthtonl(curr_cmap.subTableOffset);
        std::cout << curr_cmap.platformID << "\t" << curr_cmap.encodingID << "\t" << curr_cmap.subTableOffset << std::endl;

        if (FORMAT_UNICODE == curr_cmap.platformID)
        {
            readFormat4(crBuffer, cOffset + curr_cmap.subTableOffset);
        }
    }
}

void Cmap::readFormat4(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset)
{
    uint32_t curr_offset = cOffset;
    std::cout << "Start offset: " << curr_offset << std::endl;
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

    std::cout << "Format: " << mFormat.format << ", Length: " << mFormat.length << ", Language: " << mFormat.language <<
    ", Segment Count: " << mFormat.segCountX2 / 2 << ", Search Range: " << mFormat.searchRange << ", Entry selector: " <<
    mFormat.entrySelectory << ", Range Shift: " << mFormat.rangeShift << std::endl;

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


    std::cout << "Segment Ranges:\tstartCode\tendCode\tidDelta\tidRangeOffset" << std::endl;
    for (uint16_t i = 0; i < mFormat.segCountX2 / sizeof(uint16_t); i ++)
    {
        mFormat.endCode[i] = lesthtons(mFormat.endCode[i]);
        mFormat.startCode[i] = lesthtons(mFormat.startCode[i]);
        mFormat.idDelta[i] = lesthtons(mFormat.idDelta[i]);
        mFormat.idRangeOffsets[i] = lesthtons(mFormat.idRangeOffsets[i]);
        std::cout << "--------------:\t" << (int)mFormat.startCode[i] << "\t\t" << (int)mFormat.endCode[i] << "\t" << (int)mFormat.idDelta[i] << "\t" <<
        mFormat.idRangeOffsets[i] << std::endl;
    }

    std::cout << "Curr offset: " << curr_offset << std::endl;

    mFormat.glyphIdArray.resize((mFormat.length - (curr_offset - cOffset)) / sizeof(uint16_t));
    memcpy(mFormat.glyphIdArray.data(), crBuffer.data() + curr_offset, mFormat.glyphIdArray.size() * sizeof(uint16_t));

    for (auto& glyph : mFormat.glyphIdArray)
    {
        glyph = lesthtons(glyph);
    }
}

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
            return mFormat.idDelta[glyph_index] + cCodePoint;
        }
        // if not 0 then take ID Range Offsets Value and add it to the address of the value
        else
        {
            // This algorithm is pretty genius it lets us iterate from the idRangeOffsets contiguous pointer data into 
            // the glyphIdArray contiguous pointer data
            // However, due to how this is stored in a struct, the size of the pointer for glyphIdArray must be skipped
            // as those bytes come right after the data of idRangeOffsets end.
            // That is why we are adding the sizeof(glyphIdArray)
            uint16_t *ptr = mFormat.idRangeOffsets.data() + glyph_index + mFormat.idRangeOffsets[glyph_index] / 2 
                            + sizeof(mFormat.glyphIdArray.data());
            ptr += cCodePoint - mFormat.startCode[glyph_index];

            if (0 != *ptr)
            {
                return *ptr + mFormat.idDelta[glyph_index];
            }
        }
    }

    return 0;
}