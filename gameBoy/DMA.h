#pragma once
#include "BUS.h"
class DMA
{
private:
	BUS* bus;
	void connectToBus(BUS* bus);
	void transfer();
};

