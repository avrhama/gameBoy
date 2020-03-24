#include "Cartridge.h"

void Cartridge::loadRom(const char* path)
{
	ifstream rf(path, ios::out | ios::binary);
	if (!rf) {
		cout << "Cannot open file!" << endl;
	}
	else {
		rf.seekg(0, rf.end);
		int size = rf.tellg();
		rf.seekg(0, rf.beg);
		mem = (uint8_t*)malloc(size);
		int p = 0;
		rf.read((char*)mem, size);		
	}
	rf.close();
	setCartridgeHeader();
}

void Cartridge::setCartridgeHeader()
{
	memcpy(title, (mem + 0x0134), 9);
}
