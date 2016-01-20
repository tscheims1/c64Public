#include "DecayState.h"

namespace Sid
{
    DecayState::DecayState(EnvelopeGenerator* envGenerator,float lastAmplitudeValue)
    :AbstractState(envGenerator,lastAmplitudeValue)
    {}
    DecayState::DecayState()
    {
        //ctor
    }
    std::string DecayState::getClassName()
    {
        return "DecayState";
    }
    float DecayState::modifyAmplitude()
    {
        int decayTicks = this->envelopeGenerator->getDecayRateInSamples();

        int samplePerWave = this->envelopeGenerator->getSamplesPerWave();

        if(clock % (samplePerWave) == 0)
        {
            this->positionForAmplitudeCalc = (clock >= decayTicks)?decayTicks:clock;


            //Release begins
            if(this->envelopeGenerator->getGate() == false)
            {
                this->envelopeGenerator->changeState(new ReleaseState(this->envelopeGenerator,this->lastAmplitudeValue));
            }
        }



        //Decay Cylcle is over -> switch to SustainState
        if(clock >= decayTicks)
        {
            this->envelopeGenerator->changeState(new SustainState(this->envelopeGenerator,this->lastAmplitudeValue));
         }

        clock++;

        float m = -1.0f/decayTicks*(255.0f - (255.0f*this->envelopeGenerator->getSustainRateInPercent()));
        this->lastAmplitudeValue = 255.0f + m*this->positionForAmplitudeCalc;
        return lastAmplitudeValue;
    }
    DecayState::~DecayState()
    {
        //dtor
    }
}
