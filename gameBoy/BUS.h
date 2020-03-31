#pragma once
using namespace std;
class CPU;
class MMU;
class CARTRIDGE;
class GPU;
class DMA;
class DISPLAY;
class BUS
{
public:
	CPU* cpu;
	MMU* mmu;
	CARTRIDGE* cartridge;
	GPU* gpu;
	DMA* dma;
	DISPLAY* display;
	void connectCPU(CPU* cpu);
	void connectMMU(MMU* mmu);
	void connectGPU(GPU* gpu);
	void connectDMA(DMA* dma);
	void connectDisplay(DISPLAY* display);
	void insertCartridge(CARTRIDGE* cartridge);
};

