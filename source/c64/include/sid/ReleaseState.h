#ifndef RELEASESTATE_H
#define RELEASESTATE_H

#include "AbstractState.h"
#include "AttackState.h"

namespace Sid
{
    class ReleaseState : public  AbstractState
    {
        public:
            ReleaseState();
            ReleaseState(EnvelopeGenerator *envGenerator, float lastAmplitudeValue);
            virtual ~ReleaseState();
            float modifyAmplitude();
            virtual std::string getClassName();

        protected:
            float lastStateAmplitudeValue;
        private:
    };
}
#endif // RELEASESTATE_H
