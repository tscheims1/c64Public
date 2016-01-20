#include "Cia.h"
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;
namespace Cia
{
std::mutex mtx; //handle Locking
bool Cia::isAsync(){return false;}
Cia::Cia(struct _6510_cpu* cpuPtr,Service::MemoryManager* memoryManager)
{
    this->memManager = memoryManager;
    this->cpuPtr = cpuPtr;
    this->PA = 0xDC00; //Daten Port A
    this->PB = 0xDC01; //Daten Port B
    this->DDRA = 0xDC02; //Datenrichtung Port A
    this->DDRB = 0xDC03; //Datenrichtung Port B
    this->TALO = 0xDC04; //Timer A Low Byte
    this->TAHI = 0xDC05; //Timer A High Byte
    this->TBLO = 0xDC06; //Timer B Low Byte
    this->TBHI = 0xDC07; //Timer B High Byte
    this->TOD10THS = 0xDC08; //Echtzeituhr 1/10s
    this->TODSEC = 0xDC09; //Echtzeituhr Sekunden
    this->TODMIN = 0xDC0A; //Echtzeituhr Minuten
    this->TODHR = 0xDC0B; //Echtzeituhr Stunden
    this->SDR = 0xDC0C; //Serielles Schieberegister
    this->ICR = 0xDC0D; //Interrupt Control und Status
    this->CRA = 0xDC0E; //Control Timer A
    this->CRB = 0xDC0F; /**Control Timer B  @see:--> https://www.c64-wiki.de/index.php/CIA#CIA_1**/



}

Cia::~Cia()
{

}
void Cia::readMemory(Service::MemoryManager* memManager)
{
    /**
     * Calculate Port B of the CIA
     * -> If the PORT A is Set to Read and
     *   and Port B is set to write/
     */
    if(memManager->getValue(0xDC03)==0x00 && memManager->getValue(0xDC02) == 0xff)
    {
        int maskB =0x1;
        int maskA = 0x1;
        int dc00 = memManager->getValue(0xDC00);
        int dc01 = 0xff;
        dc00 = (dc00^0xff);//invert


        for(int i = 0; i < 8;i++,maskA<<=1 )
        {
            if((dc00&maskA))
            {
                maskB = 0x1;
                for(int y = 0; y < 8;y++, maskB <<=1)
                {
                    if(keyArray[maskA][maskB] == false)
                    {
                        dc01 = dc01 & ~(maskB);
                    }
                }
            }
        }
        memManager->assignValue(dc01,0xdc01);
    }
    /**
     * CIA Timer Interrupt A
     */
    //check if the interrupt is enable
    if(memManager->getValue(0xDC0D) == 0x81)
    {
        if(irqTimerIsRunning == false)
        {
            irqCycles =0;
            irqTimerIsRunning = true;
        }
        irqCycles+=2;//average is 2 cylces per asm-command

        /**
         * fire interrupt after "60"hz
         * theres a bug: the Timer Interrupt fires to early if the
         * interrupt fires after 60Hz and the Comodore wont enter
         * into Basic (Realtimeproblem) -> 2500 is the best possible Value
         */
        if(irqTimerIsRunning == true && irqCycles > 2500 )
        {
            cpuPtr->interrupt = '0';
            irqTimerIsRunning = false;
        }
    }

}
/**
 *dispatch a keyboard Event
 */
void Cia::keyEvent(SDL_Event word,bool value)
{
            /*
             * Because of the Realtimeproblem we
             * cannot Evaluate more than one  pressed keys
             * so we have to errase the last pressed key
             */
            for(int i =0; i < 255; i++)
                for(int y =0; y < 255; y++)
                    keyArray[i][y] = true;
            mtx.lock();

            cpuPtr->interrupt = '0';
            switch(word.key.keysym.sym)
            {
                case SDLK_1:
                keyArray[0x80][0x1] = value;
                break;
                case SDLK_9:
                keyArray[0x10][0x1] = value;
                break;
                case SDLK_7:
                keyArray[0x8][0x1] = value;
                break;
                case SDLK_5:
                keyArray[0x4][0x1] = value;
                break;
                case SDLK_3:
                keyArray[0x2][0x1] = value;
                break;
                case SDLK_DELETE:
                keyArray[0x1][0x1] = value;
                break;

                case SDLK_LEFT:
                    keyArray[0x80][0x2] = value;
                    break;
                case SDLK_ASTERISK:
                    keyArray[0x40][0x2] = value;
                    break;
                case SDLK_p:
                    keyArray[0x20][0x2] = value;
                    break;
                case SDLK_i:
                    keyArray[0x10][0x2] = value;
                    break;
                case SDLK_y:
                    keyArray[0x8][0x2] = value;
                    break;
                case SDLK_r:
                    keyArray[0x4][0x2] = value;
                    break;
                case SDLK_w:
                    keyArray[0x2][0x2] = value;
                    break;
                case SDLK_RETURN:
                    keyArray[0x1][0x2] = value;
                    break;
                case 167: //§-key for +
                    keyArray[0x20][0x1] = value;
                    break;

                case SDLK_RCTRL:
                case SDLK_LCTRL:
                    keyArray[0x80][0x4] = value;
                    break;
                case SDLK_SEMICOLON:
                    keyArray[0x40][0x4] = value;
                    break;
                case SDLK_l:

                    keyArray[0x20][0x4] = value;
                    break;
                case SDLK_j:
                    keyArray[0x10][0x4] = value;
                    break;
                case SDLK_g:
                    keyArray[0x8][0x4] = value;
                    break;
                case SDLK_d:
                    keyArray[0x4][0x4] = value;
                    break;
                case SDLK_a:
                    keyArray[0x2][0x4] = value;
                    break;
                case SDLK_RIGHT:
                    keyArray[0x1][0x4] = value;
                    break;

                case SDLK_2:
                    keyArray[0x80][0x8] = value;
                    break;
                case SDLK_HOME:
                    keyArray[0x40][0x8] = value;
                    break;
                case SDLK_MINUS:
                    keyArray[0x20][0x8] = value;
                    break;
                case SDLK_0:
                    keyArray[0x10][0x8] = value;
                    break;
                case SDLK_8:
                    keyArray[0x8][0x8] = value;
                    break;
                case SDLK_6:
                    keyArray[0x4][0x8] = value;
                    break;
                case SDLK_4:
                    keyArray[0x2][0x8] = value;
                    break;
                case SDLK_F7:
                    keyArray[0x1][0x8] = value;
                    break;

                case SDLK_SPACE:
                    keyArray[0x80][0x10] = value;
                    break;
                case SDLK_RSHIFT:
                    keyArray[0x40][0x10] = value;
                    break;
                case SDLK_PERIOD:
                    keyArray[0x20][0x10] = value;
                    break;
                case SDLK_m:
                    keyArray[0x10][0x10] = value;
                    break;
                case SDLK_b:
                    keyArray[0x8][0x10] = value;
                    break;
                case SDLK_c:
                    keyArray[0x4][0x10] = value;
                    break;
                case SDLK_z:
                    keyArray[0x2][0x10] = value;
                    break;
                case SDLK_F1:
                    keyArray[0x1][0x10] = value;
                    break;


                case SDLK_MODE:
                    keyArray[0x80][0x20] = value;
                    break;
                case 39://= zeichen
                    keyArray[0x40][0x20] = value;
                    break;
                case SDLK_DOLLAR:
                    keyArray[0x20][0x20] = value;
                    break;
                case SDLK_k:
                    keyArray[0x10][0x20] = value;
                    break;
                case SDLK_h:
                    keyArray[0x8][0x20] = value;
                    break;
                case SDLK_f:
                    keyArray[0x4][0x20] = value;
                    break;
                case SDLK_s:
                    keyArray[0x2][0x20] = value;
                    break;
                case SDLK_F3:
                    keyArray[0x1][0x20] = value;
                    break;

                case SDLK_q:
                    keyArray[0x80][0x40] = value;
                    break;
                case SDLK_UP:
                    keyArray[0x40][0x40] = value;
                    break;
                case SDLK_AT:
                    keyArray[0x20][0x40] = value;
                    break;
                case SDLK_o:
                    keyArray[0x10][0x40] = value;
                    break;
                case SDLK_u:
                    keyArray[0x8][0x40] = value;
                    break;
                case SDLK_t:
                    keyArray[0x4][0x40] = value;
                    break;
                case SDLK_e:
                    keyArray[0x2][0x40] = value;
                    break;
                case SDLK_F5:
                    keyArray[0x1][0x40] = value;
                    break;

                case SDLK_POWER:
                    keyArray[0x80][0x80] = value;
                    break;
                case SDLK_SLASH:
                    keyArray[0x40][0x80] = value;
                    break;
                case SDLK_COMMA:
                    keyArray[0x20][0x80] = value;
                    break;
                case SDLK_n:
                    keyArray[0x10][0x80] = value;
                    break;
                case SDLK_v:
                    keyArray[0x8][0x80] = value;
                    break;
                case SDLK_x:
                    keyArray[0x4][0x80] = value;
                    break;
                case SDLK_LSHIFT:
                    keyArray[0x2][0x80] = value;
                    break;
                case SDLK_DOWN:
                    keyArray[0x1][0x80] = value;
                    break;


    }
    mtx.unlock();
}
void Cia::timerSet()
{
}
void Cia::run()
{
    for(;;)
    {

    }
}
}
