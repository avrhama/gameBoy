#pragma once
#include<cstdint>
#include <memory.h>
#include<fstream>
#include<iostream>
using namespace std;
class Cartridge
{
private:
	uint8_t* mem;

public:
	char title[9];
	void loadRom(const char* path);
	void setCartridgeHeader();
};

