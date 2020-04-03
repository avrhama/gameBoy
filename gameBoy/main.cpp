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
#include <thread>
#include "pipeChannel.h"
#include <conio.h>
using namespace std;
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
				p.wBuffer[0] ='a';
				f=p.write(1);
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
void displayThreadFunc(DISPLAY display)
{
	while (true) {
		display.update();
	}
	// do stuff...
}
uint16_t AF = 0;
uint8_t* getA() {
	return (uint8_t*)&AF + 1;
}
uint8_t* getF() {
	return (uint8_t*)&AF;
}
uint16_t BC = 0;
uint8_t* getB() {
	return (uint8_t*)&BC + 1;
}
uint8_t* getC() {
	return (uint8_t*)&BC;
}
int main(void) {
	//testPipe();
	//return 0;
	AF = 0xABCD;
	//BC = 0x12FE;
	//*getA() = *getC();
	//printf("%02x\n", *getF());
	//printf("%02x\n", *getA());
	//return 0;
	//const char* romPath = "alleyway.gb";
	//const char* romPath = "megaman.gb";
	const char* romPath = "test\\cpu_instrs\\individual\\03-op sp,hl.gb";
	BUS  bus;
	CPU cpu;
	GPU gpu;
	gpu.reset();
	CARTRIDGE cartridge;
	INTERRUPT interrupt;
	interrupt.reset();
	MMU mmu;
	DISPLAY display(0, 0, 160, 144, 1);
	bus.connectCPU(&cpu);
	bus.connectMMU(&mmu);
	bus.connectInterrupt(&interrupt);
	bus.connectGPU(&gpu);
	bus.connectDisplay(&display);
	bus.insertCartridge(&cartridge);
	cartridge.loadRom(romPath);
	cartridge.load();
	printf("title:%s\n", cartridge.title);
	printf("rom banks count:%d\n", cartridge.romSize);
	printf("romBank size:%d\n", cartridge.romBankSize);
	printf("ram banks count:%d\n", cartridge.ramSize);
	printf("romBank size:%d\n", cartridge.ramBankSize);
	
	ofstream myfile;
	myfile.open("C:\\Users\\Brain\\go\\src\\goboy\\goboy-0.4.2\\cmd\\goboy\\instructions_me.txt");
	
	
	//gpu.drawTest();

	//return 0;
	uint16_t opcode;
	int counter = 270274;
	char d;
	char buff[100];
	unsigned char c = 0;
	bus.pipeEnable = false;
	pipeChannel p;
	bus.p = &p;
	bool f;
	if (bus.pipeEnable) {
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
	do {
		opcode = (uint16_t)*cpu.getN();
		opcode = opcode == 0xCB ? 0XCB00 | *cpu.getN() : opcode;
		if (opcode == 0x38) {
			counter = counter + 0;
		}
#pragma region pipeChannelOpcode
		if (bus.pipeEnable) {
			f = p.read(1);
			uint8_t o = p.rBuffer[0];
			if (f) {
				if ((p.rBuffer[0] & 0xff00) != 0xcb00) {
					if (opcode != p.rBuffer[0]) {
						printf("not same opcode me:%04x pipe:%04x steps:%d\n", opcode, p.rBuffer[0], 270274 - counter);
						return 0;
					}

				}
				else {
					uint16_t pipeOpcode = (p.rBuffer[0] << 8 | p.rBuffer[1]);
					if (opcode != pipeOpcode) {
						printf("not same opcode me:%04x pipe:%04x steps:%d\n", opcode, pipeOpcode, 270274 - counter);
						return 0;
					}
				}
			}
			else {
				printf("faild to read\n");
				return 0;
			}
			p.wBuffer[0] = 'a';
			f = p.write(1);
			if (!f) {
				printf("faild to write\n");
				return 0;
			}
			p.rBuffer[0] = 0;
		}
#pragma endregion
#pragma region writeToFile
		//myfile << "opcode:0x%01x PC:0x%01x SP:0x%01x AF:0x%01x BC:0x%01x DE:0x%01x HL:0x%01x\n", opcode, * (uint16_t*)cpu.PC, * (uint16_t*)cpu.getSP(), * (uint16_t*)cpu.getAF(), * (uint16_t*)cpu.getBC(), * (uint16_t*)cpu.getDE(), * (uint16_t*)cpu.getHL();
				//if (counter >= 2000) {
					//snprintf(buff, sizeof(buff), "opcode:0x%01x PC:0x%01x SP:0x%01x AF:0x%01x BC:0x%01x DE:0x%01x HL:0x%01x\n", opcode, cpu.PC, *(uint16_t*)cpu.getSP(), *(uint16_t*)cpu.getAF(), *(uint16_t*)cpu.getBC(), *(uint16_t*)cpu.getDE(), *(uint16_t*)cpu.getHL());
					//myfile << buff;
					//myfile.flush();
				//}
#pragma endregion	
		counter--;
		if (mmu.read(0xFF02) == 0x81) {
			char t = mmu.read(0xFF01);
			if (t != c) {
				
				printf("%04x\n",t);
				c = t;
			}
		}
		if (opcode != 0) {
			 //printf("lineScan:%d LCD:%d\n", mmu.read(0xFF44), mmu.read(0xFF40));
			//printf("first opcode:%u\n", opcode);
			
		}
		cpu.Execute(opcode);
		if (renderCounter == renderTimer)
			gpu.drawTest();
#pragma region pipeChannelExe
		if (bus.pipeEnable) {
			p.read(2);
			if (p.rBuffer[0] != 0)
			{
				switch (p.rBuffer[0])
				{
				case 1:
					printf("write mem not same address\n");
					break;
				case 2:
					printf("write mem not same value\n");
					break;
				}
			}
		}
#pragma endregion
	} while(counter>0); //(cpu.PC != 0x100 && counter > 0);//||mmu.biosLoaded);//cpu.PC!=0x100

	myfile.close();
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