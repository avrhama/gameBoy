#pragma once
#include<cstdint>
#include <memory.h>
#include<fstream>
#include<iostream>
#include "BUS.h"
#include "Mmu.h"
#include "CPU.h"
using namespace std;
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
	None, _1_bank_2kB, _1_bank_8kB, _4_banks_32kB, _16_banks_128kB, _8_banks_64kB
	//None,_1_bank_2kB,_1_bank_8kB,_4_banks_32kB,_16_banks_128kB
};
enum class MaximumMemoryMode :uint8_t {
	_16_8_mode, _4_32_mode
};

struct CartridgeHeader {
public:
	MaximumMemoryMode maxMemMode = MaximumMemoryMode::_16_8_mode;
	CartridgeType cartridgeType = CartridgeType::Bandai_TAMA5;
	RomSizeType romSizeType = RomSizeType::_128_banks;
	RamSizeType ramSizeType = RamSizeType::None;
	char title[9] = {};
	bool colorGB = false;
	char GB_SGBIndicator = 0;
	uint8_t romSize = 0;//How many rom banks( in mbc1 each rom bank is 16KB size[0x4000 B])
	uint8_t ramSize = 0;//How many ram banks( in mbc1 each ram bank is 8KB size[0x2000 B])
	//int romBankSize = 0x4000;//MBC1
	//int ramBankSize = 0x2000;//MBC1
	int romBankSize;
		int ramBankSize;
};
class CARTRIDGE
{

public:
	
   //rom bank size=16KB(16384 B)
	//ram bank size=8KB(8192 B)
	CartridgeHeader header;
	CARTRIDGE(uint8_t* rom, CartridgeHeader header) {
		this->rom = rom;
		this->header = header;
		int size_ = (int)header.ramSize * header.ramBankSize;
		if(size_>0)
		this->ram = (uint8_t*)calloc(size_, 1);
	}
	CARTRIDGE() {
		/*int size_ = (int)header.ramSize * header.ramBankSize;
		ram = (uint8_t*)calloc(size_, 1);*/
	}
	virtual ~CARTRIDGE() {}
	
	virtual void write(uint16_t address, uint8_t value) {
		return;
	}
	virtual uint8_t read(uint16_t address) {
		if (address >= 0 && address <= 0x3fff)
			return rom[address];
	}
	
	static uint8_t getRomSize(RomSizeType romSizeType)
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
	static uint8_t getRamSize(RamSizeType ramSizeType)
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
	void connectToBus(BUS* bus)
	{
		this->bus = bus;
	}
	
	static void printCartridgeType(CartridgeType cartridgeType)
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
	
protected:

	BUS* bus = NULL;
	uint8_t* rom = NULL;
	uint8_t* ram = NULL;
	bool ramEnable = false;
	//MaximumMemoryMode maxMemMode = MaximumMemoryMode::_16_8_mode;
	//BUS* bus=NULL;
	//uint8_t* rom = NULL;
	//CartridgeType cartridgeType = CartridgeType::Bandai_TAMA5;
	//RomSizeType romSizeType = RomSizeType::_128_banks;
	//RamSizeType ramSizeType = RamSizeType::None;
	//char title[9] = {};
	//bool colorGB = false;
	//char GB_SGBIndicator = 0;
	//uint8_t romSize = 0;//How many rom banks( in mbc1 each rom bank is 16KB size[0x4000 B])
	//uint8_t ramSize = 0;//How many ram banks( in mbc1 each ram bank is 8KB size[0x2000 B])
	//int romBankSize = 0x4000;//MBC1
	//int ramBankSize = 0x2000;//MBC1
};

