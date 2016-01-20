#include "Sid.h"
#include <iostream>
namespace Sid
{
    bool Sid::isAsync(){return true;}
    Sid::Sid(Service::MemoryManager* memoryManager)
    {
        this->memManager = memoryManager;
        for(int i =0; i < 3; i++)
        {
            envGen[i] = new EnvelopeGenerator(SAMPLE_RATE);
            oscilators[i] = new Oscilator(envGen[i],SAMPLE_RATE);
        }
        for(int i =0; i < 32;i++)
            this->registers[i] = 0;
        mixer = new Mixer(oscilators[0],oscilators[1],oscilators[2],this);
    }
    void Sid::readMemory(Service::MemoryManager* memManager)
    {
        //readMemory();
    }
    void Sid::readMemory()
    {

        for(int i =0; i < 32; i++)
        {
            unsigned char value = this->memManager->getValue(SID_START_ADDRESS+i);
            this->writeRegister(i,value);
        }
    }

    void Sid::writeRegister(int address,Uint8 value)
    {
        if(address < 0 || address >= 32)
            return;

        this->registers[address] = value;

        int osNo = -1;//current oscilator
        if(address < 0x21)
        {
            if(address > 0x0D)
            {
                osNo = 2;
            }
            else if(address > 0x6)
            {
                osNo = 1;
            }
            else
                osNo = 0;
        }

        switch(address)
        {
            /*
             *VOICE 1- 3
             */
            case 0x0:
            case 0x1:
            case 0x7:
            case 0x8:
            case 0xE:
            case 0xF:
                if(osNo == 0)
                {
                    this->oscilators[osNo]->setFrequency(
                        this->registers[RegName::FreqLo1]+(this->registers[RegName::FreqHi1]<<8));
                }
                else if(osNo == 1)
                {
                    this->oscilators[osNo]->setFrequency(
                        this->registers[RegName::FreqLo2]+(this->registers[RegName::FreqHi2]<<8));
                }
                else
                {
                    this->oscilators[osNo]->setFrequency(
                        this->registers[RegName::FreqLo3]+(this->registers[RegName::FreqHi3]<<8));

                }
                break;
            case 0x2:
            case 0x3:
            case 0x9:
            case 0xA:
            case 0x10:
            case 0x11:
                if(osNo == 0)
                {
                    int pw = ((0xF & this->registers[RegName::PWHi1])<<8) + this->registers[RegName::PWLo1];
                    this->oscilators[osNo]->setPulseWith(pw);
                }
                else if(osNo == 1)
                {
                    int pw = ((0xF & this->registers[RegName::PWHi2])<<8) + this->registers[RegName::PWLo2];
                    this->oscilators[osNo]->setPulseWith(pw);
                }
                else
                {
                    int pw = ((0xF & this->registers[RegName::PWHi3])<<8) + this->registers[RegName::PWLo3];
                    this->oscilators[osNo]->setPulseWith(pw);
                }
                break;
                break;
            case 0x4:
            case 0xB:
            case 0x12:
                //Noise
                if(!!(0x80 & value))
                {
                    this->oscilators[osNo]->setWaveForm(WaveForm::NOISE);
                }
                //pulse
                if(!!(0x40 & value))
                {
                    this->oscilators[osNo]->setWaveForm(WaveForm::PULSE);
                }
                //sawtooth
                if(!!(0x20 & value))
                {
                    this->oscilators[osNo]->setWaveForm(WaveForm::SAWTHOOTH);
                }
                //triangle
                if(!!(0x10 & value))
                {
                    this->oscilators[osNo]->setWaveForm(WaveForm::TRIANGLE);
                }
                //Test
                this->oscilators[osNo]->setTestbit((0x8 & value));

                //ring modulation
                this->oscilators[osNo]->setRingModulation((0x4 & value));
                //sync
                if(!!(0x2 & value))
                {

                }
                //gate
                envGen[osNo]->setGate((0x1 & value));
                break;
            case 0x5:
            case 0xC:
            case 0x13:
                envGen[osNo]->setAttack(((value&0xF0)>>4));
                envGen[osNo]->setDecay((value&0xF));
                break;
            case 0x6:
            case 0xD:
            case 0x14:
                envGen[osNo]->setSustain(((value&0XF0)>>4));
                envGen[osNo]->setRelease((value&0xF));
                break;
            /*
             * filters
             */
            case 0x15:
            case 0x16:
            {
                int filterFreq = this->registers[RegName::FCLo] + (this->registers[RegName::FCHi]<<8);
                mixer->setFilterFreq(filterFreq);
                break;
            }
            case 0x17:

                mixer->filterVoice1((value&0x1));
                mixer->filterVoice2((value&0x2));
                mixer->filterVoice3((value&0x4));

                break;
            case 0x18:

                mixer->disconnectVoice3((value&0x80));
                mixer->setHiPassFilter((value&0x40));
                mixer->setBandPassFilter((value&0x20));
                mixer->setLowPassFilter((value&0x10));
                mixer->setVolume((value&0xF));
                break;
        }
    }
    Sid::~Sid()
    {
        for(int i = 0; i < 3;i++)
        {
            delete envGen[i];
            delete oscilators[i];
        }
        delete mixer;
    }
    void Sid::run()
    {

        mixer->start();

        while(isRunning)
        {
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
        }
        this->stop();
        this->drawWaveForm();
    }
    void Sid::drawWaveForm()
    {
        mixer->drawWaveForm();
    }
    void Sid::stop()
    {
        mixer->stop();
        this->isRunning = false;
    }
}
