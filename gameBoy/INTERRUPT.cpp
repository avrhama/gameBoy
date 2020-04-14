#include "INTERRUPT.h"

void INTERRUPT::write(uint16_t address, uint8_t value)
{
	bool print = false;
	switch (address)
	{
	case 0x46:
		if(print)
		printf("write opcode:0x46 // DMA value:%04x\n", value);
		break;
	case 0x41:
		if (value > 0)
			if (print)
			printf("write opcode:0x41 // LCD STATUS value:%04x\n", value);
		break;
	case 0x4D:
		if (value > 0)
			if (print)
			printf("write opcode:0x4D // Speed Mode value:%04x\n", value);
		break;
	}
	if (address < 0x80)
		switch (address)
		{
		case 0x04:
			io[0x04] = 0;
			bus->cpu->cyclesPerIncrementDIVIDER = 256;
			bus->cpu->updateCycelPerIncrementTIMA(io[0x07] & 0x03);
			break;
		case 0x07:
			if ((value & 0x03) != (io[0x07] & 0x03)) {
				bus->cpu->updateCycelPerIncrementTIMA(value & 0x03);
			}
			io[0x07] = value | 0xf8;
			//printf("opcode:0x07 // TAC value:%04x\n", value);
			break;
		case 0x0f:
			io[0x0f] = value | 0xE0;
			//printf("opcode:0x0f // IE value:%04x\n", value);
			break;
		case 0x41:
			io[0x41] = value|0x80;
			break;
		case 0x44:
			io[0x44] = 0;
			break;
		case 0x46:
			bus->dma->transfer(value);
			break;
		case 0x4f:
		//	if(bus->cartridge->colorGB){
				bus->gpu->vRamBank = value & 0x01;
				io[address] = 0xfe|value;
				if (print)
				printf("write opcode:0x4f // VRAM Bank value:%04x value(io):%04x\n", value, io[address]);
				break;
		//	}
		case 0x55:
			if (print)
			printf("write opcode:0x55 // Start DMA Transfer value:%04x value(io):%04x\n", value, io[address]);
		case 0x70:
			
			bus->mmu->workingRamBank = value & 0x07;
			if ( bus->mmu->workingRamBank == 0)
				bus->mmu->workingRamBank = 1;
			io[address] = value;
			if (print)
			printf("write opcode:0x70 // WRAM Bank value:%04x\n", value);
			break;
		case 0x76://read 0nly
		case 0x77://read only
			return;
			break;
		default:
			io[address] = value;
		}

}

uint8_t INTERRUPT::read(uint16_t address)
{
	bool print = false;
	if (address == 0x55)
		if (print)
		printf("read dma?\n");
	if (address < 0x80)
		return io[address];
	else {
		printf("CARTRIDGE::read illeal address!");
		return NULL;
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
			bus->cpu->halt = false;
			if (bus->cpu->IME) {
				
				bus->cpu->IME = false;
				resetInterruptRequest(i);
				InterruptHandler(i);
				return 4;
			}
		/*	if (bus->cpu->halt) {
				bus->cpu->halt = false;
				
			}*/
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
	bool print = true;
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

	switch (interrupt) {
	case 0:
		printf("vBlacking interrupt\n");
		break;
	case 1:
		printf("lcd stat interrupt\n");
		break;
	case 2:
		printf("timer interrupt\n");
		break;
	case 3:
		printf("serial interrupt\n");
		break;
	case 4:
		printf("joypad interrupt\n");
		break;
	}

	/*if (bus->cpu->halt)
		bus->cpu->PC++;*/
	bus->cpu->PUSH_nn(((uint8_t*)&bus->cpu->PC), NULL);
	bus->cpu->PC = interruptHandlerAdd;
	//bus->cpu->lastOpcodeCycles += 5;

}


void INTERRUPT::connectToBus(BUS* bus)
{
	this->bus = bus;
}

void INTERRUPT::reset()
{
	int stage = 2;
	switch (stage) {
	case 0:
		for (uint8_t i = 0;i < 0x80;i++)
			io[i] = 1;

		io[0] = 0xff;
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
		//io[0x44] = 0x88;
		io[0x44] = 0x00;

		//io[0x40] = 0;// LCDC

		
		break;
	case 1:
		for (uint8_t i = 0;i < 0x80;i++)
			io[i] = 1;
		io[0x04] = 0x1E;
		io[0x05] = 0x00;
		io[0x06] = 0x00;
		io[0x07] = 0xF8;
		io[0x0F] = 0xE1;
		io[0x10] = 0x80;
		io[0x11] = 0xBF;
		io[0x12] = 0xF3;
		io[0x14] = 0xBF;
		io[0x16] = 0x3F;
		io[0x17] = 0x00;
		io[0x19] = 0xBF;
		io[0x1A] = 0x7F;
		io[0x1B] = 0xFF;
		io[0x1C] = 0x9F;
		io[0x1E] = 0xBF;
		io[0x20] = 0xFF;
		io[0x21] = 0x00;
		io[0x22] = 0x00;
		io[0x23] = 0xBF;
		io[0x24] = 0x77;
		io[0x25] = 0xF3;
		io[0x26] = 0xF1;
		io[0x40] = 0x91;
		io[0x41] = 0x85;
		io[0x42] = 0x00;
		io[0x43] = 0x00;
		io[0x45] = 0x00;
		io[0x47] = 0xFC;
		io[0x48] = 0xFF;
		io[0x49] = 0xFF;
		io[0x4A] = 0x00;
		io[0x4B] = 0x00;
		io[0x4d] = 0x7e;

		if (bus->cartridge->colorGB) {
			io[0x04] = 0x1E;
			bus->cpu->cyclesPerIncrementDIVIDER = 256 - 0xA0;
			bus->cpu->cyclesPerIncrementDIVIDER = 0xA0;
		}
		else {
			io[0x04] = 0x26;
			bus->cpu->cyclesPerIncrementDIVIDER = 256 - 0x7c;
			bus->cpu->cyclesPerIncrementDIVIDER = 0x7c;
		}
		break;
	case 2:
		for (uint8_t i = 0;i < 0x80;i++)
			io[i] = 0xff;
		io[0x05] = 0x00;//TIMA
		io[0x06] = 0x00;//TMA
		io[0x07] = 0x00;//TAC
		io[0x10] = 0x80;//NR10
		io[0x11] = 0xBF;//NR11
		io[0x12] = 0xF3;//NR12
		io[0x14] = 0xBF;//NR14
		io[0x16] = 0x3F;//NR21
		io[0x17] = 0x00;//NR22
		io[0x19] = 0xBF;//NR24
		io[0x1A] = 0x7F;//NR30
		io[0x1B] = 0xFF;//NR31
		io[0x1C] = 0x9F;//NR32
		io[0x1E] = 0xBF;//NR33
		io[0x20] = 0xFF;//NR41
		io[0x21] = 0x00;//NR42
		io[0x22] = 0x00;//NR43
		io[0x23] = 0xBF;//NR30
		io[0x24] = 0x77;//NR50
		io[0x25] = 0xF3;//NR51
		io[0x26] = 0xF1;//-GB,0xF0-SGB NR52
		io[0x40] = 0x91;//LCDC
		io[0x42] = 0x00;//SCY
		io[0x43] = 0x00;//SCX
		io[0x45] = 0x00;//LYC
		io[0x47] = 0xFC;//BGP
		io[0x48] = 0xFF;//OBP0
		io[0x49] = 0xFF;//OBP1
		io[0x4A] = 0x00;//WY
		io[0x4B] = 0x00;//WX

		
		io[0x72] = 0x00;// - Undocumented(00h) - Bit 0 - 7 (Read / Write)
		io[0x73] = 0x00;// - Undocumented(00h) - Bit 0 - 7 (Read / Write)
		
		io[0x75] = 0x8F;// - Undocumented(8Fh) - Bit 4 - 6 (Read / Write)
		io[0x76] = 0x00;// - Undocumented(00h) - Always 00h(Read Only)
		io[0x77] = 0x00;// - Undocumented(00h) - Always 00h(Read Only)
		//if (bus->cartridge->colorGB) {
			io[0x04] = 0x1E;
			bus->cpu->cyclesPerIncrementDIVIDER = 255 - 0xA0;
			bus->cpu->cyclesPerIncrementDIVIDER = 0xA0;
			io[0x6C] = 0xFE;// Undocumented(FEh) - Bit 0 (Read / Write) - CGB Mode Only
			io[0x74] = 0x00;// - Undocumented(00h) - Bit 0 - 7 (Read / Write) - CGB Mode Only
		//}
		//else {
		//	io[0x04] = 0x26;
		//	bus->cpu->cyclesPerIncrementDIVIDER = 255 - 0x7c;
		//	bus->cpu->cyclesPerIncrementDIVIDER = 0x7c;
		//	io[0x6C] = 0xFF;// Undocumented(FEh) - Bit 0 (Read / Write) - CGB Mode Only
		//	io[0x74] = 0xFF;// - Undocumented(00h) - Bit 0 - 7 (Read / Write) - CGB Mode Only
		//}

		break;
	
	}


}
