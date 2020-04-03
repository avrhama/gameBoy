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
		void(CPU::* operate)(uint8_t*, uint8_t*, void(CPU::* setValue)(uint8_t*, uint8_t));
		uint8_t* (CPU::* param1)();
		uint8_t* (CPU::* param2)();
		void(CPU::* setValue)(uint8_t*, uint8_t);
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
	uint16_t * nn;
	uint8_t  * n;
	uint8_t immidiateN = 0;
	uint16_t immidiateNN = 0;
	/*uint8_t $n;
	uint16_t $nn;*/
	
	
	//Mmu mmu;
	uint8_t haltCPU = 0;
	uint8_t haltDisplay = 0;
	uint8_t IME = 1;
	bool IMEScheduled = true;
	enum Flag : uint8_t { NZ, Z, NC, C };
	uint8_t flagsNames[4] = { NZ,Z,NC,C };
	uint8_t RSTOffsets[8] = { 0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38 };
	int time;
	int cpuFreq = 4194304;
	int cyclesPerIncrementTIMA = 1024;//each cyclesPerSecond cycles of cpu increment tima register by 1.
	uint8_t cyclesPerIncrementDIVIDER = 255;// 4194304/16382
public:
	map<uint16_t, Opcode> opcodes;
	CPU();
	void connectToBus(BUS* bus);
	void setRegisterValue(uint8_t* dst, uint8_t value);
	void setMemoryValue(uint8_t* dst, uint8_t value);
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
	//typedef void(CPU::* setValue)(uint8_t*, uint8_t) ;
	typedef void (CPU::*setValueFunc)(uint8_t* dst, uint8_t value);
	void Execute(uint16_t opcode);
	void LD_nn_N(uint8_t* nn, uint8_t* n,setValueFunc setFunc);
	void LD_r1_r2(uint8_t* r1, uint8_t* r2, setValueFunc setFunc);
	void LD_A_n(uint8_t* A, uint8_t* n, setValueFunc setFunc);
	void LD_n_A(uint8_t* n, uint8_t* A, setValueFunc setFunc);
	void LD_A_C(uint8_t* A, uint8_t* C, setValueFunc setFunc);
	void LD_C_A(uint8_t* C, uint8_t* A, setValueFunc setFunc);
	void LDD_A_$HL(uint8_t* A, uint8_t* none2, setValueFunc setFunc);
	void LDD_$HL_A(uint8_t* none1, uint8_t* A, setValueFunc setFunc);
	void LDI_A_$HL(uint8_t* A, uint8_t* none2, setValueFunc setFunc);
	void LDI_$HL_A(uint8_t* none1, uint8_t* A, setValueFunc setFunc);
	void LDH_n_A(uint8_t* N, uint8_t* A, setValueFunc setFunc);
	void LDH_A_n(uint8_t* A, uint8_t* N, setValueFunc setFunc);
	void LD_n_nn(uint8_t* n, uint8_t* nn, setValueFunc setFunc);
	void LD_SP_HL(uint8_t* SP, uint8_t* HL, setValueFunc setFunc);
	void LDHL_SP_$n(uint8_t* SP,uint8_t* $N, setValueFunc setFunc);
	void LD_$NN_SP(uint8_t* $NN, uint8_t* SP, setValueFunc setFunc);
	void PUSH_nn(uint8_t* nn, uint8_t* SP, setValueFunc setFunc);
	void POP_nn(uint8_t* SP, uint8_t* nn, setValueFunc setFunc);
	void ADD_A_n(uint8_t* A, uint8_t* n, setValueFunc setFunc);
	void ADC_A_n(uint8_t* A, uint8_t* n, setValueFunc setFunc);
	void SUB_n(uint8_t* A, uint8_t* n, setValueFunc setFunc);
	void SBC_A_n(uint8_t* A, uint8_t* n, setValueFunc setFunc);
	void AND_n(uint8_t* A, uint8_t* n, setValueFunc setFunc);
	void OR_n(uint8_t* A, uint8_t* n, setValueFunc setFunc);
	void XOR_n(uint8_t* A, uint8_t* n, setValueFunc setFunc);
	void CP_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void INC_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void DEC_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void ADD_HL_n(uint8_t* HL, uint8_t* n, setValueFunc setFunc);
	void ADD_SP_n(uint8_t* SP, uint8_t* n, setValueFunc setFunc);
	void INC_nn(uint8_t* nn, uint8_t* none, setValueFunc setFunc);
	void DEC_nn(uint8_t* nn, uint8_t* none, setValueFunc setFunc);
	void SWAP_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void DAA(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void CPL(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void CCF(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void SCF(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void NOP(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void HALT(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void STOP(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void DI(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void EI(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void RLCA(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void RLA(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void RRCA(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void RRA(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void RLC_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void RL_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void RRC_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void RR_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void SLA_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void SRA_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void SRL_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void BIT_$n_r(uint8_t* $n, uint8_t* r, setValueFunc setFunc);
	void SET_$n_r(uint8_t* $n, uint8_t* r, setValueFunc setFunc);
	void RES_$n_r(uint8_t* $n, uint8_t* r, setValueFunc setFunc);
	void RES_$n_$HL(uint8_t* $n, uint8_t* none, setValueFunc setFunc);
	void JP_nn(uint8_t* nn, uint8_t* none, setValueFunc setFunc);
	void JP_cc_nn(uint8_t* cc, uint8_t* nn, setValueFunc setFunc);
	void JP_HL(uint8_t* HL, uint8_t* none, setValueFunc setFunc);
	void JR_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void JR_cc_n(uint8_t* cc, uint8_t* n, setValueFunc setFunc);
	void CALL_nn(uint8_t* nn, uint8_t* none, setValueFunc setFunc);
	void CALL_cc_nn(uint8_t* cc, uint8_t* nn, setValueFunc setFunc);
	void RST_n(uint8_t* n, uint8_t* none, setValueFunc setFunc);
	void RET(uint8_t* none, uint8_t* none2, setValueFunc setFunc);
	void RET_cc(uint8_t* cc, uint8_t* none, setValueFunc setFunc);
	void RETI(uint8_t* none, uint8_t* none2, setValueFunc setFunc);

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
	
	void reset();
	void updateCycelPerIncrementTIMA(uint8_t freqIndex);
	void updateTimers();
	uint8_t lastOpcodeCycles;
	

};


