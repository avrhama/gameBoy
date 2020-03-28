#include "Ram.h"
#include <memory.h>

Ram::Ram() {

}
void Ram::write(uint16_t address, uint8_t value)
{
	mem[address] = value;
	if(address>= 0xE000 && address<= 0xFE00)
		mem[address-0x1000] = value;
	else if(address >= 0xC000 && address <= 0xDE00)
		mem[address + 0x1000] = value;
}

uint8_t Ram::read(uint16_t address)
{
	return mem[address];
}
