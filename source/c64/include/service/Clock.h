#ifndef CLOCK_H
#define CLOCK_H
#include "MemoryManager.h"
#include "C64Component.h"
#include <vector>
#include <thread>
#include <chrono>
namespace Service
{
/**
 * is responsive for calling the C64 Components
 * in a given time
 */
class Clock
{
    public:
        Clock(MemoryManager* memManager);
        void start();
        void addComponent(C64Component* component);
        virtual ~Clock();
    protected:
        MemoryManager* memManager;
        std::vector<C64Component*>components;
    private:
};
};
#endif // CLOCK_H
