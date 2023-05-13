#include "lestTtf.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 0;
    }
    
    std::string path = argv[1];
    LestTrueType test;

    test.read(path);
}