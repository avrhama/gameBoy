#pragma once
#include "BUS.h"
#include<cstdint>
class GPU
{
private:
	BUS* bus;
public:
	//videoRam
	uint8_t vRam[0x2000];
	uint8_t* oam;//object attribute memory
	uint8_t mode = 0;
	uint8_t modeclock = 0;
	uint8_t line = 0;
	void connectToBus(BUS* bus);
};

