#include "MMU.h"
#include <memory.h>
#include "pipeChannel.h"
void MMU::reset()
{
	zeroRam[0x7F] = 0x00;// IE
}

MMU::MMU()
{
	reset();
}

void MMU::connectToBus(BUS* bus)
{
	this->bus = bus;
}
void MMU::write(uint16_t address, uint8_t value)
{
	if (bus->dma->transfering)
		if (address < 0xff80)
			return;
	if (bus->pipeEnable&&false) {
		bus->p->read();
		uint16_t pipeAddress = bus->p->rBuffer[0] << 8 | bus->p->rBuffer[1];
		uint8_t pipeValue = bus->p->rBuffer[2];
		bus->p->rBuffer[0] = 0;
		if (pipeAddress != address)
			bus->p->rBuffer[0] = 1;
		else if (pipeValue != value)
			bus->p->rBuffer[0] = 2;
		else
			printf("write mem OK!");
		bus->p->wBuffer[0] = 'a';
		bus->p->write(1);
	}
		
	if (0 <= address &&address <= 0x7fff) {//bios ROM0 and ROM1 (unbanked) (16k)
		if (biosLoaded)
			return;
		bus->cartridge->write(address,value);
		//rom[address] = value;
	}
	else if (0x8000 <= address &&address <= 0x9fff) {// Graphics: VRAM (8k)
		if (vRamLock)
			return;
		bus->gpu->vRam[address & 0x1FFF] = value; // GPU._vram[addr & 0x1FFF];
		if (value != 1)
			int d = 0;
	}
	else if (0xa000 <= address &&address <= 0xbfff) {// External RAM (8k)
		//externalRam[address & 0x1FFF] = value;
		bus->cartridge->write(address, value);
	}
	else if (0xc000 <= address &&address <= 0xdfff) {// Working RAM (8k)
		workingRam[address & 0x1FFF] = value;
		workingRam[(address | 0x2000) & 0x1FFF] = value;//echo to wram bank 1
	}
	else if (0xe000 <= address &&address <= 0xfdff) {// Working RAM (8k)
		workingRam[address & 0x1FFF] = value;
		workingRam[(address & 0xD000) & 0x1FFF] = value;//echo to wram bank 0
	}
	else if (0xfe00 <= address &&address <= 0xfe9f) {
		if (OAMLock)
			return;
		bus->gpu->oam[address - 0xfe00] = value;
	}
	else if (0xff00 <= address &&address <= 0xff7f) {//I/O
		bus->interrupt->write(address & 0xFF, value);
		if (address == 0xff46) {
			int test = 0;
		}
	}
	else if (0xff80 <= address &&address <= 0xfffe) {//Zero-page RAM
		zeroRam[address - 0xff80] = value;
	}else if (address == 0xffff) {
		bus->interrupt->InterruptEnabledRegister = value;
	}
	else {
		int notfound=3;
	}
	
}
uint8_t MMU::read(uint16_t address)
{
	if (0<=address &&address <= 0x7fff) {//bios ROM0 and ROM1 (unbanked) (16k)
		if (biosLoaded) {
			if (address < 0x0100)
				//return bios[address];
				return bus->cartridge->read(address);
			else if (bus->cpu->PC == 0x100)
				biosLoaded = false;
		}
		return bus->cartridge->read(address);
		//return rom[address];
	}
	else if (0x8000<=address &&address <= 0x9fff) {// Graphics: VRAM (8k)
		if (vRamLock)
			return 0xff;
		return bus->gpu->vRam[address & 0x1FFF]; // GPU._vram[addr & 0x1FFF];
	}
	else if (0xa000<=address&&address<=0xbfff) {// External RAM (8k)
		//return externalRam[address & 0x1FFF];
		return bus->cartridge->read(address);
	}
	else if (0xc000 <= address &&address <= 0xfdff) {// Working RAM (8k)
		return workingRam[address & 0x1FFF];
	}
	else if (0xfe00 <= address &&address <= 0xfe9f) {
		if (OAMLock)
			return 0xff;
		return bus->gpu->oam[address - 0xfe00];
	}else if (0xff00 <= address &&address <= 0xff7f) {//I/O
		return bus->interrupt->read(address-0xff00);
	}else if (0xff80 <= address &&address <= 0xfffe) {//Zero-page RAM
		return zeroRam[address - 0xff80];
	}
	else if (address == 0xffff) {
		return bus->interrupt->InterruptEnabledRegister;
	}
	return NULL;
}
//uint8_t* MMU::getMemCell(uint16_t address)
//{
//	if (0 <= address && address <= 0x7fff) {//bios ROM0 and ROM1 (unbanked) (16k)
//		if (biosLoaded) {
//			if (address < 0x0100)
//				return bios+address;
//			else if (bus->cpu->PC == 0x100)
//				biosLoaded = false;
//		}
//		return bus->cartridge->getMemCell(address);
//		//return rom+address;
//	}
//	else if (0x8000 <= address && address <= 0x9fff) {// Graphics: VRAM (8k)
//		return bus->gpu->vRam+(address & 0x1FFF); // GPU._vram[addr & 0x1FFF];
//	}
//	else if (0xa000 <= address && address <= 0xbfff) {// External RAM (8k)
//		//return externalRam+(address & 0x1FFF);
//		return bus->cartridge->getMemCell(address);
//	}
//	else if (0xc000 <= address && address <= 0xfdff) {// Working RAM (8k)
//		return workingRam+(address & 0x1FFF);
//	}
//	else if (0xfe00 <= address && address <= 0xfe9f) {
//		return bus->gpu->oam+(address & 0xFF);
//	}
//	else if (0xff00 <= address && address <= 0xff7f) {//I/O
//		return bus->interrupt->getMemCell(address & 0xFF);
//	}
//	else if (0xff80 <= address && address <= 0xffff) {//Zero-page RAM
//		return zeroRam+(address & 0xFF);
//	}
//	return NULL;
//}



