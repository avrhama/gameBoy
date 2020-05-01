#include "CARTRIDGE.h"
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <iostream>
//class CARTRIDGE {};
using namespace std::chrono;
class MBC3 :public CARTRIDGE {

private:
	uint8_t ramAndTimerBankIndex = 0;
	uint8_t romBankIndex = 1;
	bool ramAndTimerEnable = false;
	bool latcheTimeFlag = false;
	steady_clock::time_point startTimer;
	struct TIMER {
		//write
		uint8_t RTCSeconds;//0x08
		uint8_t RTCMinutes;//0x09
		uint8_t RTCHours;//0x0A
		uint8_t RTCDays;//0x0B 8 lower bits
		uint8_t RTCFlags;//0x0C 
		//read
		uint8_t latcheRTCSeconds;//0x08
		uint8_t latcheRTCMinutes;//0x09
		uint8_t latcheRTCHours;//0x0A
		uint8_t latcheRTCDays;//0x0B 8 lower bits
		uint8_t latcheRTCFlags;//0x0C 
	};
	TIMER timer = TIMER{0x0,0x0,0x0,0x0,0x40,0x0,0x0,0x0,0x0,0x40};
public:
	MBC3() {
	}
	//MBC1(uint8_t* rom, CARTRIDGE::CartridgeHeader header);
	MBC3(uint8_t* rom, CartridgeHeader header) {
		CARTRIDGE::rom = rom;
		CARTRIDGE::header = header;
		CARTRIDGE::header.romBankSize = 0x4000;
		CARTRIDGE::ram = (uint8_t*)calloc(header.ramSize, 1);
		startTimer = steady_clock::now();

	
	}
	static CARTRIDGE* create(uint8_t* rom, CartridgeHeader header) {
		//return new MBC1();
		return new MBC3(rom, header);
	}


	uint8_t read(uint16_t address)
	{
		
		if (address <= 0x3fff) {//Rom 0
			return rom[address];
		}
		else if (0x4000 <= address && address <= 0x7fff) {//Rom n
			/*uint8_t r = romBankIndex;
			if (maxMemMode == MaximumMemoryMode::_4_32_mode)
				r &= 0x1f;*/
			return rom[address + (romBankIndex - 1) * header.romBankSize];

		}else if (0xa000 <= address && address <= 0xbfff) {//Ram m
			if (ramAndTimerEnable) {
				if (ramAndTimerBankIndex < 0x04)
					return ram[(address - 0xa000) + ramAndTimerBankIndex * 0x2000];
				else if (ramAndTimerBankIndex < 0x0d)
					return *((uint8_t*)&timer + (ramAndTimerBankIndex - 0x04));
					
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
		if (romBankIndex == 0)
			romBankIndex = 1;
		if (header.maxMemMode == MaximumMemoryMode::_4_32_mode)
			romBankIndex &= 0x1f;


	}
	void latcheTime() {
		steady_clock::time_point end = steady_clock::now();
		duration<double> elapsed_seconds = end - startTimer;
		int secondsCounter = elapsed_seconds.count();
		
		timer.RTCSeconds = secondsCounter % 60;
		timer.RTCMinutes = (secondsCounter / 60) % 60;
		timer.RTCHours = (secondsCounter /3600) % 24;
		int days = secondsCounter /86400;
		if (days >= 512) {
			timer.RTCDays = 0;
			timer.RTCFlags &= 0xfe;//reset days significant bit.
			timer.RTCFlags |=0x80;//set days carry bit.
		}
		else {
			
			timer.RTCDays = days &0xff;
			timer.RTCFlags &= 0xfe;//reset days significant bit.
			timer.RTCFlags |=((days >> 8) & 0x01);
		}
		
		timer.latcheRTCDays = timer.RTCDays;
		timer.latcheRTCHours = timer.RTCHours;
		timer.latcheRTCMinutes = timer.RTCMinutes;
		timer.latcheRTCSeconds = timer.RTCSeconds;
		timer.latcheRTCFlags = timer.RTCFlags;
	}
	void write(uint16_t address, uint8_t value)
	{
		if (address <= 0x1fff) {//ROM 0
			ramAndTimerEnable = ((value & 0xf) == 0xA&&header.ramSizeType!=RamSizeType::None) ? true : false;
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
			
			latcheTimeFlag = value==0x0?true:false;
		}else if (0xa000 <= address && address <= 0xbfff) {
			if (ramAndTimerEnable) {
				if (ramAndTimerBankIndex < 0x04)
					 ram[(address - 0xa000) + ramAndTimerBankIndex * 0x2000]=value;
				else if (ramAndTimerBankIndex < 0x0d) {
					timer.RTCFlags |= 0x40;
					*((uint8_t*)&timer + (ramAndTimerBankIndex - 0x08)) = value;
				}

			}
		}
	}
};
