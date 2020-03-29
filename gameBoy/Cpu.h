#pragma once
#include<cstdint>
#include <map>
#include "Structures.h"
#include "BUS.h"

using namespace std;


class CPU
{
	struct Opcode
	{
	public:
		const char* name;
		void(CPU::* operate)(uint8_t*, uint8_t*);
		uint8_t* (CPU::* param1)();
		uint8_t* (CPU::* param2)();
		uint8_t cycles;
	
	};
private:
	BUS* bus;
	uint16_t AF=0;
	uint16_t BC = 0;
	uint16_t DE = 0;
	uint16_t HL = 0;
	uint16_t SP = 0xfffe;
	//uint16_t PC = 0x100;
	map<uint16_t, Opcode> opcodes;
	//Mmu mmu;
	uint8_t haltCPU = 0;
	uint8_t haltDisplay = 0;
	uint8_t IME = 1;
	bool IMEScheduled = true;
	enum Flag : uint8_t { NZ, Z, NC, C };
	uint8_t flagsNames[4] = { NZ,Z,NC,C };
	uint8_t RSTOffsets[8] = { 0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38 };
	
public:
	
	CPU();
	void connectToBus(BUS* bus);
	uint16_t PC = 0;
	uint8_t* getA();
	uint8_t* getB();
	uint8_t* getC();
	uint8_t* getD();
	uint8_t* getE();
	uint8_t* getF();
	uint8_t* getH();
	uint8_t* getL();
	uint8_t* getAF();
	uint8_t* getBC();
	uint8_t* get$BC();
	uint8_t* getDE();
	uint8_t* get$DE();
	uint8_t* getHL();
	uint8_t* get$HL();
	uint8_t* getN();
	uint8_t* getNN();
	uint8_t* get$N();
	uint8_t* get$NN();
	uint8_t* getImmediate();
	uint8_t* getSP();
	uint8_t* getNULL();
	void Execute(uint16_t opcode);
	void LD_nn_N(uint8_t* nn, uint8_t* n);
	void LD_r1_r2(uint8_t* r1, uint8_t* r2);
	void LD_A_n(uint8_t* A, uint8_t* n);
	void LD_n_A(uint8_t* n, uint8_t* A);
	void LD_A_C(uint8_t* A, uint8_t* C);
	void LD_C_A(uint8_t* C, uint8_t* A);
	void LDD_A_$HL(uint8_t* A, uint8_t* $HL);
	void LDD_$HL_A(uint8_t* $HL, uint8_t* A);
	void LDI_A_$HL(uint8_t* A, uint8_t* $HL);
	void LDI_$HL_A(uint8_t* $HL, uint8_t* A);
	void LDH_n_A(uint8_t* N, uint8_t* A);
	void LDH_A_n(uint8_t* A, uint8_t* N);
	void LD_n_nn(uint8_t* n, uint8_t* nn);
	void LD_SP_HL(uint8_t* SP, uint8_t* HL);
	void LDHL_SP_$n(uint8_t* SP,uint8_t* $N);
	void LD_$NN_SP(uint8_t* $NN, uint8_t* SP);
	void PUSH_nn(uint8_t* nn, uint8_t* SP);
	void POP_nn(uint8_t* SP, uint8_t* nn);
	void ADD_A_n(uint8_t* A, uint8_t* n);
	void ADC_A_n(uint8_t* A, uint8_t* n);
	void SUB_n(uint8_t* A, uint8_t* n);
	void SBC_A_n(uint8_t* A, uint8_t* n);
	void AND_n(uint8_t* A, uint8_t* n);
	void OR_n(uint8_t* A, uint8_t* n);
	void XOR_n(uint8_t* A, uint8_t* n);
	void CP_n(uint8_t* n, uint8_t* none);
	void INC_n(uint8_t* n, uint8_t* none);
	void DEC_n(uint8_t* n, uint8_t* none);
	void ADD_HL_n(uint8_t* HL, uint8_t* n);
	void ADD_SP_n(uint8_t* SP, uint8_t* n);
	void INC_nn(uint8_t* nn, uint8_t* none);
	void DEC_nn(uint8_t* nn, uint8_t* none);
	void SWAP_n(uint8_t* n, uint8_t* none);
	void DAA(uint8_t* none, uint8_t* none2);
	void CPL(uint8_t* none, uint8_t* none2);
	void CCF(uint8_t* none, uint8_t* none2);
	void SCF(uint8_t* none, uint8_t* none2);
	void NOP(uint8_t* none, uint8_t* none2);
	void HALT(uint8_t* none, uint8_t* none2);
	void STOP(uint8_t* none, uint8_t* none2);
	void DI(uint8_t* none, uint8_t* none2);
	void EI(uint8_t* none, uint8_t* none2);
	void RLCA(uint8_t* none, uint8_t* none2);
	void RLA(uint8_t* none, uint8_t* none2);
	void RRCA(uint8_t* none, uint8_t* none2);
	void RRA(uint8_t* none, uint8_t* none2);
	void RLC_n(uint8_t* n, uint8_t* none);
	void RL_n(uint8_t* n, uint8_t* none);
	void RRC_n(uint8_t* n, uint8_t* none);
	void RR_n(uint8_t* n, uint8_t* none);
	void SLA_n(uint8_t* n, uint8_t* none);
	void SRA_n(uint8_t* n, uint8_t* none);
	void SRL_n(uint8_t* n, uint8_t* none);
	void BIT_$n_r(uint8_t* $n, uint8_t* r);
	void SET_$n_r(uint8_t* $n, uint8_t* r);
	void RES_$n_r(uint8_t* $n, uint8_t* r);
	void JP_nn(uint8_t* nn, uint8_t* none);
	void JP_cc_nn(uint8_t* cc, uint8_t* nn);
	void JP_HL(uint8_t* HL, uint8_t* none);
	void JR_n(uint8_t* n, uint8_t* none);
	void JR_cc_n(uint8_t* cc, uint8_t* n);
	void CALL_nn(uint8_t* nn, uint8_t* none);
	void CALL_cc_nn(uint8_t* cc, uint8_t* nn);
	void RST_n(uint8_t* n, uint8_t* none);
	void RET(uint8_t* none, uint8_t* none2);
	void RET_cc(uint8_t* cc, uint8_t* none);
	void RETI(uint8_t* none, uint8_t* none2);

	uint8_t getFlag(uint8_t flag);
	void setFlag(uint8_t flag);
	void resetFlag(uint8_t flag);
	uint8_t* NZFlag();
	uint8_t* ZFlag();
	uint8_t* NCFlag();
	uint8_t* CFlag();
	uint8_t* getRST00();
	uint8_t* getRST08();
	uint8_t* getRST10();
	uint8_t* getRST18();
	uint8_t* getRST20();
	uint8_t* getRST28();
	uint8_t* getRST30();
	uint8_t* getRST38();
	void setFlags(uint16_t r, uint16_t n);
	uint16_t BytesToWord(uint8_t msb, uint8_t lsB);
	Word WordToBytes(uint16_t word);
	Byte getByte(uint8_t byte);
	uint8_t getByte(Byte byte);
	void SetWordIntoBytes(uint16_t* a, uint16_t* b);
	void SetByte(uint16_t* word, uint8_t value);
	

};


