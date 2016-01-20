#include "MemoryMap.h"
#include <iostream>
#include <cstdio>

using namespace std;
struct _6510_cpu;
namespace Service
{
MemoryMap::MemoryMap(MemoryManager* memManager,struct _6510_cpu *cpuPtr)
{
    this->cpuPtr = cpuPtr;
    this->memManager = memManager;

     if(!SDL_WasInit(SDL_INIT_VIDEO))
        SDL_Init(SDL_INIT_VIDEO);
    if(!TTF_WasInit())
        TTF_Init();


    this->windowX = 800;
    this->windowY = 600;
    window = SDL_CreateWindow(
        "C64 Memory Manager",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        this->windowX ,
        this->windowY,
        SDL_WINDOW_OPENGL
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE|SDL_RENDERER_TARGETTEXTURE);

    gFont = TTF_OpenFont("FreeMono.ttf",20);
    if(gFont == nullptr)
    {
        std::cout <<"font error"<< TTF_GetError()<< std::endl;
        return;
    }
    /* set properties of UI Elements (e.g. Goto; PC display and so on*/
    uiElementPos[SelectableElement::gotoAddress].x = 10;
    uiElementPos[SelectableElement::gotoAddress].y = 50;
    uiElementPos[SelectableElement::gotoAddress].height = 20;
    uiElementPos[SelectableElement::gotoAddress].width = 170;
    uiElementPos[SelectableElement::gotoAddress].maxTextInput = 4;

    uiElementPos[SelectableElement::pc].x = 10;
    uiElementPos[SelectableElement::pc].y = 150;
    uiElementPos[SelectableElement::pc].height = 20;
    uiElementPos[SelectableElement::pc].width = 170;
    uiElementPos[SelectableElement::pc].maxTextInput = 4;

    uiElementPos[SelectableElement::areg].x = 10;
    uiElementPos[SelectableElement::areg].y = 250;
    uiElementPos[SelectableElement::areg].height = 20;
    uiElementPos[SelectableElement::areg].width = 110;
    uiElementPos[SelectableElement::areg].maxTextInput = 2;

    uiElementPos[SelectableElement::xreg].x = 10;
    uiElementPos[SelectableElement::xreg].y = 350;
    uiElementPos[SelectableElement::xreg].height = 20;
    uiElementPos[SelectableElement::xreg].width = 110;
    uiElementPos[SelectableElement::xreg].maxTextInput = 2;

    uiElementPos[SelectableElement::yreg].x = 10;
    uiElementPos[SelectableElement::yreg].y = 450;
    uiElementPos[SelectableElement::yreg].height = 20;
    uiElementPos[SelectableElement::yreg].width = 110;
    uiElementPos[SelectableElement::yreg].maxTextInput = 2;


    selectedUIElement = SelectableElement::none;
}
/**
 * renders a memory cell
 */
void MemoryMap::drawMemoryCell(int address,int value,int x, int y)
{

    SDL_Color color;
    color.b = 255;
    color.r = 255;
    color.a = 255;
    color.g = 255;


    std::string textAddress = this->intToHexString(address,4);
    /* only displav value if there are no given textinput*/
    std::string textValue = this->selectedElement.address == address&&textInput.length()>0?"":this->intToHexString(value,2);

    SDL_Surface* textSurfaceAddress = TTF_RenderText_Blended( gFont, (hexStringToUpper(textAddress)).c_str(),  color);
    SDL_Texture* mTextureAddress = SDL_CreateTextureFromSurface( renderer, textSurfaceAddress );
    SDL_Surface* textSurfaceValue = TTF_RenderText_Blended( gFont, hexStringToUpper(textValue).c_str(),  color);
    SDL_Texture* mTextureValue = SDL_CreateTextureFromSurface( renderer, textSurfaceValue );


    SDL_Rect addressRect;
    addressRect.x = x+5*this->currentZoom;
    addressRect.y = y+10*this->currentZoom;
    addressRect.w = 10*textAddress.length()*this->currentZoom;
    addressRect.h = 20*this->currentZoom;

    SDL_Rect boxRect;
    boxRect.x = x+5*this->currentZoom;
    boxRect.y = y+10*this->currentZoom;
    boxRect.w = sizeOfRect*this->currentZoom;
    boxRect.h = sizeOfRect*this->currentZoom;

   SDL_Rect valueRect;
    valueRect.x = x+5*this->currentZoom;
    valueRect.y = y+30*this->currentZoom;
    valueRect.w = 10*textValue.length()*this->currentZoom;
    valueRect.h = 20*this->currentZoom;



    SDL_SetRenderDrawColor(renderer,255,255,0,255);
    SDL_RenderCopy(renderer, mTextureAddress, nullptr, &addressRect);

    SDL_RenderCopy(renderer, mTextureValue, nullptr, &valueRect);

    if(address == selectedElement.address && selectedUIElement == SelectableElement::cell)
    {
        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        SDL_RenderDrawRect(renderer,&boxRect);
        /*simulate a border of 2 pixels
         */
        boxRect.x--;
        boxRect.y--;
        SDL_RenderDrawRect(renderer,&boxRect);
        SDL_SetRenderDrawColor(renderer,255,255,0,255);
    }
    else
        SDL_RenderDrawRect(renderer,&boxRect);

    SDL_FreeSurface(textSurfaceAddress);
    SDL_FreeSurface(textSurfaceValue);
    SDL_DestroyTexture(mTextureAddress);
    SDL_DestroyTexture(mTextureValue);
}
/**
 * draws a specific memoryBlock from the shared memory
 */
void MemoryMap::drawMemoryMap(int startAddress)
{


    this->firstDrawedAddress = startAddress;

    int count =0;

    for(int x=0,y=0; (y+1)*sizeOfRect*this->currentZoom< this->windowY;x++)
    {
        if(startAddress+count >0xFFFF)
        {
            count++;
            break;
        }
        if((x+1)*sizeOfRect*this->currentZoom >= this->windowX-marginLeft)
        {
            x=-1;
            y++;
            continue;
        }
        int value = this->memManager->getValue(startAddress+count);
        drawMemoryCell(startAddress+count,value,marginLeft+x*sizeOfRect*this->currentZoom,y*sizeOfRect*this->currentZoom);
        count++;
    }
    this->lastDrawedAddress = startAddress+count-1;
}
/**
 * Update the window
 */
void MemoryMap::updateRenderer()
{
    this->render(this->currentAddress);
}
MemoryMap::~MemoryMap()
{
  TTF_CloseFont(gFont);
  TTF_Quit();
  SDL_Quit();
}
/**
 * Init Drawing with a given startadress and a given zoom Level
 */
void MemoryMap::draw(int startAddress,float zoom)
{


    selectedElement.address =0;
    selectedElement.x =0;
    selectedElement.y =0;

    this->currentZoom = zoom;

    this->currentAddress = startAddress;
    this->startAddress = startAddress;

    this->render(startAddress);
}
/**
 * Dispatch a given SDL_Event
 */
void MemoryMap::eventHandler(SDL_Event event)
{
    switch(event.type)
    {

        case SDL_KEYDOWN:
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    break;
                case SDLK_F1:
                    this->currentZoom/=1.1;
                    break;
                case SDLK_F2:
                    this->currentZoom*=1.1;
                    break;
                case SDLK_RIGHT:
                    if(this->selectedElement.address == this->lastDrawedAddress)
                        break;

                    this->selectedElement.address++;
                    if(this->selectedElement.x+1 == blocksPerColumn)
                    {
                        this->selectedElement.x=0;
                        this->selectedElement.y++;
                    }
                    else
                        this->selectedElement.x++;

                    break;
                case SDLK_LEFT:
                    if(this->selectedElement.address == this->firstDrawedAddress)
                        break;

                    this->selectedElement.address--;
                    if(this->selectedElement.x == 0)
                    {
                        this->selectedElement.x=blocksPerColumn-1;
                        this->selectedElement.y--;
                    }
                    else
                        this->selectedElement.x--;
                    break;
                case SDLK_UP:
                    if(this->selectedElement.y ==0)
                        break;
                    this->selectedElement.address-=blocksPerRow;
                    this->selectedElement.y--;
                    break;
                case SDLK_DOWN:
                    if(this->selectedElement.y+1 == blocksPerRow)
                        break;

                    this->selectedElement.address+=blocksPerRow;
                    this->selectedElement.y++;
                    break;
                case SDLK_BACKSPACE:
                    if(textInput.length()>0)
                        textInput.pop_back();
                    break;
                case SDLK_RETURN:
                {
                    if(!textInput.length())
                        break;
                    int input = std::stoul(textInput, nullptr, 16);//HexString to int convert
                    switch(selectedUIElement)
                    {
                        case SelectableElement::cell:
                        this->memManager->assignValue(input,selectedElement.address);
                        break;

                        case SelectableElement::gotoAddress:
                        {
                            this->currentAddress = input;
                            break;
                        }
                        case SelectableElement::areg:
                        {
                            unsigned char buffer[9];
                            unsigned char areg = input;
                            memManager->convertIntoMemoryValue(areg,buffer);
                            memcpy(cpuPtr->rega,buffer,9);
                            break;
                        }
                        case SelectableElement::xreg:
                        {
                            unsigned char buffer[9];
                            unsigned char xreg = input;
                            memManager->convertIntoMemoryValue(xreg,buffer);
                            memcpy(cpuPtr->regx,buffer,9);
                            break;
                        }
                        case SelectableElement::pc:
                        {
                            unsigned char buffer[9];
                            unsigned char pcl = (input&0xff);
                            unsigned char pch = ((input>>8)&0xff);
                            memManager->convertIntoMemoryValue(pcl,buffer);
                            memcpy(cpuPtr->pcl,buffer,9);
                            memManager->convertIntoMemoryValue(pch,buffer);
                            memcpy(cpuPtr->pch,buffer,9);
                            break;
                        }
                        case SelectableElement::yreg:
                        {
                            unsigned char buffer[9];
                            unsigned char yreg = input;
                            memManager->convertIntoMemoryValue(yreg,buffer);
                            memcpy(cpuPtr->regy,buffer,9);
                            break;
                        }
                        case SelectableElement::none:
                        break;
                    }
                    textInput ="";
                    break;
                }
                case SDLK_0:
                case SDLK_1:
                case SDLK_2:
                case SDLK_3:
                case SDLK_4:
                case SDLK_5:
                case SDLK_6:
                case SDLK_7:
                case SDLK_8:
                case SDLK_9:
                case SDLK_a:
                case SDLK_b:
                case SDLK_c:
                case SDLK_d:
                case SDLK_e:
                case SDLK_f:
                    if(selectedUIElement == SelectableElement::none)
                        break;

                    if((selectedUIElement == SelectableElement::cell && textInput.length() <=1) ||
                    (selectedUIElement != SelectableElement::cell && textInput.length() <= (unsigned int)uiElementPos[selectedUIElement].maxTextInput-1))
                        textInput += toupper(event.key.keysym.sym);
                    break;
            }
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            if(event.wheel.y<=0)
            {
                currentAddress = this->lastDrawedAddress+1>0XFFFF?0XFFFF:this->lastDrawedAddress+1;
            }
            else
            {
                currentAddress = firstDrawedAddress -(this->lastDrawedAddress-firstDrawedAddress)-1;
                currentAddress = 0> currentAddress?0:currentAddress;
            }
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            //check all UI Elements
            for(int i = 0; i < 5;i++)
            {
                if(event.motion.y >uiElementPos[i].y && event.motion.y < uiElementPos[i].y+uiElementPos[i].height)
                {
                    if(event.motion.x >uiElementPos[i].x && event.motion.x < uiElementPos[i].x+uiElementPos[i].width)
                    {
                        selectedUIElement = (SelectableElement)i;
                        break;
                    }
                }

            }

            /* check if memory cell is selected*/
            int columnWidth = this->sizeOfRect*this->currentZoom;
            int rowWith =  this->sizeOfRect*this->currentZoom;
            int selectedColumn = -1;
            int selectedRow = -1;
            for(int i =0; i < blocksPerColumn;i++)
            {
                if(i*columnWidth+marginLeft <= (event.motion.x) && (i+1)*columnWidth+marginLeft>= (event.motion.x))
                    selectedColumn = i;
            }
            for(int i =0; i < blocksPerRow;i++)
            {
                if(i*rowWith <= event.motion.y && (i+1)*rowWith>= event.motion.y)
                    selectedRow = i;
            }
            if(selectedColumn != -1 && selectedRow != -1)
            {
                selectedUIElement = SelectableElement::cell;
                this->selectedElement.address  = currentAddress+(blocksPerColumn)*(selectedRow)+selectedColumn;
                this->selectedElement.x = selectedColumn;
                this->selectedElement.y = selectedRow;
            }
            break;
        }
        case SDL_QUIT:
            break;
    }
    this->render(currentAddress);

}
void MemoryMap::renderText(string text,int size,int x, int y,SDL_Color color)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended( gFont, text.c_str(),  color);
    SDL_Texture* mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );

    SDL_Rect textRect;
    textRect.x = x;
    textRect.y = y;
    textRect.w = size*text.length()*this->currentZoom;
    textRect.h = size*this->currentZoom;

    SDL_RenderCopy(renderer, mTexture, nullptr, &textRect);
    SDL_FreeSurface(textSurface);

}
/**
 * Renders the entire Screen
 */
void MemoryMap::render(int startAddress)
{
    this->blocksPerColumn = (this->windowX-marginLeft)/(this->sizeOfRect*this->currentZoom);
    this->blocksPerRow    =  this->windowY/(this->sizeOfRect*this->currentZoom);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(this->renderer);
    this->drawMemoryMap(startAddress);
    int x = this->selectedElement.x*(sizeOfRect*this->currentZoom)+(this->sizeOfRect*currentZoom/3.0f);
    int y = this->selectedElement.y*(sizeOfRect*this->currentZoom)+(this->sizeOfRect*currentZoom/2.0f);

    SDL_Color color;
    color.b = 0;
    color.r = 255;
    color.a = 255;
    color.g = 0;

    if(selectedUIElement == SelectableElement::cell)
    {
        this->renderText(textInput,20,x+marginLeft,y,color);
    }
    else
    {
        int xPos = (uiElementPos[selectedUIElement].x + uiElementPos[selectedUIElement].x +uiElementPos[selectedUIElement].width)/2;
        int yPos = (uiElementPos[selectedUIElement].y);
        this->renderText(textInput,20,xPos,yPos,color);
    }
    this->renderStaticText();
    SDL_RenderPresent(renderer);
}
string MemoryMap::hexStringToUpper(string hexString)
{
    for(unsigned int i = 0; i < hexString.length(); i++)
        hexString[i] = toupper(hexString[i]);
    return hexString;
}
/**
 * renders Static content and it's Values
 * (e.g. PC, GOTO, X and Y Registers)
 */
void MemoryMap::renderStaticText()
{
    SDL_Color color;
    color.b = 255;
    color.r = 255;
    color.a = 255;
    color.g = 255;

    SDL_Color selectColor;
    selectColor.b = 0;
    selectColor.r = 0;
    selectColor.a = 255;
    selectColor.g = 255;


    int pc = memManager->convertMemoryValue((unsigned char*)cpuPtr->pcl) | (memManager->convertMemoryValue((unsigned char*)cpuPtr->pch)<<8);
    int xReg = memManager->convertMemoryValue((unsigned char*)cpuPtr->regx);
    int yReg = memManager->convertMemoryValue((unsigned char*)cpuPtr->regy);
    int aReg = memManager->convertMemoryValue((unsigned char*)cpuPtr->rega);

    /*
     * hide the actuval value if the element is Selected and text for changing is already inserted*/
    string gotoValue = selectedUIElement == SelectableElement::gotoAddress&&this->textInput.length()>0?"":"____";
    string pcValue  = selectedUIElement == SelectableElement::pc&&this->textInput.length()>0?"":this->intToHexString(pc,4);
    string regaValue = selectedUIElement == SelectableElement::areg&&this->textInput.length()>0?"":this->intToHexString(aReg,2);
    string regxValue = selectedUIElement == SelectableElement::xreg&&this->textInput.length()>0?"":this->intToHexString(xReg,2);
    string regyValue = selectedUIElement == SelectableElement::yreg&&this->textInput.length()>0?"":this->intToHexString(yReg,2);

    renderText("Goto:"+gotoValue,15,
            uiElementPos[SelectableElement::gotoAddress].x,
            uiElementPos[SelectableElement::gotoAddress].y,
            selectedUIElement == SelectableElement::gotoAddress?selectColor:color);

    renderText("PC: "+pcValue,15,
            uiElementPos[SelectableElement::pc].x,
            uiElementPos[SelectableElement::pc].y,
            selectedUIElement == SelectableElement::pc?selectColor:color);

    renderText("A: "+regaValue,15,
            uiElementPos[SelectableElement::areg].x,
            uiElementPos[SelectableElement::areg].y,
            selectedUIElement == SelectableElement::areg?selectColor:color);

    renderText("X: "+regxValue,15,
            uiElementPos[SelectableElement::xreg].x,
            uiElementPos[SelectableElement::xreg].y,
            selectedUIElement == SelectableElement::xreg?selectColor:color);

    renderText("Y: "+regyValue,15,
            uiElementPos[SelectableElement::yreg].x,
            uiElementPos[SelectableElement::yreg].y,
            selectedUIElement == SelectableElement::yreg?selectColor:color);
}
/**
 * Helper Method to Convert integer Values to a Hex String
 */
string MemoryMap::intToHexString(int value,int fillWith)
{
    std::stringstream stream;
    stream
         << std::setfill ('0') << std::setw(fillWith)
         << std::hex << value;

    string hexString = stream.str();
    stream.clear();
    return hexString;
}
};
