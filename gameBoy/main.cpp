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
#include <thread>
#include "Structures.h"
#include "pipeChannel.h"
#include <conio.h>
#include <SDL.h>
#undef main
using namespace std;

void pipeRecive(BUS* bus, uint16_t opcode,uint16_t lastOpcode, int steps,string funcName) {
	if (steps == 5) {
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
			errorCode = 2;
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
		if (errorCode!=-1) {//lastOpcode!=0xf0

			bus->p->wBuffer[0] = 1;
		}
		bus->p->write();
		if (bus->p->wBuffer[0] == 1) {
			int x = 0;
		}


	}
}
int main(void) {

	

	
	//SDL_DestroyTexture(texture);
	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);
	//SDL_Quit();
	//return EXIT_SUCCESS;
	//testRun();
	//return 0;
	string romsPaths[30] =
	{ 
	"test\\cpu_instrs\\cpu_instrs.gb",
	"test\\cpu_instrs\\individual\\01-special.gb",//good//blargg
	"test\\cpu_instrs\\individual\\02-interrupts.gb" ,//good
		"test\\cpu_instrs\\individual\\03-op sp,hl.gb",//faild
	"test\\cpu_instrs\\individual\\04-op r,imm.gb",//good
	"test\\cpu_instrs\\individual\\05-op rp.gb",//good
	"test\\cpu_instrs\\individual\\06-ld r,r.gb",//good
	"test\\cpu_instrs\\individual\\07-jr,jp,call,ret,rst.gb",//no answer-> crashed
	"test\\cpu_instrs\\individual\\08-misc instrs.gb",//good
	"test\\cpu_instrs\\individual\\09-op r,r.gb",//good
	"test\\cpu_instrs\\individual\\10-bit ops.gb",//good
	"test\\cpu_instrs\\individual\\11-op a,(hl).gb",//good
	"test2\\daa.gb",
	"roms\\alleyway.gb",
	 "roms\\megaman.gb",
	"roms\\pokemon.gb", 
	"roms\\tetris.gb",
	"test\\instr_timing\\instr_timing.gb",
	"roms\\mooneye-gb_hwtests\\acceptance\\timer\\div_write.gb",//faild
	"roms\\mooneye-gb_hwtests\\acceptance\\timer\\tima_reload.gb",
	"roms\\mooneye-gb_hwtests\\acceptance\\timer\\tima_write_reloading.gb",
	"roms\\mooneye-gb_hwtests\\acceptance\\timer\\tma_write_reloading.gb",
	"roms\\mooneye-gb_hwtests\\misc\\boot_regs-cgb.gb" };
	uint8_t romIndex = 18;
	//char * romPath = roms[5];
	
	//BC = 0x12FE;
	//*getA() = *getC();
	//printf("%02x\n", *getF());
	//printf("%02x\n", *getA());
	//return 0;
	//const char* romPath = "alleyway.gb";
	//const char* romPath = "megaman.gb";
	//const char* romPath = "test\\cpu_instrs\\individual\\03-op sp,hl.gb";
	//const char* romPath = "pokemon.gb";
	//
	BUS * bus=new BUS();
	CPU * cpu=new CPU();
	GPU * gpu=new GPU();
	DMA* dma = new DMA();
	CARTRIDGE* cartridge = new CARTRIDGE();
	INTERRUPT* interrupt = new INTERRUPT();
	MMU* mmu = new MMU();
	DISPLAY* display = new DISPLAY(0, 0, 160, 144, 1);
	JOYPAD* joypad = new JOYPAD();
	

	bus->connectCPU(cpu);
	bus->connectMMU(mmu);
	bus->connectInterrupt(interrupt);
	bus->connectGPU(gpu);
	bus->connectDMA(dma);
	bus->connectDisplay(display);
	bus->connectCartridge(cartridge);
	bus->connectJoypad(joypad);
	cartridge->loadRom(romsPaths[romIndex]);
	cartridge->load();
	cpu->reset();
	mmu->reset();
	gpu->reset();
	interrupt->reset();
	printf("title:%s\n", cartridge->title);
	printf("rom banks count:%d\n", cartridge->romSize);
	printf("romBank size:%d\n", cartridge->romBankSize);
	printf("ram banks count:%d\n", cartridge->ramSize);
	printf("romBank size:%d\n", cartridge->ramBankSize);
	
	
	ofstream myfile;
	myfile.open("C:\\Users\\Brain\\go\\src\\goboy\\goboy-0.4.2\\cmd\\goboy\\instructions_me.txt");
	
	
	//gpu.drawTest();

	//return 0;
	uint16_t opcode;
	int counter = 270274;
	char buff[100];
	unsigned char c = 0;
	bus->pipeEnable = false;
	pipeChannel p;
	bus->p = &p;
	bool f;
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
	int renderTimer = 69905;
	int renderCounter = 0;
	bool writeToFile = false;
	uint8_t last_key = 1;
	int x = 0;
	int y = 0;
	Scalar white = Scalar(255, 255, 255, 0);
	Scalar black = Scalar(0, 0, 0, 0);
	//std::thread displayThread(displayThreadFunc, display);
	int cyclesInFrameCounter = 0;
	int framesForSeconds = 60;
	int cyclesInFrame = cpu->cpuFreq/framesForSeconds;
	uint16_t lastopcode = 0;
	int steps = 0;
	
	
	while (true) {
		/*if (SDL_PollEvent(&display->windowEvent))
					if (SDL_QUIT == display->windowEvent.type) 
						break;*/
		do {
			opcode = cpu->getOpcode();
			opcode = (opcode == 0xCB) ? 0XCB00 | cpu->getOpcode() : opcode;			
			//cpu->Execute(opcode);
			//pipeRecive(bus, opcode, lastopcode, steps,"Execute");
			if (!cpu->halt) {
				cpu->ExecuteOpcode(opcode);
				steps++;
				pipeRecive(bus, opcode, lastopcode, steps, "Execute");
				//cpu->Execute(opcode);
			}
			else {
				cpu->lastOpcodeCycles = 1;
			}

			
			
			cpu->lastOpcodeCycles *=(4 * (cpu->speedMode + 1));
		
			
			//cpu->lastOpcodeCycles *=4;
			gpu->tick();
			cpu->updateTimers();
			
		
			joypad->updateKeys();
			cpu->lastOpcodeCycles += interrupt->InterruptsHandler()* (4 * (cpu->speedMode + 1));;
			
			//pipeRecive(bus, opcode, lastopcode, steps, "InterruptsHandler");
			
			cyclesInFrameCounter += cpu->lastOpcodeCycles;
			//printf("speed:%d\n", interrupt->io[0x4D]>>7);
			
			/*if (renderCounter == renderTimer)
				gpu->drawTest();*/

			/*if (cpu->cycelsCounter > 4)
				cpu->cycelsCounter = cpu->cycelsCounter%4;*/
			
			
			//cpu->cycelsCounter += cpu->lastOpcodeCycles;
		} while (cyclesInFrameCounter< cyclesInFrame); //(cpu.PC != 0x100 && counter > 0);//||mmu.biosLoaded);//cpu.PC!=0x100
		cyclesInFrameCounter = 0;
		//Sleep(1);
		//display->render();
		display->update();
		//printf("render\n");
	}

	myfile.close();
	

	while (true) {
		
		
		KEYS key=display->waitForKey();
		display->setPixel(x, y,black);
		if (display->keysMapper[KEYS::Up]->isPressed) {
			cout << "Up";
			y = (y - 1) % 144;
			y = y < 0 ? 144 + y : y;
			
		}else if (display->keysMapper[KEYS::Down]->isPressed) {
			cout << "Down";
			y = (y+ 1) % 144;
		}else if(display->keysMapper[KEYS::Left]->isPressed){
			cout << "Left";
			x = (x - 1) % 160;
			x = x < 0 ? 160 + x :x;
		}
		else if (display->keysMapper[KEYS::Right]->isPressed) {
			
			cout << "Right: ";
			x= (x + 1) % 160;
			
		}
		display->setPixel(x, y, white);
		Sleep(250);

		
	}
	
	
	return 0;

	}

void testRomSwitching(CARTRIDGE cartridge) {
	cartridge.write(0x2000, 0);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 20);
	cartridge.write(0x5fff, 0);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 40);
	cartridge.write(0x5fff, 1);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 60);
	cartridge.write(0x5fff, 1);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 1);
	cartridge.write(0x5fff, 0);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 21);
	cartridge.write(0x5fff, 0);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 41);
	cartridge.write(0x5fff, 41);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 61);
	cartridge.write(0x5fff, 1);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 2);
	cartridge.write(0x5fff, 0);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 22);
	cartridge.write(0x5fff, 0);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 42);
	cartridge.write(0x5fff, 1);
	printf("romBank:%d\n", cartridge.romBankIndex);
	cartridge.write(0x2000, 62);
	cartridge.write(0x5fff, 1);
	printf("romBank:%d\n", cartridge.romBankIndex);
}
int testPipe() {


	char c;
	bool f;
	pipeChannel p;
	cin >> c;
	if (c == 'c') {
		f = p.createPipe(1, "Pipe", 100, 100);
		printf("created?:%s\n", f ? "true" : "false");
		if (!f)
			f = p.wait();
		printf("connect?:%s\n", f ? "true" : "false");
		if (f) {
			p.wBuffer[0] = 'a';
			f = p.write(1);
			p.wBuffer[0] = 'b';
			f = p.write(1);
			printf("write?:%s\n", f ? "true" : "false");
		}
	}
	else {

		f = p.createPipe(0, "Pipe", 100, 100);
		if (!f)
			f = p.wait();
		printf("connect?:%s\n", f ? "true" : "false");
		if (f) {
			f = p.read();

			printf("read?:%s\n", f ? "true" : "false");
			if (f)
				printf("read:%c\n", p.rBuffer[0]);

		}

	}

	p.close();
	return 0;

}
void displayThreadFunc(DISPLAY* display)
{
	while (true) {
		display->update();
		//Sleep(16.6);
		Sleep(500);
	}
	// do stuff...
}

void tests(BUS  bus) {

	//testPipe();
}


void runTest(uint8_t romIndex) {
	string romsPaths[16] =
	{
	"test\\cpu_instrs\\cpu_instrs.gb",
	"test\\cpu_instrs\\individual\\01-special.gb",//good
	"test\\cpu_instrs\\individual\\02-interrupts.gb" ,//no answer
		"test\\cpu_instrs\\individual\\03-op sp,hl.gb",//no answer
	"test\\cpu_instrs\\individual\\04-op r,imm.gb",//good
	"test\\cpu_instrs\\individual\\05-op rp.gb",//good
	"test\\cpu_instrs\\individual\\06-ld r,r.gb",//good
	"test\\cpu_instrs\\individual\\07-jr,jp,call,ret,rst.gb",//no answer-> crashed
	"test\\cpu_instrs\\individual\\08-misc instrs.gb",//no answer
	"test\\cpu_instrs\\individual\\09-op r,r.gb",//faild
	"test\\cpu_instrs\\individual\\10-bit ops.gb",//good
	"test\\cpu_instrs\\individual\\11-op a,(hl).gb",
	"test2\\daa.gb",
	"alleyway.gb",
	 "megaman.gb",
	"pokemon.gb", };//faild

	BUS* bus = new BUS();
	CPU* cpu = new CPU();
	GPU* gpu = new GPU();
	gpu->reset();
	CARTRIDGE* cartridge = new CARTRIDGE();
	INTERRUPT* interrupt = new INTERRUPT();
	JOYPAD* joypad = new JOYPAD();
	interrupt->reset();
	MMU* mmu = new MMU();
	DISPLAY* display = new DISPLAY(0, 0, 160, 144, 1, romIndex);
	bus->connectCPU(cpu);
	bus->connectMMU(mmu);
	bus->connectInterrupt(interrupt);
	bus->connectGPU(gpu);
	bus->connectDisplay(display);
	bus->connectCartridge(cartridge);
	bus->connectJoypad(joypad);
	cartridge->loadRom(romsPaths[romIndex]);
	cartridge->load();
	cpu->reset();
	printf("title:%s\n", cartridge->title);
	printf("rom banks count:%d\n", cartridge->romSize);
	printf("romBank size:%d\n", cartridge->romBankSize);
	printf("ram banks count:%d\n", cartridge->ramSize);
	printf("romBank size:%d\n", cartridge->ramBankSize);





	//gpu.drawTest();

	//return 0;
	uint16_t opcode;
	int counter = 270274;
	bus->pipeEnable = true;
	pipeChannel p;
	bus->p = &p;

	int renderTimer = 69905;
	int renderCounter = 0;
	bool writeToFile = false;
	uint8_t last_key = 1;
	int x = 0;
	int y = 0;
	Scalar white = Scalar(255, 255, 255, 0);
	Scalar black = Scalar(0, 0, 0, 0);
	std::thread displayThread(displayThreadFunc, display);

	do {
		opcode = cpu->getOpcode();
		opcode = (opcode == 0xCB) ? 0XCB00 | cpu->getOpcode() : opcode;
		cpu->Execute(opcode);
		if (last_key != interrupt->io[0]) {
			printf("keys:%04x\n", interrupt->io[0]);
			last_key = interrupt->io[0];
		}
		cpu->updateTimers();
		gpu->tick();
		joypad->updateKeys();
		interrupt->InterruptsHandler();

	} while (true); //(cpu.PC != 0x100 && counter > 0);//||mmu.biosLoaded);//cpu.PC!=0x100
		/*std::thread displayThread1(runTest, 1);
	std::thread displayThread2(runTest, 2);
	std::thread displayThread3(runTest, 3);
	std::thread displayThread4(runTest, 4);
	std::thread displayThread5(runTest, 5);
	std::thread displayThread6(runTest, 6);
	std::thread displayThread7(runTest, 7);
	std::thread displayThread8(runTest, 8);
	std::thread displayThread9(runTest, 9);
	std::thread displayThread10(runTest, 10);
	std::thread displayThread11(runTest, 11);
	displayThread1.join();
	displayThread2.join();
	displayThread3.join();
	displayThread4.join();
	displayThread5.join();
	displayThread6.join();
	displayThread7.join();
	displayThread8.join();
	displayThread9.join();
	displayThread10.join();
	displayThread11.join();*/
}