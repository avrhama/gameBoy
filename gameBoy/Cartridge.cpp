#include "CARTRIDGE.h"
#include <stdlib.h>
#include "SHA1.h"
void CARTRIDGE::connectToBus(BUS* bus)
{
	this->bus = bus;
}
void CARTRIDGE::loadRom(const char* path)
{
	ifstream rf(path, ios::out | ios::binary);
	if (!rf) {
		cout << "Cannot open file!" << endl;
	}
	else {
		rf.seekg(0, rf.end);
		int size = rf.tellg();
		rf.seekg(0, rf.beg);
		rom = (uint8_t*)calloc(size, 1);
		int p = 0;
		//rf.read((char*)mem, size);
		rf.read((char*)rom, size);

	}
	rf.close();
	setCartridgeHeader();
	
	//rom[0x7fff+1] = '\0';
	colorGB = rom[0x0143];
	GB_SGBIndicator = rom[0x0146];
	cartridgeType =(CartridgeType)rom[0x0147];
	romSizeType = (RomSizeType)rom[0x0148];
	ramSizeType= (RamSizeType)rom[0x0149];
	romSize = getRomSize();
	ramSize = getRomSize();
	ram = (uint8_t*)calloc(ramSize*ramBankSize, 1);
	printf("colorGB?:%d\n", colorGB);

	
}

void CARTRIDGE::setCartridgeHeader()
{
	memcpy(title, (rom + 0x0134), 9);
}



char CARTRIDGE::getRomSize()
{
	switch (romSizeType)
	{
	case _2_banks:
		return 2;
	case _4_banks:
		return 4;
	case _8_banks:
		return 8;
	case _16_banks:
		return 16;
	case _32_banks:
		return 32;
	case _64_banks:
		return 64;
	case _128_banks:
		return 128;
	case _72_banks:
		return 72;
	case _80_banks:
		return 80;
	case _96_banks:
		return 96;
	}
	return 0;
}
char CARTRIDGE::getRamSize()
{
	switch (ramSizeType)
	{
	case None:
		return 0;
	case _1_bank_2kB:// 1/4 of ram bank
		return 1;
	case _1_bank_8kB:
		return 1;
	case _4_banks_32kB:
		return 4;
	case _16_banks_128kB:
		return 16;
	case _8_banks_64kB:
		return  8;
	}
	return 0;
}

uint8_t CARTRIDGE::read(uint32_t address)
{
	if (address <= 0x3fff) {//Rom 0
		return rom[address];
	}
	else if (0x4000 <= address && address <= 0x7fff) {//Rom n
		rom[address + (romBankIndex - 1) * romBankSize];
	}
	else if (0xa000 <= address && address <= 0xbfff) {//Ram m
		ram[(address- 0xa000) + ramBankIndex * ramBankSize];
	}
}
void CARTRIDGE::setRomBankIndex(uint8_t v) {
	romBankIndex = v;
	if (romBankIndex <= 60 && romBankIndex % 20 == 0)
		romBankIndex += 1;

}
void CARTRIDGE::write(uint16_t address, uint8_t value)
{
	if (address <= 0x3fff) {//ROM 0
		//mem[address] = value;
		if (0x2000 <= address) {
			setRomBankIndex((romBankIndex & 0x60) | value & 0x1f);
		}
	}else if (0x4000<= address && address <= 0x7fff) {
		//mem[address] = value;
		if (address <= 0x6000)
			maxMemMode =(MaximumMemoryMode)(value & 1);//0: 16/8 mode, 1: 4/32 mode
		if (address <= 0x5fff) {
			uint8_t n = value & 3;
			if (maxMemMode == _4_32_mode)
				ramBankIndex = n;
			else
				setRomBankIndex(n<<5 | (romBankIndex & 0x1f));
		}

	}else if (0xa000 <= address && address <= 0xbfff) {
		ram[(address - 0xa000) + ramBankIndex * ramBankSize];
	}
}

uint8_t* CARTRIDGE::getMemCell(uint16_t address)
{
	if (address <= 0x3fff) {//Rom 0
		return rom + address;
	}
	else if (0x4000 <= address && address <= 0x7fff) {//Rom n
		return rom + address + (int)(romBankIndex - 1) * romBankSize;
	}
	else if (0xa000 <= address && address <= 0xbfff) {//Ram m
		return ram + (address - 0xa000) + (int)ramBankIndex * ramBankSize;
	}
}

void CARTRIDGE::load()
{
	return;
	bus->mmu->biosLoaded = false;
	for (int i = 0;i < 0x7fff;i++)
		bus->mmu->write(i, read(i));
	bus->mmu->biosLoaded = true;
}
