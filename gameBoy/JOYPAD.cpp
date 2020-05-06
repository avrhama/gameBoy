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
                trigerInterrupt = true;
            keys[i].isPressed = true;
           bus->interrupt->io[0x00] &=(0xff^(0x01 << (i % 4)));          
        }
        else {
            if (keys[i].isPressed) {
                keys[i].isPressed = false;
            }
        }
    }
    return trigerInterrupt;
}
bool JOYPAD::scanKeysExtra()
{
    for (uint8_t i = 8;i < 17 ;i++) {
        SHORT v = GetAsyncKeyState((int)keys[i].key);
        if ((v & 0x8000) == 0x8000) {
            if (!keys[i].isPressed) {
                keys[i].isPressed = true;
                switch (keys[i].key)
                {
                case KEYS::W:
                    
                    if (bus->f < 4194304) {
                        bus->f *= 2;
                        bus->modified = true;
                    }
                    printf("frequency:%d\n", bus->f);
                    break;
                case KEYS::E:
                    if (bus->f > 1) {
                        bus->f /= 2;
                        bus->modified = true;
                    }
                    printf("frequency:%d\n", bus->f);
                    break;
                case KEYS::S:
                    if (bus->s < 4194304) {
                        bus->s *= 2;
                        bus->modified = true;
                    }
                    printf("samples:%d\n", bus->s);
                    break;
                case KEYS::D:
                    if (bus->s > 1) {
                        bus->s /= 2;
                        bus->modified = true;
                    }
                    printf("samples:%d\n", bus->s);
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
                case KEYS::N:
                   
                    printf("frequency:%d samples:%d\n",bus->f,bus->s);
                    break;
                }
                

            }

            

            //bus->interrupt->io[0x00] |= (0x01 << (i % 4));
        }
        else{
            if (keys[i].isPressed) {
                keys[i].isPressed = false;
            }
        }
    }
    return false;
}
void JOYPAD::updateKeys()
{
    uint8_t joypadRegister = bus->interrupt->io[0x00];
    bool trigerInterrupt = false;
    bus->interrupt->io[0x00] |= 0xCF;
    
    if (!((joypadRegister >> 5) & 0x01)) {//checks buttons keys
        if (scanKeys(0)) {
            trigerInterrupt = true;
        }
    }
    if (!((joypadRegister >> 4) & 0x01)) {//checks directions keys
        if (scanKeys(4)) {
            trigerInterrupt = true;
        }
        //scanKeysExtra();
    }
    if(trigerInterrupt)
        bus->interrupt->setInterruptRequest(4);

}


