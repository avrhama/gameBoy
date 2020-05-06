#include "DMA.h"

void DMA::connectToBus(BUS* bus)
{
	this->bus = bus;
}

void DMA::FF46DMATransferStart(uint8_t sourceAddressPrefix)
{
	FF46sourceAddress = sourceAddressPrefix << 8;
	transfer = true;
	FF46DMATransfer();
}

void DMA::FF46DMATransfer()
{
	if (!transfer)
		return;
	transfer = false;
	for (uint8_t i = 0;i < 160;i++)
		bus->gpu->oam[i] = bus->mmu->read(FF46sourceAddress + i);
	FF46transfering = false;
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
	if (bus->consoleModel != ConsoleModel::GBC) {
		hDMA5 = value;
		return;
	}
		
	if (dmaMode==DMAMode::hBlank) {
		if (!((value >> 7) & 0x01)) {
			// It is also possible to terminate an active H-Blank transfer by writing zero to Bit 7 of FF55
			hDMA5 |= 0x80;
			stopHDMATransfer();
			return;
		}
	}
	hBlankDMAStep = 0;
	transferLen = (((value & 0x7f) + 1) << 4);
	dmaMode = (DMAMode)((value >> 7) & 0x01);
	vRamBankOffset = (uint16_t)bus->gpu->vRamBank * 0x2000;
	if (dmaMode == DMAMode::General)
		generalPurposeDMA();
}
void DMA::generalPurposeDMA()
{
	
	if (dmaMode != DMAMode::General||bus->consoleModel!=ConsoleModel::GBC)
		return;
	for (uint16_t i = 0;i < transferLen;i++) {
		if (destinationAddress + i < 0x1ff0) {
			bus->gpu->vRam[destinationAddress + i + ((uint16_t)bus->gpu->vRamBank * 0x2000)] = bus->mmu->read(sourceAddress + i);
		}else {
			stopHDMATransfer();
			return;
		}
	}
	hDMA5 = 0xff;
	dmaMode = DMAMode::None;
}

void DMA::hBlankDMA()
{
	
	if (dmaMode != DMAMode::hBlank || bus->consoleModel != ConsoleModel::GBC)
		return;
	
	uint8_t count = transferLen < 0x10 ? transferLen : 0x10;
	uint16_t offset = (hBlankDMAStep << 4);
	uint16_t address= destinationAddress + offset;
	for (uint8_t i = 0;i < count;i++) {
		if (address + i < 0x1ff0) {
			bus->gpu->vRam[address+i + ((uint16_t)bus->gpu->vRamBank * 0x2000)] = bus->mmu->read(sourceAddress + i+ offset);
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
	}
	
}

void DMA::stopHDMATransfer()
{
	hBlankDMAStep = 0;
	dmaMode = DMAMode::None;
}
