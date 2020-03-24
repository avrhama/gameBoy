#pragma once
#include<cstdint>
#include <memory.h>

class  Ram
{
public:
	Ram();
	uint8_t mem[0xffff];
	void write(uint16_t address, uint8_t value);
	uint8_t read(uint16_t address);
};

