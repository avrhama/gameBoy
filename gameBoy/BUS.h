#pragma once
using namespace std;
class CPU;
class MMU;
class CARTRIDGE;
class GPU;
class DMA;
class DISPLAY;
class INTERRUPT;
class pipeChannel;
class BUS
{
public:
	bool pipeEnable;
	pipeChannel *p;
	CPU* cpu;
	MMU* mmu;
	CARTRIDGE* cartridge;
	GPU* gpu;
	DMA* dma;
	DISPLAY* display;
	INTERRUPT* interrupt;
	void connectCPU(CPU* cpu);
	void connectMMU(MMU* mmu);
	void connectGPU(GPU* gpu);
	void connectDMA(DMA* dma);
	void connectDisplay(DISPLAY* display);
	void connectInterrupt(INTERRUPT* interrupt);
	void insertCartridge(CARTRIDGE* cartridge);
};

