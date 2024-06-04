#ifndef LEST_TTF_HPP
#define LEST_TTF_HPP

#include <string>
#include <memory>
#include <map>

#include "lestTtfData.hpp"
#include "ttfTable.hpp"
#include "glyf.hpp"
#include "head.hpp"
#include "hhea.hpp"
#include "os2.hpp"
#include "vdmx.hpp"
#include "hmtx.hpp"

class LestTrueType
{
    public:
        LestTrueType();
        ~LestTrueType(){}
        int8_t read(const std::string& crTtfPath);
        uint16_t getGlyphIndex(const uint16_t cCodePoint);
        std::vector<GlyfHeader> getGlyphOutlines();
        int8_t getSpecifcCharacterOutline(const uint16_t cCodePoint, GlyfHeader& rGlyph) const;
        HheaHeader getHheaHeader() const;
        HeadHeader getHeadHeaderTable() const;
        OS2Table getOS2Table() const;
        bool hasOS2Table() const;
        VdmxHeader getVdmxHeader() const;
        LongHorMetric getSpecificCharacterHorMetrics(const uint16_t cCodePoint) const;
    private:
        int8_t readFontDirectory(const std::vector<uint8_t>& crBuffer);
        void readOffsetSubTable(const std::vector<uint8_t>& crBuffer);
        int8_t readTableDirectory(const std::vector<uint8_t>& crBuffer);
        int8_t copyTableBitMask(const std::string& crTag, uint16_t& rBitMask);
        std::shared_ptr<TrueTypeTable> tableFactory(const std::string& crTag);
        std::shared_ptr<TrueTypeTable> processRemainingTables(const std::string& crTag);

        FontDirectory mFontDirectory;
        uint64_t mBufferOffset;
        std::map<std::string, std::shared_ptr<TrueTypeTable>> mTables;
};

#endif