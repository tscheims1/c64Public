#include "Screen.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "VIC.h"
using namespace std;


Screen::Screen(VIC* vic,Cia::Cia* cia,MemoryManager* memManager,MemoryMap* memMap)
    :frameSkip(0), running(0), window(NULL), renderer(NULL)
{
    this->vic = vic;
    this->cia = cia;
    this->memManager = memManager;
    this->memMap = memMap;
}
bool Screen::isAsync(){return true;}
void Screen::readMemory(MemoryManager* memManager)
{
    vic->readMemory(memManager);
}
void Screen::power()
{
    int flags = SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL ;
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        return;
    }
        window = SDL_CreateWindow(
        "VIC Monitor II XXL Black Mamba Edition",                  // window title
        0,                                 // initial x position
        0,                                 // initial y position
        DISPLAY_WIDTH,                     // width, in pixels
        DISPLAY_HEIGHT,                    // height, in pixels
        flags            // flags - see below
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE|SDL_RENDERER_TARGETTEXTURE);

    /*if (SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, DISPLAY_HEIGHT, flags, &window, &renderer))
    {
        return;
    }*/
    this->running = 1;

    display();
}

void Screen::display()
{
    int past = SDL_GetTicks();
    int now = past, pastFps = past ;
    int fps = 0, framesSkipped = 0 ;
    SDL_Event event ;
    bool hasFocus = true;
    bool otherHasFocus = false;
    while ( running )
    {
        //We have to update the Memory Map every Cycle
        memMap->updateRenderer();
        int timeElapsed = 0 ;
        if (SDL_PollEvent(&event))
        {
            //dispatch event to other SDL Event handler
            if(otherHasFocus && event.type != SDL_WINDOWEVENT)
            {
                memMap->eventHandler(event);
                continue;
            }
            switch (event.type)
            {
                /*
                 * dispatch events to the selected Screen
                 * the SDL_WindowEvents recognize when the screen Focus changes
                 */
                case SDL_WINDOWEVENT :
                {
                    SDL_Window* targetWindow=SDL_GetWindowFromID(event.window.windowID);


                    switch(event.window.event)
                    {
                      case SDL_WINDOWEVENT_FOCUS_GAINED :
                        if(targetWindow == window)
                        {
                            hasFocus = true;
                            otherHasFocus = false;
                        }
                        else
                        {
                            otherHasFocus = true;
                            hasFocus = false;
                        }

                        break;
                      case SDL_WINDOWEVENT_FOCUS_LOST :
                        if(targetWindow == window)
                        {
                            hasFocus = false;
                        }
                        break;
                    }
                    break;
                }

            case SDL_QUIT:
                if(!hasFocus)break;
                onQuit();
                break;
            case SDL_KEYDOWN:
                if(!hasFocus)break;
                onKeyDown( &event );
                break ;
            case SDL_KEYUP:
                if(!hasFocus)break;
                onKeyUp( &event );
                break ;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
                break ;
            }
        }
        // update/draw
        timeElapsed = (now=SDL_GetTicks()) - past ;
        if ( timeElapsed >= UPDATE_INTERVAL  )
        {
            past = now ;
            update();
            if ( framesSkipped++ >= frameSkip )
            {
                draw();
                ++fps ;
                framesSkipped = 0;
            }
        }
        // fps
        if ( now - pastFps >= 1000 )
        {
            pastFps = now ;
            fpsChanged( fps );
            fps = 0;
        }
        // sleep?
        SDL_Delay( 1 );
    }
}

void Screen::update()
{
}

void Screen::fillRect(SDL_Rect* rc, int r, int g, int b )
{
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, rc);
}

void Screen::draw()
{
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    for(int r = 0; r < PIXEL_ROWS; r++)
    {
        for(int c = 0; c < PIXEL_COLS; c++)
        {
            int rgb = ((int*)vic->screenBuffer())[c+r*320];

            this->vic->fireRasterInterrupt(r*c,memManager);

            SDL_Rect pixel;
            pixel.x = c*PIXELWIDTH;
            pixel.y = r*PIXELWIDTH;
            pixel.w = pixel.h = PIXELWIDTH;
            fillRect(&pixel, (uint)((rgb & 0xFF000000) >> 24), (uint)((rgb & 0x00FF0000) >> 16), (uint)((rgb & 0x0000FF00) >> 8));
        }
    }
    SDL_RenderPresent(renderer);
}

void Screen::fpsChanged( int fps )
{
    char szFps[ 128 ] ;
    sprintf( szFps, "%s: %d FPS", "C64", fps );
    SDL_SetWindowTitle(window, szFps);
}
/**
 * delegate pressed Screen Event to the CIA Controller
 */
void Screen::onKeyDown( SDL_Event* evt )
{
    cia->keyEvent(*evt,false);
    keys[ evt->key.keysym.sym ] = 1 ;
}

void Screen::onKeyUp( SDL_Event* evt )
{
   //because of the realtimeproblem: don't remove the key from the matrix
}

void Screen::onQuit()
{
    running = 0 ;
}

Screen::~Screen()
{
    //dtor
}
