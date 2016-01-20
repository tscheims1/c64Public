#include "CharGen.h"
#include <iostream>
#include <fstream>
using namespace std;

CharGen::CharGen()
{
    //tidy up memory
    for(int i =0;i < 200*320;i++)
        screenBuffer1[i] = 0x0;
     streampos size;


    ifstream osFile ("character.rom", ios::in|ios::binary|ios::ate);
    if (osFile.is_open())
    {
        size = osFile.tellg();
        osFile.seekg (0, ios::beg);
        osFile.read ((char*)charMem, size);
        osFile.close();

        cout << "the entire charMemory content is in memory";
    }
}

CharGen::~CharGen()
{
    //dtor
}
