#include "Oscilator.h"
#include <future>
namespace Sid
{
    Oscilator::Oscilator(EnvelopeGenerator* envelopeGenerator):
        Oscilator(envelopeGenerator,SAMPLE_RATE){}
    Oscilator::Oscilator(EnvelopeGenerator* envelopeGenerator, int sampleRate)
    {
        this->envelopeGenerator = envelopeGenerator;
        this->sampleRate = sampleRate;
    }
    void Oscilator::setWaveForm(WaveForm waveForm)
    {
        this->waveForm = waveForm;
    }
    void Oscilator::setFrequency(int frequency)
    {
        this->frequency = frequency*0.05960f;
    }
    Oscilator::~Oscilator()
    {
    }
    void Oscilator::fillAudioBuffer(Uint8* stream, int length)
    {
        if(this->frequency ==0|| this->testbit == true)
        {
            for(int i=0; i< length;stream[i++] = 127);
            return;
        }

        this->envelopeGenerator->setFrequency(this->frequency);
        switch(this->waveForm)
        {
            case WaveForm::NOISE:
                generateNoiseWave(stream,length);
                break;
            case WaveForm::SAWTHOOTH:
                generateSawtoothWave(stream,length);
                break;
            case WaveForm::TRIANGLE:
                generateTriangleWave(stream,length);
                break;
            case WaveForm::PULSE:
                generatePulseWave(stream,length);
                break;

        }


    }
    /**
     * ####                      #####
     *                  ####
     *      ####
     *              ####     ####
     * Noise wave generator
     */
    void Oscilator::generateNoiseWave(Uint8* stream, int length)
    {
        std::default_random_engine randomDevice;
        std::uniform_int_distribution<int> dist(0,255);
        int samplesPerWave = this->sampleRate/ this->frequency;
        int noiseSignal = 0;

        for(int i =0; i < length; i++)
        {
            if(i % samplesPerWave ==0)
                noiseSignal =    dist(randomDevice);
            stream[i] = noiseSignal;
        }
    }
    /**      #        #
     *     #        #
     *   #        #
     * #        #
     * Generates Sawtooth Waves*/
    void Oscilator::generateSawtoothWave(Uint8* stream, int length)
    {

        int samplesPerWave = this->sampleRate/ this->frequency;
        for(int i = 0; i < length; i++)
        {
            float amp = this->envelopeGenerator->modifyAmplitude();
            stream[i] = std::round(amp/samplesPerWave*(i%samplesPerWave));
            stream[i] += (255.0f - amp)/2; //translate sinus wave in the middle

        }
    }
    /**
     * generate a triangle Wave Form
     *     #
     *   #   #
     * #       #
     */
    void Oscilator::generateTriangleWave(Uint8* stream, int length)
    {
        int samplesPerWave = this->sampleRate/ (float)this->frequency*0.5f;
        bool isRising = true;
        for(int i = 0; i < length; i++)
        {
            float modifiedAmplitude = this->envelopeGenerator->modifyAmplitude();
            if(isRising)
                stream[i] = std::round(modifiedAmplitude/(samplesPerWave)*(i%samplesPerWave));
            else
                stream[i] = std::round(modifiedAmplitude - modifiedAmplitude/(samplesPerWave)*(i%samplesPerWave));

            if((i+1)%(samplesPerWave)==0)
                isRising = not isRising;//toggle check bit
            stream[i] += (255.0f - modifiedAmplitude)/2; //translate sinus wave in the middle

        }
    }
    /**
     *      ######      #####
     *
     *
     * #####      ######
     * Generates Pulse Waves with the given Pulse With
     */
    void Oscilator::generatePulseWave(Uint8* stream, int length)
    {

        int samplesPerHighPulse = std::ceil(this->sampleRate/ (float)this->frequency*this->pulseWithPercent);
        int samplesPerLowPulse = std::floor(this->sampleRate/ (float)this->frequency*(1.0f-this->pulseWithPercent));
        bool isHigh = false;

        for(int i = 0,hiCount = 0,lowCount =0; i < length; i++)
        {
            int modifiedAmplitude = this->envelopeGenerator->modifyAmplitude();


            if(samplesPerHighPulse ==0)isHigh = false;
            if(samplesPerLowPulse == 0)isHigh = true;
            if(isHigh)
                hiCount++;
            else
                lowCount++;

            if(samplesPerHighPulse >= hiCount && isHigh)
            {
                stream[i] = modifiedAmplitude;

            }
            if(samplesPerLowPulse >=lowCount &&!isHigh)
            {
                stream [i] = 0;

            }
            /*toggle pulse*/
            if(samplesPerHighPulse == hiCount)
            {
                hiCount =0;
                isHigh = false;
            }
            if(samplesPerLowPulse == lowCount)
            {
                lowCount = 0;
                isHigh = true;
            }
            stream[i] += (255.0f - modifiedAmplitude)/2; //translate pulse wave in the middle

        }
    }
    void Oscilator::setPulseWith(int pulseWith)
    {
        this->pulseWith = pulseWith;
        if(this->pulseWith == 0x0 || this->pulseWithPercent == 0xFFF)
            this->pulseWithPercent = 1;
        else
            this->pulseWithPercent = ((float)this->pulseWith)/0xFFF;
    }
    int Oscilator::getFrequency()
    {
        return this->frequency;
    }
    WaveForm Oscilator::getWaveForm()
    {
        return this->waveForm;
    }
    void Oscilator::setRingModulation(bool ringModulation)
    {
        this->ringModulation = ringModulation;
    }
    bool Oscilator::ringModulationIsSet()
    {
        return this->ringModulation;
    }
    void Oscilator::setTestbit(bool value)
    {
        this->testbit = value;
    }
    bool Oscilator::getTestbit()
    {
        return this->testbit;
    }
    /**
     * Prints the most important Registers
     * For Debug output
     */
    std::string Oscilator::toString()
    {
        std::string desc = "";
        desc = "freq: "+std::to_string(this->frequency)+ " waveForm: "+ std::to_string(this->waveForm);
        desc += "\ngate bit "+std::to_string(this->envelopeGenerator->getGate());
        return desc;
    }
}
