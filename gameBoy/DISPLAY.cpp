#include "DISPLAY.h"

DISPLAY::DISPLAY(int posX, int posY, int width, int height, int pixelSize,int displayNum)
{
	this->posX = posX;
	this->posY = posY;
	this->width = width;
	this->height = height;
	this->pixelSize = pixelSize;
    this->displayNum = displayNum;
	//window = Mat(height * pixelSize, width * pixelSize, CV_8UC3, Scalar::all(0));
    window = Mat(height * pixelSize, width * pixelSize, CV_8UC4, Scalar::all(0));
    
   
    for(int i=0;i<keysCount;i++)
    keysMapper[keys[i].key] = keys+i;
   
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "error with sdl " << SDL_GetError() << std::endl;
    }

    win = SDL_CreateWindow("gameBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144, 0);
    if (NULL == win) {
        std::cout << "error with sdl window" << SDL_GetError() << std::endl;
       
    }

   renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  
    texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 160, 144);
   
    //SDL_Texture* bitmapTex = NULL;
    //SDL_Surface* bitmapSurface = NULL;


   
    pixels = new Uint32[width * height * pixelSize]; 
    memset(pixels, 255, width * height * pixelSize* sizeof(Uint32));
    memset(BGPaletteData, 0xff, 0x40);
    memset(OBPaletteData, 0xff, 0x40);

    /*for (uint8_t paletteIndex = 0;paletteIndex < 8;paletteIndex++) {
        BGPlatettes[paletteIndex][0].setRGB(0xff,0xff,0xff);
        BGPlatettes[paletteIndex][1].setRGB(0xff, 0xff, 0xff);
        BGPlatettes[paletteIndex][2].setRGB(0xff, 0xff, 0xff);
        BGPlatettes[paletteIndex][3].setRGB(0xff, 0xff, 0xff);
    }
    for (uint8_t paletteIndex = 0;paletteIndex < 8;paletteIndex++) {
        OBPlatettes[paletteIndex][0].setRGB(0xff, 0xff, 0xff);
        OBPlatettes[paletteIndex][1].setRGB(0xff, 0xff, 0xff);
        OBPlatettes[paletteIndex][2].setRGB(0xff, 0xff, 0xff);
        OBPlatettes[paletteIndex][3].setRGB(0xff, 0xff, 0xff);
    }*/
    //SDL_RenderClear(renderer);
   

    
    /*while (true) {
        if (SDL_PollEvent(windowEvent)) {
            if (SDL_QUIT == windowEvent->type) {
                break;
            }
        }

    }*/
   
    
}

void DISPLAY::setPixel(int posX, int posY, Scalar color)
{
    /*Rect RectangleToDraw(posX, posY, pixelSize, pixelSize);
    rectangle(window, RectangleToDraw.tl(), RectangleToDraw.br(), color, FILLED, 8, 0);
    uchar* data = window.data;
    int y = 0;
    Rect RectangleToDraw(posX, posY, pixelSize, pixelSize);
    rectangle(window, RectangleToDraw.tl(), RectangleToDraw.br(), color, FILLED, 8, 0);*/

    
    int offset = posY * width*4 + posX*4;
    *(window.data+offset++) = color.val[0];//Blue
    *(window.data + offset++) = color.val[1];//Green
    *(window.data + offset++) = color.val[2];//Red
    offset = posY * width * 4 + posX * 4;
   
   
   
}
uint32_t* DISPLAY::getLineCV(uint8_t y) {
   return (uint32_t*)(window.data + 4 * (int)y * width);
}
void DISPLAY::drawBGLineCV(uint8_t y, uint32_t* BGLine)
{
   
    
    uint32_t* p = (uint32_t*)(window.data  + 4*(int)y * width);
    for (int i = 0;i < width;i++) {
        //*(p + i) = bgPalette[0].getHex();
        *(p + i) = BGLine[i];
    }
   

    //for (uint8_t i = 0;i < 20;i++) {
    //    uint8_t index = 2 * i;
    //    uint8_t lsb = BGLine[index];
    //    uint8_t msb = BGLine[index + 1];
    //    uint8_t j = i * 8;
    //    for (int k = 0;k < 8;k++) {
    //        uint8_t colorPaletteDataIndex = (((msb >> (7 - k)) & 0x1) << 1) | ((lsb >> (7 - k)) & 0x1);
    //        uint8_t  colorIndex = palette[colorPaletteDataIndex];
    //        //if(colorIndex==0)
    //            //*BGcolos |= 0x1 << (7 - (((i+k) + (offset - pixelIndex)) % 8));
    //        //printf("color id:%d\n",colorIndex);
    //        Scalar color = colors[colorIndex];
    //        int x = 4*(j + k);
    //        //*(uint32_t*)(window.data + offset) = *(uint32_t*)color.val;
    //       uint32_t* p = (uint32_t*) window.data + offset + (j + k);;
    //       //*p = 0xE0F8D0;
    //        *p = bgPalette[colorIndex].getHex();
    //        //*(window.data + offset+x) = color.val[0];//Blue
    //        //*(window.data + offset+x+1) = color.val[1];//Green
    //        //*(window.data + offset+x+2) = color.val[2];//Red
    //        //bus->display->setPixel(j + k, y, bus->display->colors[colorIndex]);
    //        //bus->display->setPixel(x + (offset - pixelIndex), y, bus->display->bgPalette[colorIndex]);
    //    }
    //}

    

}
void DISPLAY::drawBGLineSDL(uint8_t y, uint32_t* BGLine) {
    
    int index = y * width;
    uint32_t* p = (uint32_t*)(window.data + 4 *index);
    for (int i = 0;i < width;i++) {
        pixels[y+i]= BGLine[i];
       
    }
}
uint32_t* DISPLAY::getLineSDL(uint8_t y) {
    return (uint32_t*)pixels + y*width;
}
void DISPLAY::setPixel(int x, int y, Color color) {
    //int index = y * width * 4 + x * 4;
    int index = y * width + x;
   // pixels[index] = 0xE0F8D0;//rgb
    pixels[index] = color.getHex();
}
void DISPLAY::render() {
    //
    //SDL_RenderCopy(renderer, texture, NULL, NULL);
    //SDL_RenderPresent(renderer);
   // SDL_RenderClear(renderer);
   
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));
   
}
void DISPLAY::setPixel(int posX, int posY, uint8_t red, uint8_t blue, uint8_t green)
{
    int offset = posY * width * 4 + posX * 4;
    *(window.data + offset++) = blue;//Blue
    *(window.data + offset++) = green;//Green
    *(window.data + offset++) = red;//Red
    *(window.data + offset) = 255;//Alpha
}
void DISPLAY::setPixelChannel(int posX, int posY,ColorChannel ch,uint8_t value)
{

    int offset = posY * width * 4 + posX * 4;
    *(window.data + offset + ch) = value;
   
}
void DISPLAY::update()
{
   
   /* if (displayLock)
        return;*/
    string windowName = "image" + displayNum;
	imshow(windowName, window);
   
	//waitKey(25);
     waitKey(1);
	//destroyAllWindows();
}
void DISPLAY::setLock(bool value) {
    displayLock = value;
}
KEYS DISPLAY::waitForKey()
{
    int keyIndex = -1;
    while ((keyIndex = updateKeys()) < 0)
        Sleep(1);
    return (KEYS)keyIndex;
}

KEY DISPLAY::getKey(uint8_t index)
{
    return keys[index];
}
//return true if any key was pressed
int DISPLAY::updateKeys()
{
    //SHORT 16 bits
    int keyIndex = -1;
    for (int i = 0;i < keysCount;i++) {
        SHORT v = GetAsyncKeyState((int)keys[i].key);
        if ((v & 0x8000) == 0x8000) {
            keys[i].isPressed = true;
            keyIndex = i;
        }
        else
            keys[i].isPressed = false;
    }
    return keyIndex;
}

void DISPLAY::setPaletteColor(uint8_t paletteByteIndex, uint8_t value, bool isBGP)
{
   
    uint8_t* palatteData;
    Color * palette;
    uint8_t colorIndex = paletteByteIndex - paletteByteIndex % 2;
   // colorIndex = (paletteByteIndex / 2) % 4;
    if (isBGP) {
        palatteData = BGPaletteData;
       // palette = BGPlatettes[colorIndex/8]+colorIndex%8;
        if (paletteByteIndex > 15)
            int i = 0;
        int h = 10/2;
        h = h % 4;
        palette = BGPlatettes[paletteByteIndex / 8] + (paletteByteIndex / 2) % 4;

       /* if (paletteByteIndex < 8) {
            palette = BGPlatettes[0];
        }else if (paletteByteIndex < 16) {
            palette = BGPlatettes[1];
        }
        else if (paletteByteIndex < 24) {
            palette = BGPlatettes[2];
        }
        else if (paletteByteIndex < 32) {
            palette = BGPlatettes[3];
        }
        else if (paletteByteIndex < 40) {
            palette = BGPlatettes[4];
        }
        else if (paletteByteIndex < 48) {
            palette = BGPlatettes[5];
        }
        else if (paletteByteIndex < 56) {
            palette = BGPlatettes[6];
        }
        else {
            palette = BGPlatettes[7];
        }
        palette+=(paletteByteIndex / 2) % 4;*/
    }
    else {
        palatteData = OBPaletteData;
        palette = OBPlatettes[paletteByteIndex / 8] + (paletteByteIndex / 2) % 4;
    }
    palatteData[paletteByteIndex] = value;
   
    uint8_t red = palatteData[colorIndex] & 0x1f;
   // red = (red * 0xff) / 0x1f;//5 bit rgb to 8 bit rgb
    uint8_t green = (palatteData[colorIndex + 1] & 0x03)<<3 | (palatteData[colorIndex] >> 5) & 0x07;
   //green = (green * 0xff) / 0x1f;//5 bit rgb to 8 bit rgb
    uint8_t blue = (palatteData[colorIndex +1] >> 2) & 0x1f;
   //blue = (blue * 0xff) / 0x1f;//5 bit rgb to 8 bit rgb
    

    red = rgb5BitToRbg8Bit[red];
    green = rgb5BitToRbg8Bit[green];
    blue = rgb5BitToRbg8Bit[blue];
    
    palette->setRGB(red,green,blue);

    return;
    uint8_t v = 0xbd;
    if (value==151){
        int y = 0;
    }
    for (uint8_t paletteIndex = 0;paletteIndex < 8;paletteIndex++) {
        if (BGPlatettes[paletteIndex][0].getR() == v || BGPlatettes[paletteIndex][1].getR() == v ||
            BGPlatettes[paletteIndex][2].getR() == v ||BGPlatettes[paletteIndex][3].getR() == v) {
            int u = 0;
        }
      
    }
 
}

void DISPLAY::tick(int cycles)
{
    
     cyclesInGpuFrameCounter -= cycles;
     if (cyclesInGpuFrameCounter <= 0) {
         cyclesInGpuFrameCounter += cyclesInGpuFrame;
         render();
     }
}

void DISPLAY::close()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

