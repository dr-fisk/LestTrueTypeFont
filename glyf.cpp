#include "glyf.hpp"

// See https://learn.microsoft.com/en-us/typography/opentype/spec/glyf
enum ComponentGlyphFlags
{
    ARG1_AND_2_ARE_WORDS = 0x0001,
    ARGS_ARE_XY_VALUES = 0x0002,
    ROUND_XY_TO_GRID = 0x0004,
    WE_HAVE_A_SCALE = 0x0008,
    MORE_COMPONENTS = 0x0020,
    WE_HAVE_AN_X_AND_Y_SCALE = 0x0040,
    WE_HAVE_A_TWO_BY_TWO = 0x0080,
    WE_HAVE_INSTRUCTIONS = 0x0100,
    USE_MY_METRICS = 0x0200,
    OVERLAP_COMPOUND = 0x0400,
    SCALED_COMPONENT_OFFSET = 0x0800,
    UNSCALED_COMPONENT_OFFSET = 0x1000,
    COMPONENT_RESERVED = 0xE010 
};

Glyf::Glyf(const uint16_t cNumGlyphs, const std::shared_ptr<Loca>& crpLoca)
{
    mLoca = crpLoca;
    mGlyfHeader.resize(cNumGlyphs);
}

int8_t Glyf::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
    if (0 >= cNumBytes)
    {
        std::cout << "Error: cnumbytes less than 0." << std::endl;
        return -1;
    }

    uint8_t *ptr = nullptr;

    int i = 0;
    for (auto& glyph : mGlyfHeader)
    {
        ptr = (uint8_t *)crBuffer.data() + cOffset + mLoca->getGlyphOffset(i);
        glyph.numberofContours = read2BytesPtr(ptr);
        glyph.xMin = read2BytesPtr(ptr);
        glyph.yMin = read2BytesPtr(ptr);
        glyph.xMax = read2BytesPtr(ptr);
        glyph.yMax = read2BytesPtr(ptr);

        #ifdef DEBUG
            std::cout << "Glyph #" << i << "." << std::endl;
            std::cout << "Num Contours: " << glyph.numberofContours << std::endl;
            std::cout << "xMin: " << glyph.xMin << std::endl;
            std::cout << "yMin: " << glyph.yMin << std::endl;
            std::cout << "xMax: " << glyph.xMax << std::endl;
            std::cout << "yMax: " << glyph.yMax << std::endl;
        #endif

        if (0 > glyph.numberofContours)
        {
            readCompoundGlyph(ptr, glyph);
        }
        else
        {
            if (-1 == readSimpleGlyph(ptr, glyph))
            {
                std::cout << "Error reading simple glyf." << std::endl;
                return -1;
            }
        }

        i ++;
        #ifdef DEBUG
            std::cout << std::endl;
        #endif
    }

    return 1;
}

void Glyf::readCompoundGlyph(uint8_t *pPtr, GlyfHeader& rGlyph)
{
    rGlyph.CompoundGlyfData = std::make_shared<CompoundGlyf>();
    int j = 0;

    do
    {
        rGlyph.CompoundGlyfData->flags = read2BytesPtr(pPtr);
        rGlyph.CompoundGlyfData->glyphIndex = read2BytesPtr(pPtr);

        #ifdef DEBUG
            std::cout << std::endl;
            std::cout << j << ": " << "Flags: 0x" << std::hex << rGlyph.CompoundGlyfData->flags << std::dec << std::endl;
            std::cout << "   Glyf index: " << rGlyph.CompoundGlyfData->glyphIndex << std::endl;
        #endif

        if (ARG1_AND_2_ARE_WORDS & rGlyph.CompoundGlyfData->flags)
        {
            rGlyph.CompoundGlyfData->arg1 = (int16_t)read2BytesPtr(pPtr);
            rGlyph.CompoundGlyfData->arg2 = (int16_t)read2BytesPtr(pPtr);

            #ifdef DEBUG
                std::cout << "   X WOffset: " << rGlyph.CompoundGlyfData->arg1 << std::endl;
                std::cout << "   Y WOffset: " << rGlyph.CompoundGlyfData->arg2 << std::endl;
            #endif
        }
        else
        {
            rGlyph.CompoundGlyfData->arg1 = (int8_t)readBytePtr(pPtr);
            rGlyph.CompoundGlyfData->arg2 = (int8_t)readBytePtr(pPtr);

            #ifdef DEBUG
                std::cout << "   X BOffset: " << (int16_t)rGlyph.CompoundGlyfData->arg1 << std::endl;
                std::cout << "   Y BOffset: " << (int16_t)rGlyph.CompoundGlyfData->arg2 << std::endl;
            #endif
        }

        // Below are in F2Dot14 format meaning
        // first 2 bits are a signed int
        // last 14 bits are a decimal value divided by 16384
        if (WE_HAVE_A_SCALE & rGlyph.CompoundGlyfData->flags)
        {
            int16_t  scale = read2BytesPtr(pPtr);

            #ifdef DEBUG
                std::cout << "   Scale: " << f2Dot14(scale) << std::endl;
            #endif
            
            //TODO: USE VALUES
            scale++;
        }
        else if (WE_HAVE_AN_X_AND_Y_SCALE & rGlyph.CompoundGlyfData->flags)
        {
            int16_t  xscale = read2BytesPtr(pPtr);
            int16_t  yscale = read2BytesPtr(pPtr);

            #ifdef DEBUG
                std::cout << "   X Scale: " << f2Dot14(xscale) << std::endl;
                std::cout << "   Y Scale: " << f2Dot14(yscale) << std::endl;
            #endif

            //TODO: USE VALUES
            xscale++;
            yscale++;
        }
        else if (WE_HAVE_A_TWO_BY_TWO & rGlyph.CompoundGlyfData->flags)
        {
            #ifdef DEBUG
                std::cout << "   2 BY 2" << std::endl;
            #endif

            int16_t  xscale = read2BytesPtr(pPtr);
            int16_t  scale01 = read2BytesPtr(pPtr);
            int16_t  scale10 = read2BytesPtr(pPtr);
            int16_t  yscale = read2BytesPtr(pPtr);
            xscale++;
            scale01++;
            scale10++;
            yscale++;
        }

        #ifdef DEBUG
            if ((ARGS_ARE_XY_VALUES & rGlyph.CompoundGlyfData->flags) && (ROUND_XY_TO_GRID & rGlyph.CompoundGlyfData->flags))
            {
                std::cout << "   Round X,Y to Grid" << std::endl;
            }
        #endif

        // rGlyph.CompoundGlyphData.push_back(rGlyph.CompoundGlyfData->;
        j++;
    }
    while(MORE_COMPONENTS & rGlyph.CompoundGlyfData->flags);

    if (WE_HAVE_INSTRUCTIONS & rGlyph.CompoundGlyfData->flags)
    {
        #ifdef DEBUG
            std::cout << "We got instructions." << std::endl;
        #endif

        const uint16_t numInstr = read2BytesPtr(pPtr);
        uint8_t instr[numInstr];
        memcpy(instr, pPtr, numInstr);
        pPtr += numInstr;
    }
}

int8_t Glyf::readSimpleGlyph(uint8_t *pPtr, GlyfHeader& rGlyph)
{
    #ifdef DEBUG
        std::cout << std::endl;
    #endif
    rGlyph.SimpleGlyfData = std::make_shared<SimpleGlyf>();
    rGlyph.SimpleGlyfData->endPtsOfContours.resize(rGlyph.numberofContours);

    #ifdef DEBUG
        int num_endpts = 0;
        std::cout << "EndPoints" << std::endl;
        std::cout << "---------" << std::endl;
    #endif

    for(auto& end_pts : rGlyph.SimpleGlyfData->endPtsOfContours)
    {
        end_pts = read2BytesPtr(pPtr);
        
        #ifdef DEBUG
            std::cout << " " << num_endpts << ": " << end_pts << std::endl;
        #endif
    }
    #ifdef DEBUG
        std::cout << std::endl;
    #endif

    rGlyph.SimpleGlyfData->instructionLength = read2BytesPtr(pPtr);

    #ifdef DEBUG
        std::cout << "Length of instructions: " << rGlyph.instructionLength << std::endl;
    #endif

    if (0 < rGlyph.SimpleGlyfData->instructionLength)
    {
        rGlyph.SimpleGlyfData->instructions.resize(rGlyph.SimpleGlyfData->instructionLength);
        memcpy(rGlyph.SimpleGlyfData->instructions.data(), pPtr, rGlyph.SimpleGlyfData->instructions.size());
        pPtr += rGlyph.SimpleGlyfData->instructions.size();
    }
    
    if (0 >= rGlyph.SimpleGlyfData->endPtsOfContours[rGlyph.SimpleGlyfData->endPtsOfContours.size() - 1] + 1)
    {
        std::cout << "ERROR" << std::endl;
        return -1;
    }

    rGlyph.SimpleGlyfData->flags.resize(rGlyph.SimpleGlyfData->endPtsOfContours[rGlyph.SimpleGlyfData->endPtsOfContours.size() - 1] + 1);
    rGlyph.SimpleGlyfData->xCoordinates.resize(rGlyph.SimpleGlyfData->flags.size());
    rGlyph.SimpleGlyfData->yCoordinates.resize(rGlyph.SimpleGlyfData->flags.size());

    size_t j = 0;

    #ifdef DEBUG
        std::cout << "Flags" << std::endl;
        std::cout << "-----" << std::endl;
    #endif

    while (j < rGlyph.SimpleGlyfData->flags.size())
    {
        rGlyph.SimpleGlyfData->flags[j] = readBytePtr(pPtr);
  
        #ifdef DEBUG
            std::cout << " " << j << ": ";

            if (Y_SHORT_VECTOR & rGlyph.flags[j])
            {
                std::cout << "YShort";
            }
            
            std::cout << "\t";

            if (Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR & rGlyph.flags[j])
            {
                std::cout << "YDual";
            }

            std::cout << "\t";

            if (X_SHORT_VECTOR & rGlyph.flags[j])
            {
                std::cout << " XShort";
            }

            std::cout << "\t";
            
            if (X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR & rGlyph.flags[j])
            {
                std::cout << " XDual";
            }

            std::cout << "\t";

            if (ON_CURVE_POINT & rGlyph.flags[j])
            {
                std::cout << "On";
            }
            else
            {
                std::cout << "Off";
            }
            std::cout << std::endl;
        #endif

        if (REPEAT_FLAG & rGlyph.SimpleGlyfData->flags[j])
        {
            const uint8_t NUM_FLAGS = readBytePtr(pPtr);
            for (uint8_t m = 0; m < NUM_FLAGS && j < rGlyph.SimpleGlyfData->flags.size(); m++)
            {
                rGlyph.SimpleGlyfData->flags[j + 1] = rGlyph.SimpleGlyfData->flags[j];
                j++;

                #ifdef DEBUG
                    std::cout << " " << j << ": " << std::hex << (int)rGlyph.flags[j] << std::dec << std::endl;
                #endif
            }
        }

        j++;
    }

    return readSimpleGlyphCoords(pPtr, rGlyph.SimpleGlyfData);
}

int8_t Glyf::readSimpleGlyphCoords(uint8_t *pPtr, std::shared_ptr<SimpleGlyf>& rpGlyph)
{
    int16_t prev_coord = 0;
    uint8_t curr_flag = 0;
    
    #ifdef DEBUG
        std::cout << "Coordinates" << std::endl;
        std::cout << "-----------" << std::endl;
    #endif

    for(size_t i = 0; i < rpGlyph->flags.size(); i++)
    {
        curr_flag = ((rpGlyph->flags[i] & X_SHORT_VECTOR) >> 1) | ((rpGlyph->flags[i] & X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) >> 3);
        
        if (-1 == readCoord(pPtr, rpGlyph->xCoordinates[i], curr_flag))
        {
            std::cout << "Error occurred reading xcoodrinate from simple glyf." << std::endl;
            return -1;
        }
        
        rpGlyph->xCoordinates[i] += prev_coord;
        prev_coord = rpGlyph->xCoordinates[i];
    }

    curr_flag = 0;
    prev_coord = 0;
    for(size_t i = 0; i < rpGlyph->flags.size(); i++)
    {
        curr_flag = ((rpGlyph->flags[i] & Y_SHORT_VECTOR) >> 2) | ((rpGlyph->flags[i] & Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) >> 4);
        
        if (-1 == readCoord(pPtr, rpGlyph->yCoordinates[i], curr_flag))
        {
            std::cout << "Error occurred reading ycoodrinate from simple glyf." << std::endl;
            return -1;
        }

        rpGlyph->yCoordinates[i] += prev_coord;
        prev_coord = rpGlyph->yCoordinates[i];
    }

    #ifdef DEBUG
        for(size_t i = 0; i < rpGlyph->flags.size(); i++)
        {
            std::cout << "(" << rpGlyph->xCoordinates[i] << ", " << rpGlyph->yCoordinates[i]  << ")" << std::endl;
        }
    #endif

    return 1;
}

int8_t Glyf::readCoord(uint8_t* &rpPtr, int16_t& rCoord, const uint8_t cFlag)
{
    switch(cFlag)
    {
        case 0:
            rCoord = read2BytesPtr(rpPtr);
            break;
        case 1:
            rCoord = readBytePtr(rpPtr) * -1;
            break;
        case 2:
            rCoord = 0;
            break;
        case 3:
            rCoord = readBytePtr(rpPtr);
            break;
        default:
            std::cout << "Error: Unknown flag encountered in Glyf" << std::endl;
            return -1;
    }

    return 1;
}

std::vector<GlyfHeader> Glyf::getGlyphOutlines()
{
    return mGlyfHeader;
}

GlyfHeader Glyf::getSpecifcCharacterOutline(const uint16_t cIndex)
{
    return mGlyfHeader[cIndex];
}