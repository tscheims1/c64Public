#include "Mixer.h"
namespace Sid
{
    Mixer::Mixer(Oscilator* os1, Oscilator* os2, Oscilator* os3,Sid* sid)
    : Mixer(SAMPLE_RATE,os1,os2,os3,sid){}
    /**
     * initialize all oscilators, EnvelopeGenerators and SDL Audio
     */
    Mixer::Mixer(int sampleRate,Oscilator* os1, Oscilator* os2, Oscilator* os3,Sid* sid)
    {
        this->sid = sid;
        this->waveViewer = new WaveViewer();

        this->sampleRate = sampleRate;
        stream2= new Uint8[SAMPLE_AMOUNT];
        stream3 = new Uint8[SAMPLE_AMOUNT];

        this->oscilators[0] = os1;
        this->oscilators[1] = os2;
        this->oscilators[2] = os3;


        if(!SDL_WasInit(SDL_INIT_AUDIO));
            SDL_Init(SDL_INIT_AUDIO);
        SDL_AudioSpec desiredSpec;

        desiredSpec.freq = this->sampleRate;
        desiredSpec.format = AUDIO_U8;
        desiredSpec.channels = 1;
        desiredSpec.samples = SAMPLE_AMOUNT;
        desiredSpec.callback = fillAudioBuffer;
        desiredSpec.userdata = this;

        SDL_AudioSpec obtainedSpec;

        // you might want to look for errors here
        SDL_OpenAudio(&desiredSpec, &obtainedSpec);
    }
    Mixer::~Mixer()
    {
        delete this->oscilators[0];
        delete this->oscilators[1];
        delete this->oscilators[2];
        delete this->waveViewer;
    }
    /**
     * static callback method
     */
    void Mixer::fillAudioBuffer(void* userData,Uint8* buffer,int length)
    {
            ((Mixer*)userData)->fillAudioBuffer( buffer, length);
    }
    /**
     *let the three oscilators generate the enveloped Wave Signals
      */
    void Mixer::fillAudioBuffer(Uint8* stream,int length)
    {

        this->oscilators[0]->fillAudioBuffer(stream,  length);
        //this->waveViewer->storeStream(stream,length,0);


        this->oscilators[1]->fillAudioBuffer(stream2,length);
        //this->waveViewer->storeStream(stream2,length,1);

       /*
         *do not use filter 3 for the mixer:if specifc flag isset
         */
        if(voice3IsDisconnected)
            for(int i = 0; i < length;i++)stream3[i] = 127;
        else
            this->oscilators[2]->fillAudioBuffer(stream3,length);

        //this->waveViewer->storeStream(stream3,length,2);

        //add ring modulation
        if(oscilators[0]->ringModulationIsSet() && oscilators[0]->getWaveForm() == WaveForm::TRIANGLE)
            this->ringModulation(stream,stream3,length);

        if(oscilators[1]->ringModulationIsSet() && oscilators[1]->getWaveForm() == WaveForm::TRIANGLE)
            this->ringModulation(stream2,stream,length);

        if(oscilators[2]->ringModulationIsSet() && oscilators[2]->getWaveForm() == WaveForm::TRIANGLE)
            this->ringModulation(stream3,stream,length);

        //Set Filters and clip the audio
        this->filterStreams(stream,stream2,stream3,length);
        this->mixAndHardClip(stream,stream2,stream3,length);

        this->changeVolue(stream,length);
        //this->waveViewer->storeStream(stream,length,3);

        this->sid->readMemory();
    }
    Oscilator** Mixer::getOscilatorArrayPtr()
    {
        return this->oscilators;
    }
    void Mixer::start()
    {
        SDL_PauseAudio(0);
    }
    void Mixer::stop()
    {
        SDL_PauseAudio(1);
    }
    void Mixer::drawWaveForm()
    {
        this->waveViewer->drawWaveForm();
        this->waveViewer->scrollWaveForm();
    }
    /**
     * mix all stream together: add the values and clip to values
     */
    void Mixer::mixAndHardClip(Uint8* stream1,Uint8* stream2, Uint8* stream3, int length)
    {
        int mixedValue = 0;
        for(int i = 0; i < length; i++)
        {
            //mix all straems together
            int s1 = stream1[i];
            int s2 = stream2[i];
            int s3 = stream3[i];
            s1-=127;
            s2-=127;
            s3-=127;
            mixedValue = s1+s2+s3;

            //clip values that are out of range
            if(mixedValue >127)mixedValue = 127;
            if(mixedValue <-128)mixedValue = -128;

            stream1[i] = (Uint8)(mixedValue+127);
            //stream1[i] = stream2[i];
        }

    }
    void Mixer::changeVolue(Uint8* stream,int length)
    {
        float factor;
        if(this->volume == 0)
            factor = 0;
        else
            factor = (15.0f/((float)this->volume));

        for(int i =0; i < length;i++)
        {
            int ele = stream[i];
            ele -= 127;
            ele *= factor;
            ele += 127;
            stream[i] = ele;
        }
    }
    void Mixer::filterStreams(Uint8* stream1,Uint8* stream2, Uint8* stream3,int length)
    {
        //LowPassFilter
        if(lpIsSet)
        {
            LowPassFilter* lpFilter = new LowPassFilter(filterFreq);
            if(voice1IsFiltered)
            {
                lpFilter->filter(oscilators[0]->getFrequency(),stream1,length);
            }
            if(voice2IsFiltered)
            {
                lpFilter->filter(oscilators[1]->getFrequency(),stream2,length);

            }
            if(voice3IsFiltered && !voice3IsDisconnected)
            {
                lpFilter->filter(oscilators[2]->getFrequency(),stream3,length);
            }
            delete lpFilter;
        }
           //LowPassFilter
        if(hpIsSet)
        {
            HighPassFilter* hpFilter = new HighPassFilter(filterFreq);
            if(voice1IsFiltered)
            {
                hpFilter->filter(oscilators[0]->getFrequency(),stream1,length);
            }
            if(voice2IsFiltered)
            {
                hpFilter->filter(oscilators[1]->getFrequency(),stream2,length);

            }
            if(voice3IsFiltered && !voice3IsDisconnected)
            {
                hpFilter->filter(oscilators[2]->getFrequency(),stream3,length);
            }
            delete hpFilter;
        }
        if(bpIsset)
        {
            BandPassFilter* bpFilter = new BandPassFilter(filterFreq);
            if(voice1IsFiltered)
            {
                bpFilter->filter(oscilators[0]->getFrequency(),stream1,length);
            }
            if(voice2IsFiltered)
            {
                bpFilter->filter(oscilators[1]->getFrequency(),stream2,length);

            }
            if(voice3IsFiltered && !voice3IsDisconnected)
            {
                bpFilter->filter(oscilators[2]->getFrequency(),stream3,length);
            }
            delete bpFilter;
        }
    }
    void Mixer::setLowPassFilter(bool enable)
    {
        this->lpIsSet = enable;
    }
    void Mixer::setHiPassFilter(bool enable)
    {
        this->hpIsSet = enable;
    }
    void Mixer::setBandPassFilter(bool enable)
    {
        this->bpIsset = enable;
    }
    void Mixer::setFilterFreq(int frequency)
    {
        this->filterFreq = frequency;
    }
    void Mixer::filterVoice1(bool enable)
    {
        this->voice1IsFiltered = enable;
    }
    void Mixer::filterVoice2(bool enable)
    {
        this->voice2IsFiltered = enable;
    }
    void Mixer::filterVoice3(bool enable)
    {
        this->voice3IsFiltered = enable;
    }
    void Mixer::disconnectVoice3(bool enable)
    {
        this->voice3IsDisconnected = enable;
    }
    void Mixer::setVolume(int volume)
    {
        this->volume = volume;
    }
    /**
     * calculate ring modulation:
     * ring mod is a multiplication of two signals
     */
    void Mixer::ringModulation(Uint8* stream1,Uint8* stream2,int length)
    {
        for(int i =0; i < length;i++)
        {
            int value = (stream1[i]-127) * (stream2[i]-127)+127;
            if(value >=127)value = 127;
            if(value <= -128)value= -128;
            stream1[i] = value;
        }
    }
}
