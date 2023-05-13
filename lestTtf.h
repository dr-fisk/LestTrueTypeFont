#ifndef LEST_TTF_H
#define LEST_TTF_H

#include <string>
#include <memory>
#include <map>

#include "lestTtfData.h"
#include "cmap.h"

class LestTrueType
{
    public:
        LestTrueType() : mBufferOffset(0) {}
        ~LestTrueType(){}
        int8_t read(std::string& rTtfPath);
    private:
        int8_t readFontDirectory(const std::vector<uint8_t>& crBuffer);
        void readOffsetSubTable(const std::vector<uint8_t>& crBuffer);
        int8_t readTableDirectory(const std::vector<uint8_t>& crBuffer);
        int8_t copyTableBitMask(const std::string& crTag, uint16_t& rBitMask);
        std::shared_ptr<TrueTypeTable> tableFactory(const std::string& crTag);

        FontDirectory mFontDirectory;
        uint64_t mBufferOffset;
        std::map<std::string, std::shared_ptr<TrueTypeTable>> mTables;
};

#endif