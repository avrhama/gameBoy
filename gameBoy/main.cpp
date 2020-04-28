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
#include "Speaker.h"
#undef main
using namespace std;
//sound
#include "SDL.h"
#include <stdio.h>
#include <math.h>
#define FREQ 200 /* the frequency we want */

unsigned int audio_pos; /* which sample we are up to */
int audio_len; /* how many samples left to play, stops when <= 0 */
float audio_frequency; /* audio frequency in cycles per sample */
float audio_volume; /* audio volume, 0 - ~32000 */

void MyAudioCallback2(void* userdata, Uint8* stream, int len) {
	len /= 2; /* 16 bit */
	int i;
	Sint16* buf = (Sint16*)stream;
	for (i = 0; i < len; i++) {
		buf[i] = audio_volume * sin(2 * M_PI * audio_pos * audio_frequency);
		audio_pos++;
	}
	audio_len -= len;
	return;
}
int sound() {
	SDL_Init(SDL_INIT_AUDIO);

	int numAudioDrivers = SDL_GetNumAudioDrivers();

	
	SDL_AudioSpec want, have;
	SDL_zero(want);

	

	want.freq = 131072;
	want.format = AUDIO_S16;
	want.channels = 1;
	want.samples = 4096;
	want.callback = MyAudioCallback2;
	SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

	audio_len = have.freq * 5; /* 5 seconds */
	audio_pos = 0;
	audio_frequency = 1.0 * FREQ / have.freq; /* 1.0 to make it a float */
	audio_volume = 6000; /* ~1/5 max volume */

	SDL_PauseAudioDevice(dev, 0); /* play! */

	while (audio_len > 0) {
		SDL_Delay(5000);
	}
	return 0;
}

//sound


void displayThreadFunc(DISPLAY* display, bool* running)
{
	while (*running) {
		if (!display->displayLock) {
			display->render();
			//display->update();
			//Sleep(16.6);
			Sleep(16);
		}
		else
			Sleep(5);
	}
	printf("goodbye thread!\n");
	// do stuff...
}
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

int main(void) {
	
	
	
	//sound();

	//return 0;
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
	"roms\\pokemon.gb",//15
	"roms\\tetris.gb",
	"test\\instr_timing\\instr_timing.gb",//failed
	"roms\\mooneye-gb_hwtests\\acceptance\\timer\\div_write.gb",
	"roms\\mooneye-gb_hwtests\\acceptance\\timer\\tima_reload.gb",
	"roms\\mooneye-gb_hwtests\\acceptance\\timer\\tima_write_reloading.gb",
	"roms\\mooneye-gb_hwtests\\acceptance\\timer\\tma_write_reloading.gb",
	"roms\\mooneye-gb_hwtests\\misc\\boot_regs-cgb.gb",
	"roms\\mooneye-gb_hwtests\\acceptance\\ld_hl_sp_e_timing.gb",
	"roms\\mooneye-gb_hwtests\\acceptance\\oam_dma_start.gb",
	"test\\halt_bug.gb",
	"roms\\Pokemon8.gb",
	"roms\\mario.gb",//27
	"roms\\zelda.gbc" };
	uint8_t romIndex =15;

	//
	BUS* bus = new BUS();
	CPU* cpu = new CPU();
	GPU* gpu = new GPU();
	DMA* dma = new DMA();
	CARTRIDGE* cartridge = createCartrige(romsPaths[romIndex]);
	if (cartridge == NULL)
		return 1;
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
	/*cartridge->loadRom(romsPaths[romIndex]);
	cartridge->load();*/
	cpu->reset();
	mmu->reset();
	gpu->reset();
	interrupt->reset();
	apu->start();
	//apu->fs.apu = apu;
	printf("title:%s\n", cartridge->header.title);
	printf("rom banks count:%d\n", cartridge->header.romBanksCount);
	printf("romBank size:%d\n", cartridge->header.romBankSize);
	printf("ram banks count:%d\n", cartridge->header.ramBanksCount);
	printf("ramBank size:%d\n", cartridge->header.ramBankSize);


	//ofstream myfile;
	//myfile.open("C:\\Users\\Brain\\go\\src\\goboy\\goboy-0.4.2\\cmd\\goboy\\instructions_me.txt");


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
	bool running = true;

	//std::thread displayThread(displayThreadFunc, display,&running);

	int cyclesInFrameCounter = 0;
	int framesForSeconds = 60;
	int cyclesInFrame = cpu->cpuFreq / framesForSeconds;
	uint16_t lastopcode = 0;

	/*while (cpu->running) {
		if (SDL_PollEvent(&display->windowEvent))
			if (SDL_QUIT == display->windowEvent.type)
			{
				running = false;
				bus->cpu->running = false;
			}
		if (apu->restart)
			apu->play();
		Sleep(100);
	}
	display->close();
	return 0;*/

	int counters[5] = { 3,6,12,15,30 };
	int sleeps[5] = { 50,100,200,250,500 };
	int rendersCounter = 0;
	while (true) {
		if (SDL_PollEvent(&display->windowEvent))
			if (SDL_QUIT == display->windowEvent.type)
			{
				running = false;
				break;
			}
		//steady_clock::time_point start = steady_clock::now();
		int timeBefore = SDL_GetTicks();
		do {

			//cpu->Execute(opcode);
			//pipeRecive(bus, opcode, lastopcode, steps,"Execute");
			if (!cpu->halt) {
				opcode = cpu->getOpcode();
				opcode = (opcode == 0xCB) ? 0XCB00 | cpu->getOpcode() : opcode;
				//cpu->Execute(opcode);
				cpu->ExecuteOpcode(opcode);
				//cpu->steps++;
				//pipeRecive(bus, opcode, lastopcode, cpu->steps, "Execute");
				//cpu->Execute(opcode);
			}
			else {
				cpu->lastOpcodeCycles = 1;
			}

	

			cpu->lastOpcodeCycles *= (4 * (cpu->speedMode + 1));
			cpu->steps+= cpu->lastOpcodeCycles;
			//apu->fs.tick(cpu->lastOpcodeCycles);
			apu->tick();
			gpu->tick();
			cpu->updateTimers();


			joypad->updateKeys();
			cpu->lastOpcodeCycles += interrupt->InterruptsHandler() * (4 * (cpu->speedMode + 1));;

			//pipeRecive(bus, opcode, lastopcode, steps, "InterruptsHandler");

			cyclesInFrameCounter += cpu->lastOpcodeCycles;
			//printf("speed:%d\n", interrupt->io[0x4D]>>7);

			/*if (renderCounter == renderTimer)
				gpu->drawTest();*/

				/*if (cpu->cycelsCounter > 4)
					cpu->cycelsCounter = cpu->cycelsCounter%4;*/


					//cpu->cycelsCounter += cpu->lastOpcodeCycles;
			/*cpu->time.addMCycles(cpu->lastOpcodeCycles / 4);
			cpu->time.print(2);*/
		//	if (cpu->time.timeChanged[1]) {
			//	cpu->time.timeChanged[1] = false;
			//	Sleep(1);
			//	//printf("sleep\n");
			//}
		
		} while (cyclesInFrameCounter < cyclesInFrame); //(cpu.PC != 0x100 && counter > 0);//||mmu.biosLoaded);//cpu.PC!=0x100
		cyclesInFrameCounter = 0;

		display->render();
		int timePassed = SDL_GetTicks() - timeBefore;
		int sleep = 15 - timePassed;
		if (sleep > 0)
			Sleep(sleep);
		rendersCounter++;
		
		
		/*steady_clock::time_point end = steady_clock::now();
		duration<double> elapsed_seconds = end - start;*/
		int i = 4;
		/*if (rendersCounter == counters[i]) {
			float factor = 0.5;
			SDL_PauseAudioDevice(apu->adc.dev, 0);
			Sleep(sleeps[i]*factor);
			SDL_PauseAudioDevice(apu->adc.dev, 0);
			rendersCounter = 0;
		}*/
		//double elapse = 1-elapsed_seconds.count();
		/*if (elapse > 0) {
			Sleep(elapse * 1000);
		}*/
		
		bool lastState = apu->adc.paused;
		apu->adc.paused = true;
		//Sleep(4);
		apu->adc.paused = lastState;
		//display->update();
		//printf("render\n");
		if (apu->restart)
			apu->play();
	}
	display->close();
	//displayThread.join();
	return 0;

	//myfile.close();


	while (true) {


		KEYS key = display->waitForKey();
		display->setPixel(x, y, black);
		if (display->keysMapper[KEYS::Up]->isPressed) {
			cout << "Up";
			y = (y - 1) % 144;
			y = y < 0 ? 144 + y : y;

		}
		else if (display->keysMapper[KEYS::Down]->isPressed) {
			cout << "Down";
			y = (y + 1) % 144;
		}
		else if (display->keysMapper[KEYS::Left]->isPressed) {
			cout << "Left";
			x = (x - 1) % 160;
			x = x < 0 ? 160 + x : x;
		}
		else if (display->keysMapper[KEYS::Right]->isPressed) {

			cout << "Right: ";
			x = (x + 1) % 160;

		}
		display->setPixel(x, y, white);
		//Sleep(250);


	}


	return 0;

}

//void testRomSwitching(MBC1 cartridge) {
//	cartridge.write(0x2000, 0);
//	printf("romBank:%d\n", cartridge.romBankIndex);
//	cartridge.write(0x2000, 20);	 
//	cartridge.write(0x5fff, 0);		 
//	printf("romBank:%d\n", cartridge.romBankIndex);
//	cartridge.write(0x2000, 40);	 
//	cartridge.write(0x5fff, 1);		 
//	printf("romBank:%d\n", cartridge.romBankIndex);
//	cartridge.write(0x2000, 60);	 
//	cartridge.write(0x5fff, 1);		 
//	printf("romBank:%d\n", cartridge.romBankIndex);
//	cartridge.write(0x2000, 1);		 
//	cartridge.write(0x5fff, 0);		 
//	printf("romBank:%d\n", cartridge.romBankIndex);
//	cartridge.write(0x2000, 21);	 
//	cartridge.write(0x5fff, 0);		 
//	printf("romBank:%d\n", cartridge.romBankIndex);
//	cartridge.write(0x2000, 41);	 
//	cartridge.write(0x5fff, 41);	 
//	printf("romBank:%d\n", cartridge.romBankIndex);
//	cartridge.write(0x2000, 61);	 
//	cartridge.write(0x5fff, 1);		 
//	printf("romBank:%d\n", cartridge.romBankIndex);
//	cartridge.write(0x2000, 2);		 
//	cartridge.write(0x5fff, 0);		 
//	printf("romBank:%d\n", cartridge.romBankIndex);
//	cartridge.write(0x2000, 22);	 
//	cartridge.write(0x5fff, 0);		 
//	printf("romBank:%d\n", cartridge.header.romBankIndex);
//	cartridge.write(0x2000, 42);	 
//	cartridge.write(0x5fff, 1);		 
//	printf("romBank:%d\n", cartridge.header.romBankIndex);
//	cartridge.write(0x2000, 62);	 
//	cartridge.write(0x5fff, 1);		 
//	printf("romBank:%d\n", cartridge.header.romBankIndex);
//}
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


void tests(BUS  bus) {

	//testPipe();
}


