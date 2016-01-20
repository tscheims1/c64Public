#include <iostream>
#include "MemoryManager.h"
#include "Sid.h"
#include "Clock.h"
#include "Loader.h"
using namespace std;
using namespace Service;
using namespace Sid;
thread sidThread2;
void startComponents(char*);

void startComponents(char* memPtr)
{
/*
std::cout << "loading components..."<<std::endl;
    //MemoryManager memoryManager = MemoryManager((MemoryCell)memPtr);
    //Loader loader = Loader(&memoryManager);
    //loader.load(0x800,"example.sid");
    std::cout << "loding sid file into memory..."<<std::endl;

    //memoryManager.printMemMap();
    Sid::Sid* sid =  new Sid::Sid();

    //clock.start();
     //sidThread =thread([sid]{sid->run();});
     sidThread2 =thread([]{std::cout <<"test"<<std::endl;});*/
}

int main()
{
    startComponents((char*)0x1);
     for(;;);
    return 0;
}
