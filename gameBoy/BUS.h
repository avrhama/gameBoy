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
class JOYPAD;
class APU;
class BUS
{
public:
	double r = 0;
	int nums[18] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072 };
	int h = 1;
	int f = 131072;
	int s = 256;
	bool modified = false;
	bool pipeEnable;
	pipeChannel *p;
	CPU* cpu;
	MMU* mmu;
	CARTRIDGE* cartridge;
	GPU* gpu;
	DMA* dma;
	DISPLAY* display;
	INTERRUPT* interrupt;
	JOYPAD* joypad;
	APU* apu;
	void connectCPU(CPU* cpu);
	void connectMMU(MMU* mmu);
	void connectGPU(GPU* gpu);
	void connectDMA(DMA* dma);
	void connectDisplay(DISPLAY* display);
	void connectInterrupt(INTERRUPT* interrupt);
	void connectJoypad(JOYPAD* joypad);
	void connectAPU(APU* apu);
	void connectCartridge(CARTRIDGE* cartridge);
};

