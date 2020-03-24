#pragma once
#include<cstdint>
#include <map>
#include "Ram.h"
#include "Structures.h"
using namespace std;


class Cpu
{
	struct Opcode
	{
	public:
		const char* name;
		void(Cpu::* operate)(uint16_t*, uint16_t*);
		uint16_t* (Cpu::* param1)();
		uint16_t* (Cpu::* param2)();
		uint8_t cycles;
	};
private:
	uint16_t AF=0;
	uint16_t BC = 0;
	uint16_t DE = 0;
	uint16_t HL = 0;
	/*uint8_t a = 0;
	uint8_t f = 0;
	uint8_t b = 0;
	uint8_t c = 0;
	uint8_t d = 0;
	uint8_t e = 0;
	uint8_t h = 0;
	uint8_t l = 0;*/
	uint16_t sp =0;
	uint16_t pc = 0x100;
	map<uint16_t, Opcode> opcodes;
	Ram ram;
public:

	Cpu();
	uint16_t* getA();
	uint16_t* getB();
	uint16_t* getC();
	uint16_t* getD();
	uint16_t* getE();
	uint16_t* getF();
	uint16_t* getH();
	uint16_t* getL();
	uint16_t* getAF();
	uint16_t* getBC();
	uint16_t* get$BC();
	uint16_t* getDE();
	uint16_t* get$DE();
	uint16_t* getHL();
	uint16_t* get$HL();
	uint16_t* getN();
	void Execute(uint16_t opcode);
	void LD(uint16_t* nn, uint16_t* n);
	void LD_nn_n(uint16_t* nn, uint16_t* n);
	void LD_r1_r2(uint16_t* r1, uint16_t* r2);
	void LD_A_C(uint16_t* A, uint16_t* C);
	void LD_C_A(uint16_t* C, uint16_t* A);
	void LDD_A_$HL(uint16_t* A,uint16_t* $HL);
	void LDD_$HL_A(uint16_t* $HL, uint16_t* A);
	void LDI_A_$HL(uint16_t* A, uint16_t* $HL);
	void LDI_$HL_A(uint16_t* $HL, uint16_t* A);
	void LD_n_nn(uint16_t* n, uint16_t* nn);
	uint16_t BytesToWord(uint8_t msb, uint8_t lsB);
	Word WordToBytes(uint16_t word);
	void SetWordIntoBytes(uint16_t* a, uint16_t* b);
	void SetByte(uint16_t* word, uint8_t value);
	//void* nn2nWrapper(void (*operate)(uint16_t, uint16_t),uint16_t* nn, uint8_t n) {
	//	return operate;
	//}
};


