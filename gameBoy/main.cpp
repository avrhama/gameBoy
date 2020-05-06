#include <iostream>
#include <memory.h>
#include<fstream>
#include<cstdint>
#include<stdlib.h>
#include <stdio.h> 
#include "CARTRIDGE.h"
#include "BUS.h"
#include <map>
#include "main.h"
#include "DISPLAY.h"
#include "MMU.h"
#include "INTERRUPT.h"
#include "JOYPAD.h"
#include "APU.h"
#include <thread>
#include "Structures.h"
#include "pipeChannel.h"
#include "CartridgeFactory.h"
#include <conio.h>
#include <SDL.h>
#include <SDL_thread.h>

//memory leak checking.
/*
#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
*/
#undef main
using namespace std;



bool openFile(string* romPath) {
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	wstring filename(MAX_PATH, L'\0');
	char f1[MAX_PATH];
	f1[0] = 0;
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrTitle = L"Select A Rom File";
	ofn.lpstrFilter = L"All Roms\0*.gbc;*.gb\0GB Roms\0*.gb\0GBC Roms\0*.gbc\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = &filename[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (::GetOpenFileName(&ofn) != FALSE)
	{
		const std::wstring ws = filename;
		const std::locale locale("");
		typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
		const converter_type& converter = std::use_facet<converter_type>(locale);
		std::vector<char> to(ws.length() * converter.max_length());
		std::mbstate_t state;
		const wchar_t* from_next;
		char* to_next;
		const converter_type::result result = converter.out(state, ws.data(), ws.data() + ws.length(), from_next, &to[0], &to[0] + to.size(), to_next);
		if (result == converter_type::ok or result == converter_type::noconv) {
			const std::string s(&to[0], to_next);
			*romPath = s;
			return true;
		}
	}
	return false;
}

//debugging func.
void pipeRecive(BUS* bus, uint16_t opcode, uint16_t lastOpcode, int steps, string funcName) {
	if (steps == 123176) {
		int h = 0;
	}
	if (bus->pipeEnable) {
		bus->p->read(14);
		uint16_t otherOp = bus->p->rBuffer[1];
		if (bus->p->rBuffer[0] == 0xCB) {
			otherOp = 0xCB00 | bus->p->rBuffer[1];
		}
		uint16_t otherPC = bus->p->rBuffer[2] << 8 | bus->p->rBuffer[3];

		uint16_t otherAF = bus->p->rBuffer[4] << 8 | bus->p->rBuffer[5];
		uint16_t otherBC = bus->p->rBuffer[6] << 8 | bus->p->rBuffer[7];
		uint16_t otherDE = bus->p->rBuffer[8] << 8 | bus->p->rBuffer[9];
		uint16_t otherHL = bus->p->rBuffer[10] << 8 | bus->p->rBuffer[11];
		uint16_t otherSP = bus->p->rBuffer[12] << 8 | bus->p->rBuffer[13];
		int errorCode = -1;
		if (otherOp != opcode) {
			errorCode = 0;
		}
		if (otherPC != bus->cpu->PC) {
			errorCode = 1;
		}
		if (otherAF != bus->cpu->AF) {
			//errorCode = 2;
			int y = 0;
		}
		if (otherBC != bus->cpu->BC) {
			errorCode = 3;
		}
		if (otherDE != bus->cpu->DE) {
			errorCode = 4;
		} if (otherHL != bus->cpu->HL) {
			errorCode = 5;
		}
		if (otherSP != bus->cpu->SP) {
			errorCode = 6;
		}
		/*for (int i = 0;i < 0xffff;i++) {
			if (bus->p->rBuffer[14 + i] != bus->mmu->read(i)) {
				errorCode = 14 + i;
				break;
			}
		}*/
		bus->p->wBuffer[0] = 0;
		if (errorCode != -1) {//lastOpcode!=0xf0

			bus->p->wBuffer[0] = 1;
		}
		bus->p->write();
		if (bus->p->wBuffer[0] == 1) {
			int x = 0;
		}


	}
}
//display render thread's func.(currently not in used)
static int renderDisplay(void* ptr) {
	DISPLAY* display=(DISPLAY*)ptr;
	do {
		display->render();
		Sleep(1);
	} while (display->displayLock);
	/*MBC5* mbc = (MBC5*)ptr;
	printf("save ram thread starts\n");
	string saveFile = "roms\\pokemon.sav";
	uint8_t wait = 0;
	do {
		Sleep(10);
		mbc->saveRamMutex.lock();
		mbc->saveRamWait = mbc->saveRamWait - 1;
		wait = mbc->saveRamWait;
		mbc->saveRamMutex.unlock();
		
	} while (wait > 0);

	ofstream myFile(saveFile, ios::out | ios::binary);
	myFile.write((char*)mbc->ram, mbc->header.ramSize);*/
	//printf("save ram thread ended\n");
	return 0;
}
void freeEmulator(BUS* bus);
int main(void) {
	//memory leak checking
    /*_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);*/

	uint8_t romIndex;
	string romPath;
	bool openFileDialog = true;
	if (openFileDialog&&!openFile(&romPath)) {
		return 1;
	}

	BUS* bus = new BUS();
	CPU* cpu = new CPU();
	GPU* gpu = new GPU();
	DMA* dma = new DMA();
	CARTRIDGE* cartridge = createCartrige(romPath);
	if (cartridge == NULL) {
		freeEmulator(bus);
		return 1;
	}
	INTERRUPT* interrupt = new INTERRUPT();
	MMU* mmu = new MMU();
	DISPLAY* display = new DISPLAY(0, 0, 160, 144, 1);
	JOYPAD* joypad = new JOYPAD();
	APU* apu =new APU();

	bus->connectCPU(cpu);
	bus->connectMMU(mmu);
	bus->connectInterrupt(interrupt);
	bus->connectGPU(gpu);
	bus->connectDMA(dma);
	bus->connectDisplay(display);
	bus->connectCartridge(cartridge);
	bus->connectJoypad(joypad);
	bus->connectAPU(apu);
	cpu->reset();
	mmu->reset();
	gpu->reset();
	interrupt->reset();
	apu->start();
	apu->mute = false;
	printf("title:%s\n", cartridge->header.title);
	printf("rom banks count:%d\n", cartridge->header.romBanksCount);
	printf("romBank size:%d\n", cartridge->header.romBankSize);
	printf("ram banks count:%d\n", cartridge->header.ramBanksCount);
	printf("ram size:%d\n", cartridge->header.ramSize);

	uint16_t opcode;
	int counter = 270274;
	char buff[100];
	unsigned char c = 0;
	bus->pipeEnable = false;
	//debugging tool
	pipeChannel p;
	bus->p = &p;
	bool f;
	//debugging tool.
	if (bus->pipeEnable) {
		f = p.createPipe(0, "Pipe", 100, 100);

		if (!f) {
			printf("waiting...\n");
			f = p.wait();
		}
		printf("connect?:%s\n", f ? "true" : "false");
		if (!f) {
			printf("faild to open pipe\n");
			return 0;
		}
	}

	int renderCounter = 0;
	bool running = true;


	int cyclesInFrameCounter = 0;
	int framesForSeconds = 60;
	int cyclesInFrame = cpu->cpuFreq / framesForSeconds;
	uint16_t lastopcode = 0;
	while (true) {
		if (SDL_PollEvent(&display->windowEvent))
			if (SDL_QUIT == display->windowEvent.type)
			{
				running = false;
				break;
			}
		int timeBefore = SDL_GetTicks();
		do {
			if (!cpu->halt) {
				opcode = cpu->getOpcode();
				opcode = (opcode == 0xCB) ? 0XCB00 | cpu->getOpcode() : opcode;
				cpu->ExecuteOpcode(opcode);
			}
			else {
				cpu->lastOpcodeCycles = 1;
			}
			//cpu->lastOpcodeCycles *= 4;
			cpu->lastOpcodeCycles *= (4 * (cpu->speedMode + 1));

			cpu->steps+= cpu->lastOpcodeCycles;
			apu->tick();
			gpu->tick();
			cpu->updateTimers();


		    joypad->updateKeys();
			cpu->lastOpcodeCycles += interrupt->InterruptsHandler() * (4 * (cpu->speedMode + 1));
			//cpu->lastOpcodeCycles += interrupt->InterruptsHandler()*4;
			cyclesInFrameCounter += cpu->lastOpcodeCycles;
		} while ((cyclesInFrameCounter < (cyclesInFrame * (cpu->speedMode + 1))));
		cyclesInFrameCounter = 0;
		
			display->render();
			int timePassed = SDL_GetTicks() - timeBefore;
			int sleep = 15 - timePassed;
		
		/*if (sleep > 0)
			Sleep(sleep);*/
	
	}
	apu->close();
	display->close(true);

	bus->p->close();

	freeEmulator(bus);

	//memoty leak checking
	//_CrtDumpMemoryLeaks();
	
	return 0;

}
void freeEmulator(BUS* bus) {
	
	delete bus->cpu;
	delete  bus->gpu;
	delete  bus->dma;
	delete  bus->cartridge;
	delete  bus->interrupt;
	delete  bus->mmu;
	delete  bus->display;
	delete  bus->joypad;

	delete  bus->apu;
	delete bus;
}

