#include "DMA.h"

void DMA::connectToBus(BUS* bus)
{
	this->bus = bus;
}

void DMA::transfer(uint8_t sourceAddressPrefix)
{
	
	transfering = true;
	//bus->interrupt->io[0x55]=
	uint16_t sourceAddress = sourceAddressPrefix << 8;
	for(uint8_t i=0;i<160;i++)
		bus->gpu->oam[i] = bus->mmu->read(sourceAddress + i);
	transfering = false;
		
	//Sleep(160);
}
