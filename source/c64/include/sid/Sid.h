#ifndef SID_H
#define SID_H
#include "Oscilator.h"
#include "Mixer.h"
#include "MemoryManager.h"
#include "C64Component.h"
#include <mutex>
#define SID_START_ADDRESS 0xD400
#define SAMPLE_RATE (44100*2)


namespace Sid
{
    enum RegName{FreqLo1,FreqHi1,PWLo1,PWHi1,ControlReg1,AttackDecay1,SustainRelease1,
                 FreqLo2,FreqHi2,PWLo2,PWHi2,ControlReg2,AttackDecay2,SustainRelease2,
                 FreqLo3,FreqHi3,PWLo3,PWHi3,ControlReg3,AttackDecay3,SustainRelease3,
                 FCLo,FCHi,ResFilt,ModeVol,Potx,poty,OSC3Random,Env3};
    class Mixer;
    class Oscilator;
    /** Interface of the SID-Controller
     * Reads and interprets the coresponding memory values
     * and delegates to other instances (like Oscilators, EnvelopGenerator or Mixer)
    */
    class Sid  : public Service::C64Component
    {
        public:
            bool isAsync();
            Sid(Service::MemoryManager* memManager);
            void writeRegister(int address,Uint8 value);
            void readMemory(Service::MemoryManager* memManager);
            virtual ~Sid();
            void run();
            void drawWaveForm();
            void stop();
            void readMemory();
        protected:
            volatile int registers[32];
        private:
            Oscilator* oscilators[3];
            EnvelopeGenerator* envGen[3];
            Mixer* mixer;
            bool isRunning = true;
            Service::MemoryManager* memManager;
            std::mutex sidMtx;
    };
}
#endif // SID_H
