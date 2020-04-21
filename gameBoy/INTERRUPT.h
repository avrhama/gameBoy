#pragma once
#include<cstdint>
#include "BUS.h"
#include "CPU.h"
#include "MMU.h"
#include "DMA.h"
#include "APU.h"
#include<iostream>
class INTERRUPT
{
private:
	
	BUS* bus;
public:
	uint8_t io[0x80];
	uint8_t InterruptEnabledRegister=0;
	void write(uint16_t address,uint8_t value);
	uint8_t read(uint16_t address);
	void setInterruptRequest(uint8_t bit);
	void resetInterruptRequest(uint8_t bit);
	void setInterruptEnabledRegister(uint8_t bit);
	void resetInterruptEnabledRegister(uint8_t bit);
	bool isInterruptEnable(uint8_t bit);
	bool isInterruptRequsted(uint8_t bit);
	uint8_t InterruptsHandler();
	void InterruptHandler(uint8_t interrupt);
	void connectToBus(BUS* bus);
	void reset();
};

