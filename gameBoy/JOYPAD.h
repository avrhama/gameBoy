#pragma once
#include "BUS.h"
#include "INTERRUPT.h"

class JOYPAD
{
private:
	BUS* bus;
	char const   keysCount = 8;
	KEY keys[8] = {
		KEY{KEYS::A,false,"A"},
		KEY{KEYS::B,false,"B"},
		KEY{KEYS::Space,false,"SELECT"},
		KEY{KEYS::Enter,false,"START"},
		KEY{KEYS::Right,false,"RIGHT"},
		KEY{KEYS::Left,false,"LEFT"},
		KEY{KEYS::Up,false,"UP"},
		KEY{KEYS::Down,false,"DOWN"},
	};
public:
	void connectToBus(BUS* bus);
	bool scanKeys(int start);
	void updateKeys();
};

