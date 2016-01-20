#include "AttackState.h"
#include <iostream>
namespace Sid
{
    AttackState::AttackState()
    {
    //    this->envelopeGenerator = 0;
        //ctor

    }
    AttackState::AttackState(EnvelopeGenerator* envGenerator,float lastAmplitudeValue) :
        AbstractState(envGenerator,lastAmplitudeValue)
    {
        clock = 0;

    }
    AttackState::~AttackState()
    {
        //dtor
    }
    std::string AttackState::getClassName()
    {
        return "AttackState";
    }
    float AttackState::modifyAmplitude()
    {
        int attackTicks = this->envelopeGenerator->getAttackRateInSamples();

        int samplePerWave = this->envelopeGenerator->getSamplesPerWave();

        //rise amplitude only once per frequency
        if(clock % (samplePerWave) == 0)
        {

            this->positionForAmplitudeCalc = (clock >= attackTicks)?attackTicks:clock;

            //Release begins
            if(this->envelopeGenerator->getGate() == false)
            {
                this->envelopeGenerator->changeState(new ReleaseState(this->envelopeGenerator,this->lastAmplitudeValue));
            }
        }
        //Attack Cylce is over -> switch to DecayState
        if(clock >= attackTicks)
        {
            this->envelopeGenerator->changeState(new DecayState(this->envelopeGenerator,this->lastAmplitudeValue));
        }


        clock++;
         //Calculate raise level from bottom to peak
        this->lastAmplitudeValue= ((this->positionForAmplitudeCalc) /(float)attackTicks)*255.0f;
        return this->lastAmplitudeValue;
    }
}
