#ifndef MIXER_H
#define MIXER_H
#include "Oscilator.h"
#include "LowPassFilter.h"
#include "HighPassFilter.h"
#include "BandPassFilter.h"
#include "Sid.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#define SAMPLE_AMOUNT 4096
namespace Sid
{
/**
 * this class mixes all oscliators waves together
 * and add's filters, adjust volume control, doing Ring-Modulation
 *
 */
    class Sid;
    class Oscilator;
    class Mixer
    {
        public:
            Mixer(Oscilator* os1, Oscilator* os2, Oscilator* os3,Sid* sid);
            Mixer(int sampleRate,Oscilator* os1, Oscilator* os2, Oscilator* os3,Sid* sid);
            virtual ~Mixer();
            static void fillAudioBuffer(void* userData, Uint8* buffer, int length);
            void fillAudioBuffer(Uint8* buffer,int length);
            Oscilator** getOscilatorArrayPtr();
            void start();
            void stop();
            void setHiPassFilter(bool enable);
            void setLowPassFilter(bool enable);
            void setBandPassFilter(bool enalbe);
            void setFilterFreq(int freqency);
            void filterVoice1(bool enable);
            void filterVoice2(bool enable);
            void filterVoice3(bool enable);
            void disconnectVoice3(bool enable);
            void setVolume(int volume);
            void ringModulation(Uint8* stream1,Uint8*stream2,int length);
            void muteOscilator(Uint8* stream,int length);
            void drawWaveForm();
        protected:
            int sampleRate;
            WaveViewer* waveViewer;
            Oscilator* oscilators[3];
            void mixAndHardClip(Uint8* stream1,Uint8* stream2, Uint8* stream3, int length);
            void filterStreams(Uint8* stream1,Uint8* stream2,Uint8* stream3,int length);
            void changeVolue(Uint8* stream,int length);
            bool hpIsSet = false;
            bool lpIsSet = false;
            bool bpIsset = false;
            bool voice1IsFiltered = false;
            bool voice2IsFiltered = false;
            bool voice3IsFiltered = false;
            int filterFreq = 0;
            int volume =0;
            bool voice3IsDisconnected = false;
            Sid* sid;
            uint8_t* stream2;
            uint8_t* stream3;

        private:
    };
}
#endif // MIXER_H
