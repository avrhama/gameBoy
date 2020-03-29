#pragma once
using namespace std;
class CPU;
class MMU;
class CARTRIDGE;
class GPU;
class DMA;
class BUS
{
public:
	CPU* cpu;
	MMU* mmu;
	CARTRIDGE* cartridge;
	GPU* gpu;
	DMA* dma;
	void connectCPU(CPU* cpu);
	void connectMMU(MMU* mmu);
	void connectGPU(GPU* gpu);
	void connectDMA(DMA* dma);
	void insertCartridge(CARTRIDGE* cartridge);
};

