#ifndef LOADER_H
#define LOADER_H
#include "MemoryManager.h"
#include "Config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
namespace Service
{
/**
 * Responsible for Binary File loading
 */
class Loader
{
public:
    Loader(MemoryManager* memManager);
    virtual ~Loader();
    void loadConfig();
    bool load(int address,string filename);
    std::vector<string>splitString(string s,char delimeter);
protected:
    MemoryManager* memManager;

    void binToChar(unsigned char* bin,int address,int length);
private:
};
}
#endif // LOADER_H
