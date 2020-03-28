#include "Cartridge.h"
#include <stdlib.h>
#include "SHA1.h"
void Cartridge::loadRom(const char* path)
{
	ifstream rf(path, ios::out | ios::binary);
	if (!rf) {
		cout << "Cannot open file!" << endl;
	}
	else {
		rf.seekg(0, rf.end);
		int size = rf.tellg();
		rf.seekg(0, rf.beg);
		mem = (uint8_t*)malloc(size);
		int p = 0;
		rf.read((char*)mem, size);		
	}
	rf.close();
	setCartridgeHeader();

	
	//rom[0x7fff+1] = '\0';
	colorGB = mem[0x0143];
	GB_SGBIndicator = mem[0x0146];
	cartridgeType =(CartridgeType)mem[0x0147];
	romSize = (RomSize)mem[0x0148];
	ramSize= (RamSize)mem[0x0149];
	romBanksSize = getRomBanksSize();
	printf("true:%d\n", colorGB);
	printf("banks:%d\n", romBanksSize);
	
}

void Cartridge::setCartridgeHeader()
{
	memcpy(title, (mem + 0x0134), 9);
}

char Cartridge::getRomBanksSize()
{
	switch (romSize)
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
