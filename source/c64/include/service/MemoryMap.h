#ifndef MEMORYMAP_H
#define MEMORYMAP_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iomanip>
#include <sstream>
#include "MemoryManager.h"
#include "C64Component.h"
#include <string>


struct _6510_cpu;
namespace Service
{

class MemoryManager;
/**
 * Represents a Memory Cell
 */
struct MemoryElement
{
    int x;
    int y;
    int address;
};
/**
 * Represents an UI-Element and it's position
 * e.g. Goto button, PC, AC Registers ect
 */
struct UIElementPosition
{
    int x;
    int y;
    int height;
    int width;
    int maxTextInput;
};
enum SelectableElement {gotoAddress,pc,areg,xreg,yreg,cell,none};
class MemoryMap
{
    public:
        MemoryMap(MemoryManager* memManager,struct ::_6510_cpu* cpuPtr);
        virtual ~MemoryMap();
        void draw(int startAddress,float =1);
        void eventHandler(SDL_Event event);
        void updateRenderer();

    protected:
        SDL_Window* window;
        SDL_Renderer* renderer;
        TTF_Font *gFont = NULL;
        int windowX = 768;
        int windowY = 1024;
        int marginLeft = 200;
        const int sizeOfRect = 70;
        void drawMemoryCell(int address,int value,int x, int y);
        void drawMemoryMap(int startAddress);
        void renderText(std::string text,int size, int x, int y,SDL_Color color);
        void renderStaticText();
        void render(int startAddress);
        std::string hexStringToUpper(std::string hexString);
        std::string intToHexString(int value,int fillWith);
        MemoryManager* memManager;
        float currentZoom;
        int currentAddress;
        int startAddress;
        int firstDrawedAddress;
        int lastDrawedAddress;
        std::string textInput ="";
        int blocksPerColumn =0;
        int blocksPerRow = 0;
        UIElementPosition uiElementPos[5];
        MemoryElement selectedElement;
        struct _6510_cpu* cpuPtr;
        SelectableElement selectedUIElement;
    private:
};
};
#endif // MEMORYMAP_H
