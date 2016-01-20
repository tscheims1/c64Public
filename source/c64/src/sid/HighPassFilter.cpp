#include "HighPassFilter.h"
#include <iostream>
HighPassFilter::HighPassFilter(int filterFrequency)
:BaseFilter(filterFrequency)
{
    //ctor
}

HighPassFilter::~HighPassFilter()
{
    //dtor
}
/**
 * the attenuated above the filterFrequency
 * is 12db/Octave
 * The Maximum amplitude of the sid is 48db
 * the attenuate over a octave is linear
 */
void HighPassFilter::filter(int frequency,Uint8* stream, int length)
{
      if(frequency >= filterFrequency)
        return;

    double m = 0;
    double q = 0;
    double deltaFreq=0;
    for(int count =1; count <= 4; count++)
    {
        double factor = std::pow(0.5d,(int)count);
        if(frequency >= filterFrequency*factor)
        {
            deltaFreq = frequency -filterFrequency*factor;
            double delta = filterFrequency*(factor*2.0d)-filterFrequency*factor;
            /*
             * calculate a linear funktion for the specific octave
             */
            m = 0.25d/delta;
            q = 1 - count/4.0d;
            break;
        }
    }
    for(int i =0; i < length; i++)
    {
        int value = stream[i];
        value-=127;
        value = value*(q+m*deltaFreq)*CUSTOM_ATT_VALUE;
        value+=127;
        stream[i] = value;
    }

}
