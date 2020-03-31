#pragma once
#include "BUS.h"
#include "MMU.h"
#include <Windows.h>
class DMA
{
private:
	BUS* bus;
public:
	void connectToBus(BUS* bus);
	void transfer(uint8_t sourceAddress);
};

