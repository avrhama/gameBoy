#include "BUS.h"
#include "CPU.h"
#include "MMU.h"
#include "CARTRIDGE.h"
#include "GPU.h"
#include "DMA.h"
#include "INTERRUPT.h"
#include "JOYPAD.h"
void BUS::connectCPU(CPU* cpu)
{
	this->cpu = cpu;
	cpu->connectToBus(this);
	
}

void BUS::connectMMU(MMU* mmu)
{
	this->mmu = mmu;
	mmu->connectToBus(this);
}

void BUS::connectGPU(GPU* gpu)
{
	this->gpu=gpu;
	gpu->connectToBus(this);

}

void BUS::connectDMA(DMA* dma)
{
	this->dma = dma;
	dma->connectToBus(this);
}

void BUS::connectDisplay(DISPLAY* display)
{
	this->display = display;
}

void BUS::connectInterrupt(INTERRUPT* interrupt)
{
	this->interrupt = interrupt;
	interrupt->connectToBus(this);
}
void BUS::connectJoypad(JOYPAD* joypad)
{
	this->joypad = joypad;
	joypad->connectToBus(this);

}

void BUS::connectCartridge(CARTRIDGE* cartridge)
{
	this->cartridge = cartridge;
	cartridge->connectToBus(this);

}

