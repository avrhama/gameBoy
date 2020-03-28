#pragma once
#include<cstdint>
#include <map>
#include "Ram.h"
#include "Structures.h"
#include "Cartridge.h"
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
	//uint16_t PC = 0;
	map<uint16_t, Opcode> opcodes;
	Ram ram;
	char haltCpu = 0;
	char haltDisplay = 0;
	char IME = 1;
	bool IMEScheduled = true;
	enum Flag : char { NZ, Z, NC, C };
	char flagsNames[4] = { NZ,Z,NC,C };
	char RSTOffsets[8] = { 0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38 };
	Cartridge cartridge;
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
	void InsertCartridge(Cartridge cartridge);
	void Execute(uint16_t opcode);
	void LD_nn_N(char* nn, char* n);
	void LD_r1_r2(char* r1, char* r2);
	void LD_A_n(char* A, char* n);
	void LD_n_A(char* n, char* A);
	void LD_A_C(char* A, char* C);
	void LD_C_A(char* C, char* A);
	void LDD_A_$HL(char* A, char* $HL);
	void LDD_$HL_A(char* $HL, char* A);
	void LDI_A_$HL(char* A, char* $HL);
	void LDI_$HL_A(char* $HL, char* A);
	void LDH_n_A(char* N, char* A);
	void LDH_A_n(char* A, char* N);
	void LD_n_nn(char* n, char* nn);
	void LD_SP_HL(char* SP, char* HL);
	void LDHL_SP_$n(char* SP,char* $N);
	void LD_$NN_SP(char* $NN, char* SP);
	void PUSH_nn(char* nn, char* SP);
	void POP_nn(char* SP, char* nn);
	void ADD_A_n(char* A, char* n);
	void ADC_A_n(char* A, char* n);
	void SUB_n(char* A, char* n);
	void SBC_A_n(char* A, char* n);
	void AND_n(char* A, char* n);
	void OR_n(char* A, char* n);
	void XOR_n(char* A, char* n);
	void CP_n(char* n, char* none);
	void INC_n(char* n, char* none);
	void DEC_n(char* n, char* none);
	void ADD_HL_n(char* HL, char* n);
	void ADD_SP_n(char* SP, char* n);
	void INC_nn(char* nn, char* none);
	void DEC_nn(char* nn, char* none);
	void SWAP_n(char* n, char* none);
	void DAA(char* none, char* none2);
	void CPL(char* none, char* none2);
	void CCF(char* none, char* none2);
	void SCF(char* none, char* none2);
	void NOP(char* none, char* none2);
	void HALT(char* none, char* none2);
	void STOP(char* none, char* none2);
	void DI(char* none, char* none2);
	void EI(char* none, char* none2);
	void RLCA(char* none, char* none2);
	void RLA(char* none, char* none2);
	void RRCA(char* none, char* none2);
	void RRA(char* none, char* none2);
	void RLC_n(char* n, char* none);
	void RL_n(char* n, char* none);
	void RRC_n(char* n, char* none);
	void RR_n(char* n, char* none);
	void SLA_n(char* n, char* none);
	void SRA_n(char* n, char* none);
	void SRL_n(char* n, char* none);
	void BIT_$n_r(char* $n, char* r);
	void SET_$n_r(char* $n, char* r);
	void RES_$n_r(char* $n, char* r);
	void JP_nn(char* nn, char* none);
	void JP_cc_nn(char* cc, char* nn);
	void JP_HL(char* HL, char* none);
	void JR_n(char* n, char* none);
	void JR_cc_n(char* cc, char* n);
	void CALL_nn(char* nn, char* none);
	void CALL_cc_nn(char* cc, char* nn);
	void RST_n(char* n, char* none);
	void RET(char* none, char* none2);
	void RET_cc(char* cc, char* none);
	void RETI(char* none, char* none2);

	char getFlag(char flag);
	void setFlag(char flag);
	void resetFlag(char flag);
	char* NZFlag();
	char* ZFlag();
	char* NCFlag();
	char* CFlag();
	char* getRST00();
	char* getRST08();
	char* getRST10();
	char* getRST18();
	char* getRST20();
	char* getRST28();
	char* getRST30();
	char* getRST38();
	void setFlags(uint16_t r, uint16_t n);
	uint16_t BytesToWord(uint8_t msb, uint8_t lsB);
	Word WordToBytes(uint16_t word);
	Byte getByte(char byte);
	char getByte(Byte byte);
	void SetWordIntoBytes(uint16_t* a, uint16_t* b);
	void SetByte(uint16_t* word, uint8_t value);

	//void* nn2nWrapper(void (*operate)(uint16_t, uint16_t),uint16_t* nn, uint8_t n) {
	//	return operate;
	//}
};


