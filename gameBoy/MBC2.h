#include "CARTRIDGE.h"
#include <stdlib.h>
class MBC2 :public CARTRIDGE {

private:
	uint8_t romBankIndex = 1;
	bool ramEnable = false;
	bool latcheTimeFlag = false;
public:
	MBC2() {
	}
	//MBC1(uint8_t* rom, CARTRIDGE::CartridgeHeader header);
	MBC2(uint8_t* rom, CartridgeHeader header) {
		CARTRIDGE::rom = rom;
		CARTRIDGE::header = header;
		CARTRIDGE::header.romBankSize = 0x4000;
		CARTRIDGE::ram = (uint8_t*)calloc(header.ramSize, 1);
	}
	static CARTRIDGE* create(uint8_t* rom, CartridgeHeader header) {
		return new MBC2(rom, header);
	}


	uint8_t read(uint16_t address)
	{
		
		if (address <= 0x3fff) {//Rom 0
			return rom[address];
		}
		else if (0x4000 <= address && address <= 0x7fff) {//Rom n
			
			return rom[address + (romBankIndex - 1) * header.romBankSize];
		}
		else if (0xa000 <= address && address <= 0xa1ff) {//Ram m
			if (ramEnable) {
					return ram[(address - 0xa000)];
				
			}
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
			if (((address >> 4) & 0x1) == 0) 
				ramEnable  = ((value & 0xf) == 0xA && header.ramSizeType != RamSizeType::None) ? true : false;
		}
		else if (0x2000 <= address && address <= 0x3fff) {
			if (((address >> 4) & 0x1) == 0) {
				romBankIndex = value & 0x10;
				if (romBankIndex == 0)romBankIndex = 1;
			}
		}else if (0xa000 <= address && address <= 0xa1ff && ramEnable) {
				ram[address - 0xa000] = value & 0xf;
			
		}
		
	}
};
