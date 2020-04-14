#include "JOYPAD.h"
#include "CPU.h"
void JOYPAD::connectToBus(BUS* bus)
{
	this->bus = bus;
}
bool JOYPAD::scanKeys(int start) {
    bool trigerInterrupt = false;
    uint8_t bit = 0x01;
    for (uint8_t i = start;i < start+4;i++) {
        SHORT v = GetAsyncKeyState((int)keys[i].key);
        if ((v & 0x8000) == 0x8000) {
            if (!keys[i].isPressed)
                printf("pressed\n");
            if (!keys[i].isPressed)
                trigerInterrupt = true;
            keys[i].isPressed = true;
            bus->interrupt->io[0x00] |=(0x01 << (i % 4));
           
          
        }
        else {
            keys[i].isPressed = false;
        }
    }
    return trigerInterrupt;
}
void JOYPAD::updateKeys()
{
    uint8_t joypadRegister = bus->interrupt->io[0x00];
    bool trigerInterrupt = false;
	
	if ((joypadRegister >> 4) & 0x01) {//checks operations buttons
        if (scanKeys(0)) {
            trigerInterrupt = true;
        }
	}
    if ((joypadRegister >> 5) & 0x01) {//checks directions buttons
        if (scanKeys(4)) {
            trigerInterrupt = true;
        }
    }
    if(trigerInterrupt)
        bus->interrupt->setInterruptRequest(4);

}


