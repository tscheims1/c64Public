#include "SustainState.h"
namespace Sid
{
    SustainState::SustainState()
    {
        //ctor
    }
    SustainState::SustainState(EnvelopeGenerator* envGenerator,float lastAmplitudeValue) :
    AbstractState(envGenerator,lastAmplitudeValue)
    {
    }
    SustainState::~SustainState()
    {
        //dtor
    }
    std::string SustainState::getClassName()
    {
        return "SustanState";
    }
    float SustainState::modifyAmplitude()
    {
        this->lastAmplitudeValue = this->envelopeGenerator->getSustainRateInPercent()*255.0f;
        if(this->envelopeGenerator->getGate() == true)
        {
            return lastAmplitudeValue;

        }
        //Release begins -> change to release State
        else
        {

            this->envelopeGenerator->changeState(new ReleaseState(this->envelopeGenerator,this->lastAmplitudeValue));
            return this->lastAmplitudeValue;
        }

    }
}
