#include "WaveViewer.h"
#include<iostream>
namespace Sid
{
    WaveViewer::WaveViewer()
    {
        if(!SDL_WasInit(SDL_INIT_VIDEO));
            SDL_Init(SDL_INIT_VIDEO);

        this->windowX = 800;
        this->windowY = 768;

    }
    void WaveViewer::storeStream(Uint8* stream, int length,int streamNo)
    {
        for(int i =0; i < length;i++)
        {
            this->streams[streamNo].push_back(stream[i]);
        }
    }
    void WaveViewer::drawWaveForm()
    {
        SDL_CreateWindowAndRenderer(this->windowX, this->windowY, SDL_WINDOW_OPENGL, &window, &renderer);
        this->drawWaveForms(0);
    }
    void WaveViewer::scrollWaveForm()
    {
        bool quit = false;
        SDL_Event event;
        while(!quit)
        {
            while(SDL_PollEvent(&event) && !quit)
            {
                switch(event.type)
                {
                    case SDL_KEYDOWN:
                    {
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_RETURN:
                            case SDLK_ESCAPE:
                                quit = true;
                                break;
                            case SDLK_LEFT:
                                this->scrollLeft();
                                break;
                            case SDLK_RIGHT:
                                this->scrollRight();
                                break;
                        }
                        break;
                    }
                    case SDL_QUIT:
                        quit = true;
                        break;

                }
            }
        }
    }
    void WaveViewer::scrollLeft()
    {
        this->drawWaveForms(-this->windowX/2.5);
    }
    void WaveViewer::scrollRight()
    {
        this->drawWaveForms(this->windowX/2.5);
    }
    void WaveViewer::drawWaveForms(int bufferOffset)
    {

        std::cout << "draw wave form..."<<std::endl;
        int offsetX = 20;
        int offsetY = 20;

        SDL_SetRenderDrawColor(this->renderer,0,0,0,1);//Reset Render Color
        SDL_RenderClear(renderer);


        /*
         * avoid memory collision
         */
        if(currentBufferPosition + bufferOffset<0)
            bufferOffset=0;
        if(currentBufferPosition + (unsigned int)bufferOffset>=streams[0].size())
            bufferOffset = this->streams[0].size()-1;

        currentBufferPosition +=bufferOffset;

        /*
         * init iterators
         */
        std::vector<Uint8>::iterator it;
        it = this->streams[0].begin();
        it+= currentBufferPosition;

        std::cout<<"Stream1:"<<std::endl;
        SDL_SetRenderDrawColor(this->renderer,255,0,0,1);
        this->renderWaveForm(&this->streams[0],&it,offsetX,offsetY);

        std::cout<<"Stream2:"<<std::endl;
        SDL_SetRenderDrawColor(this->renderer,255,255,0,1);
        it = this->streams[1].begin();
        it += currentBufferPosition;
        this->renderWaveForm(&this->streams[1],&it,offsetX,offsetY+180);

        std::cout<<"Stream3"<<std::endl;
        SDL_SetRenderDrawColor(this->renderer,0,255,0,1);
        it = this->streams[2].begin();
        it += currentBufferPosition;
        this->renderWaveForm(&this->streams[2],&it,offsetX,offsetY+360);

        std::cout<<"StreamMixed:"<<std::endl;
        SDL_SetRenderDrawColor(this->renderer,0,0,255,1);
        it = this->streams[3].begin();
        it += currentBufferPosition;
        this->renderWaveForm(&this->streams[3],&it,offsetX,offsetY+540);

        std::cout<< "current Buffer Pos.: "<<currentBufferPosition<< "buffer size"<< streams[0].size() <<std::endl;

        SDL_RenderPresent(renderer);
    }
    WaveViewer::~WaveViewer()
    {
        SDL_DestroyWindow(window);
    }
    void WaveViewer::renderWaveForm(std::vector<Uint8>* streamPtr,std::vector<Uint8>::iterator*itPtr,int offsetX,int offsetY)
    {
        SDL_Rect rect;
        int rectSize = 2;
        float rectSizeY = 0.5;
        rect.h =rectSize;
        rect.w = rectSize;
        int count =0;
        for(;(*itPtr) != streamPtr->end(); ++(*itPtr))
        {
            int x = offsetX+count;
            int y = offsetY+(255*rectSizeY)-((int)**itPtr)*rectSizeY;
            count+=rectSize;
            if(x >= windowX)
                break;
            /*
             * workaround: drawPixel dosent works properly
             */
            rect.x = x;
            rect.y = y;
            SDL_RenderDrawRect(renderer,&rect);
            std::cout<< ((int)**itPtr) <<std::endl;
        }
    }

}
