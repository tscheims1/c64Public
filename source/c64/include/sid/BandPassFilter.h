#ifndef BANDPASSFILTER_H
#define BANDPASSFILTER_H

#include "BaseFilter.h"


class BandPassFilter : public BaseFilter
{
    public:
        BandPassFilter(int filterFrequency);
        virtual ~BandPassFilter();
        virtual void filter(int frequency,Uint8* stream,int length);
    protected:
    private:
};

#endif // BANDPASSFILTER_H
