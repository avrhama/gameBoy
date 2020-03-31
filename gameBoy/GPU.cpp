#include "GPU.h"

void GPU::reset()
{
	for (int i = 0;i < 0x2000;i++)
		vRam[i] = 0;
	for (int i = 0;i < 0xa0;i++)
		oam[i] = 0;
}

void GPU::connectToBus(BUS* bus)
{
	this->bus = bus;
}

void GPU::tick()
{
	modeClock += bus->cpu->lastOpcodeCycles;
	switch (mode) {
	case 2:
		if (modeClock >= 80) {
			modeClock = 0;
			mode = 3;
		}
		break;
	case 3:
		if (modeClock >= 172) {
			modeClock = 0;
			mode = 0;
			// GPU.renderscan();
		}
		break;
	case 0:
		if (modeClock >= 204) {
			modeClock = 0;
			char currScanLine=bus->mmu->read(0xFF44);
			bus->mmu->write(0xFF44,currScanLine++);
			if (bus->mmu->read(0xFF44) == 143) {
				mode = 1;
				//GPU._canvas.putImageData(GPU._scrn, 0, 0);
			}else {
				
				mode = 2;
			}
		}
		break;
	case 1:
		if (modeClock >= 4560) {
			modeClock = 0;
			char currScanLine = bus->mmu->read(0xFF44);
			bus->mmu->write(0xFF44, currScanLine++);
			if (bus->mmu->read(0xFF44)==154)
			{
				// Restart scanning modes
				mode = 2;
				bus->mmu->write(0xFF44, 0);
			}
		}
		break;
	}
}

void GPU::drawTest()
{
	uint8_t heart[16] = {
		0x66,0x66, 0x99,0xff,
		0x81,0xff,0x81,0xff,
		0x42,0x7e,0x24,0x3c,
		0x18,0x18,0x0,0x0 };
	int x = 0;
	int y = 0;
	
	bool flipy = false;
	int vflip = flipy?14:0;
	for (;y < 8;y++) {

		uint8_t lsb = heart[!flipy?(2 * y):(15-2*y)];
		uint8_t msb = heart[!flipy ? (2 * y+1) : (15 - (2 * y+1))];
		for (x = 0;x < 8;x++) {
			uint8_t colorIndex = ((msb >> (7 - x)) & 0x1) << 1 | (lsb >> 7 - x)&0x1;
			bus->display->setPixel(x, y, bus->display->colors[colorIndex]);
		}
	}
	
}
