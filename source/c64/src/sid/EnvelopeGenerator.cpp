#include "EnvelopeGenerator.h"
#include "AttackState.h"
#include <iostream>
namespace Sid
{
    EnvelopeGenerator::EnvelopeGenerator() :
    EnvelopeGenerator(SAMPLE_RATE)
    {


    }
    EnvelopeGenerator::EnvelopeGenerator(int sampleRate)
    {
        tick = 0;

        currentState = new AttackState(this,0);
        this->sampleRate = sampleRate;

        //precalc attack/release and decay tables
        for(int i =0; i < 16; i++)
        {
            attackRatesInSamples[i] = envelopeRatesAttack[i] * this->sampleRate / 1000;
            decayRatesInSamples[i] = envelopeRatesDecay[i] * this->sampleRate / 1000;
        }
    }
    EnvelopeGenerator::~EnvelopeGenerator()
    {
        if(this->currentState != nullptr)
            delete this->currentState;
        if(this->stateToDelete != nullptr)
            delete this->stateToDelete;
    }
    /**
     * State Event Machine Method Interface
     */
    float EnvelopeGenerator::modifyAmplitude()
    {
        return this->currentState->modifyAmplitude();
    }
    int EnvelopeGenerator::getSampleRate()
    {
        return this->sampleRate;
    }
    int EnvelopeGenerator::getSamplesPerWave()
    {
        return this->sampleRate/this->frequency;
    }
    int EnvelopeGenerator::getAttackRateInSamples()
    {
        return this->attackRatesInSamples[this->attack];
    }
    int EnvelopeGenerator::getDecayRateInSamples()
    {
        return this->decayRatesInSamples[this->decay];
    }
    int EnvelopeGenerator::getReleaseRateInSamples()
    {
        return this->decayRatesInSamples[this->release];
    }
    /**
     * Change the state in the State Event Machine
     */
    void EnvelopeGenerator::changeState(AbstractState* newState)
    {
         /* Because we have no Garbage Collector, we have
         * to store the last State and delete it later
         */
        if(this->stateToDelete != nullptr)
        {
            delete this->stateToDelete;
        }
        this->stateToDelete = this->currentState;
        this->currentState = newState;
    }
    void EnvelopeGenerator::setFrequency(int frequency)
    {
        this->frequency = frequency;
    }
    bool EnvelopeGenerator::getGate()
    {
        return this->gate;
    }
    void EnvelopeGenerator::setGate(bool value)
    {
        this->gate = value;
    }
    float EnvelopeGenerator::getSustainRateInPercent()
    {
        return (float)this->sustain/0xF;
    }
    void EnvelopeGenerator::setAttack(int attack)
    {
        this->attack = attack;
    }
    void EnvelopeGenerator::setDecay(int decay)
    {
        this->decay = decay;
    }
    void EnvelopeGenerator::setSustain(int sustain)
    {
        this->sustain = sustain;
    }
    void EnvelopeGenerator::setRelease(int release)
    {
        this->release = release;
    }
}
