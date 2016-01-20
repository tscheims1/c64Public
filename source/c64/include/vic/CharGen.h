#ifndef CHARGEN_H
#define CHARGEN_H

#include <iostream>

using namespace std;

class CharGen
{
public:
    CharGen();
    virtual ~CharGen();

    inline void *screenBuffer() { return screenBuffer1; }
    inline unsigned char* getCharMem(){return charMem;}
protected:

private:
    //! Ausgabe erfolgt direkt an den Bildschirm
    // 200 Zeilen, 320 Spalten (Buffer for SDL Screen Renderer)
    int screenBuffer1[200*320];
    unsigned char charMem[512*8];
};

#endif // CHARGEN_H
