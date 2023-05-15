#include "name.h"

void Name::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
    uint8_t *ptr = (uint8_t *)crBuffer.data() + cOffset;
    mNameTable.version = read2BytesPtr(ptr);
    mNameTable.count = read2BytesPtr(ptr);
    mNameTable.storageOffset = read2BytesPtr(ptr);
    mNameTable.nameRecord.resize(mNameTable.count);

    std::cout << "Name Version: " << mNameTable.version<< std::endl;
    std::cout << "Name Count: " << mNameTable.count << std::endl;
    std::cout << "Name storage offset: " << mNameTable.storageOffset << std::endl;
    std::cout << "Name total length: " << mNameTable.nameRecord.size() * sizeof(NamingRecords) + 6 << std::endl;

    memcpy(mNameTable.nameRecord.data(), ptr, mNameTable.nameRecord.size() * sizeof(NamingRecords));
    ptr += mNameTable.nameRecord.size() * sizeof(NamingRecords);
    if (mNameTable.version)
    {
        std::cout << "Exception: Name Table version 1 not currently implemented." << std::endl;
    }

    int i = 0;
    for (auto& record : mNameTable.nameRecord)
    {
        record.platformID = lesthtons(record.platformID);
        record.encodingID = lesthtons(record.encodingID);
        record.languageID = lesthtons(record.languageID);
        record.nameID = lesthtons(record.nameID);
        record.length = lesthtons(record.length);
        record.stringOffset = lesthtons(record.stringOffset);
        std::cout << "Name Table: " << i << std::endl;
        std::cout << "Platform ID: " << record.platformID << std::endl;
        std::cout << "Encoding ID: " << record.encodingID << std::endl;
        std::cout << "Language ID: " << record.languageID << std::endl;
        std::cout << "Name ID: " << record.nameID << std::endl;
        std::cout << "Name Length: " << record.length << std::endl;
        std::cout << "String Offset: " << record.stringOffset << std::endl;
        i ++;
    }
}