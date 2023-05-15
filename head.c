#include "head.h"

HeadTable::HeadTable()
{
    memset(&mHeadHeader, 0, sizeof(mHeadHeader));
}

void HeadTable::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
    if (0 == cNumBytes)
    {
        std::cout << "Error: Cannot copy 0 bytes." << std::endl;
        return;
    }

    uint8_t *temp_ptr = (uint8_t *)crBuffer.data();
    temp_ptr = temp_ptr + cOffset;
    mHeadHeader.majorVersion = read2BytesPtr(temp_ptr);
    mHeadHeader.minorVersion = read2BytesPtr(temp_ptr);
    mHeadHeader.fontRevision = read4BytesPtr(temp_ptr);
    mHeadHeader.checksumAdjustment = read4BytesPtr(temp_ptr);
    mHeadHeader.magicNumber = read4BytesPtr(temp_ptr);
    mHeadHeader.flags = read2BytesPtr(temp_ptr);
    mHeadHeader.unitsPerEm = read2BytesPtr(temp_ptr);
    mHeadHeader.created = read8BytesPtr(temp_ptr);
    mHeadHeader.modified = read8BytesPtr(temp_ptr);
    mHeadHeader.xMin = read2BytesPtr(temp_ptr);
    mHeadHeader.xMax = read2BytesPtr(temp_ptr);
    mHeadHeader.yMin = read2BytesPtr(temp_ptr);
    mHeadHeader.yMax = read2BytesPtr(temp_ptr);
    mHeadHeader.macStyle = read2BytesPtr(temp_ptr);
    mHeadHeader.lowestRecPPEM = read2BytesPtr(temp_ptr);
    mHeadHeader.fontDirectionHint = read2BytesPtr(temp_ptr); 
    mHeadHeader.indexToLocFormat = read2BytesPtr(temp_ptr);
    mHeadHeader.glyphDataFormat = read2BytesPtr(temp_ptr);

    std::cout << " Major Version: " << mHeadHeader.majorVersion << " Minor version: " << mHeadHeader.minorVersion << " Font Revision: " << mHeadHeader.fontRevision << " Checksum Adjustment: " << mHeadHeader.checksumAdjustment << " Magic Number: "
    << mHeadHeader.magicNumber << " Flags: " << mHeadHeader.flags << " Units Per Em: " << mHeadHeader.unitsPerEm << " Created: " << mHeadHeader.created << " Modified: " << mHeadHeader.modified << " xMin: " << mHeadHeader.xMin << " xMax: " << mHeadHeader.xMax << " yMin: " << mHeadHeader.yMin << " yMax: "
    << mHeadHeader.yMax << " macStyle: " << mHeadHeader.macStyle << " lowestRecPPEM: " << mHeadHeader.lowestRecPPEM << " FontDirectionHint: " << mHeadHeader.fontDirectionHint << " IndexToLocFormat: " << mHeadHeader.indexToLocFormat << " GlyphDataFormat: " << mHeadHeader.glyphDataFormat << std::endl; 
}