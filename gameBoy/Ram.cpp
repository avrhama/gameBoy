#include "Ram.h"
#include <memory.h>

Ram::Ram() {

}
void Ram::write(uint16_t address, uint8_t value)
{
	mem[address] = value;
}

uint8_t Ram::read(uint16_t address)
{
	return mem[address];
}
