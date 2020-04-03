#pragma once
#include<cstdint>
#include "BUS.h"
#include "CPU.h"
#include<iostream>
class INTERRUPT
{
private:
	
	BUS* bus;
public:
	uint8_t io[0x80];
	void write(uint16_t address,uint8_t value);
	uint8_t read(uint16_t address);
	uint8_t* getMemCell(uint16_t address);
	void connectToBus(BUS* bus);
	void reset();
};

