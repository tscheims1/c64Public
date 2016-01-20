#include "LowPassFilter.h"
#include <iostream>
LowPassFilter::LowPassFilter(int filterFrequency)
:BaseFilter(filterFrequency)
{
    //ctor
}

LowPassFilter::~LowPassFilter()
{
    //dtor
}
/**
 * the attenuated below the filterFrequency
 * is 12db/Octave
 * The Maximum amplitude of the sid is 48db
 * the attenuate in a octave is linear
 * https://en.wikipedia.org/wiki/Low-pass_filter#First_order
 */
void LowPassFilter::filter(int frequency,Uint8* stream, int length)
{
        if(frequency <= filterFrequency)
        return;

    double m = 0;
    double q = 0;
    double deltaFreq=0;
    for(int count =1; count <= 4; count++)
    {
        double factor = std::pow(2,(int)count);
        if(frequency <= filterFrequency*factor)
        {
            deltaFreq = frequency - filterFrequency*(factor/2.0d);
            double delta = filterFrequency*factor -filterFrequency*(factor/2.0d);
            /*
             * calculate a linear funktion for the specific octave
             */
            m = 0.25d/delta;
            q = 1.25 - count/4.0d;
            break;
        }
    }
    for(int i =0; i < length; i++)
    {
        int value = stream[i];
        value-=127;
        value = value*(q-m*deltaFreq)*CUSTOM_ATT_VALUE;
        value+=127;
        stream[i] = value;
    }
}
