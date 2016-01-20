#ifndef WAVEVIEWER_H
#define WAVEVIEWER_H
#include <SDL2/SDL.h>
#include <cstdio>
#include <vector>

namespace Sid
{
    /**
     * Debug Class for drawing the computed Soundoutput
     */
    class WaveViewer
    {
        public:
            WaveViewer();
            virtual ~WaveViewer();
            void drawWaveForm();
            void scrollWaveForm();
            void storeStream(Uint8* stream, int length,int streamNo);
        protected:
            SDL_Window *window;
            SDL_Renderer *renderer;
            std::vector<Uint8>streams[4];


            int windowX;
            int windowY;
            int currentBufferPosition = 0;
            void scrollLeft();
            void scrollRight();
            void drawWaveForms(int bufferOffset);
            void renderWaveForm(std::vector<Uint8>* streamPtr,std::vector<Uint8>::iterator* itPtr,int offsetX,int offsetY);

        private:
    };
}
#endif // WAVEVIEWER_H
