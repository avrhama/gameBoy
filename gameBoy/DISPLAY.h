#pragma once
#include <Windows.h>
#include<opencv2/opencv.hpp>
#include<map>
#include "Structures.h"
#include <SDL.h>
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
	//Scalar black= Scalar(0, 0, 0,0);
	
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
	int cyclesInGpuFrame = 69905;
	int cyclesInGpuFrameCounter = 0;
	bool displayLock = false;
	SDL_Window* win;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	Uint32* pixels;
	SDL_Event windowEvent;
	Mat window;
	//Scalar colors[4] = { Scalar(0, 0, 0,0),Scalar(96, 96, 96,0),Scalar(192, 192, 192,0),Scalar(255, 255, 255,0) };
	//Scalar colors[4] = { Scalar(255, 255, 255,0),Scalar(192, 192, 192,0) ,Scalar(96, 96, 96,0),Scalar(0, 0, 0,0) };
	Scalar colors[4] = { Scalar(0xD0,0xF8,0xE0,0),Scalar(0x70, 0xC0,0x88 ,0) ,Scalar(0x56, 0x68,0x34 ,0),Scalar(0x20, 0x18,0x08 ,0) };//BGR
	Color bgPalette[4] = { Color(0xE0,0xF8,0xD0), Color(0x88,0xC0,0x70), Color(0x34,0x68,0x56), Color(0x08,0x18,0x20) };
	Color spritesPalette[4] = { Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) };
	
	uint8_t BGPaletteData[0x40];
	uint8_t OBPaletteData[0x40];
	Color BGPlatettes[8][4] = {
	{Color(255, 255, 255), Color(192, 192, 192), Color(96, 96, 96), Color(0, 0, 0)  },
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } };
	
	Color OBPlatettes[8][4] = {
	{Color(255, 255, 255), Color(192, 192, 192), Color(96, 96, 96), Color(0, 0, 0)  },
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } ,
	{Color(0xD0,0xF8,0xE0), Color(0x70, 0xC0,0x88), Color(0x56, 0x68,0x34), Color(0x20, 0x18,0x0) } };


	uint8_t rgb5BitToRbg8Bit[0xf1] = {
	0x0,0x8,0x10,0x18,0x20,0x29,0x31,0x39,0x41,0x4a,0x52,0x5a,0x62,0x6a,0x73,0x7b,
	0x83,0x8b,0x94,0x9c,0xa4,0xac,0xb4,0xbd,0xc5,0xcd,0xd5,0xde,0xe6,0xee,0xf6,0xff,
	};

	map<KEYS, KEY*>keysMapper;
	DISPLAY(int posX , int posY,int width,int height,int pixelSize,int displayNum=0);
	~DISPLAY() {
		delete pixels;
	}
	void render();
	void setPixel(int posX, int posY, Scalar color);
	uint32_t* getLineCV(uint8_t y);
	void setPixel(int x, int y, Color color);
	void drawBGLineCV(uint8_t y, uint32_t* BGLine);
	void drawBGLineSDL(uint8_t y, uint32_t* BGLine);
	uint32_t* getLineSDL(uint8_t y);
	void setPixel(int posX, int posY, uint8_t red, uint8_t blue, uint8_t green);
	void setPixelChannel(int posX, int posY, ColorChannel ch, uint8_t value);
	void update();
	void setLock(bool value);
	KEYS waitForKey();
	KEY getKey(uint8_t index);
	int updateKeys();

	void drawPixel(uint8_t x, uint8_t y, uint8_t sprite);
	void setPaletteColor(uint8_t paletteByteIndex, uint8_t value, bool isBGP);
	void tick(int cycles);
	void close(bool closeSDL = false);

};

