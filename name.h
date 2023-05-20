#ifndef NAME_H
#define NAME_H

#include "ttfTable.h"

struct NamingRecords
{
    uint16_t platformID;   // Platform ID
    uint16_t encodingID;   // Platform-specific encoding ID
    uint16_t languageID;   // Language ID
    uint16_t nameID;       // Name ID.
    uint16_t length;       // String length (in bytes).
    uint16_t stringOffset; // String offset from start of storage area (in bytes)
};

struct LangTagRecord
{
    uint16_t length; // Language-tag string length (in bytes)
    uint16_t offset; // Language-tag string offset from start of storage area (in bytes)
};

// Contains both version 1 structure but will work for version 0 too
struct NamingTable
{
    uint16_t version;                         // Table version number (=0)
    uint16_t count;                           // Number of name records
    uint16_t storageOffset;                   // Offset to start of string storage (from start of table)
    std::vector<NamingRecords> nameRecord;    // The name records where count is the number of records
    uint16_t langTagCount;                    // Number of language-tag records
    std::vector<LangTagRecord> langTagRecord; // The language-tag records where langTagCount is the number of records
};


class Name : public TrueTypeTable
{
    public:
        Name(){}
        ~Name(){}
        int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0);
    private:
        NamingTable mNameTable;
};
#endif