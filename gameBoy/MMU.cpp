#include "MMU.h"
#include <memory.h>

void MMU::reset()
{
	io[0x05] = 0x00;// TIMA
	io[0x06] = 0x00;// TMA
	io[0x07] = 0x00;// TAC
	io[0x10] = 0x80;// NR10
	io[0x11] = 0xBF;// NR11
	io[0x12] = 0xF3;// NR12
	io[0x14] = 0xBF;// NR14
	io[0x16] = 0x3F;// NR21
	io[0x17] = 0x00;// NR22
	io[0x19] = 0xBF;// NR24
	io[0x1A] = 0x7F;// NR30
	io[0x1B] = 0xFF;// NR31
	io[0x1C] = 0x9F;// NR32
	io[0x1E] = 0xBF;// NR33
	io[0x20] = 0xFF;// NR41
	io[0x21] = 0x00;// NR42
	io[0x22] = 0x00;// NR43
	io[0x23] = 0xBF;// NR30
	io[0x24] = 0x77;// NR50
	io[0x25] = 0xF3;// NR51
	io[0x26] = 0xF1;//-GB, 0xF0-SGB ;// NR52
	io[0x40] = 0x91;// LCDC
	io[0x42] = 0x00;// SCY
	io[0x43] = 0x00;// SCX
	io[0x45] = 0x00;// LYC
	io[0x47] = 0xFC;// BGP
	io[0x48] = 0xFF;// OBP0
	io[0x49] = 0xFF;// OBP1
	io[0x4A] = 0x00;// WY
	io[0x4B] = 0x00;// WX
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
	if (0 <= address &&address <= 0x7fff) {//bios ROM0 and ROM1 (unbanked) (16k)
		if (biosLoaded)
			return;
		rom[address] = value;
	}
	else if (0x8000 <= address &&address <= 0x9fff) {// Graphics: VRAM (8k)
		bus->gpu->vRam[address & 0x1FFF] = value; // GPU._vram[addr & 0x1FFF];
		if (value != 1)
			int d = 0;
	}
	else if (0xa000 <= address &&address <= 0xbfff) {// External RAM (8k)
		externalRam[address & 0x1FFF] = value;
	}
	else if (0xc000 <= address &&address <= 0xddff) {// Working RAM (8k)
		workingRam[address & 0x1FFF] = value;
		workingRam[(address | 0x2000) & 0x1FFF] = value;//echo to wram bank 1
	}
	else if (0xe000 <= address &&address <= 0xfdff) {// Working RAM (8k)
		workingRam[address & 0x1FFF] = value;
		workingRam[(address & 0xD000) & 0x1FFF] = value;//echo to wram bank 0
	}
	else if (0xfe00 <= address &&address <= 0xfe9f) {
		bus->gpu->oam[address & 0xFF] = value;
	}
	else if (0xff00 <= address &&address <= 0xff7f) {//I/O
		io[address & 0xFF] = value;
	}
	else if (0xff80 <= address &&address <= 0xffff) {//Zero-page RAM
		zeroRam[address & 0xFF] = value;
	}
	
}
uint8_t MMU::read(uint16_t address)
{
	if (0<=address &&address <= 0x7fff) {//bios ROM0 and ROM1 (unbanked) (16k)
		if (biosLoaded) {
			if (address < 0x0100)
				return bios[address];
			else if (bus->cpu->PC == 0x100)
				biosLoaded = false;
		}
		return rom[address];
	}
	else if (0x8000<=address &&address <= 0x9fff) {// Graphics: VRAM (8k)
		return bus->gpu->vRam[address & 0x1FFF]; // GPU._vram[addr & 0x1FFF];
	}
	else if (0xa000<=address&&address<=0xbfff) {// External RAM (8k)
		return externalRam[address & 0x1FFF];
	}
	else if (0xc000 <= address &&address <= 0xfdff) {// Working RAM (8k)
		return workingRam[address & 0x1FFF];
	}
	else if (0xfe00 <= address &&address <= 0xfe9f) {
		return bus->gpu->oam[address & 0xFF];
	}else if (0xff00 <= address &&address <= 0xff7f) {//I/O
		return io[address & 0xFF];
	}else if (0xff80 <= address &&address <= 0xffff) {//Zero-page RAM
		return zeroRam[address & 0xFF];
	}
	return NULL;
}
uint8_t* MMU::getMemCell(uint16_t address)
{
	if (0 <= address && address <= 0x7fff) {//bios ROM0 and ROM1 (unbanked) (16k)
		if (biosLoaded) {
			if (address < 0x0100)
				return bios+address;
			else if (bus->cpu->PC == 0x100)
				biosLoaded = false;
		}
		return rom+address;
	}
	else if (0x8000 <= address && address <= 0x9fff) {// Graphics: VRAM (8k)
		return bus->gpu->vRam+(address & 0x1FFF); // GPU._vram[addr & 0x1FFF];
	}
	else if (0xa000 <= address && address <= 0xbfff) {// External RAM (8k)
		return externalRam+(address & 0x1FFF);
	}
	else if (0xc000 <= address && address <= 0xfdff) {// Working RAM (8k)
		return workingRam+(address & 0x1FFF);
	}
	else if (0xfe00 <= address && address <= 0xfe9f) {
		return bus->gpu->oam+(address & 0xFF);
	}
	else if (0xff00 <= address && address <= 0xff7f) {//I/O
		return io+(address & 0xFF);
	}
	else if (0xff80 <= address && address <= 0xffff) {//Zero-page RAM
		return zeroRam+(address & 0xFF);
	}
	return NULL;
}
//void MMU::write(uint16_t address, uint8_t value)
//{
//	mem[address] = value;
//	if(address>= 0xE000 && address<= 0xFE00)
//		mem[address-0x2000] = value;
//	else if(address >= 0xC000 && address <= 0xDE00)
//		mem[address + 0x2000] = value;
//}
/*void MMU::write(uint16_t address, uint8_t value)
{
	switch (address) {
	// ROM0
	case 0x0000:
		if (biosLoaded)
			return;
	case 0x1000:
	case 0x2000:
	case 0x3000:
		 break;
		// ROM1 (unbanked) (16k)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		 break;
		// Graphics: VRAM (8k)
	case 0x8000:
	case 0x9000:
		 bus->gpu->vRam[address & 0x1FFF]= value;
		// bus->gpu->.updatetile(addr & 0x1FFF, val);
		 break;
		// External RAM (8k)
	case 0xA000:
	case 0xB000:
		 externalRam[address & 0x1FFF]= value;
		 break;
		 // Working RAM shadow
	case 0xC000:
	case 0xD000:
	case 0xE000:
		 workingRam[address & 0x1FFF] = value;
		 break;
	// Everything else
	case 0xF000:
		switch (address & 0x0F00)
		{
		// Echo RAM
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			 workingRam[address & 0x1FFF]= value;
			 break;
		// Graphics: object attribute memory
		// OAM is 160 bytes
		case 0xE00:
			if ((address & 0xFF) < 0xA0)
				 bus->gpu->oam[address & 0xFF]=value;
			   // bus->gpu->.updatetile(addr & 0x1FFF, val);
			break;

			// Zero-page
		case 0xF00:
			if (address >= 0xFF80)
			{
				 zeroRam[address & 0x7F]=value;
			}
			else
			{
				// I/O control handling
				// Currently unhandled
				//return 0;
			}
			break;
		}
	}
	
}
uint8_t MMU::read(uint16_t address)
{
	switch (address) {
	case 0x0000:
		if (biosLoaded) {
			if (address < 0x0100)
				return bios[address];
			else if (bus->cpu->PC == 0x100)
				biosLoaded = false;
		}
		return rom[address];
	// ROM0
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return rom[address];
	// ROM1 (unbanked) (16k)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return rom[address];
	// Graphics: VRAM (8k)
	case 0x8000:
	case 0x9000:
		return bus->gpu->vRam[address & 0x1FFF]; // GPU._vram[addr & 0x1FFF];

		// External RAM (8k)
	case 0xA000:
	case 0xB000:
		return externalRam[address & 0x1FFF];

		// Working RAM (8k)
	case 0xC000:
	case 0xD000:
	case 0xE000:
		return workingRam[address & 0x1FFF];
		// Working RAM shadow, I/O, Zero-page RAM
	case 0xF000:
		switch (address & 0x0F00)
		{
			// Working RAM shadow
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			return workingRam[address & 0x1FFF];

			// Graphics: object attribute memory
			// OAM is 160 bytes, remaining bytes read as 0
		case 0xE00:
			if (address < 0xFEA0)
				return bus->gpu->oam[address & 0xFF];
			else
				return 0;

			// Zero-page
		case 0xF00:
			if (address >= 0xFF80)
			{
				return zeroRam[address & 0x7F];
			}
			else
			{
				// I/O control handling
				// Currently unhandled
				return 0;
			}
		}
	}
	return mem[address];
}*/


