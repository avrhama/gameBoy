#include "DMA.h"

void DMA::connectToBus(BUS* bus)
{
	this->bus = bus;
}

void DMA::transfer(uint8_t sourceAddress)
{
	bus->mmu->write(0xff46, sourceAddress);
	for(int i=0;i<160;i++)
		bus->mmu->write(0xfe00+i, sourceAddress+i);
		
	//Sleep(160);
}
