#include "Clock.h"

namespace Service
{
Clock::Clock(MemoryManager* memManager)
{
    this->memManager = memManager;
    //ctor
}

Clock::~Clock()
{
    //dtor
}
/**

    Call every Component every Clock cycle
    */
void Clock::start()
{
        std::vector<C64Component*>::iterator it = components.begin();
        for(;it != components.end();it++)
            (*it)->readMemory(this->memManager);
}
void Clock::addComponent(C64Component* component)
{
    this->components.push_back(component);
}

};
