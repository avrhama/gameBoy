#include "DISPLAY.h"

DISPLAY::DISPLAY(int posX, int posY, int width, int height, int pixelSize)
{
	this->posX = posX;
	this->posY = posY;
	this->width = width;
	this->height = height;
	this->pixelSize = pixelSize;
	//window = Mat(height * pixelSize, width * pixelSize, CV_8UC3, Scalar::all(0));
    window = Mat(height * pixelSize, width * pixelSize, CV_8UC4, Scalar::all(0));
    
   
    for(int i=0;i<keysCount;i++)
    keysMapper[keys[i].value] = keys+i;

    
}

void DISPLAY::setPixel(int posX, int posY,Scalar color)
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
    *(window.data + offset) = color.val[3];//Alpha
    // resizeWindow("image", width, height);
   
}
void DISPLAY::setPixelChannel(int posX, int posY,ColorChannel ch,uint8_t value)
{

    int offset = posY * width * 4 + posX * 4;
    *(window.data + offset + ch) = value;
}
void DISPLAY::update()
{
	imshow("image", window);
   
	waitKey(25);
    
	//destroyAllWindows();
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
        SHORT v = GetAsyncKeyState(keys[i].value);
        if ((v & 0x8000) == 0x8000) {
            keys[i].isPressed = true;
            keyIndex = i;
        }
        else
            keys[i].isPressed = false;
    }
    return keyIndex;
}
