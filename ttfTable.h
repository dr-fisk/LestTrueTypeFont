#ifndef TTF_TABLE_H
#define TTF_TABLE_H

class TrueTypeTable
{
    public:
        TrueTypeTable(){}
        virtual ~TrueTypeTable(){}
        virtual void readTable(const std::vector<uint8_t>& crBuffer, const uint32_t cOffset) = 0;
};

#endif