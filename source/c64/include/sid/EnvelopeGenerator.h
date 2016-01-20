#ifndef ENVELOPEGENERATOR_H
#define ENVELOPEGENERATOR_H
#include "AbstractState.h"
#include "Sid.h"
namespace Sid
{
    class AbstractState;
    /**
     * this class represents the envelopegenerator of the
     * SID controller
     */
    class EnvelopeGenerator
    {
        public:
            EnvelopeGenerator();
            EnvelopeGenerator(int sampleRate);
            virtual ~EnvelopeGenerator();
            float modifyAmplitude();
            int getSampleRate();
            int getSamplesPerWave();
            void changeState(AbstractState* newState);
            int getAttackRateInSamples();
            int getDecayRateInSamples();
            int getReleaseRateInSamples();
            void setFrequency(int frequency);
            bool getGate();
            void setGate(bool value);
            float getSustainRateInPercent();
            void setAttack(int attack);
            void setDecay(int decay);
            void setSustain(int sustain);
            void setRelease(int release);
        protected:
            /*
             * Envelope's Configuration
             */
            int attack;
            int decay;
            int sustain;
            int release;
            volatile bool gate;
            /*
             * Envelope Rates for Attack in milli Seconds
             */
            const unsigned int envelopeRatesAttack[16]
            {
                2,
                8,
                16,
                24,
                38,
                56,
                68,
                80,
                100,
                250,
                500,
                800,
                1000,
                3000,
                5000,
                8000
            };
            /*
             Envelope Rates for Decay and Release
             */
            const unsigned int envelopeRatesDecay[16]
            {
                6,
                24,
                48,
                72,
                114,
                168,
                204,
                240,
                300,
                750,
                1500,
                2400,
                3000,
                9000,
                15000,
                24000
                };
            int attackRatesInSamples[16];
            int decayRatesInSamples[16];
            int sampleRate;
            int curveValue;
            int frequency;
            //counts the samples
            long tick;
            AbstractState* currentState = nullptr;
            AbstractState* stateToDelete = nullptr;
        private:
    };
}
#endif // ENVELOPEGENERATOR_H
