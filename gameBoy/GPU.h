#pragma once
#include "BUS.h"
#include "CPU.h"
#include "MMU.h"
#include "DISPLAY.h"
#include<cstdint>
class GPU
{
private:
	BUS* bus;
public:
	//videoRam
	uint8_t vRam[0x2000];
	uint8_t oam[0xa0];//object attribute memory
	uint8_t mode = 0;
	uint8_t modeClock = 0;
	uint8_t line = 0;
	void reset();
	void connectToBus(BUS* bus);
	void tick();
	void drawTest();
};

