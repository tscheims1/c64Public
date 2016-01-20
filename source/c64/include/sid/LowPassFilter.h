#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include "BaseFilter.h"


class LowPassFilter : public BaseFilter
{
    public:
        LowPassFilter(int filterFrequency);
        virtual ~LowPassFilter();
        virtual void filter(int frequency,Uint8* stream,int length);
    protected:
    private:
};

#endif // LOWPASSFILTER_H
