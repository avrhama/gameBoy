#include "BUS.h"
#include "CPU.h"
#include "MMU.h"
#include "CARTRIDGE.h"
#include "GPU.h"
#include "DMA.h"
#include "INTERRUPT.h"
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

void BUS::insertCartridge(CARTRIDGE* cartridge)
{
	this->cartridge = cartridge;
	cartridge->connectToBus(this);

}


