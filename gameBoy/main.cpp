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
#include <thread> 
using namespace std;

void displayThreadFunc(DISPLAY display)
{
	while (true) {
		display.update();
	}
	// do stuff...
}
void a(uint16_t* p) {
	*(uint8_t*)p = 4;
	//*p = 4;
}
int main(void) {
	//const char* romPath = "alleyway.gb";
	const char* romPath = "megaman.gb";
	//const char* romPath = "cpu_instrs.gb";
	BUS  bus;
	CPU cpu;
	GPU gpu;
	gpu.reset();
	CARTRIDGE cartridge;
	MMU mmu;
	DISPLAY display(0, 0, 160, 144, 1);
	bus.connectCPU(&cpu);
	bus.connectMMU(&mmu);
	bus.connectGPU(&gpu);
	bus.connectDisplay(&display);
	bus.insertCartridge(&cartridge);
	cartridge.loadRom(romPath);
	cartridge.load();
	printf("title:%s\n", cartridge.title);
	//gpu.drawTest();

	//return 0;
	uint8_t opcode;
	int n = 10;
	do {
		opcode = *(uint8_t*)cpu.getN();
		if (opcode == 0xcb)
			opcode = opcode == 0xCB ? 0XCB00 | *(uint8_t*)cpu.getN() : opcode;
		
		/*if(mmu.read(0xFF02)==0x81)
			printf("%d\n", mmu.read(0xFF01));*/
		if (opcode != 0) {
			 //printf("lineScan:%d LCD:%d\n", mmu.read(0xFF44), mmu.read(0xFF40));
			//printf("first opcode:%u\n", opcode);
			n--;
		}
		cpu.Execute(opcode);


	} while (cpu.PC != 0x100||n>0);//cpu.PC!=0x100

	
	std::thread displayThread(displayThreadFunc, display);
	int x=0;
	int y = 0;
	Scalar white = Scalar(255, 255, 255, 0);
	Scalar black = Scalar(0, 0, 0,0);
	while (true) {
		
		
		KEYS key=display.waitForKey();
		display.setPixel(x, y,black);
		if (display.keysMapper[Up]->isPressed) {
			cout << "Up";
			y = (y - 1) % 144;
			y = y < 0 ? 144 + y : y;
			
		}else if (display.keysMapper[Down]->isPressed) {
			cout << "Down";
			y = (y+ 1) % 144;
		}else if(display.keysMapper[Left]->isPressed){
			cout << "Left";
			x = (x - 1) % 160;
			x = x < 0 ? 160 + x :x;
		}
		else if (display.keysMapper[Right]->isPressed) {
			
			cout << "Right: ";
			x= (x + 1) % 160;
			
		}
		display.setPixel(x, y, white);
		Sleep(250);

		
	}
	
	
	return 0;

	}