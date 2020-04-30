#include "DMA.h"

void DMA::connectToBus(BUS* bus)
{
	this->bus = bus;
}

void DMA::transfer(uint8_t sourceAddressPrefix)
{

	//FF46transfering = true;
	//bus->interrupt->io[0x55]=
	sourceAddress = sourceAddressPrefix << 8;
	for (uint8_t i = 0;i < 160;i++)
		bus->gpu->oam[i] = bus->mmu->read(sourceAddress + i);
	//FF46transfering = false;

	//Sleep(160);
}

void DMA::setSourceHi(uint8_t value)
{
	if (dmaMode == DMAMode::None)
		sourceAddress = (value << 8)|(sourceAddress & 0x00f0);
}

void DMA::setSourceLo(uint8_t value)
{
	if (dmaMode == DMAMode::None)
		sourceAddress = (sourceAddress & 0xff00) | (value & 0xf0);
}

void DMA::setDestinationHi(uint8_t value)
{
	if (dmaMode==DMAMode::None)
		destinationAddress = ((value & 0x1f) << 8)| (destinationAddress & 0x00f0);
}

void DMA::setDestinationLo(uint8_t value)
{
	if (dmaMode == DMAMode::None)
		destinationAddress = (destinationAddress & 0x1f00) | (value & 0xf0);
}

void DMA::HDMATransfer(uint8_t value)
{

	if (dmaMode==DMAMode::hBlank) {
		if (!((value >> 7) & 0x01)) {
			// It is also possible to terminate an active H-Blank transfer by writing zero to Bit 7 of FF55
			//hDMA5 |= 0x80;
			stopHDMATransfer();
			return;
		}
	}
	//transfering = true;
	//hDMA5 = 0x80 | value;
	transferLen = (((value & 0x7f) + 1) << 4);
	dmaMode = (DMAMode)((value >> 7) & 0x01);
	if (dmaMode == DMAMode::hBlank) {
		printf("mode:%d\n", bus->interrupt->io[0x41] & 0x03);
	}
}
void DMA::generalPurposeDMA()
{
	if (dmaMode != DMAMode::General)
		return;
	hDMA5 &= 0x7f;
	printf("generalPurposeDMA starts!\n");

	/*hDMA5 = 0xff;
	dmaMode = DMAMode::None;
	printf("generalPurposeDMA ended!\n");
	return;*/
	uint16_t address = 0;
	for (uint16_t i = 0;i < transferLen;i++) {
		if (destinationAddress + i < 0x1ff0) {
			//bus->gpu->vRam[destinationAddress + i] = bus->mmu->read(sourceAddress + i);
			address = destinationAddress + i;
			//bus->mmu->write(address + 0x8000, bus->mmu->read(sourceAddress + i));
			bus->gpu->vRam[address + (bus->gpu->vRamBank * 0x2000)] = bus->mmu->read(sourceAddress + i);
			//int test = destinationAddress + i;
		}else {
			stopHDMATransfer();
			//hDMA5 = 0xff;
			return;
		}
	}
	hDMA5 = 0xff;
	dmaMode = DMAMode::None;
	printf("generalPurposeDMA ended!\n");
}

void DMA::hBlankDMA()
{
	if (dmaMode != DMAMode::hBlank)
		return;
	if(hBlankDMAStep==0)
		printf("hBlankDMA starts!\n");
	//hDMA5 |= 0x80;
	uint8_t count = transferLen < 0x10 ? transferLen : 0x10;
	uint16_t offset = (hBlankDMAStep << 4);
	uint16_t address=0;
	for (uint8_t i = 0;i < count;i++) {
		if (destinationAddress + offset + i < 0x1ff0) {
			//bus->gpu->vRam[destinationAddress + offset + i] = bus->mmu->read(sourceAddress + offset + i);
			address = destinationAddress + offset + i;
			//bus->mmu->write(address + 0x8000, bus->mmu->read(sourceAddress+ offset + i));
			bus->gpu->vRam[address + (bus->gpu->vRamBank * 0x2000)] = bus->mmu->read(sourceAddress + i+ offset);

		}
		else {
			stopHDMATransfer();
			return;
		}
	}
	
	transferLen = transferLen > 0x10 ? transferLen - 0x10 : 0;
	hDMA5 = (transferLen >> 4) - 1;
	hBlankDMAStep++;
	if (transferLen == 0) {
		dmaMode = DMAMode::None;
		hDMA5 = 0xff;
		hBlankDMAStep = 0;
		printf("hBlankDMA ended!\n");
	}
	
}

void DMA::stopHDMATransfer()
{
	hBlankDMAStep = 0;
	dmaMode = DMAMode::None;
	printf("transfer stopped!\n");
}
