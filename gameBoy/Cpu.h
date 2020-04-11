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
	BUS* bus=NULL;
	
	uint16_t lastOpcode=0;
	//uint16_t PC = 0x100;
	//uint16_t* nn;
	//uint8_t* n;
	uint8_t immidiateN = 0;
	uint16_t immidiateNN = 0;
	/*uint8_t $n;
	uint16_t $nn;*/


	//Mmu mmu;
	enum class  ccFlag : uint8_t { NZ, Z, NC, C };
	ccFlag flagsNames[4] = { ccFlag::NZ,ccFlag::Z,ccFlag::NC,ccFlag::C };
	uint8_t RSTOffsets[8] = { 0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38 };
	enum  Flags : uint8_t {Z,N,H,C };
	int8_t flagsArray[4] = { -1,-1,-1,-1 };
	uint8_t bit=0;
	
	int cyclesPerIncrementTIMACounter = 0;
	int cyclesPerIncrementTIMA = 1024;//each cyclesPerSecond cycles of cpu increment tima register by 1.
	
public:
	int steps = 0;
	TIME time;
	uint8_t cycelsCounter = 0;
	int16_t cyclesPerIncrementDIVIDER = 255;// 4194304/16382
	uint16_t AF = 0;
	uint16_t BC = 0;
	uint16_t DE = 0;
	uint16_t HL = 0;
	uint16_t SP = 0xfffe;
	bool timaOverflow = false;
	bool IME=false;
	bool setIME = false;
	bool halt = false;
	int cpuFreq = 4194304;
	uint8_t speedMode = 1;
	 
	map<uint16_t, Opcode> opcodes;
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
	uint8_t* get$NN();
	uint8_t* getSP();
	uint8_t* getNULL();
	void Execute(uint16_t opcode);
	void ExecuteOpcode(uint16_t opcode);
	void LD_nn_N(uint8_t* nn, uint8_t* n);
	void LD_r1_r2(uint8_t* r1, uint8_t* r2);
	void LD_n_$HL(uint8_t* none, uint8_t* r2);
	void LD_A_n(uint8_t* A, uint8_t* n);
	void LD_n_A(uint8_t* n, uint8_t* A);
	void LD_$n_A(uint8_t* n, uint8_t* A);
	void LD_A_C(uint8_t* A, uint8_t* C);
	void LD_C_A(uint8_t* C, uint8_t* A);
	void LDD_A_$HL(uint8_t* A, uint8_t* none);
	void LDD_$HL_A(uint8_t* none, uint8_t* A);
	void LDI_A_$HL(uint8_t* A, uint8_t* none);
	void LDI_$HL_A(uint8_t* none, uint8_t* A);
	void LDH_n_A(uint8_t* N, uint8_t* A);
	void LDH_A_n(uint8_t* A, uint8_t* N);
	void LD_n_nn(uint8_t* n, uint8_t* nn);
	void LD_SP_HL(uint8_t* none1, uint8_t* none2);
	void LDHL_SP_$n(uint8_t* none1, uint8_t* none2);
	void LD_$NN_SP(uint8_t* $NN, uint8_t* none);
	void PUSH_nn(uint8_t* nn, uint8_t* none);
	void POP_nn(uint8_t* none, uint8_t* nn);
	void ADD_A_n(uint8_t* A, uint8_t* n);
	void ADC_A_n(uint8_t* A, uint8_t* n);
	void SUB_n(uint8_t* A, uint8_t* n);
	void SBC_A_n(uint8_t* A, uint8_t* n);
	void AND_n(uint8_t* A, uint8_t* n);
	void OR_n(uint8_t* A, uint8_t* n);
	void XOR_n(uint8_t* A, uint8_t* n);
	void CP_n(uint8_t* n, uint8_t* none);
	void INC_n(uint8_t* n, uint8_t* none);
	void INC_$HL(uint8_t* none1, uint8_t* none2);
	void DEC_n(uint8_t* n, uint8_t* none);
	void DEC_$HL(uint8_t* none1, uint8_t* none2);
	void ADD_HL_n(uint8_t* none, uint8_t* n);
	void ADD_SP_n(uint8_t* SP, uint8_t* n);
	void INC_nn(uint8_t* nn, uint8_t* none);
	void DEC_nn(uint8_t* nn, uint8_t* none);
	void SWAP_n(uint8_t* n, uint8_t* none);
	void SWAP_$HL(uint8_t* none1, uint8_t* none2);
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
	void RLC_$HL(uint8_t* none1, uint8_t* none2);
	void RL_n(uint8_t* n, uint8_t* none);
	void RL_$HL(uint8_t* none1, uint8_t* none2);
	void RRC_n(uint8_t* n, uint8_t* none);
	void RRC_$HL(uint8_t* none1, uint8_t* none2);
	void RR_n(uint8_t* n, uint8_t* none);
	void RR_$HL(uint8_t* none1, uint8_t* none2);
	void SLA_n(uint8_t* n, uint8_t* none);
	void SLA_$HL(uint8_t* none1, uint8_t* none2);
	void SRA_n(uint8_t* n, uint8_t* none);
	void SRA_$HL(uint8_t* none1, uint8_t* none2);
	void SRL_n(uint8_t* n, uint8_t* none);
	void SRL_$HL(uint8_t* none1, uint8_t* none2);
	void BIT_$n_r(uint8_t* $n, uint8_t* r);
	void SET_$n_r(uint8_t* $n, uint8_t* r);
	void SET_$n_$HL(uint8_t* $n, uint8_t* none);
	void RES_$n_r(uint8_t* $n, uint8_t* r);
	void RES_$n_$HL(uint8_t* $n, uint8_t* none);
	void JP_nn(uint8_t* nn, uint8_t* none);
	void JP_cc_nn(uint8_t* cc, uint8_t* nn);
	void JP_HL(uint8_t* none1, uint8_t* none2);
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
	void updateFlags();
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
	uint8_t* getBIT0();
	uint8_t* getBIT1();
	uint8_t* getBIT2();
	uint8_t* getBIT3();
	uint8_t* getBIT4();
	uint8_t* getBIT5();
	uint8_t* getBIT6();
	uint8_t* getBIT7();
	void setFlags(uint16_t r, uint16_t n);
	uint16_t BytesToWord(uint8_t msb, uint8_t lsB);
	Word WordToBytes(uint16_t word);
	Byte getByte(uint8_t byte);
	uint8_t getByte(Byte byte);
	void SetWordIntoBytes(uint16_t* a, uint16_t* b);
	void SetByte(uint16_t* word, uint8_t value);

	void reset();
	int getCycelPerIncrementTIMA(uint8_t freqIndex);
	void isCycelPerIncrementTIMAPassedHalf(int currCycelPerIncrementTIMA);
	void updateCycelPerIncrementTIMA(uint8_t freqIndex);
	void updateTimers();
	uint8_t lastOpcodeCycles;
	uint8_t getOpcode();

	


};

