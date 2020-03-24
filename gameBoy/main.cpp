#include <iostream>
#include <memory.h>
#include<fstream>
#include<cstdint>
#include<stdlib.h>
#include <stdio.h> 
#include "Cartridge.h"
#include "Cpu.h"

#include <map>
using namespace std;


void a(uint16_t* p) {
	*(uint8_t*)p = 4;
	//*p = 4;
}
int main(void) {
	//a((uint16_t*)((uint16_t)0x08));
	Cpu cpu;
	
	uint16_t* u = cpu.get$DE();
	printf("%d\n", *u);
	cpu.Execute(6);
	uint8_t d = 2;
	printf("%d\n", d);
	a((uint16_t*)&d);
	printf("%d\n", d);
	cout << "gameBoy\n";
	const char* path = "megaman.gb";
	Cartridge c;
	c.loadRom(path);
	printf("title:%s\n", c.title);
	return 0;
}