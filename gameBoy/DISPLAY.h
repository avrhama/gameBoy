#pragma once
#include <Windows.h>
#include<opencv2/opencv.hpp>
#include<map>
#include "Structures.h"
using namespace std;
using namespace cv;
class DISPLAY
{
private:
	int posX;
	int posY;
	int width;
	int height;
	int pixelSize;
	Mat window;
	Scalar white = Scalar(255, 255, 255,0);
	Scalar black= Scalar(0, 0, 0,0);
	char const   keysCount = 4;
	KEY keys[4] = {
				KEY{Up,false},
				KEY{Down,false},
				KEY{Left,false},
				KEY{Right,false},

	};
	
	
public:
	map<int, KEY*>keysMapper;
	DISPLAY(int posX , int posY,int width,int height,int pixelSize);
	void setPixel(int posX, int posY, Scalar color);
	void setPixelChannel(int posX, int posY, ColorChannel ch, uint8_t value);
	void update();
	KEYS waitForKey();
	KEY getKey(uint8_t index);
	int updateKeys();

};

