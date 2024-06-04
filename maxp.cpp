#include "maxp.hpp"

int8_t Maxp::readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes)
{
    if (0 >= cNumBytes)
    {
        std::cout << "Error: cnumbytes less than 0." << std::endl;
        return -1;
    }

    memcpy(&mMaxpV1, crBuffer.data() + cOffset, sizeof(mMaxpV1));

    mMaxpV1.version = lesthtonl(mMaxpV1.version);
    mMaxpV1.numGlyphs = lesthtons(mMaxpV1.numGlyphs);
    mMaxpV1.maxPoints = lesthtons(mMaxpV1.maxPoints);
    mMaxpV1.maxContours = lesthtons(mMaxpV1.maxContours);
    mMaxpV1.maxComponentPoints = lesthtons(mMaxpV1.maxComponentPoints);
    mMaxpV1.maxComponentContours = lesthtons(mMaxpV1.maxComponentContours);
    mMaxpV1.maxZones = lesthtons(mMaxpV1.maxZones);
    mMaxpV1.maxTwilightPoints = lesthtons(mMaxpV1.maxTwilightPoints);
    mMaxpV1.maxStorage = lesthtons(mMaxpV1.maxStorage);
    mMaxpV1.maxFunctionDefs = lesthtons(mMaxpV1.maxFunctionDefs);
    mMaxpV1.maxInstructionDefs = lesthtons(mMaxpV1.maxInstructionDefs);
    mMaxpV1.maxStackElements = lesthtons(mMaxpV1.maxStackElements);
    mMaxpV1.maxSizeOfInstructions = lesthtons(mMaxpV1.maxSizeOfInstructions);
    mMaxpV1.maxComponentElements = lesthtons(mMaxpV1.maxComponentElements);
    mMaxpV1.maxComponentDepth = lesthtons(mMaxpV1.maxComponentDepth);

    #ifdef DEBUG
        std::cout << "Version: " << mMaxpV1.version << std::endl;
        std::cout << "Num Glyphs: " << mMaxpV1.numGlyphs << std::endl;
        std::cout << "MaxPoints: " << mMaxpV1.maxPoints << std::endl;
        std::cout << "Max Contours: " << mMaxpV1.maxContours << std::endl;
        std::cout << "Max Component Points: " << mMaxpV1.maxComponentPoints << std::endl;
        std::cout << "Max Component Countours: " << mMaxpV1.maxComponentContours << std::endl;
        std::cout << "Max Zones: " << mMaxpV1.maxZones << std::endl;
        std::cout << "Max Twilight Points: " << mMaxpV1.maxTwilightPoints << std::endl;
        std::cout << "Max Storage: " << mMaxpV1.maxStorage << std::endl;
        std::cout << "Max Func Defs: " << mMaxpV1.maxFunctionDefs << std::endl;
        std::cout << "Max instruct defs: " << mMaxpV1.maxInstructionDefs << std::endl;
        std::cout << "Max Stack elemetns: " << mMaxpV1.maxStackElements << std::endl;
        std::cout << "Max Size of instructions: " << mMaxpV1.maxSizeOfInstructions << std::endl;
        std::cout << "Max Component Elements: " << mMaxpV1.maxComponentElements << std::endl;
        std::cout << "Max Component Depth: " << mMaxpV1.maxComponentDepth << std::endl;
    #endif

    return 1;
}

uint16_t Maxp::getNumGlyphs() const
{
    return mMaxpV1.numGlyphs;
}