#pragma once
using namespace std;
#include "Structures.h"

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
	bool pipeEnable;
	pipeChannel *p;
	ConsoleModel consoleModel = ConsoleModel::GBC;
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

