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
	int maxY=-1;
	int lastY = -1;
	int displayNum;
	Scalar white = Scalar(255, 255, 255,0);
	Scalar black= Scalar(0, 0, 0,0);
	bool displayLock = false;
	char const   keysCount = 8;
	KEY keys[8] = {
		KEY{KEYS::A,false},
		KEY{KEYS::Right,false},
		KEY{KEYS::B,false},
		KEY{KEYS::Left,false},
		KEY{KEYS::Space,false},
		KEY{KEYS::Up,false},
		KEY{KEYS::Enter,false},
		KEY{KEYS::Down,false},
	};
	
	
public:
	Mat window;
	//Scalar colors[4] = { Scalar(0, 0, 0,0),Scalar(96, 96, 96,0),Scalar(192, 192, 192,0),Scalar(255, 255, 255,0) };
	Scalar colors[4] = { Scalar(255, 255, 255,0),Scalar(192, 192, 192,0) ,Scalar(96, 96, 96,0),Scalar(0, 0, 0,0) };
	map<KEYS, KEY*>keysMapper;
	DISPLAY(int posX , int posY,int width,int height,int pixelSize,int displayNum=0);
	void setPixel(int posX, int posY, Scalar color);
	void setPixel(int posX, int posY, uint8_t red, uint8_t blue, uint8_t green);
	void setPixelChannel(int posX, int posY, ColorChannel ch, uint8_t value);
	void update();
	void setLock(bool value);
	KEYS waitForKey();
	KEY getKey(uint8_t index);
	int updateKeys();

};

