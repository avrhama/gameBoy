#include "Cpu.h"
Cpu::Cpu()
{
	//LD nn,n
	opcodes[0x06] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getB,&Cpu::getN,8 };
	opcodes[0x0E] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getC,&Cpu::getN,8 };
	opcodes[0x16] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getD,&Cpu::getN,8 };
	opcodes[0x1E] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getE,&Cpu::getN,8 };
	opcodes[0x26] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getH,&Cpu::getN,8 };
	opcodes[0x2E] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getL,&Cpu::getN,8 };
	//LD r1,r2
	opcodes[0x7F] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x78] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x79] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x7A] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x7B] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x7C] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x7D] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x7E] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0x40] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getB,4 };
	opcodes[0x41] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getC,4 };
	opcodes[0x42] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getD,4 };
	opcodes[0x43] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getE,4 };
	opcodes[0x44] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getH,4 };
	opcodes[0x45] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getL,4 };
	opcodes[0x46] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::get$HL,8 };
	opcodes[0x48] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getB,4 };
	opcodes[0x49] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getC,4 };
	opcodes[0x4A] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getD,4 };
	opcodes[0x4B] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getE,4 };
	opcodes[0x4C] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getH,4 };
	opcodes[0x4D] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getL,4 };
	opcodes[0x4E] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::get$HL,8 };
	opcodes[0x50] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getB,4 };
	opcodes[0x51] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getC,4 };
	opcodes[0x52] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getD,4 };
	opcodes[0x53] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getE,4 };
	opcodes[0x54] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getH,4 };
	opcodes[0x55] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getL,4 };
	opcodes[0x56] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::get$HL,8 };
	opcodes[0x58] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getB,4 };
	opcodes[0x59] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getC,4 };
	opcodes[0x5A] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getD,4 };
	opcodes[0x5B] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getE,4 };
	opcodes[0x5C] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getH,4 };
	opcodes[0x5D] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getL,4 };
	opcodes[0x5E] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::get$HL,8 };
	opcodes[0x60] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getB,4 };
	opcodes[0x61] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getC,4 };
	opcodes[0x62] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getD,4 };
	opcodes[0x63] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getE,4 };
	opcodes[0x64] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getH,4 };
	opcodes[0x65] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getL,4 };
	opcodes[0x66] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::get$HL,8 };
	opcodes[0x68] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getB,4 };
	opcodes[0x69] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getC,4 };
	opcodes[0x6A] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getD,4 };
	opcodes[0x6B] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getE,4 };
	opcodes[0x6C] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getH,4 };
	opcodes[0x6D] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getL,4 };
	opcodes[0x6E] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::get$HL,8 };
	opcodes[0x70] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getB,8 };
	opcodes[0x71] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getC,8 };
	opcodes[0x72] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getD,8 };
	opcodes[0x73] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getE,8 };
	opcodes[0x74] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getH,8 };
	opcodes[0x75] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getL,8 };
	opcodes[0x36] = { "LD_r1_r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getN,12 };
	//LD A,n
	opcodes[0x0A] = { "LD_A_n",&Cpu::LD_A_n,&Cpu::getA,&Cpu::get$BC,8 };
	opcodes[0x1A] = { "LD_A_n",&Cpu::LD_A_n,&Cpu::getA,&Cpu::get$DE,8 };
	opcodes[0xFA] = { "LD_A_n",&Cpu::LD_A_n,&Cpu::getA,&Cpu::get$NN,16 };
	opcodes[0x3E] = { "LD_A_n",&Cpu::LD_A_n,&Cpu::getA,&Cpu::get$N,8 };
	//LD n,A
	opcodes[0x47] = { "LD_n_A",&Cpu::LD_n_A,&Cpu::getB,&Cpu::getA,4 };
	opcodes[0x4F] = { "LD_n_A",&Cpu::LD_n_A,&Cpu::getC,&Cpu::getA,4 };
	opcodes[0x57] = { "LD_n_A",&Cpu::LD_n_A,&Cpu::getD,&Cpu::getA,4 };
	opcodes[0x5F] = { "LD_n_A",&Cpu::LD_n_A,&Cpu::getE,&Cpu::getA,4 };
	opcodes[0x67] = { "LD_n_A",&Cpu::LD_n_A,&Cpu::getH,&Cpu::getA,4 };
	opcodes[0x6F] = { "LD_n_A",&Cpu::LD_n_A,&Cpu::getL,&Cpu::getA,4 };
	opcodes[0x02] = { "LD_n_A",&Cpu::LD_n_A,&Cpu::get$BC,&Cpu::getA,8 };
	opcodes[0x12] = { "LD_n_A",&Cpu::LD_n_A,&Cpu::get$DE,&Cpu::getA,8 };
	opcodes[0x77] = { "LD_n_A",&Cpu::LD_n_A,&Cpu::get$HL,&Cpu::getA,8 };
	opcodes[0xEA] = { "LD_n_A",&Cpu::LD_n_A,&Cpu::get$NN,&Cpu::getA,16 };
	//LD A,(C)
	opcodes[0xF2] = { "LD_A_$C",&Cpu::LD_A_$C,&Cpu::getA,&Cpu::getF,8 };
	//LD (C),A
	opcodes[0xE2] = { "LD_$C_A",&Cpu::LD_$C_A,&Cpu::getF,&Cpu::getA,8 };
	//LD A,(HLD)
	//LD A,(HLminus)
	//LDD A,(HL)
	opcodes[0x3A] = { "LDD_A_$HL",&Cpu::LDD_A_$HL,&Cpu::getA,&Cpu::get$HL,8 };
	//LD (HLD),A
	//LD (HLminus),A
	//LDD (HL),A
	opcodes[0x32] = { "LDD_$HL_A",&Cpu::LDD_$HL_A,&Cpu::get$HL,&Cpu::getA,8 };
	//LD A,(HLI)
	//LD A,(HLplus)
	//LDI A,(HL)
	opcodes[0x2A] = { "LDI_A_$HL",&Cpu::LDI_A_$HL,&Cpu::getA,&Cpu::get$HL,8 };
	//LD (HLI),A
	//LD (HLplus),A
	//LDI (HL),A
	opcodes[0x22] = { "LDI_$HL_A",&Cpu::LDI_$HL_A,&Cpu::get$HL,&Cpu::getA,8 };
	//LDH n,A
	opcodes[0xE0] = { "LDH_n_A",&Cpu::LDH_n_A,&Cpu::getN,&Cpu::getA,12 };
	
	//LDH A,n
	opcodes[0xF0] = { "LDH_A_n",&Cpu::LDH_A_n,&Cpu::getA,&Cpu::getN,12 };
	//LD n,nn
	opcodes[0x01] = { "LD_n_nn",&Cpu::LD_n_nn,&Cpu::getBC,&Cpu::getNN,12 };
	opcodes[0x11] = { "LD_n_nn",&Cpu::LD_n_nn,&Cpu::getDE,&Cpu::getNN,12 };
	opcodes[0x21] = { "LD_n_nn",&Cpu::LD_n_nn,&Cpu::getHL,&Cpu::getNN,12 };
	opcodes[0x31] = { "LD_n_nn",&Cpu::LD_n_nn,&Cpu::getSP,&Cpu::getNN,12 };
	//LD SP,HL
	opcodes[0xF9] = { "LD_SP_HL",&Cpu::LD_SP_HL,&Cpu::getSP,&Cpu::getHL,8 };
	//LD_HL_SP
	//LDHL SP,(n)
	opcodes[0xF8] = { "LDHL_SP_$n",&Cpu::LDHL_SP_$n,&Cpu::getSP,&Cpu::get$N,12 };
	//LD nn,SP
	opcodes[0x08] = { "LD_nn_SP",&Cpu::LD_nn_SP,&Cpu::getNN,&Cpu::getSP,20 };
	//PUSH nn
	opcodes[0xF5] = { "PUSH_nn",&Cpu::PUSH_nn,&Cpu::getAF,&Cpu::getSP,16 };
	opcodes[0xC5] = { "PUSH_nn",&Cpu::PUSH_nn,&Cpu::getBC,&Cpu::getSP,16 };
	opcodes[0xD5] = { "PUSH_nn",&Cpu::PUSH_nn,&Cpu::getDE,&Cpu::getSP,16 };
	opcodes[0xE5] = { "PUSH_nn",&Cpu::PUSH_nn,&Cpu::getHL,&Cpu::getSP,16 };
	//POP nn
	opcodes[0xF1] = { "POP_nn",&Cpu::POP_nn,&Cpu::getSP,&Cpu::getAF,12 };
	opcodes[0xC1] = { "POP_nn",&Cpu::POP_nn,&Cpu::getSP,&Cpu::getBC,12 };
	opcodes[0xD1] = { "POP_nn",&Cpu::POP_nn,&Cpu::getSP,&Cpu::getDE,12 };
	opcodes[0xE1] = { "POP_nn",&Cpu::POP_nn,&Cpu::getSP,&Cpu::getHL,12 };
	//ADD A,n
	opcodes[0x87] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x80] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x81] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x82] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x83] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x84] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x85] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x86] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xC6] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::get$N,8 };
	//ADC A,n
	opcodes[0x8F] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x88] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x89] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x8A] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x8B] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x8C] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x8D] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x8E] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xCE] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::get$N,8 };
	//SUB n
	opcodes[0x97] = { "SUB_n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x90] = { "SUB_n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x91] = { "SUB_n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x92] = { "SUB_n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x93] = { "SUB_n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x94] = { "SUB_n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x95] = { "SUB_n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x96] = { "SUB_n",&Cpu::SUB_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xD6] = { "SUB_n",&Cpu::SUB_n,&Cpu::getA,&Cpu::get$N,8 };
	//SBC A,n
	opcodes[0x9F] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x98] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x99] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x9A] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x9B] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x9C] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x9D] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x9E] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::get$HL,8 };
	//AND n
	opcodes[0xA7] = { "AND_n",&Cpu::AND_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0xA0] = { "AND_n",&Cpu::AND_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0xA1] = { "AND_n",&Cpu::AND_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0xA2] = { "AND_n",&Cpu::AND_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0xA3] = { "AND_n",&Cpu::AND_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0xA4] = { "AND_n",&Cpu::AND_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0xA5] = { "AND_n",&Cpu::AND_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0xA6] = { "AND_n",&Cpu::AND_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xE6] = { "AND_n",&Cpu::AND_n,&Cpu::getA,&Cpu::get$N,8 };
	//OR n
	opcodes[0xB7] = { "OR_n",&Cpu::OR_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0xB0] = { "OR_n",&Cpu::OR_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0xB1] = { "OR_n",&Cpu::OR_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0xB2] = { "OR_n",&Cpu::OR_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0xB3] = { "OR_n",&Cpu::OR_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0xB4] = { "OR_n",&Cpu::OR_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0xB5] = { "OR_n",&Cpu::OR_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0xB6] = { "OR_n",&Cpu::OR_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xF6] = { "OR_n",&Cpu::OR_n,&Cpu::getA,&Cpu::get$N,8 };
	//XOR n
	opcodes[0xAF] = { "XOR_n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0xA8] = { "XOR_n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0xA9] = { "XOR_n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0xAA] = { "XOR_n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0xAB] = { "XOR_n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0xAC] = { "XOR_n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0xAD] = { "XOR_n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0xAE] = { "XOR_n",&Cpu::XOR_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xEE] = { "XOR_n",&Cpu::XOR_n,&Cpu::getA,&Cpu::get$N,8 };
	//CP n
	opcodes[0xBF] = { "CP_n",&Cpu::CP_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0xB8] = { "CP_n",&Cpu::CP_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0xB9] = { "CP_n",&Cpu::CP_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0xBA] = { "CP_n",&Cpu::CP_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0xBB] = { "CP_n",&Cpu::CP_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0xBC] = { "CP_n",&Cpu::CP_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0xBD] = { "CP_n",&Cpu::CP_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0xBE] = { "CP_n",&Cpu::CP_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xFE] = { "CP_n",&Cpu::CP_n,&Cpu::getA,&Cpu::get$N,8 };
	//INC n
	opcodes[0x3C] = { "INC_n",&Cpu::INC_n,&Cpu::getA,NULL,4 };
	opcodes[0x04] = { "INC_n",&Cpu::INC_n,&Cpu::getB,NULL,4 };
	opcodes[0x0C] = { "INC_n",&Cpu::INC_n,&Cpu::getC,NULL,4 };
	opcodes[0x14] = { "INC_n",&Cpu::INC_n,&Cpu::getD,NULL,4 };
	opcodes[0x1C] = { "INC_n",&Cpu::INC_n,&Cpu::getE,NULL,4 };
	opcodes[0x24] = { "INC_n",&Cpu::INC_n,&Cpu::getH,NULL,4 };
	opcodes[0x2C] = { "INC_n",&Cpu::INC_n,&Cpu::getL,NULL,4 };
	opcodes[0x34] = { "INC_n",&Cpu::INC_n,&Cpu::get$HL,NULL,12 };
	//DEC n
	opcodes[0x3D] = { "DEC_n",&Cpu::DEC_n,&Cpu::getA,NULL,4 };
	opcodes[0x05] = { "DEC_n",&Cpu::DEC_n,&Cpu::getB,NULL,4 };
	opcodes[0x0D] = { "DEC_n",&Cpu::DEC_n,&Cpu::getC,NULL,4 };
	opcodes[0x15] = { "DEC_n",&Cpu::DEC_n,&Cpu::getD,NULL,4 };
	opcodes[0x1D] = { "DEC_n",&Cpu::DEC_n,&Cpu::getE,NULL,4 };
	opcodes[0x25] = { "DEC_n",&Cpu::DEC_n,&Cpu::getH,NULL,4 };
	opcodes[0x2D] = { "DEC_n",&Cpu::DEC_n,&Cpu::getL,NULL,4 };
	opcodes[0x35] = { "DEC_n",&Cpu::DEC_n,&Cpu::get$HL,NULL,12 };
	//ADD HL,n
	opcodes[0x09] = { "ADD_HL_n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getBC,8 };
	opcodes[0x19] = { "ADD_HL_n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getDE,8 };
	opcodes[0x29] = { "ADD_HL_n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getHL,8 };
	opcodes[0x39] = { "ADD_HL_n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getSP,8 };
	//ADD SP,n
	opcodes[0xE8] = { "ADD_SP_n",&Cpu::ADD_SP_n,&Cpu::getSP,&Cpu::get$N,16 };
	//INC nn
	opcodes[0x03] = { "INC_nn",&Cpu::INC_nn,&Cpu::getBC,NULL,8 };
	opcodes[0x13] = { "INC_nn",&Cpu::INC_nn,&Cpu::getDE,NULL,8 };
	opcodes[0x23] = { "INC_nn",&Cpu::INC_nn,&Cpu::getHL,NULL,8 };
	opcodes[0x33] = { "INC_nn",&Cpu::INC_nn,&Cpu::getSP,NULL,8 };
	//DEC nn
	opcodes[0x0B] = { "DEC_nn",&Cpu::DEC_nn,&Cpu::getBC,NULL,8 };
	opcodes[0x1B] = { "DEC_nn",&Cpu::DEC_nn,&Cpu::getDE,NULL,8 };
	opcodes[0x2B] = { "DEC_nn",&Cpu::DEC_nn,&Cpu::getHL,NULL,8 };
	opcodes[0x3B] = { "DEC_nn",&Cpu::DEC_nn,&Cpu::getSP,NULL,8 };
	//SWAP n
	opcodes[0xCB37] = { "SWAP_n",&Cpu::SWAP_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB30] = { "SWAP_n",&Cpu::SWAP_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB31] = { "SWAP_n",&Cpu::SWAP_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB32] = { "SWAP_n",&Cpu::SWAP_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB33] = { "SWAP_n",&Cpu::SWAP_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB34] = { "SWAP_n",&Cpu::SWAP_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB35] = { "SWAP_n",&Cpu::SWAP_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB36] = { "SWAP_n",&Cpu::SWAP_n,&Cpu::get$HL,NULL,16 };
	//DAA
	opcodes[0x27] = { "DAA",&Cpu::DAA,NULL,NULL,4 };
	//CPL
	opcodes[0x2F] = { "CPL",&Cpu::CPL,NULL,NULL,4 };
	//CCF
	opcodes[0x3F] = { "CCF",&Cpu::CCF,NULL,NULL,4 };
	//SCF
	opcodes[0x37] = { "SCF",&Cpu::SCF,NULL,NULL,4 };
	//NOP
	opcodes[0x00] = { "NOP",&Cpu::NOP,NULL,NULL,4 };
	//HALT
	opcodes[0x76] = { "HALT",&Cpu::HALT,NULL,NULL,4 };
	//STOP
	opcodes[0x1000] = { "STOP",&Cpu::STOP,NULL,NULL,4 };
	//DI
	opcodes[0xF3] = { "DI",&Cpu::DI,NULL,NULL,4 };
	//EI
	opcodes[0xFB] = { "EI",&Cpu::EI,NULL,NULL,4 };
	//RLCA
	opcodes[0x07] = { "RLCA",&Cpu::RLCA,NULL,NULL,4 };
	//RLA
	opcodes[0x17] = { "RLA",&Cpu::RLA,NULL,NULL,4 };
	//RRCA
	opcodes[0x0F] = { "RRCA",&Cpu::RRCA,NULL,NULL,4 };
	//RRA
	opcodes[0x1F] = { "RRA",&Cpu::RRA,NULL,NULL,4 };
	//RLC n
	opcodes[0xCB07] = { "RLC_n",&Cpu::RLC_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB00] = { "RLC_n",&Cpu::RLC_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB01] = { "RLC_n",&Cpu::RLC_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB02] = { "RLC_n",&Cpu::RLC_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB03] = { "RLC_n",&Cpu::RLC_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB04] = { "RLC_n",&Cpu::RLC_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB05] = { "RLC_n",&Cpu::RLC_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB06] = { "RLC_n",&Cpu::RLC_n,&Cpu::get$HL,NULL,16 };
	//RL n
	opcodes[0xCB17] = { "RL_n",&Cpu::RL_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB10] = { "RL_n",&Cpu::RL_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB11] = { "RL_n",&Cpu::RL_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB12] = { "RL_n",&Cpu::RL_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB13] = { "RL_n",&Cpu::RL_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB14] = { "RL_n",&Cpu::RL_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB15] = { "RL_n",&Cpu::RL_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB16] = { "RL_n",&Cpu::RL_n,&Cpu::get$HL,NULL,16 };
	//RRC n
	opcodes[0xCB0F] = { "RRC_n",&Cpu::RRC_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB08] = { "RRC_n",&Cpu::RRC_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB09] = { "RRC_n",&Cpu::RRC_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB0A] = { "RRC_n",&Cpu::RRC_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB0B] = { "RRC_n",&Cpu::RRC_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB0C] = { "RRC_n",&Cpu::RRC_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB0D] = { "RRC_n",&Cpu::RRC_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB0E] = { "RRC_n",&Cpu::RRC_n,&Cpu::get$HL,NULL,16 };
	//RR n
	opcodes[0xCB1F] = { "RR_n",&Cpu::RR_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB18] = { "RR_n",&Cpu::RR_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB19] = { "RR_n",&Cpu::RR_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB1A] = { "RR_n",&Cpu::RR_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB1B] = { "RR_n",&Cpu::RR_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB1C] = { "RR_n",&Cpu::RR_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB1D] = { "RR_n",&Cpu::RR_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB1E] = { "RR_n",&Cpu::RR_n,&Cpu::get$HL,NULL,16 };
	//SLA n
	opcodes[0xCB27] = { "SLA_n",&Cpu::SLA_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB20] = { "SLA_n",&Cpu::SLA_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB21] = { "SLA_n",&Cpu::SLA_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB22] = { "SLA_n",&Cpu::SLA_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB23] = { "SLA_n",&Cpu::SLA_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB24] = { "SLA_n",&Cpu::SLA_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB25] = { "SLA_n",&Cpu::SLA_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB26] = { "SLA_n",&Cpu::SLA_n,&Cpu::get$HL,NULL,16 };
	//SRA n
	opcodes[0xCB2F] = { "SRA_n",&Cpu::SRA_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB28] = { "SRA_n",&Cpu::SRA_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB29] = { "SRA_n",&Cpu::SRA_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB2A] = { "SRA_n",&Cpu::SRA_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB2B] = { "SRA_n",&Cpu::SRA_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB2C] = { "SRA_n",&Cpu::SRA_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB2D] = { "SRA_n",&Cpu::SRA_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB2E] = { "SRA_n",&Cpu::SRA_n,&Cpu::get$HL,NULL,16 };
	//SRL n
	opcodes[0xCB3F] = { "SRL_n",&Cpu::SRL_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB38] = { "SRL_n",&Cpu::SRL_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB39] = { "SRL_n",&Cpu::SRL_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB3A] = { "SRL_n",&Cpu::SRL_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB3B] = { "SRL_n",&Cpu::SRL_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB3C] = { "SRL_n",&Cpu::SRL_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB3D] = { "SRL_n",&Cpu::SRL_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB3E] = { "SRL_n",&Cpu::SRL_n,&Cpu::get$HL,NULL,16 };
	//BIT (n),r
	opcodes[0xCB47] = { "BIT_$n_r",&Cpu::BIT_$n_r,&Cpu::get$N,&Cpu::getA,8 };
	opcodes[0xCB40] = { "BIT_$n_r",&Cpu::BIT_$n_r,&Cpu::get$N,&Cpu::getB,8 };
	opcodes[0xCB41] = { "BIT_$n_r",&Cpu::BIT_$n_r,&Cpu::get$N,&Cpu::getC,8 };
	opcodes[0xCB42] = { "BIT_$n_r",&Cpu::BIT_$n_r,&Cpu::get$N,&Cpu::getD,8 };
	opcodes[0xCB43] = { "BIT_$n_r",&Cpu::BIT_$n_r,&Cpu::get$N,&Cpu::getE,8 };
	opcodes[0xCB44] = { "BIT_$n_r",&Cpu::BIT_$n_r,&Cpu::get$N,&Cpu::getH,8 };
	opcodes[0xCB45] = { "BIT_$n_r",&Cpu::BIT_$n_r,&Cpu::get$N,&Cpu::getL,8 };
	opcodes[0xCB46] = { "BIT_$n_r",&Cpu::BIT_$n_r,&Cpu::get$N,&Cpu::get$HL,16 };
	//SET (n),r
	opcodes[0xCBC7] = { "SET_$n_r",&Cpu::SET_$n_r,&Cpu::get$N,&Cpu::getA,8 };
	opcodes[0xCBC0] = { "SET_$n_r",&Cpu::SET_$n_r,&Cpu::get$N,&Cpu::getB,8 };
	opcodes[0xCBC1] = { "SET_$n_r",&Cpu::SET_$n_r,&Cpu::get$N,&Cpu::getC,8 };
	opcodes[0xCBC2] = { "SET_$n_r",&Cpu::SET_$n_r,&Cpu::get$N,&Cpu::getD,8 };
	opcodes[0xCBC3] = { "SET_$n_r",&Cpu::SET_$n_r,&Cpu::get$N,&Cpu::getE,8 };
	opcodes[0xCBC4] = { "SET_$n_r",&Cpu::SET_$n_r,&Cpu::get$N,&Cpu::getH,8 };
	opcodes[0xCBC5] = { "SET_$n_r",&Cpu::SET_$n_r,&Cpu::get$N,&Cpu::getL,8 };
	opcodes[0xCBC6] = { "SET_$n_r",&Cpu::SET_$n_r,&Cpu::get$N,&Cpu::get$HL,16 };
	//RES (n),r
	opcodes[0xCB87] = { "RES_$n_r",&Cpu::RES_$n_r,&Cpu::get$N,&Cpu::getA,8 };
	opcodes[0xCB80] = { "RES_$n_r",&Cpu::RES_$n_r,&Cpu::get$N,&Cpu::getB,8 };
	opcodes[0xCB81] = { "RES_$n_r",&Cpu::RES_$n_r,&Cpu::get$N,&Cpu::getC,8 };
	opcodes[0xCB82] = { "RES_$n_r",&Cpu::RES_$n_r,&Cpu::get$N,&Cpu::getD,8 };
	opcodes[0xCB83] = { "RES_$n_r",&Cpu::RES_$n_r,&Cpu::get$N,&Cpu::getE,8 };
	opcodes[0xCB84] = { "RES_$n_r",&Cpu::RES_$n_r,&Cpu::get$N,&Cpu::getH,8 };
	opcodes[0xCB85] = { "RES_$n_r",&Cpu::RES_$n_r,&Cpu::get$N,&Cpu::getL,8 };
	opcodes[0xCB86] = { "RES_$n_r",&Cpu::RES_$n_r,&Cpu::get$N,&Cpu::get$HL,16 };
	//JP nn
	opcodes[0xC3] = { "JP_nn",&Cpu::JP_nn,&Cpu::getNN,NULL,12 };
	//JP cc,nn
	opcodes[0xC2] = { "JP_cc_nn",&Cpu::JP_cc_nn,&Cpu::NZFlag,&Cpu::getNN,12 };
	opcodes[0xCA] = { "JP_cc_nn",&Cpu::JP_cc_nn,&Cpu::ZFlag,&Cpu::getNN,12 };
	opcodes[0xD2] = { "JP_cc_nn",&Cpu::JP_cc_nn,&Cpu::NCFlag,&Cpu::getNN,12 };
	opcodes[0xDA] = { "JP_cc_nn",&Cpu::JP_cc_nn,&Cpu::CFlag,&Cpu::getNN,12 };
	//JP HL
	opcodes[0xE9] = { "JP_HL",&Cpu::JP_HL,&Cpu::get$HL,NULL,4 };
	//JR n
	opcodes[0x18] = { "JR_n",&Cpu::JR_n,&Cpu::getN,NULL,8 };
	//JR cc,n
	opcodes[0x20] = { "JR_cc_n",&Cpu::JR_cc_n,&Cpu::getF,&Cpu::getN,8 };
	opcodes[0x28] = { "JR_cc_n",&Cpu::JR_cc_n,&Cpu::getF,&Cpu::getN,8 };
	opcodes[0x30] = { "JR_cc_n",&Cpu::JR_cc_n,&Cpu::getF,&Cpu::getN,8 };
	opcodes[0x38] = { "JR_cc_n",&Cpu::JR_cc_n,&Cpu::getC,&Cpu::getN,8 };
	//CALL nn
	opcodes[0xCD] = { "CALL_nn",&Cpu::CALL_nn,&Cpu::getNN,NULL,12 };
	//CALL cc,nn
	opcodes[0xC4] = { "CALL_cc_nn",&Cpu::CALL_cc_nn,&Cpu::getF,&Cpu::getNN,12 };
	opcodes[0xCC] = { "CALL_cc_nn",&Cpu::CALL_cc_nn,&Cpu::getF,&Cpu::getNN,12 };
	opcodes[0xD4] = { "CALL_cc_nn",&Cpu::CALL_cc_nn,&Cpu::getF,&Cpu::getNN,12 };
	opcodes[0xDC] = { "CALL_cc_nn",&Cpu::CALL_cc_nn,&Cpu::getC,&Cpu::getNN,12 };
	//RST n
	opcodes[0xC7] = { "RST_n",&Cpu::RST_n,&Cpu::getRST00,NULL,32 };
	opcodes[0xCF] = { "RST_n",&Cpu::RST_n,&Cpu::getRST08,NULL,32 };
	opcodes[0xD7] = { "RST_n",&Cpu::RST_n,&Cpu::getRST10,NULL,32 };
	opcodes[0xDF] = { "RST_n",&Cpu::RST_n,&Cpu::getRST18,NULL,32 };
	opcodes[0xE7] = { "RST_n",&Cpu::RST_n,&Cpu::getRST20,NULL,32 };
	opcodes[0xEF] = { "RST_n",&Cpu::RST_n,&Cpu::getRST28,NULL,32 };
	opcodes[0xF7] = { "RST_n",&Cpu::RST_n,&Cpu::getRST30,NULL,32 };
	opcodes[0xFF] = { "RST_n",&Cpu::RST_n,&Cpu::getRST38,NULL,32 };
	//RET
	opcodes[0xC9] = { "RET",&Cpu::RET,NULL,NULL,8 };
	//RET cc
	opcodes[0xC0] = { "RET_cc",&Cpu::RET_cc,&Cpu::getF,NULL,8 };
	opcodes[0xC8] = { "RET_cc",&Cpu::RET_cc,&Cpu::getF,NULL,8 };
	opcodes[0xD0] = { "RET_cc",&Cpu::RET_cc,&Cpu::getF,NULL,8 };
	opcodes[0xD8] = { "RET_cc",&Cpu::RET_cc,&Cpu::getC,NULL,8 };
	//RETI
	opcodes[0xD9] = { "RETI",&Cpu::RETI,NULL,NULL,8 };


}
char* Cpu::getA()
{
	return ((char*)&AF + 1);

}
char* Cpu::getF()
{
	return (char*)&AF;
}
char* Cpu::getB()
{
	return ((char*)&BC + 1);
}
char* Cpu::getC()
{
	return (char*)&BC;
}
char* Cpu::getD()
{
	return ((char*)&DE + 1);
}
char* Cpu::getE()
{
	return (char*)&DE;
}

char* Cpu::getH()
{
	return ((char*)&HL + 1);
}
char* Cpu::getL()
{
	return (char*)&HL;
}
char* Cpu::getAF()
{
	return (char*)&AF;
}
char* Cpu::getHL()
{
	return (char*)&HL;
}
char* Cpu::getDE()
{
	return (char*)&DE;
}
char* Cpu::get$DE()
{
	return (char*)&ram.mem+ DE;
}
char* Cpu::get$HL()
{
	return (char*)&ram.mem + HL;
}
char* Cpu::getN()
{
	return (char*)&ram.mem + (PC++);
}
char* Cpu::getNN()
{
	
	char msb = ram.read(PC++);
	char lsb = ram.read(PC++);
	uint16_t nn = msb << 8 | lsb;
	return (char*)&nn;
}
char* Cpu::get$N()
{
	uint8_t n = ram.read(PC++);
	return (char*)(ram.mem + n);
}
char* Cpu::get$NN()
{
	char lsb = ram.read(PC++);
	char msb = ram.read(PC++);
	uint16_t nn = msb << 8 | lsb;
	return (char*)&ram.mem + nn;
}

char* Cpu::getImmediate()
{
	char n=*getN();
	return &n;
}
char* Cpu::getSP()
{
	return(char*)&SP;
}
char* Cpu::getBC()
{
	return(char*)&BC;
}
char* Cpu::get$BC()
{
	return (char*)&ram.mem + BC;
}
void Cpu::Execute(uint16_t opcode)
{
	Opcode  op = opcodes[opcode];
	char n = 19;
	//opcodes[opcode]
	char* param1= (this->*op.param1)();
	(this->*op.operate)(param1, &n);
	//(this->*opcodes[opcode].operate)(op.arg1, &n);
	//op.operate(op.arg1, &n);
}


void Cpu::LD_nn_n(char* nn, char* n)
{
	*n = *nn;
}
void Cpu::LD_r1_r2(char* r1, char* r2)
{
	*r1 = *r2;
}
void Cpu::LD_A_n(char* A, char* n)
{
	*A = *n;
}
void Cpu::LD_n_A(char* n, char* A)
{
	*n = *A;
}
void Cpu::LD_A_$C(char* A, char* C)
{
	*A=ram.read(0xff00 + *C);
}
void Cpu::LD_$C_A(char* C, char* A)
{
	ram.write(0xff00 + *C, *A);
}

void Cpu::LDD_A_$HL(char* A, char* $HL)
{
	*A = *$HL;
		HL--;
}
void Cpu::LDD_$HL_A(char* $HL, char* A)
{
	uint16_t* hl = (uint16_t*)$HL;
	ram.write(*hl,*A);
	HL--;
}
void Cpu::LDI_A_$HL(char* A, char* $HL)
{
	*A = *$HL;
	HL++;
}
void Cpu::LDI_$HL_A(char* $HL, char* A)
{
	uint16_t* hl = (uint16_t*)$HL;
	ram.write(*hl, *A);
	HL++;
}
void Cpu::LDH_n_A(char* N, char* A){
	ram.write(0xff00 + *N, *A);
}
void Cpu::LDH_A_n(char* A,char* N) {
	*A=ram.read(0xff00 + *N);
}
void Cpu::LD_n_nn(char* n, char* nn)
{
	*(uint16_t*)n = *(uint16_t*)nn;

}
void Cpu::LD_SP_HL(char* SP, char* HL)
{
	*(uint16_t*) SP = *(uint16_t*)HL;
}
void Cpu::LDHL_SP_$n(char* SP, char* $N)
{
	//HL = *(uint16_t*)SP + (signed char)*N;
	HL = *(uint16_t*)SP + (signed char)*$N;
	resetFlag('Z');
	resetFlag('N');
	//resetFlag('Z');
	//resetFlag('Z');
}
void Cpu::LD_nn_SP(char* NN,char* SP)
{
	*(uint16_t*)SP = *(uint16_t*)NN;
}

void Cpu::PUSH_nn(char* nn, char* SP)
{
	uint16_t sp = *(uint16_t*)SP;
	sp--;
	uint16_t n = *(uint16_t*)nn;
	Word word = WordToBytes(n);	
	ram.write(sp--, word.msb);
	ram.write(sp--, word.lsb);
	*(uint16_t*)SP = sp;
}

void Cpu::POP_nn(char* SP, char* nn)
{
	uint16_t sp = *(uint16_t*)SP;
	uint8_t lsb=ram.read(sp++);
	uint8_t msb=ram.read(sp++);
	uint16_t word=BytesToWord(msb,lsb);
	*(uint16_t*)nn=word;
}

void Cpu::ADD_A_n(char* A, char* n)
{
	Byte byteR = getByte(*A);
	Byte byteN = getByte(*n);
	char oldA = *A;
	*A += *n;
	if (*A == 0)
		setFlag('Z');
	resetFlag('N');
	if (byteR.lsn + byteN.lsn > 0xf) 
		setFlag('H');
	if (oldA + *n > 0xff)
		setFlag('C');
}

void Cpu::ADC_A_n(char* A, char* n)
{
	
	Byte byteR = getByte(*A);
	Byte byteN = getByte(*n);
	char oldA = *A;
	*A += *n + getFlag('C');
	if (*A == 0)
		setFlag('Z');
	resetFlag('N');
	if (byteR.lsn + byteN.lsn + getFlag('C') > 0xf) 
		setFlag('H');
	if (oldA + *n + getFlag('C') > 0xff)
		setFlag('C');
}
void Cpu::SUB_n(char* A, char* n) {
	Byte byteR = getByte(*A);
	Byte byteN = getByte(*n);
	char oldA = *A;
	
	*A -= *n;
	if (*A == 0)
		setFlag('Z');
	setFlag('N');
	if (byteR.lsn < byteN.lsn)
		setFlag('H');
	if (oldA < *n)
		setFlag('C');

}
void Cpu::SBC_A_n(char* A, char* n) {
	Byte byteR = getByte(*A);
	Byte byteN = getByte(*n);
	char oldA = *A;

	*A -= (*n + getFlag('C'));
	if (*A == 0)
		setFlag('Z');
	setFlag('N');
	if (byteR.lsn < byteN.lsn + getFlag('C'))
		setFlag('H');
	if (oldA < *n + getFlag('C'))
		setFlag('C');

}
void Cpu::AND_n(char* A, char* n)
{
	Byte byteR = getByte(*A);
	Byte byteN = getByte(*n);
	*A &= *n;
	if (*A == 0)
		setFlag('Z');
	resetFlag('N');
	setFlag('H');
	resetFlag('C');
}
void Cpu::OR_n(char* A, char* n)
{
	Byte byteR = getByte(*A);
	Byte byteN = getByte(*n);
	*A |= *n;
	if (*A == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
	resetFlag('C');
}
void Cpu::XOR_n(char* A, char* n)
{
	Byte byteR = getByte(*A);
	Byte byteN = getByte(*n);
	*A ^= *n;
	if (*A == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
	resetFlag('C');
}
void Cpu::CP_n(char* n, char* none) {
	char A = *getA();
	Byte byteR = getByte(A);
	Byte byteN = getByte(*n);
	char res = A - *n;
	if (res == 0)
		setFlag('Z');
	setFlag('N');
	if (byteR.lsn < byteN.lsn)
		setFlag('H');
	if (A < *n)
		setFlag('C');
}
void Cpu::INC_n(char* n, char* none) {
	*n++;
}
void Cpu::DEC_n(char* n, char* none) {
	*n--;
}
void Cpu::ADD_HL_n(char* HL, char* n) {
	*HL += *n;
}
void Cpu::ADD_SP_n(char* SP, char* n) {
	*SP += *n;
}
void Cpu::INC_nn(char* nn, char* none) {
	(*(uint16_t*)nn)++;
}
void Cpu::DEC_nn(char* nn, char* none) {
	(*(uint16_t*)nn)--;
}
void Cpu::SWAP_n(char* n, char* none) {
	Byte oldByte = getByte(*n);
	Byte newByte{ oldByte.lsn,oldByte.msn };
	*n = getByte(newByte);
}
void Cpu::DAA(char* none, char* none2) {
	char correction = 0;
	Byte A = getByte(*getA());
	if ((!getFlag('N') && A.lsn > 9) || getFlag('H'))
		correction += 0x06;
	if((!getFlag('N') && A.msn > 9) || getFlag('C'))
		correction += 0x60;

	*(getA()) += getFlag('N')?-correction:correction;

}
void Cpu::CPL(char* none, char* none2) {
	*getA() ^= 0xff;
}
void Cpu::CCF(char* none, char* none2) {

	getFlag('C') ? resetFlag('C') : setFlag('C');
}
void Cpu::SCF(char* none, char* none2) {
	setFlag('C');
}
void Cpu::NOP(char* none, char* none2) {
}
void Cpu::HALT(char* none, char* none2) {
	haltCpu = 1;
}
void Cpu::STOP(char* none, char* none2) {
	
	haltCpu = 1;
	haltDisplay = 1;
}
void Cpu::DI(char* none, char* none2) {
	IME = 0;
}
void Cpu::EI(char* none, char* none2) {
	IMEScheduled = true;
}
void Cpu::RLCA(char* none, char* none2) {
	char A = *getA();
	char bitLeaving=(1 & (A >> 7));
	*getA()= (A<<1)|bitLeaving;
	bitLeaving ? setFlag('C') : resetFlag('C');

	/*for (char i = 0;i < 8;i++)
		newA |= (1 & (A >> i)) << (7 - i);
	*getA() = newA;*/
}
void Cpu::RLA(char* none, char* none2) {
	char A = *getA();
	char bitLeaving = (1 & (A >> 7));
	*getA() = (A << 1) | getFlag('C');
	bitLeaving ? setFlag('C') : resetFlag('C');
}
void Cpu::RRCA(char* none, char* none2) {
	char A = *getA();
	char bitLeaving = 1 & A;
	*getA() = (bitLeaving<<7) |(A >> 1);
	bitLeaving ? setFlag('C') : resetFlag('C');
}
void Cpu::RRA(char* none, char* none2) {
	char A = *getA();
	char bitLeaving = 1 & A;
	*getA() = (getFlag('C') << 7) | (A >> 1);
	bitLeaving ? setFlag('C') : resetFlag('C');
}
void Cpu::RLC_n(char* n, char* none) {
	char bitLeaving = (1 & (*n >> 7));
	*n = (*n << 1) | bitLeaving;
	bitLeaving ? setFlag('C') : resetFlag('C');
}
void Cpu::RL_n(char* n, char* none) {
	char bitLeaving = (1 & (*n >> 7));
	*n = (*n << 1) | getFlag('C');
	bitLeaving ? setFlag('C') : resetFlag('C');
}
void Cpu::RRC_n(char* n, char* none) {
	char bitLeaving = 1 & *n;
	*n = (bitLeaving << 7) | (*n >> 1);
	bitLeaving ? setFlag('C') : resetFlag('C');
}
void Cpu::RR_n(char* n, char* none) {
	char bitLeaving = 1 & *n;
	*n = getFlag('C') | (*n >> 1);
	bitLeaving ? setFlag('C') : resetFlag('C');
}
void Cpu::SLA_n(char* n, char* none) {
	char bitLeaving = (1 & (*n >> 7));
	*n <<= 1;
	bitLeaving ? setFlag('C') : resetFlag('C');
}
void Cpu::SRA_n(char* n, char* none) {
	char bitLeaving = (1 &*n);
	*n >>= 1;
	bitLeaving ? setFlag('C') : resetFlag('C');
	bitLeaving = (1 & (*n>>6));
	*n &= bitLeaving << 7;
}
void Cpu::SRL_n(char* n, char* none) {
	char bitLeaving = (1 & *n);
	*n >>= 1;
	bitLeaving ? setFlag('C') : resetFlag('C');
	*n &= 0x7f;
}
void Cpu::BIT_$n_r(char* $n, char* r) {
	char bit =1 & *r>>*$n;
	bit ? resetFlag('Z') : setFlag('Z');
}
void Cpu::SET_$n_r(char* $n, char* r) {
	//?
	*r |= 1 << *$n;
	
}
void Cpu::RES_$n_r(char* $n, char* r) {
	//?
	*r &= 0xff^(1 << *$n);
}
void Cpu::JP_nn(char* nn, char* none) {
	PC = *(uint16_t*)nn;
}
void Cpu::JP_cc_nn(char* cc, char* nn) {
	if((*cc == 'NZ'&&!getFlag('Z'))|| (*cc == 'Z' && getFlag('Z'))|| (*cc == 'NC' && !getFlag('C'))|| (*cc == 'C' && getFlag('C')))
		PC = *(uint16_t*)nn;
}
void Cpu::JP_HL(char* HL, char* none) {
	PC = *(uint16_t*)HL;
}
void Cpu::JR_n(char* n, char* none) {
	PC +=(signed)*n;
}
void Cpu::JR_cc_n(char* cc, char* n) {
	if ((*cc == 'NZ' && !getFlag('Z')) || (*cc == 'Z' && getFlag('Z')) || (*cc == 'NC' && !getFlag('C')) || (*cc == 'C' && getFlag('C')))
		PC +=(signed)*n;
}
void Cpu::CALL_nn(char* nn, char* none) {
	uint16_t next = PC + 1;
	PUSH_nn((char*)&next, (char*)&SP);
	PC = *(uint16_t*)nn;
}
void Cpu::CALL_cc_nn(char* cc, char* nn) {
	if ((*cc == 'NZ' && !getFlag('Z')) || (*cc == 'Z' && getFlag('Z')) || (*cc == 'NC' && !getFlag('C')) || (*cc == 'C' && getFlag('C'))) {
		uint16_t next = PC + 1;
		PUSH_nn((char*)&next, (char*)&SP);
		PC = *(uint16_t*)nn;
	}
}
void Cpu::RST_n(char* n, char* none) {
	PUSH_nn((char*)&PC, (char*)&SP);
	PC = *n;
}
void Cpu::RET(char* none, char* none2) {
	char lsb = ram.read(SP++);
	char msb = ram.read(SP++);
	PC=BytesToWord(msb, lsb);
	
}
void Cpu::RET_cc(char* cc, char* none) {
	if ((*cc == 'NZ' && !getFlag('Z')) || (*cc == 'Z' && getFlag('Z')) || (*cc == 'NC' && !getFlag('C')) || (*cc == 'C' && getFlag('C'))) {
		char lsb = ram.read(SP++);
		char msb = ram.read(SP++);
		PC = BytesToWord(msb, lsb);
	}
}
void Cpu::RETI(char* none, char* none2) {
	RET(none, none2);
	IME = 1;
}






char Cpu::getFlag(char flag)
{
	char byte = 16;
	switch (flag)
	{
	case 'H':
		byte = 32;
		break;
	case 'N':
		byte = 64;
		break;
	case 'Z':
		byte = 128;
		break;
	}
	if (*getF() & byte)
		return 1;
	return 0;
}

void Cpu::setFlag(char flag)
{
	char byte = 16;
	switch (flag)
	{
	case 'H':
		byte = 32;
		break;
	case 'N':
		byte = 64;
		break;
	case 'Z':
		byte = 128;
		break;
	}
	char* f = getF();
	*f = *f | byte;
	
}

void Cpu::resetFlag(char flag)
{
	char byte = 16;
	switch (flag)
	{
	case 'H':
		byte = 32;
		break;
	case 'N':
		byte = 64;
		break;
	case 'Z':
		byte = 128;
		break;
	}
	byte ^= byte;
	char* f = getF();
	*f = *f & byte;
}
char* Cpu::NZFlag() {

	return flagsNames;
}
char* Cpu::ZFlag() {
	
	return flagsNames + 1;
}
char* Cpu::NCFlag() {

	return flagsNames +2;
}
char* Cpu::CFlag() {

	return flagsNames + 3;
}
char* Cpu::getRST00() {

	return RSTOffsets;
}
char* Cpu::getRST08() {

	return RSTOffsets + 1;
}
char* Cpu::getRST10() {

	return RSTOffsets + 2;
}
char* Cpu::getRST18() {

	return RSTOffsets + 3;
}
char* Cpu::getRST20() {

	return RSTOffsets + 4;
}
char* Cpu::getRST28() {

	return RSTOffsets + 5;
}
char* Cpu::getRST30() {

	return RSTOffsets + 6;
}
char* Cpu::getRST38() {

	return RSTOffsets + 7;
}

void Cpu::setFlags(uint16_t r,uint16_t n)
{
	/*Word word = WordToBytes(r);
	if (word.lsb == 0xff) {
		setFlag('H');
		if (word.msb == 0xff)
			setFlag('C');
	}*/

}
uint16_t Cpu::BytesToWord(uint8_t msb, uint8_t lsb) {
	uint16_t n = (msb << 8) | lsb;
	return n;
}
Word Cpu::WordToBytes(uint16_t word) {
	uint8_t msb = 0xff && (word >> 8);
	uint8_t lsb = 0xff && word;
	return Word{msb,lsb};
}
Byte Cpu::getByte(char byte) {
	uint8_t msn = 0xf && (byte >> 4);
	uint8_t lsn = 0xf && byte;
	return Byte{ msn,lsn };
}
char Cpu::getByte(Byte byte) {
	return byte.msn << 4 | byte.msn;
}
void Cpu::SetWordIntoBytes(uint16_t* a, uint16_t* b) {
	Word word = WordToBytes(*a);
	
}
void Cpu::SetByte(uint16_t* word, uint8_t value) {
	uint8_t* b = (uint8_t*)word;
	*b = value;

}
