#include "lestTtf.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 3 || argc >= 4)
    {
        std::cout << "Error: Wrong number of args provided." << std::endl;
        return 0;
    }
    
    std::string path = argv[1];
    std::string idx = argv[2];

    LestTrueType test;

    test.read(path);
    GlyfHeader temp;
    test.getSpecifcCharacterOutline(stoi(idx), temp);
}