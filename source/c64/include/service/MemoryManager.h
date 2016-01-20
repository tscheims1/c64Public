#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include <iostream>
#include "MemoryMap.h"
#include <thread>
#include <chrono>



namespace Service
{
/*
 * for easy memory walking
 */
typedef char (*MemoryCell)[9];
class MemoryMap;
/**
 * Defines the interface to the C64 Memory
 * Every C64 Memory Access is done by the MemoryManager
 */
class MemoryManager
{
public:
    MemoryManager();
    MemoryManager(MemoryCell address);
    virtual ~MemoryManager();
    void assignValue(unsigned char value, int address);
    unsigned char getValue(int address);
    unsigned char convertMemoryValue(unsigned char* memValue);
    void convertIntoMemoryValue(unsigned char value,unsigned char* buff);
protected:
    MemoryCell basePointer;

private:
};
};
#endif // MEMORYMANAGER_H
