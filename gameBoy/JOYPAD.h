#pragma once
#include "BUS.h"
#include "INTERRUPT.h"
#include <SDL.h>
class JOYPAD
{
private:
	SDL_Event windowEvent;
	BUS* bus;
	char const   keysCount = 8;
	KEY keys[16] = {
		KEY{KEYS::A,false,"A"},
		KEY{KEYS::B,false,"B"},
		KEY{KEYS::Space,false,"SELECT"},
		KEY{KEYS::Enter,false,"START"},
		KEY{KEYS::Right,false,"RIGHT"},
		KEY{KEYS::Left,false,"LEFT"},
		KEY{KEYS::Up,false,"UP"},
		KEY{KEYS::Down,false,"DOWN"},
		KEY{KEYS::W,false,"W"},
		KEY{KEYS::E,false,"E"},
		KEY{KEYS::S,false,"S"},
		KEY{KEYS::D,false,"D"},
		KEY{KEYS::Z,false,"Z"},
		KEY{KEYS::X,false,"X"},
		KEY{KEYS::C,false,"C"},
		KEY{KEYS::V,false,"V"}
	};
public:
	void connectToBus(BUS* bus);
	bool scanKeys(int start);
	bool scanKeys2();
	void updateKeys();
};

