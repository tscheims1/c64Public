#ifndef ABSTRACTSTATE_H
#define ABSTRACTSTATE_H

#include "EnvelopeGenerator.h"
#include "string.h"
#include <memory>
namespace Sid
{
    class EnvelopeGenerator;
    /**
     * State Event Machine for
       - Attack
       - Decay
       - Sustain
       - Release
    */
    class AbstractState
    {
        public:
            AbstractState();
            AbstractState(EnvelopeGenerator* envGenerator,float lastAmplitudeValue);
            virtual float modifyAmplitude() = 0;//pure abstract class
            virtual std::string getClassName() = 0;
            virtual ~AbstractState();
        protected:
            EnvelopeGenerator* envelopeGenerator;
            long clock =0;//for counting the samples
            int positionForAmplitudeCalc =0;
            float lastAmplitudeValue = 0;
        private:
    };
}
#endif // STATE_H
