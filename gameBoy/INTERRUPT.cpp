#include "INTERRUPT.h"

void INTERRUPT::write(uint16_t address, uint8_t value)
{
	switch (address)
	{
	case 0x46:
		printf("opcode:0x46 // DMA value:%04x\n", value);
		break;
	case 0x41:
		if (value > 0)
			printf("opcode:0x41 // LCD STATUS value:%04x\n", value);
		break;
	case 0x4D:
		if (value > 0)
			printf("opcode:0x4D // Speed Mode value:%04x\n", value);
		break;
	}
	//printf("write:");
	switch (0) {

	case 0x05: {
		printf("opcode:0x05 // TIMA value:%04x\n", value);
		break;
	} case 0x06: {
		printf("opcode:0x06 // TMA value:%04x\n", value);
		break;
	} case 0x07: {
		if ((value & 0x03) != (io[0x07] & 0x03))
		printf("opcode:0x07 // TAC value:%04x\n", value);
		break;
	} case 0x10: {
		printf("opcode:0x10 // NR10 value:%04x\n", value);
		break;
	} case 0x11: {
		printf("opcode:0x11 // NR11 value:%04x\n", value);
		break;
	} case 0x12: {
		printf("opcode:0x12 // NR12 value:%04x\n", value);
		break;
	} case 0x14: {
		printf("opcode:0x14 // NR14 value:%04x\n", value);
		break;
	} case 0x16: {
		printf("opcode:0x16 // NR21 value:%04x\n", value);
		break;
	} case 0x17: {
		printf("opcode:0x17 // NR22 value:%04x\n", value);
		break;
	} case 0x19: {
		printf("opcode:0x19 // NR24 value:%04x\n", value);
		break;
	} case 0x1A: {
		printf("opcode:0x1A // NR30 value:%04x\n", value);
		break;
	} case 0x1B: {
		printf("opcode:0x1B // NR31 value:%04x\n", value);
		break;
	} case 0x1C: {
		printf("opcode:0x1C // NR32 value:%04x\n", value);
		break;
	} case 0x1E: {
		printf("opcode:0x1E // NR33 value:%04x\n", value);
		break;
	} case 0x20: {
		printf("opcode:0x20 // NR41 value:%04x\n", value);
		break;
	} case 0x21: {
		printf("opcode:0x21 // NR42 value:%04x\n", value);
		break;
	} case 0x22: {
		printf("opcode:0x22 // NR43 value:%04x\n", value);
		break;
	} case 0x23: {
		printf("opcode:0x23 // NR30 value:%04x\n", value);
		break;
	} case 0x24: {
		printf("opcode:0x24 // NR50 value:%04x\n", value);
		break;
	} case 0x25: {
		printf("opcode:0x25 // NR51 value:%04x\n", value);
		break;
	} case 0x26: {
		printf("opcode:0x26 //-GB, 0xF0-SGB ;// NR52 value:%04x\n", value);
		break;
	} case 0x40: {
		printf("opcode:0x40 // LCDC value:%04x\n", value);
		break;
	} case 0x41: {
		printf("opcode:0x41 // STAT - LCDC Status  value:%04x\n", value);
		break;
	} case 0x42: {
		printf("opcode:0x42 // SCY value:%04x\n", value);
		break;
	} case 0x43: {
		printf("opcode:0x43 // SCX value:%04x\n", value);
		break;
	} case 0x45: {
		printf("opcode:0x45 // LYC value:%04x\n", value);
		break;
	} case 0x46: {
		printf("opcode:0x46 // DMA value:%04x\n", value);
		break;

	} case 0x47: {
		printf("opcode:0x47 // BGP value:%04x\n", value);
		break;
	} case 0x48: {
		printf("opcode:0x48 // OBP0 value:%04x\n", value);
		break;
	} case 0x49: {

		printf("opcode:0x49 // OBP1 value:%04x\n", value);
		break;
	} case 0x4A: {
		printf("opcode:0x4A // WY value:%04x\n", value);
		break;
	} case 0x4B: {
		printf("opcode:0x4B // W value:%04x\n", value);
		break;
	case 0x68: {
		//int x = 0xff80;
		//uint8_t index = io[0x68] & 0x3f;
		//uint8_t n = bus->mmu->read(index);//address a byte in the CGBs Background Palette Memory
		//bus->mmu->write(index, value);
		//if ((io[0x68] >> 7) & 0x01) {
		//	io[0x68]= (io[0x68] >> 6) |(index+1);
		//}
		printf("opcode:0x4B //  value:%04x\n", value);
		break;
	}
	case 0x69: {
		//int x = 0xff80;
		//uint8_t index = io[0x68] & 0x3f;
		//uint8_t n = bus->mmu->read(index);//address a byte in the CGBs Background Palette Memory
		//bus->mmu->write(index, value);
		//if ((io[0x68] >> 7) & 0x01) {
		//	io[0x68]= (io[0x68] >> 6) |(index+1);
		//}
		printf("opcode:0x4B //  value:%04x\n", value);
		break;
	}
	}

	}
	if (address < 0x80)
		switch (address)
		{
		case 0x04:
			io[0x04] = 0;
			bus->cpu->updateCycelPerIncrementTIMA(io[0x07] & 0x03);
			break;
		case 0x44:
			io[0x44] = 0;
			break;
		case 0x46:
			bus->dma->transfer(value);
			break;
		case 0x07:
			if ((value & 0x03) != (io[0x07] & 0x03))
				bus->cpu->updateCycelPerIncrementTIMA(value & 0x03);
			printf("opcode:0x07 // TAC value:%04x\n", value);
		default:
			io[address] = value;
		}

}

uint8_t INTERRUPT::read(uint16_t address)
{
	if (address == 0x44)
		int y = 0;
	if (address < 0x80)
		return io[address];
	else {
		printf("CARTRIDGE::read illeal address!");
		return NULL;
	}

	//printf("read:");
	switch (0) {
	case 0x05: {
		printf("opcode:0x05 // TIMA\n");
		break;
	} case 0x06: {
		printf("opcode:0x06 // TMA\n");
		break;
	} case 0x07: {
		printf("opcode:0x07 // TAC\n");
		break;
	} case 0x10: {
		printf("opcode:0x10 // NR10\n");
		break;
	} case 0x11: {
		printf("opcode:0x11 // NR11\n");
		break;
	} case 0x12: {
		printf("opcode:0x12 // NR12\n");
		break;
	} case 0x14: {
		printf("opcode:0x14 // NR14\n");
		break;
	} case 0x16: {
		printf("opcode:0x16 // NR21\n");
		break;
	} case 0x17: {
		printf("opcode:0x17 // NR22\n");
		break;
	} case 0x19: {
		printf("opcode:0x19 // NR24\n");
		break;
	} case 0x1A: {
		printf("opcode:0x1A // NR30\n");
		break;
	} case 0x1B: {
		printf("opcode:0x1B // NR31\n");
		break;
	} case 0x1C: {
		printf("opcode:0x1C // NR32\n");
		break;
	} case 0x1E: {
		printf("opcode:0x1E // NR33\n");
		break;
	} case 0x20: {
		printf("opcode:0x20 // NR41\n");
		break;
	} case 0x21: {
		printf("opcode:0x21 // NR42\n");
		break;
	} case 0x22: {
		printf("opcode:0x22 // NR43\n");
		break;
	} case 0x23: {
		printf("opcode:0x23 // NR30\n");
		break;
	} case 0x24: {
		printf("opcode:0x24 // NR50\n");
		break;
	} case 0x25: {
		printf("opcode:0x25 // NR51\n");
		break;
	} case 0x26: {
		printf("opcode:0x26 //-GB, 0xF0-SGB ;// NR52\n");
		break;
	} case 0x40: {
		printf("opcode:0x40 // LCDC\n");
		break;
	} case 0x42: {
		printf("opcode:0x42 // SCY\n");
		break;
	} case 0x43: {
		printf("opcode:0x43 // SCX\n");
		break;
	} case 0x45: {
		printf("opcode:0x45 // LYC\n");
		break;
	} case 0x47: {
		printf("opcode:0x47 // BGP\n");
		break;
	} case 0x48: {
		printf("opcode:0x48 // OBP0\n");
		break;
	} case 0x49: {

		printf("opcode:0x49 // OBP1\n");
		break;
	} case 0x4A: {
		printf("opcode:0x4A // WY\n");
		break;
	} case 0x4B: {
		printf("opcode:0x4B // W\n");
		break;
	}
	}
}

void INTERRUPT::setInterruptRequest(uint8_t bit)
{
	io[0x0f] |= (0x1 << bit);
}

void INTERRUPT::resetInterruptRequest(uint8_t bit)
{
	io[0x0f] &= ((0x1 << bit) ^ 0xff);
}
bool INTERRUPT::isInterruptRequsted(uint8_t bit)
{
	return (io[0x0f] >> bit) & 0x01;
}
void INTERRUPT::setInterruptEnabledRegister(uint8_t bit)
{
	InterruptEnabledRegister |= (0x1 << bit);
}

void INTERRUPT::resetInterruptEnabledRegister(uint8_t bit)
{
	InterruptEnabledRegister &= ((0x1 << bit) ^ 0xff);
}
bool INTERRUPT::isInterruptEnable(uint8_t bit)
{
	return (InterruptEnabledRegister >> bit) & 0x01;
}

uint8_t INTERRUPT::InterruptsHandler()
{
	uint8_t cycles = 0;
	for (int i = 0;i < 5;i++) {
		if (isInterruptRequsted(i) && isInterruptEnable(i)) {
			
			if (bus->cpu->IME) {
				bus->cpu->IME = false;
				resetInterruptRequest(i);
				InterruptHandler(i);
				return 5;
			}
			if (bus->cpu->halt) {
				bus->cpu->halt = false;
				cycles = 0;
			}
		}
	}
	if (bus->cpu->setIME) {
		bus->cpu->setIME = false;
		bus->cpu->IME = true;
	}
	return cycles;
}
void INTERRUPT::InterruptHandler(uint8_t interrupt)
{
	uint8_t interruptHandlerAdd = 0;
	switch (interrupt) {
	case 0:
		interruptHandlerAdd = 0x40;
		break;
	case 1:
		interruptHandlerAdd = 0x48;
		break;
	case 2:
		interruptHandlerAdd = 0x50;
		break;
	case 3:
		interruptHandlerAdd = 0x58;
		break;
	case 4:
		interruptHandlerAdd = 0x60;
		break;
	}
	if (bus->cpu->halt)
		bus->cpu->PC++;
	bus->cpu->PUSH_nn(((uint8_t*)&bus->cpu->PC), NULL);
	bus->cpu->PC = interruptHandlerAdd;
	//bus->cpu->lastOpcodeCycles += 5;
	
}
//uint8_t* INTERRUPT::getMemCell(uint16_t address)
//{
//	if (address < 0x80)
//		return io + address;
//	printf("getMemCell:");
//	switch (address) {
//	case 0x05: {
//		printf("opcode:0x05 // TIMA\n");
//		break;
//	} case 0x06: {
//		printf("opcode:0x06 // TMA\n");
//		break;
//	} case 0x07: {
//		printf("opcode:0x07 // TAC\n");
//		break;
//	} case 0x10: {
//		printf("opcode:0x10 // NR10\n");
//		break;
//	} case 0x11: {
//		printf("opcode:0x11 // NR11\n");
//		break;
//	} case 0x12: {
//		printf("opcode:0x12 // NR12\n");
//		break;
//	} case 0x14: {
//		printf("opcode:0x14 // NR14\n");
//		break;
//	} case 0x16: {
//		printf("opcode:0x16 // NR21\n");
//		break;
//	} case 0x17: {
//		printf("opcode:0x17 // NR22\n");
//		break;
//	} case 0x19: {
//		printf("opcode:0x19 // NR24\n");
//		break;
//	} case 0x1A: {
//		printf("opcode:0x1A // NR30\n");
//		break;
//	} case 0x1B: {
//		printf("opcode:0x1B // NR31\n");
//		break;
//	} case 0x1C: {
//		printf("opcode:0x1C // NR32\n");
//		break;
//	} case 0x1E: {
//		printf("opcode:0x1E // NR33\n");
//		break;
//	} case 0x20: {
//		printf("opcode:0x20 // NR41\n");
//		break;
//	} case 0x21: {
//		printf("opcode:0x21 // NR42\n");
//		break;
//	} case 0x22: {
//		printf("opcode:0x22 // NR43\n");
//		break;
//	} case 0x23: {
//		printf("opcode:0x23 // NR30\n");
//		break;
//	} case 0x24: {
//		printf("opcode:0x24 // NR50\n");
//		break;
//	} case 0x25: {
//		printf("opcode:0x25 // NR51\n");
//		break;
//	} case 0x26: {
//		printf("opcode:0x26 //-GB, 0xF0-SGB ;// NR52\n");
//		break;
//	} case 0x40: {
//		printf("opcode:0x40 // LCDC\n");
//		break;
//	} case 0x42: {
//		printf("opcode:0x42 // SCY\n");
//		break;
//	} case 0x43: {
//		printf("opcode:0x43 // SCX\n");
//		break;
//	} case 0x45: {
//		printf("opcode:0x45 // LYC\n");
//		break;
//	} case 0x47: {
//		printf("opcode:0x47 // BGP\n");
//		break;
//	} case 0x48: {
//		printf("opcode:0x48 // OBP0\n");
//		break;
//	} case 0x49: {
//		printf("opcode:0x49 // OBP1\n");
//		break;
//	} case 0x4A: {
//		printf("opcode:0x4A // WY\n");
//		break;
//	} case 0x4B: {
//		printf("opcode:0x4B // W\n");
//		break;
//	}
//	}
//}

void INTERRUPT::connectToBus(BUS* bus)
{
	this->bus = bus;
}

void INTERRUPT::reset()
{
	for (uint8_t i = 0;i < 0x80;i++)
		io[i] = 1;

	io[0] = 255;
	io[0x05] = 0x00;// TIMA
	io[0x06] = 0x00;// TMA
	io[0x07] = 0x00;// TAC
	io[0x10] = 0x80;// NR10
	io[0x11] = 0xBF;// NR11
	io[0x12] = 0xF3;// NR12
	io[0x14] = 0xBF;// NR14
	io[0x16] = 0x3F;// NR21
	io[0x17] = 0x00;// NR22
	io[0x19] = 0xBF;// NR24
	io[0x1A] = 0x7F;// NR30
	io[0x1B] = 0xFF;// NR31
	io[0x1C] = 0x9F;// NR32
	io[0x1E] = 0xBF;// NR33
	io[0x20] = 0xFF;// NR41
	io[0x21] = 0x00;// NR42
	io[0x22] = 0x00;// NR43
	io[0x23] = 0xBF;// NR30
	io[0x24] = 0x77;// NR50
	io[0x25] = 0xF3;// NR51
	io[0x26] = 0xF1;//-GB, 0xF0-SGB ;// NR52
	io[0x40] = 0x91;// LCDC
	io[0x42] = 0x00;// SCY
	io[0x43] = 0x00;// SCX
	io[0x45] = 0x00;// LYC
	io[0x47] = 0xFC;// BGP
	io[0x48] = 0xFF;// OBP0
	io[0x49] = 0xFF;// OBP1
	io[0x4A] = 0x00;// WY
	io[0x4B] = 0x00;// WX
	io[0x44] = 0x88;
	io[0x44] = 0x00;
	//io[0x40] = 0;// LCDC
	
}
