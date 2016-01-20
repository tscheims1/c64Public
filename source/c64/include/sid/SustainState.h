#ifndef SUSTAINSTATE_H
#define SUSTAINSTATE_H

#include "AbstractState.h"
#include "ReleaseState.h"

namespace Sid
{
    class SustainState : public AbstractState
    {
        public:
            SustainState();
            SustainState(EnvelopeGenerator* envGenerator,float lastAmplitudeValue);
            virtual ~SustainState();
            float modifyAmplitude();
            virtual std::string getClassName();

        protected:
        private:
    };
}
#endif // SUSTAINSTATE_H
