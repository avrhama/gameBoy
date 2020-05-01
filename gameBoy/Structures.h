#pragma once
#include<cstdint>
#include <stdio.h> 
//struct Opcode
//{
//public:
//	const char* name;
//	void(Cpu::* operate)(uint16_t*, uint16_t*);
//	uint16_t* (Cpu::* param1)();
//	uint16_t* (Cpu::* param2)();
//	uint8_t cycles;
//};
struct Word
{
public:
	uint8_t msb;
	uint8_t lsb;
};
struct Byte
{
public:
	uint8_t msn;
	uint8_t lsn;
};
enum class KEYS {
	A = 65,//The A key.
	Add = 107,//The add key.
	Alt = 262144,//The ALT modifier key.
	Apps = 93,//The application key (Microsoft Natural Keyboard).
	Attn = 246,//The ATTN key.
	B = 66,//The B key.
	Back = 8,//The BACKSPACE key.
	BrowserBack = 166,//The browser back key.
	BrowserFavorites = 171,//The browser favorites key.
	BrowserForward = 167,//The browser forward key.
	BrowserHome = 172,//The browser home key.
	BrowserRefresh = 168,//The browser refresh key.
	BrowserSearch = 170,//The browser search key.
	BrowserStop = 169,//The browser stop key.
	C = 67,//The C key.
	Cancel = 3,//The CANCEL key.
	Capital = 20,//The CAPS LOCK key.
	CapsLock = 20,//The CAPS LOCK key.
	Clear = 12,//The CLEAR key.
	Control = 131072,//The CTRL modifier key.
	ControlKey = 17,//The CTRL key.
	Crsel = 247,//The CRSEL key.
	D = 68,//The D key.
	D0 = 48,//The 0 key.
	D1 = 49,//The 1 key.
	D2 = 50,//The 2 key.
	D3 = 51,//The 3 key.
	D4 = 52,//The 4 key.
	D5 = 53,//The 5 key.
	D6 = 54,//The 6 key.
	D7 = 55,//The 7 key.
	D8 = 56,//The 8 key.
	D9 = 57,//The 9 key.
	Decimal = 110,//The decimal key.
	Delete = 46,//The DEL key.
	Divide = 111,//The divide key.
	Down = 40,//The DOWN ARROW key.
	E = 69,//The E key.
	End = 35,//The END key.
	Enter = 13,//The ENTER key.
	EraseEof = 249,//The ERASE EOF key.
	Escape_ = 27,//The ESC key.
	Execute = 43,//The EXECUTE key.
	Exsel = 248,//The EXSEL key.
	F = 70,//The F key.
	F1 = 112,//The F1 key.
	F10 = 121,//The F10 key.
	F11 = 122,//The F11 key.
	F12 = 123,//The F12 key.
	F13 = 124,//The F13 key.
	F14 = 125,//The F14 key.
	F15 = 126,//The F15 key.
	F16 = 127,//The F16 key.
	F17 = 128,//The F17 key.
	F18 = 129,//The F18 key.
	F19 = 130,//The F19 key.
	F2 = 113,//The F2 key.
	F20 = 131,//The F20 key.
	F21 = 132,//The F21 key.
	F22 = 133,//The F22 key.
	F23 = 134,//The F23 key.
	F24 = 135,//The F24 key.
	F3 = 114,//The F3 key.
	F4 = 115,//The F4 key.
	F5 = 116,//The F5 key.
	F6 = 117,//The F6 key.
	F7 = 118,//The F7 key.
	F8 = 119,//The F8 key.
	F9 = 120,//The F9 key.
	FinalMode = 24,//The IME final mode key.
	G = 71,//The G key.
	H = 72,//The H key.
	HanguelMode = 21,//The IME Hanguel mode key. (maintained for compatibility; use HangulMode)
	HangulMode = 21,//The IME Hangul mode key.
	HanjaMode = 25,//The IME Hanja mode key.
	Help = 47,//The HELP key.
	Home = 36,//The HOME key.
	I = 73,//The I key.
	IMEAccept = 30,//The IME accept key, replaces IMEAceept.
	IMEAceept = 30,//The IME accept key. Obsolete, use IMEAccept instead.
	IMEConvert = 28,//The IME convert key.
	IMEModeChange = 31,//The IME mode change key.
	IMENonconvert = 29,//The IME nonconvert key.
	Insert = 45,//The INS key.
	J = 74,//The J key.
	JunjaMode = 23,//The IME Junja mode key.
	K = 75,//The K key.
	KanaMode = 21,//The IME Kana mode key.
	KanjiMode = 25,//The IME Kanji mode key.
	KeyCode = 65535,//The bitmask to extract a key code from a key value.
	L = 76,//The L key.
	LaunchApplication1 = 182,//The start application one key.
	LaunchApplication2 = 183,//The start application two key.
	LaunchMail = 180,//The launch mail key.
	LButton = 1,//The left mouse button.
	LControlKey = 162,//The left CTRL key.
	Left = 37,//The LEFT ARROW key.
	LineFeed = 10,//The LINEFEED key.
	LMenu = 164,//The left ALT key.
	LShiftKey = 160,//The left SHIFT key.
	LWin = 91,//The left Windows logo key (Microsoft Natural Keyboard).
	M = 77,//The M key.
	MButton = 4,//The middle mouse button (three-button mouse).
	MediaNextTrack = 176,//The media next track key.
	MediaPlayPause = 179,//The media play pause key.
	MediaPreviousTrack = 177,//The media previous track key.
	MediaStop = 178,//The media Stop key.
	Menu = 18,//The ALT key.
	Modifiers = -65536,//The bitmask to extract modifiers from a key value.
	Multiply = 106,//The multiply key.
	N = 78,//The N key.
	Next = 34,//The PAGE DOWN key.
	NoName = 252,//A constant reserved for future use.
	None = 0,//No key pressed.
	NumLock = 144,//The NUM LOCK key.
	NumPad0 = 96,//The 0 key on the numeric keypad.
	NumPad1 = 97,//The 1 key on the numeric keypad.
	NumPad2 = 98,//The 2 key on the numeric keypad.
	NumPad3 = 99,//The 3 key on the numeric keypad.
	NumPad4 = 100,//The 4 key on the numeric keypad.
	NumPad5 = 101,//The 5 key on the numeric keypad.
	NumPad6 = 102,//The 6 key on the numeric keypad.
	NumPad7 = 103,//The 7 key on the numeric keypad.
	NumPad8 = 104,//The 8 key on the numeric keypad.
	NumPad9 = 105,//The 9 key on the numeric keypad.
	O = 79,//The O key.
	Oem1 = 186,//The OEM 1 key.
	Oem102 = 226,//The OEM 102 key.
	Oem2 = 191,//The OEM 2 key.
	Oem3 = 192,//The OEM 3 key.
	Oem4 = 219,//The OEM 4 key.
	Oem5 = 220,//The OEM 5 key.
	Oem6 = 221,//The OEM 6 key.
	Oem7 = 222,//The OEM 7 key.
	Oem8 = 223,//The OEM 8 key.
	OemBackslash = 226,//The OEM angle bracket or backslash key on the RT 102 key keyboard.
	OemClear = 254,//The CLEAR key.
	OemCloseBrackets = 221,//The OEM close bracket key on a US standard keyboard.
	Oemcomma = 188,//The OEM comma key on any country/region keyboard.
	OemMinus = 189,//The OEM minus key on any country/region keyboard.
	OemOpenBrackets = 219,//The OEM open bracket key on a US standard keyboard.
	OemPeriod = 190,//The OEM period key on any country/region keyboard.
	OemPipe = 220,//The OEM pipe key on a US standard keyboard.
	Oemplus = 187,//The OEM plus key on any country/region keyboard.
	OemQuestion = 191,//The OEM question mark key on a US standard keyboard.
	OemQuotes = 222,//The OEM singled/double quote key on a US standard keyboard.
	OemSemicolon = 186,//The OEM Semicolon key on a US standard keyboard.
	Oemtilde = 192,//The OEM tilde key on a US standard keyboard.
	P = 80,//The P key.
	Pa1 = 253,//The PA1 key.
	Packet = 231,//Used to pass Unicode characters as if they were keystrokes. The Packet key value is the low word of a 32-bit virtual-key value used for non-keyboard input methods.
	PageDown = 34,//The PAGE DOWN key.
	PageUp = 33,//The PAGE UP key.
	Pause = 19,//The PAUSE key.
	Play = 250,//The PLAY key.
	Print = 42,//The PRINT key.
	PrintScreen = 44,//The PRINT SCREEN key.
	Prior = 33,//The PAGE UP key.
	ProcessKey = 229,//The PROCESS KEY key.
	Q = 81,//The Q key.
	R = 82,//The R key.
	RButton = 2,//The right mouse button.
	RControlKey = 163,//The right CTRL key.
	Return = 13,//The RETURN key.
	Right = 39,//The RIGHT ARROW key.
	RMenu = 165,//The right ALT key.
	RShiftKey = 161,//The right SHIFT key.
	RWin = 92,//The right Windows logo key (Microsoft Natural Keyboard).
	S = 83,//The S key.
	Scroll = 145,//The SCROLL LOCK key.
	Select = 41,//The SELECT key.
	SelectMedia = 181,//The select media key.
	Separator = 108,//The separator key.
	Shift = 65536,//The SHIFT modifier key.
	ShiftKey = 16,//The SHIFT key.
	Sleep_ = 95,//The computer sleep key.
	Snapshot = 44,//The PRINT SCREEN key.
	Space = 32,//The SPACEBAR key.
	Subtract = 109,//The subtract key.
	T = 84,//The T key.
	Tab = 9,//The TAB key.
	U = 85,//The U key.
	Up = 38,//The UP ARROW key.
	V = 86,//The V key.
	VolumeDown = 174,//The volume down key.
	VolumeMute = 173,//The volume mute key.
	VolumeUp = 175,//The volume up key.
	W = 87,//The W key.
	X = 88,//The X key.
	XButton1 = 5,//The first x mouse button (five-button mouse).
	XButton2 = 6,//The second x mouse button (five-button mouse).
	Y = 89,//The Y key.
	Z = 90,//The Z key.
	Zoom = 251//The ZOOM key.
};
struct KEY {
	KEYS key;
	bool isPressed;
	const char * name;
};
enum ColorChannel:uint8_t { Blue, Green, Red, Alpha };

enum ColorName
{
	WHITE, LIGHT_GRAY, DARK_GRAY, BLACK
};
enum class ConsoleModel
{
	DMG,GBC
};
struct Color {
private:
	uint8_t R;
	uint8_t B;
	uint8_t G;
	uint32_t hex;
	//uint8_t A;
	
public:
	uint32_t val;
	 Color(uint32_t h) {
		hex = h;
		R = (h >> 16)&0xff;
		G = (h >> 8) & 0xff;
		B = (h) & 0xff;
		val = hex;
	}
	 Color(uint8_t r, uint8_t g, uint8_t b) {
		 setRGB(r, g, b);
	 }
	 Color() {}
	uint32_t getHex() {
		return hex;
	}
	uint8_t getR() {
		return R;
	}
	uint8_t getG() {
		return G;
	}
	uint8_t getB() {
		return B;
	}
	void setR(uint8_t R) {
		setRGB(R, G, B);
	}
	void setG(uint8_t G) {
		setRGB(R, G, B);
	}
	void setB(uint8_t B) {
		setRGB(R, G, B);
	}
	void setRGB(uint8_t r, uint8_t g, uint8_t b) {
		R = r;
		G = g;
		B = b;
		hex = r << 16 | g << 8 | b;
		val = hex;
	}
};
struct TIME {
	uint8_t dots = 0;//machine cycels
	uint8_t dotsPerMicroSec = 4;//for this emulator 4 dots equal to 1 microsecond
	uint16_t microSec=0;
	uint16_t milliSec=0;
	uint8_t sec=0;
	uint8_t min=0;
	uint8_t hour=0;
	bool timeChanged[5] ={ false,false,false,false,false };
	void print(int8_t unit=-1) {
		if (unit == -1 || timeChanged[unit]) {
			printf("h:%d m:%d s:%d mil:%d micro:%d\n", hour, min, sec, milliSec, microSec);
			timeChanged[0] = false;
			timeChanged[1] = false;
			timeChanged[2] = false;
			timeChanged[3] = false;
			timeChanged[4] = false;
		}
	}
	void addMCycles(uint16_t m) {
		
		if (dots + m >= dotsPerMicroSec) {
			uint16_t temp = (dots + m);
			dots = temp % dotsPerMicroSec;
			m = temp / dotsPerMicroSec;
			if (microSec + m >= 1000) {
				temp = (microSec + m);
				microSec = temp % 1000;
				m = temp / 1000;
				if (milliSec + m >= 1000) {
					temp = (milliSec + m);
					milliSec = temp % 1000;
					m = temp / 1000;
					if (sec + m >= 60) {
						temp = (sec + m);
						sec = temp % 60;
						m = temp / 60;
						if (min + m >= 60) {
							temp = (min + m);
							min = temp % 60;
							m = temp / 60;
							hour += m;
							timeChanged[4] = true;
						}
						else {
							min += m;
							timeChanged[3] = true;
						}
					}
					else {
						sec += m;
						timeChanged[2] = true;
					}

				}
				else {
					milliSec += m;
					timeChanged[1] = true;
				}
			}
			else {
				microSec += m;
				timeChanged[0] = true;
			}
		}
		else {
			dots += m;
		}
	}
};
