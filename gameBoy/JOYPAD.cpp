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
            if (!keys[i].isPressed) {
               // string p = keys[i].name + " pressed\n";
                printf(keys[i].name);
                if (keys[i].name == "START")
                    bus->cpu->speed = -1;
            }
         
            if (!keys[i].isPressed)
                trigerInterrupt = true;
            keys[i].isPressed = true;
           bus->interrupt->io[0x00] &=(0xff^(0x01 << (i % 4)));
          
            //bus->interrupt->io[0x00] |= (0x01 << (i % 4));
        }
        else {
            if (keys[i].isPressed) {
               // string p = keys[i].name + " released\n";
              //  printf(keys[i].name);
           
            //keys[i].isPressed = false;
           // bus->interrupt->io[0x00] |= (0x01 << (i % 4));
            }
            //bus->interrupt->io[0x00] &= (0xff ^ (0x01 << (i % 4)));
        }
    }
    return trigerInterrupt;
}
//bool JOYPAD::scanKeys(int start) {
//    bool trigerInterrupt = false;
//    uint8_t bit = 0x01;
//    for (uint8_t i = start;i < start + 4;i++) {
//        if (keys[i].isPressed) {
//            trigerInterrupt = true;
//            bus->interrupt->io[0x00] &= (0xff ^ (0x01 << (i % 4)));
//        }
//    }
//    return trigerInterrupt;
//}
void JOYPAD::updateKeys()
{
    
    uint8_t joypadRegister = bus->interrupt->io[0x00];
    bool trigerInterrupt = false;
    //bus->interrupt->io[0x00] |= 0xFF;
    bus->interrupt->io[0x00] |= 0xCF;
   // bus->interrupt->io[0x00] |= 0xC0;
    
    if (!((joypadRegister >> 5) & 0x01)) {//checks buttons keys
        if (scanKeys(0)) {
            trigerInterrupt = true;
        }
    }
    if (!((joypadRegister >> 4) & 0x01)) {//checks directions keys
        if (scanKeys(4)) {
            trigerInterrupt = true;
        }
    }
    if(trigerInterrupt)
        bus->interrupt->setInterruptRequest(4);
   

}


