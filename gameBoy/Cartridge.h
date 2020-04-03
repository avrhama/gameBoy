#pragma once
#include<cstdint>
#include <memory.h>
#include<fstream>
#include<iostream>
#include "BUS.h"
#include "Mmu.h"
#include "CPU.h"
using namespace std;
class CARTRIDGE
{

	
public:
	uint8_t* rom;
	uint8_t* ram;
   //rom bank size=16KB(16384 B)
	//ram bank size=8KB(8192 B)
	enum CartridgeType :uint8_t {
		ROM_ONLY, ROM_MBC1, ROM_MBC1_RAM, ROM_MBC1_RAM_BATT, ROM_MBC2 = 0x5,
		ROM_MBC2_BATTERY, ROM_RAM = 0x8, ROM_RAM_BATTERY, ROM_MMM01 = 0xB, ROM_MMM01_SRAM,
		ROM_MMM01_SRAM_BATT, ROM_MBC3_TIMER_BATT = 0xF, ROM_MBC3_TIMER_RAM_BATT, ROM_MBC3,
		ROM_MBC3_RAM, ROM_MBC3_RAM_BATT, ROM_MBC5 = 0x19, ROM_MBC5_RAM, ROM_MBC5_RAM_BATT,
		ROM_MBC5_RUMBLE, ROM_MBC5_RUMBLE_SRAM, ROM_MBC5_RUMBLE_SRAM_BATT, Pocket_Camera,
		Bandai_TAMA5 = 0xFD, Hudson_HuC_3, Hudson_HuC_1
	};
	
	enum RomSizeType :uint8_t {
		_2_banks, _4_banks, _8_banks, _16_banks, _32_banks,
		_64_banks, _128_banks, _72_banks = 0x52, _80_banks, _96_banks
	};
	enum RamSizeType :uint8_t {
		None, _1_bank_2kB, _1_bank_8kB, _4_banks_32kB, _16_banks_128kB,_8_banks_64kB
		//None,_1_bank_2kB,_1_bank_8kB,_4_banks_32kB,_16_banks_128kB
	};
	enum MaximumMemoryMode :uint8_t {_16_8_mode,_4_32_mode
	};
	char title[9];
	bool colorGB;
	char GB_SGBIndicator;
	
	void loadRom(const char* path);
	void setCartridgeHeader();
	
	CartridgeType cartridgeType;
	RomSizeType romSizeType;
	RamSizeType ramSizeType;
	void connectToBus(BUS* bus);
	char getRomSize();
	char getRamSize();
	uint8_t read(uint32_t address);
	void write(uint16_t address, uint8_t value);
	uint8_t* getMemCell(uint16_t address);
	void load();
	uint8_t romBankIndex = 1;
	char romSize;//How many rom banks( in mbc1 each rom bank is 16KB size[0x4000 B])
	char ramSize;//How many ram banks( in mbc1 each ram bank is 8KB size[0x2000 B])
	int romBankSize=0x4000;//MBC1
	int ramBankSize = 0x2000;//MBC1
private:
	MaximumMemoryMode maxMemMode = _16_8_mode;
	BUS* bus;
	
	uint8_t ramBankIndex = 1;
	void setRomBankIndex(uint8_t v);
};

