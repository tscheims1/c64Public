#ifndef BASEFILTER_H
#define BASEFILTER_H
#include <SDL2/SDL.h>
#include <cmath>
/* to ajust filter settings*/
#define CUSTOM_ATT_VALUE 0.6
class BaseFilter
{
    public:
        BaseFilter(int filterFrequency);
        virtual ~BaseFilter();
        virtual void filter(int frequency,Uint8* stream,int length) = 0;//Pure abstract Class
    protected:
        int filterFrequency;
    private:
};

#endif // BASEFILTER_H
