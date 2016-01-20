#ifndef HIGHPASSFILTER_H
#define HIGHPASSFILTER_H

#include "BaseFilter.h"


class HighPassFilter : public BaseFilter
{
    public:
        HighPassFilter(int filterFrequency);
        virtual ~HighPassFilter();
        virtual void filter(int frequency,Uint8* stream,int length);
    protected:
    private:
};

#endif // HIGHPASSFILTER_H
