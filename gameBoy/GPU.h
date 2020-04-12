#pragma once
#include "BUS.h"
#include "CPU.h"
#include "MMU.h"
#include "DISPLAY.h"
#include "Structures.h"
#include<cstdint>
class GPU
{
private:
	BUS* bus=NULL;
	
public:
	
	//videoRam
	uint8_t vRam[0x4000] = {};
	uint8_t oam[0xA0] = {};//object attribute memory
	uint8_t mode = 0;
	uint8_t modeClock = 0;
	int cyclesPerScanline= 456;
	uint8_t vRamBank = 0;
	void reset();
	void connectToBus(BUS* bus);
	bool checkLCDStatus();
	void tick();
	void drawSprites();
	bool TestBit(BYTE n, int b);
	int BitGetVal(BYTE n, int b) const;
	COLOUR GetColour(BYTE colourNum, WORD address) const;
	void drawBG();
	void draw();
	uint8_t setPixel(int x, int y, uint8_t lsb, uint8_t msb,uint8_t pixelIndex);
	void drawTest();
};

