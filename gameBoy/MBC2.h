#include "CARTRIDGE.h"
#include <stdlib.h>
//class CARTRIDGE {};
class MBC2 :public CARTRIDGE {

private:
	uint8_t ramAndTimerBankIndex = 0;
	uint8_t romBankIndex = 1;
	bool ramAndTimerEnable = false;
	bool latcheTimeFlag = false;
public:
	MBC2() {
	}
	//MBC1(uint8_t* rom, CARTRIDGE::CartridgeHeader header);
	MBC2(uint8_t* rom, CartridgeHeader header) {
		CARTRIDGE::rom = rom;
		CARTRIDGE::header = header;
		CARTRIDGE::header.romBankSize = 0x4000;
		CARTRIDGE::header.ramBankSize = 0x2000;
		int size_ = (int)header.ramSize * header.ramBankSize;
		CARTRIDGE::ram = (uint8_t*)calloc(size_, 1);
	}
	static CARTRIDGE* create(uint8_t* rom, CartridgeHeader header) {
		//return new MBC1();
		return new MBC2(rom, header);
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
		else if (0xa000 <= address && address <= 0xa1ff) {//Ram m
			if (ramAndTimerEnable) {
				if (ramAndTimerBankIndex < 0x08)
					return ram[(address - 0xa000) + ramAndTimerBankIndex * header.ramBankSize] | 0xf0;
				else if (ramAndTimerBankIndex < 0x0c)
					return ram[address - 0xa000] | 0xf0;
			}
			else
				return 0xff;
		}
		else {
			printf("CARTRIDGE::read illeal address!");
			return NULL;
		}
		//else if (0xa000 <= address && address <= 0xbfff) {//Ram m
		//if (ramAndTimerEnable) {
		//	if (ramAndTimerBankIndex < 0x08)
		//		return ram[(address - 0xa000) + ramAndTimerBankIndex * header.ramBankSize];
		//	else if (ramAndTimerBankIndex < 0x0c)
		//		return ram[address - 0xa000];
		//}
		//else
		//	return 0xff;
		//}
		//else {
		//printf("CARTRIDGE::read illeal address!");
		//return NULL;
		//}
	}
	void setRomBankIndex(uint8_t v) {
		romBankIndex = v;
		if (romBankIndex == 0)
			romBankIndex = 1;
		if (header.maxMemMode == MaximumMemoryMode::_4_32_mode)
			romBankIndex &= 0x1f;


	}
	void latcheTime() {

	}
	void write(uint16_t address, uint8_t value)
	{
		if (address <= 0x1fff) {//ROM 0
			if ((address >> 4) & 0x1 == 0) mbc2
				ramAndTimerEnable = ramEnable = ((value & 0xf) == 0xA && header.ramSizeType != RamSizeType::None) ? true : false;
		}
		else if (0x2000 <= address && address <= 0x3fff) {
			romBankIndex = value & 0x7f;
			if (romBankIndex == 0)romBankIndex = 1;
		}
		else if (0x4000 <= address && address <= 0x5fff) {
			ramAndTimerBankIndex = value & 0xf;

		}
		else if (0x6000 <= address && address <= 0x7fff) {
			if (value == 0x01 && &latcheTimeFlag)
				latcheTime();

			latcheTimeFlag = value == 0x0 ? true : false;
		}
		else if (0xa000 <= address && address <= 0xa1ff && ramEnable) {
			if (header.maxMemMode == MaximumMemoryMode::_4_32_mode)
				ram[(address - 0xa000) + ramAndTimerBankIndex * header.ramBankSize] = value & 0xf;
			else
				ram[address - 0xa000] = value & 0xf;
		}
		/*else if (0xa000 <= address && address <= 0xbfff && ramEnable) {
			if (header.maxMemMode == MaximumMemoryMode::_4_32_mode)
				ram[(address - 0xa000) + ramAndTimerBankIndex * header.ramBankSize] = value;
			else
				ram[address - 0xa000] = value;
		}*/
	}
};
