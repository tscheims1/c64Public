#include "MemoryManager.h"

namespace Service
{
MemoryManager::MemoryManager(MemoryCell address)
{
    this->basePointer = address;

}


MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
}
/**
 * stores a value at a specific memory position
 */
void MemoryManager::assignValue(unsigned char value, int address)
{

    unsigned char buffer[9];
    this->convertIntoMemoryValue(value,buffer);
    memcpy(this->basePointer+address,buffer,9);
}
/**
 * get a value from a specific memory position
 */
unsigned char MemoryManager::getValue(int address)
{
    unsigned char buffer[8];
    memcpy(buffer,this->basePointer+address,8);
    return this->convertMemoryValue(buffer);

}
/**
 * converts a char value into a Char* MemoryValue
 */
void MemoryManager::convertIntoMemoryValue(unsigned char value,unsigned char* buffer)
{
    int mask = 0x1;
    for(int y = 0; y < 8; y++)
    {
        //48: '0' char 49: '1' char
        buffer[7-y] = ((mask&value)>>y)+48;
        mask <<=1;
    }
    buffer[8] = '\0';
}
/**
 * helper method thats convert a char* from the memory in a char value
 */
unsigned char MemoryManager::convertMemoryValue(unsigned char* memValue)
{
    int sum=0;
    for(int i=0; i<8;i++)
    if(memValue[i] == '1'){
      sum = (sum << 1) | 1;
    }else
      sum = (sum << 1);
    return sum;
}
}
