#include "ReleaseState.h"
namespace Sid
{
    ReleaseState::ReleaseState(EnvelopeGenerator* envGenerator,float lastAmplitudeValue)
    :AbstractState(envGenerator,lastAmplitudeValue)
    {
        lastStateAmplitudeValue = lastAmplitudeValue;
    }
    ReleaseState::ReleaseState()
    {
        //ctor
    }
    std::string ReleaseState::getClassName()
    {
        return "ReleaseState";
    }
    ReleaseState::~ReleaseState()
    {
        //dtor
    }
    float ReleaseState::modifyAmplitude()
    {
        int releaseTicks = this->envelopeGenerator->getReleaseRateInSamples();
        int samplePerWave = this->envelopeGenerator->getSamplesPerWave();



        //change amplitude only once per frequency
        if(clock % (samplePerWave) == 0)
        {
            this->positionForAmplitudeCalc = (clock>=releaseTicks)?releaseTicks:clock;

            //if the gate is set -> begin a new ADSR-Cycle
            if(this->envelopeGenerator->getGate() == true)
            {
                this->envelopeGenerator->changeState(new AttackState(this->envelopeGenerator,this->lastAmplitudeValue));
            }
        }
        clock++;
        if(clock >= releaseTicks)
        {
            this->lastAmplitudeValue = 0.0f;
            return 0.0f;
        }

        //Calculate descendance between sustain level and zero amplitude
        this->lastAmplitudeValue = lastStateAmplitudeValue -
                   ((this->positionForAmplitudeCalc) /(float)releaseTicks)*lastStateAmplitudeValue;
        return this->lastAmplitudeValue;

    }
}
