#include "CARTRIDGE.h"
#include <stdlib.h>
//class CARTRIDGE {};
class MBC1 :public CARTRIDGE {

private:
	uint8_t ramBankIndex = 0;
	uint8_t romBankIndex = 1;
public:
	MBC1() {
	}
	//MBC1(uint8_t* rom, CARTRIDGE::CartridgeHeader header);
	MBC1(uint8_t* rom, CartridgeHeader header) {
		CARTRIDGE::rom = rom;
		CARTRIDGE::header = header;
		CARTRIDGE::header.romBankSize = 0x4000;
		CARTRIDGE::header.ramBankSize = 0x2000;
		int size_ = (int)header.ramSize * header.ramBankSize;
		CARTRIDGE::ram = (uint8_t*)calloc(size_, 1);
	}
	static CARTRIDGE* create(uint8_t* rom, CartridgeHeader header) {
		//return new MBC1();
		return new MBC1(rom, header);
	}

	
	uint8_t read(uint16_t address)
	{
		/*if (cartridgeType == CartridgeType::ROM_ONLY)
			return rom[address];*/
		if (address <= 0x3fff) {//Rom 0
			return rom[address];
		}
		else if (0x4000 <= address && address <= 0x7fff) {//Rom n
			/*uint8_t r = romBankIndex;
			if (maxMemMode == MaximumMemoryMode::_4_32_mode)
				r &= 0x1f;*/
			return rom[address + (romBankIndex - 1) * header.romBankSize];
		}
		else if (0xa000 <= address && address <= 0xbfff) {//Ram m
			if (ramEnable) {
				if (header.maxMemMode == MaximumMemoryMode::_4_32_mode)
					return ram[(address - 0xa000) + ramBankIndex * header.ramBankSize];
				else
					return ram[address - 0xa000];
			}
			else
				return 0xff;
		}
		else {
			printf("CARTRIDGE::read illeal address!");
			return NULL;
		}
	}
	void setRomBankIndex(uint8_t v) {
		romBankIndex = v;
		if (romBankIndex <= 60 && romBankIndex % 20 == 0)
			romBankIndex += 1;
		if (romBankIndex > 15)
			int y = 0;
		if (header.maxMemMode == MaximumMemoryMode::_4_32_mode)
			romBankIndex &= 0x1f;


	}
	void write(uint16_t address, uint8_t value)
	{
		//if (cartridgeType == CartridgeType::ROM_ONLY) {
		//	 //rom[address]=value;
		//	 return;
		//}
		if (address <= 0x1fff) {//ROM 0
			ramEnable = ((value & 0xf) == 0xA&& header.ramSizeType!=RamSizeType::None) ? true : false;
			/*uint8_t t = value & 0xf;
			if (t == 0xA && header.ramSizeType != RamSizeType::None) {
				ramEnable = true;
			}
			else if (t == 0x0) {
				ramEnable = false;
			}*/
		}
		else if (0x2000 <= address && address <= 0x3fff) {
			//selecting the lower 5 bits of rom bank.
			setRomBankIndex((romBankIndex & 0x60) | value & 0x1f);
		}
		else if (0x4000 <= address && address <= 0x5fff) {
			if (header.maxMemMode == MaximumMemoryMode::_4_32_mode)
				//selecting the ram bank [0-3].
				ramBankIndex = value & 0x3;
			else
				setRomBankIndex((value & 0x60) | (romBankIndex & 0x1f));
			//selecting the upper 2 bits of rom bank.
			//setRomBankIndex(value & 0x60 | (romBankIndex & 0x1f));
		}
		else if (0x6000 <= address && address <= 0x7fff) {
			header.maxMemMode = (MaximumMemoryMode)(value & 0x1);//0: 16/8 mode, 1: 4/32 mode
		}
		else if (0xa000 <= address && address <= 0xbfff && ramEnable) {
			if (header.maxMemMode == MaximumMemoryMode::_4_32_mode)
				ram[(address - 0xa000) + ramBankIndex * header.ramBankSize] = value;
			else
				ram[address - 0xa000] = value;
		}
	}
};
