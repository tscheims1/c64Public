#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <map>
#include "VIC.h"
#include "C64Component.h"
#include "MemoryManager.h"
#include "Cia.h"
using namespace Service;
using namespace Cia;
enum
{
    DISPLAY_WIDTH  = 640,
    DISPLAY_HEIGHT = 400,
    UPDATE_INTERVAL = 1000/60,
    PIXELWIDTH = 2,
    COLUMNS = 40,
    ROWS = 25,
    PIXEL_COLS = COLUMNS * 8,
    PIXEL_ROWS = ROWS * 8
};


class VIC;
class Screen : public C64Component
{
public:
    Screen(VIC* vic,Cia::Cia* cia,MemoryManager* memManager,MemoryMap* memMap);
    virtual ~Screen();
    void power();
    void display();
    void stop();
    void draw();
    void fillRect(SDL_Rect* rc, int r, int g, int b );
    void fpsChanged( int fps );
    void onQuit();
    void onKeyDown( SDL_Event* event );
    void onKeyUp( SDL_Event* event );
    void run();
    void update();
    void readMemory(MemoryManager* memManager);
    bool isAsync();

    enum Color {
        BLACK   = 0x00,
        WHITE   = 0x01,
        RED     = 0x02,
        CYAN    = 0x03,
        PURPLE  = 0x04,
        GREEN   = 0x05,
        BLUE    = 0x06,
        YELLOW  = 0x07,
        LTBROWN = 0x08,
        BROWN   = 0x09,
        LTRED   = 0x0A,
        GREY1   = 0x0B,
        GREY2   = 0x0C,
        LTGREEN = 0x0D,
        LTBLUE  = 0x0E,
        GREY3   = 0x0F
    };
protected:
private:

    std::map<int,int> keys;
    int frameSkip;
    int running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    VIC* vic;
    Cia::Cia* cia;
    MemoryManager* memManager;
    MemoryMap* memMap;
};

#endif // SCREEN_H
