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
    int x = 2;
    //win = SDL_CreateWindow("gameBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160 * x, 144 * x, 0);

    win = SDL_CreateWindow("gameBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160*x, 144*x, SDL_WindowFlags::SDL_WINDOW_RESIZABLE);
    if (NULL == win) {
        std::cout << "error with sdl window" << SDL_GetError() << std::endl;
       
    }

   renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
   //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 160, 144);
   
   
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
}
void DISPLAY::drawBGLineSDL(uint8_t y, uint32_t* BGLine) {
    
    int index = y * width;
    uint32_t* p = (uint32_t*)(window.data + 4 *index);
    for (int i = 0;i < width;i++) {
        pixels[y+i]= BGLine[i];
       
    }
}
uint32_t* DISPLAY::getLineSDL(uint8_t y) {
    uint32_t* line = (uint32_t*)pixels + y * width;
 /*   for (int i = 0;i < width;i++)
        line[i] = 0;*/
  //  return (uint32_t*)pixels + y*width;
    return line;
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
   /* if (displayLock)
        return;*/
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
}

void DISPLAY::tick(int cycles)
{
    
     cyclesInGpuFrameCounter -= cycles;
     if (cyclesInGpuFrameCounter <= 0) {
         cyclesInGpuFrameCounter += cyclesInGpuFrame;
         render();
     }
}

void DISPLAY::close(bool closeSDL)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    if(closeSDL)
    SDL_Quit();
  
}

