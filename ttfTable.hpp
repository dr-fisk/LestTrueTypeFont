#ifndef TTF_TABLE_HPP
#define TTF_TABLE_HPP

#include <vector>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>

#include "endianGeneral.hpp"

class TrueTypeTable
{
    public:
        TrueTypeTable(){}
        virtual ~TrueTypeTable(){}
        virtual int8_t readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset, uint32_t cNumBytes=0) = 0;
};

#endif