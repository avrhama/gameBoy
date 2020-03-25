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
		void(Cpu::* operate)(char*, char*);
		char* (Cpu::* param1)();
		char* (Cpu::* param2)();
		uint8_t cycles;
	};
private:
	uint16_t AF=0;
	uint16_t BC = 0;
	uint16_t DE = 0;
	uint16_t HL = 0;
	uint16_t SP = 0xfffe;
	uint16_t PC = 0x100;
	map<uint16_t, Opcode> opcodes;
	Ram ram;
public:

	Cpu();
	char* getA();
	char* getB();
	char* getC();
	char* getD();
	char* getE();
	char* getF();
	char* getH();
	char* getL();
	char* getAF();
	char* getBC();
	char* get$BC();
	char* getDE();
	char* get$DE();
	char* getHL();
	char* get$HL();
	char* getN();
	char* getNN();
	char* get$N();
	char* get$NN();
	char* getImmediate();
	char* getSP();
	void Execute(uint16_t opcode);
	void LD_nn_n(char* nn, char* n);
	void LD_r1_r2(char* r1, char* r2);
	void LD_A_n(char* A, char* n);
	void LD_n_A(char* n, char* A);
	void LD_A_$C(char* A, char* C);
	void LD_$C_A(char* C, char* A);
	void LDD_A_$HL(char* A, char* $HL);
	void LDD_$HL_A(char* $HL, char* A);
	void LDI_A_$HL(char* A, char* $HL);
	void LDI_$HL_A(char* $HL, char* A);
	void LDH_$N_A(char* $N, char* A);
	void LDH_A_$N(char* A, char* $N);
	void LD_n_nn(char* n, char* nn);
	void LD_SP_HL(char* SP, char* HL);
	void LDHL_SP_n(char* SP,char* N);
	void LDHL_$NN_SP(char* $NN, char* SP);
	void PUSH_nn(char* nn, char* SP);
	void POP_nn(char* SP, char* nn);
	void ADD_A_n(char* A, char* n);
	void ADC_A_n(char* A, char* n);
	void SUB_n(char* A, char* n);
	void SBC_A_n(char* A, char* n);
	void ADD_n(char* A, char* n);
	void OR_n(char* A, char* n);
	void XOR_n(char* A, char* n);
	void CP_n(char* A, char* n);
	void INC_n(char* n, char* null);
	void DEC_n(char* n, char* null);
	void ADD_HL_n(char* HL, char* n);
	void ADD_SP_n(char* SP, char* n);
	void INC_nn(char* nn, char* null);
	void DEC_nn(char* nn, char* null);
	char getFlag(char flag);
	void setFlag(char flag);
	void resetFlag(char flag);
	void setFlags(uint16_t r, uint16_t n);
	uint16_t BytesToWord(uint8_t msb, uint8_t lsB);
	Word WordToBytes(uint16_t word);
	Byte getByte(char byte);
	void SetWordIntoBytes(uint16_t* a, uint16_t* b);
	void SetByte(uint16_t* word, uint8_t value);
	//void* nn2nWrapper(void (*operate)(uint16_t, uint16_t),uint16_t* nn, uint8_t n) {
	//	return operate;
	//}
};


