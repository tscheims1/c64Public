#ifndef VIC_H
#define VIC_H

#include "CharGen.h"
#include "Screen.h"
#include <iostream>
#include "C64Component.h"
using namespace std;

class CharGen;

class VIC
{

    friend class CharGen;
    CharGen charGen;

    public:
        VIC(struct _6510_cpu* cpuPtr );
        virtual ~VIC();

        //! Holt ScreenBuffer des Zeichengenerators
        inline void *screenBuffer() { return charGen.screenBuffer(); }
        void writeCharInScreenBuffer(unsigned char character,int col,int row,int colorNo);
        void writeMultiColorPixelsInScreenBuffer(unsigned char* characters,int col,int row,int colorNo0,int colorNo1,int colorNo2,int colorNo3);
        void writePixelsInScreenBuffer(unsigned char* characters,int col,int row,int colorNo1,int colorNo2);
        void readMemory(Service::MemoryManager* memManager);
        void fireRasterInterrupt(int rasterLine,Service::MemoryManager* memManager);
    protected:
    static int createRGBColor(int,int,int);
    int colorMemoryCounter =0;


    /**
     * The VIC II Color palette
     * @see: https://www.c64-wiki.com/index.php/Color
     *
     */
    int colors[16] =
    {
        createRGBColor(0,0,0),
        createRGBColor(255,255,255),
        createRGBColor(136,0,0),
        createRGBColor(170,255,238),
        createRGBColor(204,68,204),
        createRGBColor(0,204,85),
        createRGBColor(0,0,170),
        createRGBColor(238,238,119),
        createRGBColor(221,136,85),
        createRGBColor(102,68,0),
        createRGBColor(255,119,119),
        createRGBColor(51,51,51),
        createRGBColor(119,119,119),
        createRGBColor(170,255,102),
        createRGBColor(0,136,255),
        createRGBColor(187,187,187),

    };
    private:
        int backgroundColor = 0x0;
        struct _6510_cpu* cpuPtr;
};

#endif // VIC_H
