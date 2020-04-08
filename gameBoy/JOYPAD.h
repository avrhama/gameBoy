#pragma once
#include "BUS.h"
#include "INTERRUPT.h"
class JOYPAD
{
private:
	BUS* bus;
	char const   keysCount = 8;
	KEY keys[8] = {
		KEY{KEYS::A,false},
		KEY{KEYS::B,false},
		KEY{KEYS::Space,false},
		KEY{KEYS::Enter,false},
		KEY{KEYS::Right,false},
		KEY{KEYS::Left,false},
		KEY{KEYS::Up,false},
		KEY{KEYS::Down,false},
	};
public:
	void connectToBus(BUS* bus);
	bool scanKeys(int start);
	void updateKeys();
};

