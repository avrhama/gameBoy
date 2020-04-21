#include "CARTRIDGE.h"
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <iostream>

using namespace std::chrono;
class MBC5 :public CARTRIDGE {

private:
	uint8_t ramBankIndex = 0;
	uint16_t romBankIndex = 0;
	bool ramEnable = false;
public:
	MBC5() {
	}
	MBC5(uint8_t* rom, CartridgeHeader header) {
		CARTRIDGE::rom = rom;
		CARTRIDGE::header = header;
		/*CARTRIDGE::header.romBankSize = 0x4000;
		CARTRIDGE::header.ramBankSize = 0x2000;*/
		int size_ = (int)header.ramBanksCount * header.ramBankSize;
		CARTRIDGE::ram = (uint8_t*)calloc(size_, 1);
		

	}
	static CARTRIDGE* create(uint8_t* rom, CartridgeHeader header) {
		return new MBC5(rom, header);
	}


	uint8_t read(uint16_t address)
	{
		
		if (address <= 0x3fff) {//Rom 0
			return rom[address];
		}
		else if (0x4000 <= address && address <= 0x7fff) {//Rom n
			return rom[address + (romBankIndex - 1) * header.romBankSize];

		}
		else if (0xa000 <= address && address <= 0xbfff) {//Ram m
			if (ramEnable)
					return ram[(address - 0xa000) + ramBankIndex * header.ramBankSize];
			else
				return 0xff;
		}
		else {
			printf("CARTRIDGE::read illeal address!");
			return NULL;
		}


	}
	
	void write(uint16_t address, uint8_t value)
	{
		if (address <= 0x1fff) {//ROM 0
			ramEnable = ((value & 0xf) == 0xA && header.ramSizeType != RamSizeType::None) ? true : false;
		}
		else if (0x2000 <= address && address <= 0x2fff) {
			//rom bank 8 lower bits
			romBankIndex = romBankIndex&0x100|value;
			
		}
		else if (0x3000 <= address && address <= 0x3fff) {
			romBankIndex = (value&0x01)<<9|romBankIndex&0xff;
			
		}
		else if (0x4000 <= address && address <= 0x5fff) {
			ramBankIndex = value & 0xf;
		}
		else if (0xa000 <= address && address <= 0xbfff && ramEnable) {
				ram[(address - 0xa000) + ramBankIndex * header.ramBankSize] = value;
		}
	}
};
