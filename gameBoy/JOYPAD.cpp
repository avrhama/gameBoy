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
                printf(keys[i].name);
               

            }
         
            if (!keys[i].isPressed)
                trigerInterrupt = true;
            keys[i].isPressed = true;
           bus->interrupt->io[0x00] &=(0xff^(0x01 << (i % 4)));
          
            //bus->interrupt->io[0x00] |= (0x01 << (i % 4));
        }
        else {
            if (keys[i].isPressed) {
                keys[i].isPressed = false;
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
bool JOYPAD::scanKeys2()
{
    for (uint8_t i = 8;i < 16 ;i++) {
        SHORT v = GetAsyncKeyState((int)keys[i].key);
        if ((v & 0x8000) == 0x8000) {
            if (!keys[i].isPressed) {
                keys[i].isPressed = true;
                printf(keys[i].name);
                switch (keys[i].key)
                {
                case KEYS::W:
                    
                    if (bus->f < 17) {
                        bus->f += 1;
                        bus->modified = true;
                    }
                    printf("f:%f\n",pow(2, bus->f));
                    break;
                case KEYS::E:
                    if (bus->f > 5) {
                        bus->f -= 1;
                        bus->modified = true;
                    }
                    printf("f:%f\n", pow(2, bus->f));
                    break;
                case KEYS::S:
                    if (bus->s < 17) {
                        bus->s += 1;
                        bus->modified = true;
                    }
                    printf("s:%f\n", pow(2, bus->s));
                    break;
                case KEYS::D:
                    if (bus->s > 5) {
                        bus->s -= 1;
                        bus->modified = true;
                    }
                    printf("s:%f\n", pow(2, bus->s));
                    break;
                case KEYS::Z:
                    bus->h += 1;
                    printf("h:%d\n", bus->h);
                    break;
                case KEYS::X:
                    if (bus->h > 0) {
                        bus->h -= 1;
                    }
                    printf("h:%d\n", bus->h);
                    break;
                case KEYS::C:
                    bus->r += 0.1;
                    printf("r:%f\n", bus->r);
                    break;
                case KEYS::V:
                    bus->r -= 0.1;
                    printf("r:%f\n", bus->r);
                    break;
                }
                

            }

            

            //bus->interrupt->io[0x00] |= (0x01 << (i % 4));
        }
        else{
            if (keys[i].isPressed) {
                keys[i].isPressed = false;
                // string p = keys[i].name + " released\n";
               //  printf(keys[i].name);

             //keys[i].isPressed = false;
            // bus->interrupt->io[0x00] |= (0x01 << (i % 4));
            }
            //bus->interrupt->io[0x00] &= (0xff ^ (0x01 << (i % 4)));
        }
    }
    return false;
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
    //
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
        scanKeys2();
    }
    if(trigerInterrupt)
        bus->interrupt->setInterruptRequest(4);
   

}


