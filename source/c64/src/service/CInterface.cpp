#include "C64Component.h"

/** Interfaces for the C6502 Emulator*/

extern "C" void startComponents(char* memPtr,struct _6510_cpu* cpuPtr);
extern "C" void stopComponents();
extern "C" void syncMemory();
#include <iostream>
#include <unistd.h>
#include "MemoryManager.h"

#include "Sid.h"
#include "Clock.h"
#include "Loader.h"
#include "VIC.h"
#include "Screen.h"
#include "Cia.h"

using namespace std;
using namespace Service;
using namespace Sid;




thread sidThread;
thread clockThread;
thread screenThread;
thread ciaThread;
thread memViewerThread;
thread syncMemScreen;
thread syncMemSid;

Service::MemoryManager* memoryManager;
Service::MemoryMap* memMap;
Cia::Cia* cia;
Service::Clock* C64Clock;

/**
 * Loads Config
 * Starts the C64 Components and Threads
 */
void startComponents(char* memPtr,struct _6510_cpu* cpuPtr)
{
    std::cout << "loading components..."<<std::endl;
    memoryManager = new Service::MemoryManager((MemoryCell)memPtr);
    Loader loader = Loader(memoryManager);

    std::cout << "load configfile..."<< std::endl;
    loader.loadConfig();


    std::cout << "starting Sid thread..."<<std::endl;
    Sid::Sid* sid =  new Sid::Sid(memoryManager);

    std::cout << "starting cia thread..."<<std::endl;
    cia = new Cia::Cia(cpuPtr,memoryManager);
    VIC* vic = new VIC(cpuPtr);

    std::cout << "create memoryMap" <<std::endl;
    memMap = new MemoryMap(memoryManager,cpuPtr);
    memMap->draw(0x0,1.1);
    sidThread =thread([sid]{sid->run();});

    C64Clock = new Service::Clock(memoryManager);


    Screen* screen = new Screen(vic,cia,memoryManager,memMap);

    screenThread = thread([screen]{screen->power();});
    C64Clock->addComponent(screen);
    C64Clock->addComponent(cia);




}
void stopComponents()
{

}
/**
 * this methos is called before every Fetch-Cycle to avoid
 * inconsistent Memory/Register values
 */
void syncMemory()
{
    usleep(Config::getInstance().delayForSid);
    C64Clock->start();
}
