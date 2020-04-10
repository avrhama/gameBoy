#pragma once
#include "BUS.h"
#include "MMU.h"
#include "GPU.h"
#include <Windows.h>
class DMA
{
private:
	BUS* bus;
public:
	bool transfering = false;
	void connectToBus(BUS* bus);
	void transfer(uint8_t sourceAddress);
};

