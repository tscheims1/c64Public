#ifndef OSCILATOR_H
#define OSCILATOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <cmath>
#include <random>
#include "WaveViewer.h"
#include "EnvelopeGenerator.h"

namespace Sid
{
    enum WaveForm{ TRIANGLE, PULSE,SAWTHOOTH, NOISE};
    /**
     * This class represents a SID Oscilator
     * it computes the different wave forms
     * and fill it ins the SDL Audio Buffer
     */
    class EnvelopeGenerator;
    class Oscilator
    {
        public:
            Oscilator(EnvelopeGenerator* envelopeGenerator);
            Oscilator(EnvelopeGenerator* envelopeGenerator,int sampleRate);
            void setWaveForm(WaveForm waveForm);
            void setPulseWith(int pulseWith);
            virtual ~Oscilator();
            void setFrequency(int frequence);
            int getFrequency();
            void setTestbit(bool value);
            bool getTestbit();
            void setRingModulation(bool modulation);
            bool ringModulationIsSet();
            WaveForm getWaveForm();
            /* for debug use only*/
            void drawWaveForm();
            void fillAudioBuffer(Uint8* buffer,int length);
            std::string toString();
        protected:
            int sampleRate;
            int frequency = 0;
            int pulseWith = 0;
            float pulseWithPercent;
            WaveForm waveForm;
            EnvelopeGenerator* envelopeGenerator;
            bool ringModulation = false;
            bool testbit = false;

            void generateSinWave(Uint8* stream, int length);
            void generateNoiseWave(Uint8* stream, int length);
            void generateTriangleWave(Uint8* stream, int length);
            void generateSawtoothWave(Uint8* stream, int length);
            void generatePulseWave(Uint8* stream, int length);



        private:
    };
}
#endif // OSCILATOR_H
