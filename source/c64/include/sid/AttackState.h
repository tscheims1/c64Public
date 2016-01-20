#ifndef ATTACKSTATE_H
#define ATTACKSTATE_H

#include "AbstractState.h"
#include "EnvelopeGenerator.h"
#include "SustainState.h"
#include "DecayState.h"
#include "ReleaseState.h"
namespace Sid
{
    class AttackState : public AbstractState
    {
        public:
            AttackState();
            AttackState(EnvelopeGenerator* envGenerator,float lastAmplitudeValue);
            virtual ~AttackState();
            virtual std::string getClassName();
            /**
             * Every State modifies the amplitude of the computed Wave signal
             */
            float modifyAmplitude();
        protected:
        private:
    };
}
#endif // ATTACKSTATE_H
