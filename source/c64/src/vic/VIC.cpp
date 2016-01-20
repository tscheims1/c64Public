#include "VIC.h"
#include <iostream>

using namespace std;


VIC::VIC(struct _6510_cpu* cpuPtr)
{
    this->cpuPtr =cpuPtr;
    //ctor
}

VIC::~VIC()
{
    //dtor
}
/**
 * just a helper to convert RGB to int
 */
int VIC::createRGBColor(int r,int g,int b){return (r<<24|g<<16|b<<8);}
/**
 * reads all vic specific memory
 */
void VIC::readMemory(MemoryManager* memManager)
{
    /*to simulate realtime: do not render the content every cycle*/
    /*average cycles: 2 --> 1000000 (1MHZ) Cylcles /2 / 10000 = 50HZ*/
    static int count =0;
    if(count++%10000!=0)
        return;

    int controlReg = memManager->getValue(0xD011);
    int statusReg = memManager->getValue(0xD016);


    //bitmap mode or mutli color Mode = 0n
    if(controlReg & 0x20)
    {
        colorMemoryCounter =0;
        int choosenBank = (memManager->getValue(0xDD00)&0x3);
        int bankOffset = 0;
        if(choosenBank == 0)
            bankOffset = 0xC000;
        if(choosenBank == 1)
            bankOffset = 0x8000;
        if(choosenBank == 2)
            bankOffset = 0x4000;


        //Bit 3: 0 -> no bitmapoffset; 1: bitmapoffset: 0x2000
        //Bit 5-7 offset of the screen Memory * 0x400 (in the choosen Memory bank)
        int memoryOffset = memManager->getValue(0xD018);
        int bitmapOffset = (memoryOffset&0x08)?0x2000:0;
        int screenMemoryOffset = ((memoryOffset>>4))*0x400+bankOffset;

        //bitmap mode = on or Multi Color Bitmap mode = on
        if(controlReg & 0x20)
        {
            for(int rows =0; rows < 25; rows++)
            {

                for(int cols = 0; cols < 40; cols++)
                {

                    unsigned char charbuff[8];
                    for(int i =0; i < 8; i++)
                    {
                        charbuff[i] = memManager->getValue(bankOffset+bitmapOffset +40*8*rows+cols*8+i);
                    }
                    int backgroundColor = (memManager->getValue(screenMemoryOffset+40*rows+cols) & 0xF);
                    int foregroundColor = ((memManager->getValue(screenMemoryOffset+40*rows+cols)>>4));
                    int backgroundColor2 = (memManager->getValue(0xD021)&0xF);

                    int foregroundColor2 = (memManager->getValue(0Xd800+40*rows+cols)&0xF);

                    //Standard Bitmap Mode
                    if(!(statusReg &0x10))
                    {
                        this->writePixelsInScreenBuffer(charbuff,cols,rows,foregroundColor,backgroundColor);
                    }
                    //Multi Color Bitmap mode
                    else
                    {
                        this->writeMultiColorPixelsInScreenBuffer(charbuff,cols,rows,backgroundColor2,foregroundColor,backgroundColor2,foregroundColor2);
                    }
                }

            }
        }

    }
    //text mode = on
    else
    {
        backgroundColor = colors[(memManager->getValue(0xD021)&0xF)];

        for(int rows =0; rows < 25; rows++)
        {

            for(int cols = 0; cols < 40; cols++)
            {

                int value = memManager->getValue(0x400 +40*rows+cols);
                int color = memManager->getValue(0xd800+40*rows+cols);
                this->writeCharInScreenBuffer(value,cols,rows,(color&0xF));//Only the LS Nibble is relevant
            }

        }
    }
}
/**
 * Mode: Bitmap Mode
 * Renders the bitmap array for the screen
 */
void VIC::writePixelsInScreenBuffer(unsigned char* characters,int col,int row,int colorNo1,int colorNo2)
{
     int rgbForeground = colors[colorNo1];
     int rgbBackground = colors[colorNo2];

    int* buffer = (int*)this->screenBuffer();

    int bit = 0;
    int raster = 0;
    int mask = 0x1;
    for(int y =0;y <8; y++)
    {
        mask = 128;
        /*
         * Set the Starting bit of the Screen buffer
         */
        bit = (row*8+raster)*PIXEL_COLS+(col*8);
        //go one rasterline down
        raster++;

        //Render each bit of a character-line
        for(int i =0; i < 8; i++)
        {
            if(!!(characters[y] &mask)) //if a bit is set, render pixel
                buffer[bit+i] = rgbForeground;
            else
                buffer[bit+i] = rgbBackground;
            mask>>=1;
        }
    }

}
/**
 * Mode: Multicolor Bitmap Mode
 * Renders the bitmap array for the screen
 */
void VIC::writeMultiColorPixelsInScreenBuffer(unsigned char* characters,int col,int row,int colorNo0,int colorNo1,int colorNo2,int colorNo3)
{

    int* buffer = (int*)this->screenBuffer();

    int bit = 0;
    int raster = 0;
    int mask = 0x1;
    for(int y =0;y <8; y++)
    {
        mask = 192;
        /*
         * Set the Starting bit of the Screen buffer
         */
        bit = (row*8+raster)*PIXEL_COLS+(col*8);
        //go one rasterline down
        raster++;

        //Render each bit of a character-line
        for(int i =0; i < 8; i++)
        {
            if(i%2 ==1)
            {
               int choosenColor = (characters[y]&mask)>>(8-i-1);

                switch(choosenColor)
                {
                    case 0:
                        buffer[bit+i] = colors[colorNo0];
                        buffer[bit+i-1]   = colors[colorNo0];
                    break;
                    case 1:
                        buffer[bit+i] = colors[colorNo1];
                        buffer[bit+i-1]   = colors[colorNo1];
                    break;
                    case 2:
                        buffer[bit+i] = colors[colorNo2];
                        buffer[bit+i-1]   = colors[colorNo2];
                    break;
                    case 3:
                        buffer[bit+i] = colors[colorNo3];
                        buffer[bit+i-1]   = colors[colorNo3];
                        colorMemoryCounter++;
                    break;
                }
                mask>>=2;
            }
        }
    }

}
/**
 * fire a raster interrupt if the current Rasterline == Rasterline at D011/D012
 * and the Rasterinterrupt is enabled (LSB of 0xD01A is set)
 */
void VIC::fireRasterInterrupt(int currentRasterline,MemoryManager* memManager)
{
     int rasterLineHigh = (memManager->getValue(0xD011) & 0x80);
     int rasterLineLow  = memManager->getValue(0xD012);

    if((memManager->getValue(0xD01A) & 0x01))//Are RasterInterrupts enabled?
    {
        int line = ((rasterLineHigh<<8)|rasterLineLow);

        if(cpuPtr->interrupt == '1' && currentRasterline == line)
        {
            cpuPtr->interrupt = '0';
        }
    }

}
/**
 * Mode: Standard Text Mode
 * Renders the characters from the character rom
 * at the specific position on the screen
 */
void VIC::writeCharInScreenBuffer(unsigned char character,int col,int row,int colorNo)
{
    int rgb = colors[colorNo];

    unsigned char* charMem = this->charGen.getCharMem();

    int* buffer = (int*)this->screenBuffer();

    int bit = 0;
    int raster = 0;
    int mask = 0x1;
    for(int y =0;y <8; y++)
    {
        mask = 128;
        /*
         * Set the Starting bit of the Screen buffer
         */
        bit = (row*8+raster)*PIXEL_COLS+(col*8);
        //go one rasterline down
        raster++;

        //Render each bit of a character-line
        for(int i =0; i < 8; i++)
        {
            if(!!(charMem[8*character+y] &mask)) //if a bit is set, render pixel
                buffer[bit+i] = rgb;
            else
                buffer[bit+i] = backgroundColor;
            mask>>=1;
        }
    }
}

