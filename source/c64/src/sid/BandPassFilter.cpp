#include "BandPassFilter.h"
#include <iostream>

BandPassFilter::BandPassFilter(int filterFrequency)
:BaseFilter(filterFrequency)
{
    //ctor
}
BandPassFilter::~BandPassFilter()
{
    //dtor
}
/**
 * the attenuated above and below the filterFrequency
 * is 6db/Octave
 * The Maximum amplitude of the sid is 48db
 * the attenuate over a octave is linear
 */
void BandPassFilter::filter(int frequency,Uint8* stream, int length)
{
    bool isBelow;
    if(frequency == filterFrequency)
        return;
    if(frequency >= filterFrequency)
        isBelow = false;
    else
        isBelow = true;


    double m = 0;
    double q = 0;
    double deltaFreq=0;
    for(int count =1; count <= 8; count++)
    {
        double factorAbove = std::pow(2,(int)count);
        double factorBelow = std::pow(0.5d,(int)count);
        if(!isBelow && frequency <= filterFrequency*factorAbove)
        {
            //std::cout <<"gate "<<count<< "above "<<factorAbove<<std::endl;
            deltaFreq = frequency - filterFrequency*(factorAbove/2.0d);
            double delta = filterFrequency*factorAbove -filterFrequency*(factorAbove/2.0d);
            /*
             * calculate a linear funktion for the specific octave
             */
            m = 0.125d/delta;
            q = 1.125 - count/8.0d;
            break;
        }
        else if(isBelow && frequency >= filterFrequency*factorBelow)
        {

            //std::cout <<"gate "<<count<< "below "<<factorBelow<<std::endl;
            deltaFreq = frequency -filterFrequency*factorBelow;
            double delta = filterFrequency*(factorBelow*2.0d)-filterFrequency*factorBelow;
            /*
             * calculate a linear funktion for the specific octave
             */
            m = -0.125d/delta;
            q = 1 - count/8.0d;
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
