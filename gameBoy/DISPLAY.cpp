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
    if (*(window.data + offset) ==0|| *(window.data + offset+1) == 0|| *(window.data + offset+2) == 0) {
        printf("max y:%d\n", maxY);
        maxY = posY;
    }
   // *(window.data + offset) = color.val[3];//Alpha
    // resizeWindow("image", width, height);
    return;
    if (posY > maxY) {
        printf("max y:%d\n", maxY);
        maxY = posY;
    }
    else if (posY != lastY) {
        printf("y:%d\n", posY);
    }
    lastY = posY;
    if (posY > 143)
        return;
   
   
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
    return;
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

void DISPLAY::close()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

