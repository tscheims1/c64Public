
#ifndef CIA_H
#define CIA_H
#include <SDL2/SDL.h>
#include "MemoryManager.h"
#include "C64Component.h"
#include <mutex>
#include <ctime>
namespace Cia
{
class Cia : public Service::C64Component
{
    public:
        bool isAsync();
        Cia(struct _6510_cpu* cpuPtr ,Service::MemoryManager* memoryManager) ;
        virtual ~Cia();
        void keyEvent(SDL_Event,bool value);
        void readMemory(Service::MemoryManager* memManager);
        void timerSet();
        void run();
    protected:
        unsigned int PA, PB, DDRA, DDRB, TALO, TAHI, TBLO, TBHI, TOD10THS, TODSEC, TODMIN, TODHR, SDR, ICR, CRA, CRB;
        bool keyArray[0xff][0xff];
        int irqCycles = 0;
        bool irqTimerIsRunning = false;
        Service::MemoryManager* memManager;
        struct _6510_cpu* cpuPtr;
    private:

};
}
#endif // CIA_H

