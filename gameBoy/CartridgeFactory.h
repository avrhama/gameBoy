#pragma once
#include "CARTRIDGE.h"
#include "MBC1.h"
#include "MBC2.h"
#include "MBC3.h"
#include "MBC5.h"
static CARTRIDGE* createCartrige(string path) {

	uint8_t* rom;
	CartridgeHeader header;
	//ifstream rf(path, ios::out | ios::binary);
	ifstream rf(path, ios::in | ios::binary);
	if (!rf) {

		cout << "Cannot open file!" << endl;
		return NULL;
	}
	else {
		rf.seekg(0, rf.end);
		int size = rf.tellg();
		rf.seekg(0, rf.beg);
		rom = (uint8_t*)malloc(size);
		int p = 0;
		rf.read((char*)rom, size);

		int index = path.find_last_of('\\') + 1;
		header.romFileName = path.substr(index, path.find_last_of('.')- index);
		header.romFileFolder = path.substr(0, index);
		memcpy(header.title, (rom + 0x0134), 9);
		header.colorGB = (rom[0x0143] == 0x80 || rom[0x0143] == 0xc0) ? true : false;
		header.GB_SGBIndicator = rom[0x0146];
		header.cartridgeType = (CartridgeType)rom[0x0147];
		header.romSizeType = (RomSizeType)(rom[0x0148]);
		header.ramSizeType = (RamSizeType)(rom[0x0149]);
		header.romBanksCount = CARTRIDGE::getRomBanksCount(header.romSizeType);
		header.ramBanksCount = CARTRIDGE::getRamBanksCount(header.ramSizeType);
		header.ramSize = CARTRIDGE::getRamSize(header.ramSizeType);
		printf("colorGB?:%d\n", header.colorGB);
		CARTRIDGE::printCartridgeType(header.cartridgeType);
	}
	rf.close();
	//CARTRIDGE s;

	//CARTRIDGE* gg=new MBC1(rom, header);
	switch (header.cartridgeType) {
	case CartridgeType::ROM_ONLY:
	case CartridgeType::ROM_RAM:
	case CartridgeType::ROM_RAM_BATTERY:
		//return new CARTRIDGE(rom, header);
		return MBC1::create(rom, header);
		//return new CARTRIDGE();


	case CartridgeType::ROM_MBC1:
	case CartridgeType::ROM_MBC1_RAM:
	case CartridgeType::ROM_MBC1_RAM_BATT:
		return MBC1::create(rom, header);
	case CartridgeType::ROM_MBC2:
	case CartridgeType::ROM_MBC2_BATTERY:
		return MBC2::create(rom, header);
	case CartridgeType::ROM_MBC3:
	case CartridgeType::ROM_MBC3_RAM:
	case CartridgeType::ROM_MBC3_RAM_BATT:
	case CartridgeType::ROM_MBC3_TIMER_BATT:
	case CartridgeType::ROM_MBC3_TIMER_RAM_BATT:
		return MBC3::create(rom, header);
	case CartridgeType::ROM_MBC5:
	case CartridgeType::ROM_MBC5_RAM:
	case CartridgeType::ROM_MBC5_RAM_BATT:
	case CartridgeType::ROM_MBC5_RUMBLE:
	case CartridgeType::ROM_MBC5_RUMBLE_SRAM:
	case CartridgeType::ROM_MBC5_RUMBLE_SRAM_BATT:
		return MBC5::create(rom, header);
	default:
		printf("Unkown cartridge type!\n");
	}
	return NULL;
}