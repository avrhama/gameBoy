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
	uint8_t* rom=NULL;
	uint8_t* ram = NULL;
   //rom bank size=16KB(16384 B)
	//ram bank size=8KB(8192 B)
	enum class CartridgeType :uint8_t {
		ROM_ONLY, ROM_MBC1, ROM_MBC1_RAM, ROM_MBC1_RAM_BATT, ROM_MBC2 = 0x5,
		ROM_MBC2_BATTERY, ROM_RAM = 0x8, ROM_RAM_BATTERY, ROM_MMM01 = 0xB, ROM_MMM01_SRAM,
		ROM_MMM01_SRAM_BATT, ROM_MBC3_TIMER_BATT = 0xF, ROM_MBC3_TIMER_RAM_BATT, ROM_MBC3,
		ROM_MBC3_RAM, ROM_MBC3_RAM_BATT, ROM_MBC5 = 0x19, ROM_MBC5_RAM, ROM_MBC5_RAM_BATT,
		ROM_MBC5_RUMBLE, ROM_MBC5_RUMBLE_SRAM, ROM_MBC5_RUMBLE_SRAM_BATT, Pocket_Camera,
		Bandai_TAMA5 = 0xFD, Hudson_HuC_3, Hudson_HuC_1
	};
	
	enum class RomSizeType :uint8_t {
		_2_banks, _4_banks, _8_banks, _16_banks, _32_banks,
		_64_banks, _128_banks, _72_banks = 0x52, _80_banks, _96_banks
	};
	enum class RamSizeType :uint8_t {
		None, _1_bank_2kB, _1_bank_8kB, _4_banks_32kB, _16_banks_128kB,_8_banks_64kB
		//None,_1_bank_2kB,_1_bank_8kB,_4_banks_32kB,_16_banks_128kB
	};
	enum class MaximumMemoryMode :uint8_t {_16_8_mode,_4_32_mode
	};
	char title[9] = {};
	bool colorGB=false;
	char GB_SGBIndicator=0;
	
	void loadRom(string path);
	void setCartridgeHeader();
	
	CartridgeType cartridgeType= CartridgeType::Bandai_TAMA5;
	RomSizeType romSizeType= RomSizeType::_128_banks;
	RamSizeType ramSizeType= RamSizeType::None;
	void connectToBus(BUS* bus);
	uint8_t getRomSize();
	uint8_t getRamSize();
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);
	//uint8_t* getMemCell(uint16_t address);
	void load();
	uint8_t romBankIndex = 1;
	uint8_t romSize=0;//How many rom banks( in mbc1 each rom bank is 16KB size[0x4000 B])
	uint8_t ramSize=0;//How many ram banks( in mbc1 each ram bank is 8KB size[0x2000 B])
	int romBankSize=0x4000;//MBC1
	int ramBankSize = 0x2000;//MBC1
private:
	MaximumMemoryMode maxMemMode = MaximumMemoryMode::_16_8_mode;
	BUS* bus=NULL;
	
	uint8_t ramBankIndex = 1;
	void setRomBankIndex(uint8_t v);
};

