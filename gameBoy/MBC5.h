#include "CARTRIDGE.h"
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include <mutex>
#include <SDL_thread.h>
#include <SDL_mutex.h>
using namespace std::chrono;
class MBC5 :public CARTRIDGE {
protected:
	mutex saveRamMutex;
	bool saveRamThreadOn = false;
	uint8_t saveRamWait = 0;
private:
	uint8_t ramBankIndex = 0;
	uint16_t romBankIndex = 0;
	bool ramEnable = false;
	bool ramLoaded = false;

	
public:
	MBC5() {
	}
	MBC5(uint8_t* rom, CartridgeHeader header) {
		this->rom = rom;
		this->header = header;
		/*CARTRIDGE::header.romBankSize = 0x4000;
		CARTRIDGE::header.ramBankSize = 0x2000;*/
		ram = (uint8_t*)calloc(header.ramSize, 1);

		switch (header.cartridgeType) {
		case CartridgeType::ROM_MBC5_RAM_BATT:
		case CartridgeType::ROM_MBC5_RUMBLE_SRAM_BATT:
			loadRam();
			/*SDL_Thread* thread = SDL_CreateThread(saveRamThread, "SaveRam", (void*)this);
			SDL_DetachThread(thread);*/
			break;
		}
		
		//ramEnable = true;

	}
	void loadRam() {
		string path = "roms\\pokemon.sav";
		ifstream rf(path, ios::in | ios::binary);
		if (!rf) {

			cout << "Cannot open file!" << endl;
		}
		else {
			rf.seekg(0, rf.end);
			int size = rf.tellg();
			rf.seekg(0, rf.beg);
			//rom = (uint8_t*)malloc(size);
			int p = (int)ramBankIndex * 0x2000;
			p = 0;
			/*char* h =(char*)calloc(size, 1);
			rf.read(h, size);
			for (int i = 0;i < size;i++) {
				ram[i] = h[i]&0xffff;
			}*/
			rf.read((char*)(ram + p), size);
		}
		rf.close();
		printf("ram loaded!\n");

		ramLoaded = true;
	}
	static int saveRamThread(void* ptr) {
		MBC5* mbc = (MBC5*)ptr;
		printf("save ram thread starts\n");
		string saveFile = "roms\\pokemon.sav";
		uint8_t wait = 0;
		do {
			Sleep(10);
			mbc->saveRamMutex.lock();
			mbc->saveRamWait = mbc->saveRamWait - 1;
			wait= mbc->saveRamWait;
			mbc->saveRamMutex.unlock();
			//Sleep(1000);
			//ofstream myFile(saveFile, ios::out | ios::binary);
			//myFile.write((char*)mbc->ram, mbc->header.ramSize);
		} while (wait > 0);
		
		ofstream myFile(saveFile, ios::out | ios::binary);
		myFile.write((char*)mbc->ram, mbc->header.ramSize);
		printf("save ram thread ended\n");
		return 0;
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
			/*if ((ramEnable||false) &&!ramLoaded) {
					loadRam();
				}*/
			if (ramEnable) {
				//return ram[(address - 0xa000) + (int)ramBankIndex * 0x2000];
				return ram[(address - 0xa000) + (int)ramBankIndex * 0x2000];
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
			ramEnable = ((value & 0xf) == 0xA && header.ramSizeType != RamSizeType::None) ? true : false;
			/*if ((ramEnable||false) && !ramLoaded) {
				loadRam();
			}*/
		}
		else if (0x2000 <= address && address <= 0x2fff) {
			//rom bank 8 lower bits
			romBankIndex = romBankIndex & 0x100 | value;

		}
		else if (0x3000 <= address && address <= 0x3fff) {
			romBankIndex = (value & 0x01) << 9 | romBankIndex & 0xff;

		}
		else if (0x4000 <= address && address <= 0x5fff) {
			ramBankIndex = value & 0xf;
			/*if ((ramEnable||true) && !ramLoaded) {
				loadRam();
			}*/
		}
		else if (0xa000 <= address && address <= 0xbfff) {
			if(ramEnable) {
				ram[(address - 0xa000) + (int)ramBankIndex * 0x2000] = value;
				
				saveRamMutex.lock();
				if (saveRamWait==0) {
					saveRamWait = 100;
					SDL_Thread* thread = SDL_CreateThread(saveRamThread, "SaveRam", (void*)this);
					SDL_DetachThread(thread); 
				}
				saveRamWait = 100;
				saveRamMutex.unlock();
			}
			else {
				/*if (!ramLoaded) {
						loadRam();
					}*/
			}
		}
	}
};
