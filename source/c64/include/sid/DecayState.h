#ifndef DECAYSTATE_H
#define DECAYSTATE_H

#include "AbstractState.h"
#include "SustainState.h"
namespace Sid
{
    class DecayState : public AbstractState
    {
        public:
            DecayState();
            DecayState(EnvelopeGenerator* envGenerator, float lastAmplitudeValue);
            virtual ~DecayState();
            float modifyAmplitude();
            virtual std::string getClassName();

        protected:
        private:
    };
}
#endif // DECAYSTATE_H
