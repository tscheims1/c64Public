#include "AbstractState.h"

namespace Sid
{
    AbstractState::AbstractState(){}
    AbstractState::~AbstractState(){}
    AbstractState::AbstractState(EnvelopeGenerator* envGenerator,float lastAmpValue)
    {
        this->lastAmplitudeValue = lastAmpValue;
        this->envelopeGenerator = envGenerator;
    }
}
