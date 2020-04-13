#include "CARTRIDGE.h"
#include <stdlib.h>
#include "SHA1.h"
void CARTRIDGE::connectToBus(BUS* bus)
{
	this->bus = bus;
}
void CARTRIDGE::loadRom(string path)
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
	colorGB = (rom[0x0143]==0x80|| rom[0x0143] == 0xc0)?true:false;
	GB_SGBIndicator = rom[0x0146];
	cartridgeType =(CartridgeType)rom[0x0147];
	romSizeType = (RomSizeType)(rom[0x0148]);
	ramSizeType= (RamSizeType)(rom[0x0149]);
	romSize = getRomSize();
	ramSize = getRomSize();
	ram = (uint8_t*)calloc(((int)ramSize)*ramBankSize, 1);
	printf("colorGB?:%d\n", colorGB);
	printCartridgeType();
	
}

void CARTRIDGE::setCartridgeHeader()
{
	memcpy(title, (rom + 0x0134), 9);
}

void CARTRIDGE::printCartridgeType()
{
	switch (cartridgeType) {
	case CartridgeType::ROM_ONLY:
		printf("ROM_ONLY\n");
		break;
	case CartridgeType::ROM_MBC1:
		printf("ROM_MBC1\n");
		break;
	case CartridgeType::ROM_MBC1_RAM:
		printf("ROM_MBC1_RAM\n");
		break;
	case CartridgeType::ROM_MBC1_RAM_BATT:
		printf("ROM_MBC1_RAM_BATT\n");
		break;
	case CartridgeType::ROM_MBC2:
		printf("ROM_MBC2\n");
		break;
	case CartridgeType::ROM_MBC2_BATTERY:
		printf("ROM_MBC2_BATTERY\n");
		break;
	case CartridgeType::ROM_RAM:
		printf("ROM_RAM\n");
		break;
	case CartridgeType::ROM_RAM_BATTERY:
		printf("ROM_RAM_BATTERY\n");
		break;
	case CartridgeType::ROM_MMM01:
		printf("ROM_MMM01\n");
		break;
	case CartridgeType::ROM_MMM01_SRAM:
		printf("ROM_MMM01_SRAM\n");
		break;
	case CartridgeType::ROM_MMM01_SRAM_BATT:
		printf("ROM_MMM01_SRAM_BATT\n");
		break;
	case CartridgeType::ROM_MBC3_TIMER_BATT:
		printf("ROM_MBC3_TIMER_BATT\n");
		break;
	case CartridgeType::ROM_MBC3_TIMER_RAM_BATT:
		printf("ROM_MBC3_TIMER_RAM_BATT\n");
		break;
	case CartridgeType::ROM_MBC3:
		printf("ROM_MBC3\n");
		break;
	case CartridgeType::ROM_MBC3_RAM:
		printf("ROM_MBC3_RAM\n");
		break;
	case CartridgeType::ROM_MBC3_RAM_BATT:
		printf("ROM_MBC3_RAM_BATT\n");
		break;
	case CartridgeType::ROM_MBC5:
		printf("ROM_MBC5\n");
		break;
	case CartridgeType::ROM_MBC5_RAM:
		printf("ROM_MBC5_RAM\n");
		break;
	case CartridgeType::ROM_MBC5_RAM_BATT:
		printf("ROM_MBC5_RAM_BATT\n");
		break;
	case CartridgeType::ROM_MBC5_RUMBLE:
		printf("ROM_MBC5_RUMBLE\n");
		break;
	case CartridgeType::ROM_MBC5_RUMBLE_SRAM:
		printf("ROM_MBC5_RUMBLE_SRAM\n");
		break;
	case CartridgeType::ROM_MBC5_RUMBLE_SRAM_BATT:
		printf("ROM_MBC5_RUMBLE_SRAM_BATT\n");
		break;
	case CartridgeType::Pocket_Camera:
		printf("Pocket_Camera\n");
		break;
	case CartridgeType::Bandai_TAMA5:
		printf("Bandai_TAMA5\n");
		break;
	case CartridgeType::Hudson_HuC_3:
		printf("Hudson_HuC_3\n");
		break;
	case CartridgeType::Hudson_HuC_1:
		printf("Hudson_HuC_1\n");
		break;
	}

	
}



uint8_t CARTRIDGE::getRomSize()
{
	switch (romSizeType)
	{
	case RomSizeType::_2_banks:
		return 2;
	case RomSizeType::_4_banks:
		return 4;
	case RomSizeType::_8_banks:
		return 8;
	case RomSizeType::_16_banks:
		return 16;
	case RomSizeType::_32_banks:
		return 32;
	case RomSizeType::_64_banks:
		return 64;
	case RomSizeType::_128_banks:
		return 128;
	case RomSizeType::_72_banks:
		return 72;
	case RomSizeType::_80_banks:
		return 80;
	case RomSizeType::_96_banks:
		return 96;
	}
	return 0;
}
uint8_t CARTRIDGE::getRamSize()
{
	switch (ramSizeType)
	{
	case RamSizeType::None:
		return 0;
	case  RamSizeType::_1_bank_2kB:// 1/4 of ram bank
		return 1;
	case  RamSizeType::_1_bank_8kB:
		return 1;
	case  RamSizeType::_4_banks_32kB:
		return 4;
	case  RamSizeType::_16_banks_128kB:
		return 16;
	case  RamSizeType::_8_banks_64kB:
		return  8;
	}
	return 0;
}

uint8_t CARTRIDGE::read(uint16_t address)
{
	/*if (cartridgeType == CartridgeType::ROM_ONLY)
		return rom[address];*/
	if (address <= 0x3fff) {//Rom 0
		return rom[address];
	}
	else if (0x4000 <= address && address <= 0x7fff) {//Rom n
		return rom[address + (romBankIndex-1) * romBankSize];
	}
	else if (0xa000 <= address && address <= 0xbfff) {//Ram m
		return ram[(address- 0xa000) + ramBankIndex * ramBankSize];
	} else{
		printf("CARTRIDGE::read illeal address!");
		return NULL;
	}
}
void CARTRIDGE::setRomBankIndex(uint8_t v) {
	romBankIndex = v;
	if (romBankIndex <= 60 && romBankIndex % 20 == 0)
		romBankIndex += 1;

}
void CARTRIDGE::write(uint16_t address, uint8_t value)
{
	//if (cartridgeType == CartridgeType::ROM_ONLY) {
	//	 //rom[address]=value;
	//	 return;
	//}
	if (address <= 0x3fff) {//ROM 0
		if (0x2000 <= address) {
			setRomBankIndex((romBankIndex & 0x60) | value & 0x1f);
		}
	}else if (0x4000<= address && address <= 0x7fff) {
		if (address <= 0x6000)
			maxMemMode =(MaximumMemoryMode)(value & 1);//0: 16/8 mode, 1: 4/32 mode
		if (address <= 0x5fff) {
			uint8_t n = value & 3;
			if (maxMemMode == MaximumMemoryMode::_4_32_mode)
				ramBankIndex = n;
			else
				setRomBankIndex(n<<5 | (romBankIndex & 0x1f));
		}

	}else if (0xa000 <= address && address <= 0xbfff) {
		ram[(address - 0xa000) + ramBankIndex * ramBankSize]=value;
	}
}

//uint8_t* CARTRIDGE::getMemCell(uint16_t address)
//{
//	if (address <= 0x3fff) {//Rom 0
//		return rom + address;
//	}
//	else if (0x4000 <= address && address <= 0x7fff) {//Rom n
//		return rom + address + (int)(romBankIndex - 1) * romBankSize;
//	}
//	else if (0xa000 <= address && address <= 0xbfff) {//Ram m
//		return ram + (address - 0xa000) + (int)ramBankIndex * ramBankSize;
//	}
//}

void CARTRIDGE::load()
{
	return;
	bus->mmu->biosLoaded = false;
	for (int i = 0;i < 0x7fff;i++)
		bus->mmu->write(i, read(i));
	//bus->mmu->biosLoaded = true;
}
