#include "post.hpp"

enum PostVersion
{
    VERSION_1   = 0x00010000,
    VERSION_2   = 0x00020000,
    VERSION_2_5 = 0x00025000,
    VERSION_3   = 0x00030000
};

int8_t Post::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
    uint32_t bytes_copied = 0;
    uint8_t *ptr = (uint8_t *)crBuffer.data() + cOffset;
    mPostHeader.version = read4BytesPtr(ptr);
    bytes_copied += sizeof(mPostHeader.version);
    mPostHeader.italicAngle = read4BytesPtr(ptr);
    bytes_copied += sizeof(mPostHeader.italicAngle);
    mPostHeader.underlinePosition = read2BytesPtr(ptr);
    bytes_copied += sizeof(mPostHeader.underlinePosition);
    mPostHeader.underlineThickness = read2BytesPtr(ptr);
    bytes_copied += sizeof(mPostHeader.underlineThickness);
    mPostHeader.isFixedPitch = read4BytesPtr(ptr);
    bytes_copied += sizeof(mPostHeader.isFixedPitch);
    mPostHeader.minMemType42 = read4BytesPtr(ptr);
    bytes_copied += sizeof(mPostHeader.minMemType42);
    mPostHeader.maxMemType42 = read4BytesPtr(ptr);
    bytes_copied += sizeof(mPostHeader.maxMemType42);
    mPostHeader.minMemType1 = read4BytesPtr(ptr);
    bytes_copied += sizeof(mPostHeader.minMemType1);
    mPostHeader.maxMemType1 = read4BytesPtr(ptr);
    bytes_copied += sizeof(mPostHeader.maxMemType1);
    mPostHeader.numGlyphs = read2BytesPtr(ptr);
    bytes_copied += sizeof(mPostHeader.numGlyphs);

    if (VERSION_2 == mPostHeader.version)
    {
        #ifdef DEBUG
            std::cout << "Post Version: 2.0" << std::endl;
            std::cout << "Post Italic Angle: " << mPostHeader.italicAngle << std::endl;
            std::cout << "Post UnderLine Pos: " << mPostHeader.underlinePosition << std::endl;
            std::cout << "Post Underline Thick: " << mPostHeader.underlineThickness << std::endl;
            std::cout << "Post isFixedPitch: " << mPostHeader.isFixedPitch << std::endl;
            std::cout << "Post Min Mem 42: " <<  mPostHeader.minMemType42 << std::endl;
            std::cout << "Post Max Mem 42: " << mPostHeader.maxMemType42 << std::endl;
            std::cout << "Post Min Mem 1: " << mPostHeader.minMemType1 << std::endl;
            std::cout << "Post Max Mem 1: " << mPostHeader.maxMemType1 << std::endl;
            std::cout << "Post num Glyphs: " << mPostHeader.numGlyphs << std::endl;
        #endif

        mPostHeader.glyphNameIndex.resize(mPostHeader.numGlyphs);
        memcpy(mPostHeader.glyphNameIndex.data(), ptr, mPostHeader.glyphNameIndex.size() * sizeof(uint16_t));
        ptr += mPostHeader.glyphNameIndex.size() * sizeof(uint16_t);
        bytes_copied += mPostHeader.glyphNameIndex.size() * sizeof(uint16_t);
        mPostHeader.stringData.resize(cNumBytes - bytes_copied);
        memcpy(mPostHeader.stringData.data(), ptr, mPostHeader.stringData.size());

        int i = 0;
        uint16_t first_length_byte = 0;
        uint16_t length = 0;
        std::string str;
        for (auto& glyph_name : mPostHeader.glyphNameIndex)
        {
            glyph_name = lesthtons(glyph_name);

            #ifdef DEBUG
                std::cout << "Glyf " << i << " -> ";
            #endif

            if (258 > glyph_name)
            {
                #ifdef DEBUG
                    std::cout << " Mac Glyph # " << glyph_name << ", " << std::endl;
                #endif
            }
            else
            {
                length = mPostHeader.stringData[first_length_byte + glyph_name - 258];
                str.resize(length);
                memcpy(&str[0], &mPostHeader.stringData[first_length_byte + glyph_name - 258 + 1], length);
                first_length_byte += length;
                
                #ifdef DEBUG
                    std::cout << " PsGlyf Name # " << glyph_name - 258 + 1 << ", " << str << std::endl;
                #endif
            
                str.clear();
            }
            i++;
        }
    }
    else
    {
        std::cout << "Error: " << mPostHeader.version << " not currently supported." << std::endl;
        return -1;
    }

    return 1;
}