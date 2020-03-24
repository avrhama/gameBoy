#pragma once
#include<cstdint>
//struct Opcode
//{
//public:
//	const char* name;
//	void(Cpu::* operate)(uint16_t*, uint16_t*);
//	uint16_t* (Cpu::* param1)();
//	uint16_t* (Cpu::* param2)();
//	uint8_t cycles;
//};
struct Word
{
public:
	uint8_t msb;
	uint8_t lsb;
};