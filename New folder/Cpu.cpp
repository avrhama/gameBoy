#include "Cpu.h"
Cpu::Cpu()
{
	
	//LD_nn_n
	opcodes[0x06] = { "LD nn,n",&Cpu::LD_nn_n,&Cpu::getB,&Cpu::getN,8 };
	opcodes[0x0E] = { "LD nn,n",&Cpu::LD_nn_n,&Cpu::getC,&Cpu::getN,8 };
	opcodes[0x16] = { "LD nn,n",&Cpu::LD_nn_n,&Cpu::getD,&Cpu::getN,8 };
	opcodes[0x1E] = { "LD nn,n",&Cpu::LD_nn_n,&Cpu::getE,&Cpu::getN,8 };
	opcodes[0x26] = { "LD nn,n",&Cpu::LD_nn_n,&Cpu::getH,&Cpu::getN,8 };
	opcodes[0x2E] = { "LD nn,n",&Cpu::LD_nn_n,&Cpu::getL,&Cpu::getN,8 };
	//LD_r1_r2
	opcodes[0x7F] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x78] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x79] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x7A] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x7B] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x7C] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x7D] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x7E] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0x40] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getB,4 };
	opcodes[0x41] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getC,4 };
	opcodes[0x42] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getD,4 };
	opcodes[0x43] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getE,4 };
	opcodes[0x44] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getH,4 };
	opcodes[0x45] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getL,4 };
	opcodes[0x46] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::get$HL,8 };
	opcodes[0x48] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getB,4 };
	opcodes[0x49] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getC,4 };
	opcodes[0x4A] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getD,4 };
	opcodes[0x4B] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getE,4 };
	opcodes[0x4C] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getH,4 };
	opcodes[0x4D] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getL,4 };
	opcodes[0x4E] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::get$HL,8 };
	opcodes[0x50] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getB,4 };
	opcodes[0x51] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getC,4 };
	opcodes[0x52] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getD,4 };
	opcodes[0x53] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getE,4 };
	opcodes[0x54] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getH,4 };
	opcodes[0x55] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getL,4 };
	opcodes[0x56] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::get$HL,8 };
	opcodes[0x58] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getB,4 };
	opcodes[0x59] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getC,4 };
	opcodes[0x5A] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getD,4 };
	opcodes[0x5B] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getE,4 };
	opcodes[0x5C] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getH,4 };
	opcodes[0x5D] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getL,4 };
	opcodes[0x5E] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::get$HL,8 };
	opcodes[0x60] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getB,4 };
	opcodes[0x61] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getC,4 };
	opcodes[0x62] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getD,4 };
	opcodes[0x63] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getE,4 };
	opcodes[0x64] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getH,4 };
	opcodes[0x65] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getL,4 };
	opcodes[0x66] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::get$HL,8 };
	opcodes[0x68] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getB,4 };
	opcodes[0x69] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getC,4 };
	opcodes[0x6A] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getD,4 };
	opcodes[0x6B] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getE,4 };
	opcodes[0x6C] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getH,4 };
	opcodes[0x6D] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getL,4 };
	opcodes[0x6E] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::get$HL,8 };
	opcodes[0x70] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getB,8 };
	opcodes[0x71] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getC,8 };
	opcodes[0x72] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getD,8 };
	opcodes[0x73] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getE,8 };
	opcodes[0x74] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getH,8 };
	opcodes[0x75] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getL,8 };
	opcodes[0x36] = { "LD r1,r2",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getN,12 };
	//LD_A_n
	opcodes[0x0A] = { "LD A,n",&Cpu::LD_A_n,&Cpu::getA,&Cpu::get$BC,8 };
	opcodes[0x1A] = { "LD A,n",&Cpu::LD_A_n,&Cpu::getA,&Cpu::get$DE,8 };
	opcodes[0xFA] = { "LD A,n",&Cpu::LD_A_n,&Cpu::getA,&Cpu::get$NN,16 };
	opcodes[0x3E] = { "LD A,n",&Cpu::LD_A_n,&Cpu::getA,&Cpu::getImmediate,8 };
	//LD_n_A
	opcodes[0x47] = { "LD n,A",&Cpu::LD_n_A,&Cpu::getB,&Cpu::getA,4 };
	opcodes[0x4F] = { "LD n,A",&Cpu::LD_n_A,&Cpu::getC,&Cpu::getA,4 };
	opcodes[0x57] = { "LD n,A",&Cpu::LD_n_A,&Cpu::getD,&Cpu::getA,4 };
	opcodes[0x5F] = { "LD n,A",&Cpu::LD_n_A,&Cpu::getE,&Cpu::getA,4 };
	opcodes[0x67] = { "LD n,A",&Cpu::LD_n_A,&Cpu::getH,&Cpu::getA,4 };
	opcodes[0x6F] = { "LD n,A",&Cpu::LD_n_A,&Cpu::getL,&Cpu::getA,4 };
	opcodes[0x02] = { "LD n,A",&Cpu::LD_n_A,&Cpu::get$BC,&Cpu::getA,8 };
	opcodes[0x12] = { "LD n,A",&Cpu::LD_n_A,&Cpu::get$DE,&Cpu::getA,8 };
	opcodes[0x77] = { "LD n,A",&Cpu::LD_n_A,&Cpu::get$HL,&Cpu::getA,8 };
	opcodes[0xEA] = { "LD n,A",&Cpu::LD_n_A,&Cpu::get$NN,&Cpu::getA,16 };
	//LD_A_C
	opcodes[0xF2] = { "LD A,(C)",&Cpu::LD_A_$C,&Cpu::getA,&Cpu::getC,8 };
	//LD_C_A
	opcodes[0xF2] = { "LD (C),A",&Cpu::LD_$C_A,&Cpu::getC,&Cpu::getA,8 };
	//LD_A_HLD
	//LD_A_HLminus
	//LDD_A_HL
	opcodes[0x3A] = { "LDD A,(HL)",&Cpu::LDD_A_$HL,&Cpu::getA,&Cpu::get$HL,8 };
	//LD_HLD_A
	//LD_HLminus_A
	//LDD_HL_A
	opcodes[0x32] = { "LDD (HL),A",&Cpu::LDD_$HL_A,&Cpu::get$HL,&Cpu::getA,8 };
	//LD_A_HLI
	//LD_A_HLplus
	//LDI_A_HL
	opcodes[0x2A] = { "LDI A,(HL)",&Cpu::LDI_A_$HL,&Cpu::getA,&Cpu::get$HL,8 };
	//LD_HLI_A
	//LD_HLplus_A
	//LDI_HL_A
	opcodes[0x22] = { "LDI (HL),A",&Cpu::LDI_$HL_A,&Cpu::get$HL,&Cpu::getA,8 };
	//LDH_(n)_A
	opcodes[0xe0] = { "LDH (n),A",&Cpu::LDH_$N_A,&Cpu::getN,&Cpu::getA,12 };
	//LDH_A_n
	opcodes[0xf0] = { "LDH (n),A",&Cpu::LDH_A_$N,&Cpu::getA ,&Cpu::getN,12 };
	//LD_n_nn
	opcodes[0x01] = { "LD n,nn",&Cpu::LD_n_nn,&Cpu::getBC,&Cpu::get$NN,12 };
	opcodes[0x11] = { "LD n,nn",&Cpu::LD_n_nn,&Cpu::getDE,&Cpu::get$NN,12 };
	opcodes[0x21] = { "LD n,nn",&Cpu::LD_n_nn,&Cpu::getHL,&Cpu::get$NN,12 };
	opcodes[0x31] = { "LD n,nn",&Cpu::LD_n_nn,&Cpu::getSP,&Cpu::get$NN,12 };
	//LD_SP_HL
	opcodes[0xF9] = { "LD SP,HL",&Cpu::LD_SP_HL,&Cpu::getSP,&Cpu::getHL,8 };
	//LDHL SP_n
	opcodes[0xF8] = { "LDHL SP,n",&Cpu::LDHL_SP_n,&Cpu::getSP,&Cpu::getN,12 };
	//LD_nn_SP
	opcodes[0x08] = { "LD (nn),SP",&Cpu::LDHL_$NN_SP,&Cpu::get$NN,&Cpu::getSP,20 };
	//PUSH_nn_n
	opcodes[0xF5] = { "PUSH nn",&Cpu::PUSH_nn,&Cpu::getAF,&Cpu::getSP,16 };
	opcodes[0xC5] = { "PUSH nn",&Cpu::PUSH_nn,&Cpu::getBC,&Cpu::getSP,16 };
	opcodes[0xD5] = { "PUSH nn",&Cpu::PUSH_nn,&Cpu::getDE,&Cpu::getSP,16 };
	opcodes[0xE5] = { "PUSH nn",&Cpu::PUSH_nn,&Cpu::getHL,&Cpu::getSP,16 };
	//POP_nn_n
	opcodes[0xF1] = { "POP nn",&Cpu::POP_nn,&Cpu::getSP,&Cpu::getAF,12 };
	opcodes[0xC1] = { "POP nn",&Cpu::POP_nn,&Cpu::getSP,&Cpu::getBC,12 };
	opcodes[0xD1] = { "POP nn",&Cpu::POP_nn,&Cpu::getSP,&Cpu::getDE,12 };
	opcodes[0xE1] = { "POP nn",&Cpu::POP_nn,&Cpu::getSP,&Cpu::getHL,12 };
	//ADD_A_n
	opcodes[0x87] = { "ADD A,n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x80] = { "ADD A,n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x81] = { "ADD A,n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x82] = { "ADD A,n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x83] = { "ADD A,n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x84] = { "ADD A,n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x85] = { "ADD A,n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x86] = { "ADD A,n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xC6] = { "ADD A,n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getImmediate,8 };
	//ADC_A_n
	opcodes[0x8F] = { "ADC A,n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x88] = { "ADC A,n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x89] = { "ADC A,n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x8A] = { "ADC A,n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x8B] = { "ADC A,n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x8C] = { "ADC A,n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x8D] = { "ADC A,n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x8E] = { "ADC A,n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xCE] = { "ADC A,n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getImmediate,8 };
	//SUB_n_n
	opcodes[0x97] = { "SUB n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x90] = { "SUB n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x91] = { "SUB n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x92] = { "SUB n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x93] = { "SUB n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x94] = { "SUB n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x95] = { "SUB n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x96] = { "SUB n",&Cpu::SUB_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xD6] = { "SUB n",&Cpu::SUB_n,&Cpu::getA,&Cpu::getImmediate,8 };
	//SBC_A_n
	opcodes[0x9F] = { "SBC A,n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x98] = { "SBC A,n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x99] = { "SBC A,n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x9A] = { "SBC A,n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x9B] = { "SBC A,n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x9C] = { "SBC A,n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x9D] = { "SBC A,n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x9E] = { "SBC A,n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::get$HL,8 };
	//opcodes[??] = { "SBC A,n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getImmediate,?};
	//AND_n_n
	opcodes[0xA7] = { "AND n,n",&Cpu::ADD_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0xA0] = { "AND n,n",&Cpu::ADD_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0xA1] = { "AND n,n",&Cpu::ADD_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0xA2] = { "AND n,n",&Cpu::ADD_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0xA3] = { "AND n,n",&Cpu::ADD_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0xA4] = { "AND n,n",&Cpu::ADD_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0xA5] = { "AND n,n",&Cpu::ADD_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0xA6] = { "AND n,n",&Cpu::ADD_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xE6] = { "AND n,n",&Cpu::ADD_n,&Cpu::getA,&Cpu::getImmediate,8 };
	//OR n
	opcodes[0xB7] = { "OR n",&Cpu::OR_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0xB0] = { "OR n",&Cpu::OR_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0xB1] = { "OR n",&Cpu::OR_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0xB2] = { "OR n",&Cpu::OR_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0xB3] = { "OR n",&Cpu::OR_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0xB4] = { "OR n",&Cpu::OR_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0xB5] = { "OR n",&Cpu::OR_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0xB6] = { "OR n",&Cpu::OR_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xF6] = { "OR n",&Cpu::OR_n,&Cpu::getA,&Cpu::getImmediate,8 };
	//XOR_n_n
	opcodes[0xAF] = { "XOR n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0xA8] = { "XOR n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0xA9] = { "XOR n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0xAA] = { "XOR n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0xAB] = { "XOR n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0xAC] = { "XOR n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0xAD] = { "XOR n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0xAE] = { "XOR n",&Cpu::XOR_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xEE] = { "XOR n",&Cpu::XOR_n,&Cpu::getA,&Cpu::getImmediate,8 };
	//CP_n_n
	opcodes[0xBF] = { "CP n",&Cpu::CP_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0xB8] = { "CP n",&Cpu::CP_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0xB9] = { "CP n",&Cpu::CP_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0xBA] = { "CP n",&Cpu::CP_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0xBB] = { "CP n",&Cpu::CP_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0xBC] = { "CP n",&Cpu::CP_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0xBD] = { "CP n",&Cpu::CP_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0xBE] = { "CP n",&Cpu::CP_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xFE] = { "CP n",&Cpu::CP_n,&Cpu::getA,&Cpu::getImmediate,8 };
	//INC_n
	opcodes[0x3C] = { "INC n",&Cpu::INC_n,&Cpu::getA,NULL,4 };
	opcodes[0x04] = { "INC n",&Cpu::INC_n,&Cpu::getB,NULL,4 };
	opcodes[0x0C] = { "INC n",&Cpu::INC_n,&Cpu::getC,NULL,4 };
	opcodes[0x14] = { "INC n",&Cpu::INC_n,&Cpu::getD,NULL,4 };
	opcodes[0x1C] = { "INC n",&Cpu::INC_n,&Cpu::getE,NULL,4 };
	opcodes[0x24] = { "INC n",&Cpu::INC_n,&Cpu::getH,NULL,4 };
	opcodes[0x2C] = { "INC n",&Cpu::INC_n,&Cpu::getL,NULL,4 };
	opcodes[0x34] = { "INC n",&Cpu::INC_n,&Cpu::get$HL,NULL,12 };
	//DEC_n_n
	opcodes[0x3D] = { "DEC n",&Cpu::DEC_n,&Cpu::getA,NULL,4 };
	opcodes[0x05] = { "DEC n",&Cpu::DEC_n,&Cpu::getB,NULL,4 };
	opcodes[0x0D] = { "DEC n",&Cpu::DEC_n,&Cpu::getC,NULL,4 };
	opcodes[0x15] = { "DEC n",&Cpu::DEC_n,&Cpu::getD,NULL,4 };
	opcodes[0x1D] = { "DEC n",&Cpu::DEC_n,&Cpu::getE,NULL,4 };
	opcodes[0x25] = { "DEC n",&Cpu::DEC_n,&Cpu::getH,NULL,4 };
	opcodes[0x2D] = { "DEC n",&Cpu::DEC_n,&Cpu::getL,NULL,4 };
	opcodes[0x35] = { "DEC n",&Cpu::DEC_n,&Cpu::get$HL,NULL,12 };
	//ADD_HL_n
	opcodes[0x09] = { "ADD HL,n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getBC,8 };
	opcodes[0x19] = { "ADD HL,n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getDE,8 };
	opcodes[0x29] = { "ADD HL,n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getHL,8 };
	opcodes[0x39] = { "ADD HL,n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getSP,8 };
	//ADD SP,n
	opcodes[0xE8] = { "ADD SP,n",&Cpu::ADD_SP_n,&Cpu::getSP,&Cpu::getImmediate,16 };
	//INC_nn_n
	opcodes[0x03] = { "INC nn",&Cpu::INC_nn,&Cpu::getBC,NULL,8 };
	opcodes[0x13] = { "INC nn",&Cpu::INC_nn,&Cpu::getDE,NULL,8 };
	opcodes[0x23] = { "INC nn",&Cpu::INC_nn,&Cpu::getHL,NULL,8 };
	opcodes[0x33] = { "INC nn",&Cpu::INC_nn,&Cpu::getSP,NULL,8 };
	//DEC_nn_n
	opcodes[0x0B] = { "DEC nn",&Cpu::DEC_nn,&Cpu::getBC,NULL,8 };
	opcodes[0x1B] = { "DEC nn",&Cpu::DEC_nn,&Cpu::getDE,NULL,8 };
	opcodes[0x2B] = { "DEC nn",&Cpu::DEC_nn,&Cpu::getHL,NULL,8 };
	opcodes[0x3B] = { "DEC nn",&Cpu::DEC_nn,&Cpu::getSP,NULL,8 };
	//SWAP_n
	opcodes[0xCB37] = { "SWAP n",&Cpu::SWAP_n,&Cpu::getA,NULL,37 };
	//DAA_n_n
	opcodes[0x27] = { "DAA_n_n",&Cpu::DAA_n_n,NULL,NULL,4 };
	//CPL_n_n
	opcodes[0x2F] = { "CPL_n_n",&Cpu::CPL_n_n,NULL,NULL,4 };
	//CCF_n_n
	opcodes[0x3F] = { "CCF_n_n",&Cpu::CCF_n_n,NULL,NULL,4 };
	//SCF_n_n
	opcodes[0x37] = { "SCF_n_n",&Cpu::SCF_n_n,NULL,NULL,4 };
	//NOP_n_n
	opcodes[0x00] = { "NOP_n_n",&Cpu::NOP_n_n,NULL,NULL,4 };
	//HALT_n_n
	opcodes[0x76] = { "HALT_n_n",&Cpu::HALT_n_n,NULL,NULL,4 };
	//STOP_n_n
	opcodes[0x1000] = { "STOP_n_n",&Cpu::STOP_n_n,NULL,NULL,4 };
	//DI_n_n
	opcodes[0xF3] = { "DI_n_n",&Cpu::DI_n_n,NULL,NULL,4 };
	//EI_n_n
	opcodes[0xFB] = { "EI_n_n",&Cpu::EI_n_n,NULL,NULL,4 };
	//RLCA_n_n
	opcodes[0x07] = { "RLCA_n_n",&Cpu::RLCA_n_n,NULL,NULL,4 };
	//RLA_n_n
	opcodes[0x17] = { "RLA_n_n",&Cpu::RLA_n_n,NULL,NULL,4 };
	//RRCA_n_n
	opcodes[0x0F] = { "RRCA_n_n",&Cpu::RRCA_n_n,NULL,NULL,4 };
	//RRA_n_n
	opcodes[0x1F] = { "RRA_n_n",&Cpu::RRA_n_n,NULL,NULL,4 };
	//RLC_n_n
	opcodes[0xCB07] = { "RLC_n_n",&Cpu::RLC_n_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB00] = { "RLC_n_n",&Cpu::RLC_n_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB01] = { "RLC_n_n",&Cpu::RLC_n_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB02] = { "RLC_n_n",&Cpu::RLC_n_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB03] = { "RLC_n_n",&Cpu::RLC_n_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB04] = { "RLC_n_n",&Cpu::RLC_n_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB05] = { "RLC_n_n",&Cpu::RLC_n_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB06] = { "RLC_n_n",&Cpu::RLC_n_n,&Cpu::get$HL,NULL,16 };
	//RL_n_n
	opcodes[0xCB17] = { "RL_n_n",&Cpu::RL_n_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB10] = { "RL_n_n",&Cpu::RL_n_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB11] = { "RL_n_n",&Cpu::RL_n_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB12] = { "RL_n_n",&Cpu::RL_n_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB13] = { "RL_n_n",&Cpu::RL_n_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB14] = { "RL_n_n",&Cpu::RL_n_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB15] = { "RL_n_n",&Cpu::RL_n_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB16] = { "RL_n_n",&Cpu::RL_n_n,&Cpu::get$HL,NULL,16 };
	//RRC_n_n
	opcodes[0xCB0F] = { "RRC_n_n",&Cpu::RRC_n_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB08] = { "RRC_n_n",&Cpu::RRC_n_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB09] = { "RRC_n_n",&Cpu::RRC_n_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB0A] = { "RRC_n_n",&Cpu::RRC_n_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB0B] = { "RRC_n_n",&Cpu::RRC_n_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB0C] = { "RRC_n_n",&Cpu::RRC_n_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB0D] = { "RRC_n_n",&Cpu::RRC_n_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB0E] = { "RRC_n_n",&Cpu::RRC_n_n,&Cpu::get$HL,NULL,16 };
	//RR_n_n
	opcodes[0xCB1F] = { "RR_n_n",&Cpu::RR_n_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB18] = { "RR_n_n",&Cpu::RR_n_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB19] = { "RR_n_n",&Cpu::RR_n_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB1A] = { "RR_n_n",&Cpu::RR_n_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB1B] = { "RR_n_n",&Cpu::RR_n_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB1C] = { "RR_n_n",&Cpu::RR_n_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB1D] = { "RR_n_n",&Cpu::RR_n_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB1E] = { "RR_n_n",&Cpu::RR_n_n,&Cpu::get$HL,NULL,16 };
	//SLA_n_n
	opcodes[0xCB27] = { "SLA_n_n",&Cpu::SLA_n_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB20] = { "SLA_n_n",&Cpu::SLA_n_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB21] = { "SLA_n_n",&Cpu::SLA_n_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB22] = { "SLA_n_n",&Cpu::SLA_n_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB23] = { "SLA_n_n",&Cpu::SLA_n_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB24] = { "SLA_n_n",&Cpu::SLA_n_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB25] = { "SLA_n_n",&Cpu::SLA_n_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB26] = { "SLA_n_n",&Cpu::SLA_n_n,&Cpu::get$HL,NULL,16 };
	//SRA_n_n
	opcodes[0xCB2F] = { "SRA_n_n",&Cpu::SRA_n_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB28] = { "SRA_n_n",&Cpu::SRA_n_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB29] = { "SRA_n_n",&Cpu::SRA_n_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB2A] = { "SRA_n_n",&Cpu::SRA_n_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB2B] = { "SRA_n_n",&Cpu::SRA_n_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB2C] = { "SRA_n_n",&Cpu::SRA_n_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB2D] = { "SRA_n_n",&Cpu::SRA_n_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB2E] = { "SRA_n_n",&Cpu::SRA_n_n,&Cpu::get$HL,NULL,16 };
	//SRL_n_n
	opcodes[0xCB3F] = { "SRL_n_n",&Cpu::SRL_n_n,&Cpu::getA,NULL,8 };
	opcodes[0xCB38] = { "SRL_n_n",&Cpu::SRL_n_n,&Cpu::getB,NULL,8 };
	opcodes[0xCB39] = { "SRL_n_n",&Cpu::SRL_n_n,&Cpu::getC,NULL,8 };
	opcodes[0xCB3A] = { "SRL_n_n",&Cpu::SRL_n_n,&Cpu::getD,NULL,8 };
	opcodes[0xCB3B] = { "SRL_n_n",&Cpu::SRL_n_n,&Cpu::getE,NULL,8 };
	opcodes[0xCB3C] = { "SRL_n_n",&Cpu::SRL_n_n,&Cpu::getH,NULL,8 };
	opcodes[0xCB3D] = { "SRL_n_n",&Cpu::SRL_n_n,&Cpu::getL,NULL,8 };
	opcodes[0xCB3E] = { "SRL_n_n",&Cpu::SRL_n_n,&Cpu::get$HL,NULL,16 };
	//BIT_B_r
	opcodes[0xCB47] = { "BIT_B_r",&Cpu::BIT_B_r,&Cpu::getB,&Cpu::getA,8 };
	opcodes[0xCB40] = { "BIT_B_r",&Cpu::BIT_B_r,&Cpu::getB,&Cpu::getB,8 };
	opcodes[0xCB41] = { "BIT_B_r",&Cpu::BIT_B_r,&Cpu::getB,&Cpu::getC,8 };
	opcodes[0xCB42] = { "BIT_B_r",&Cpu::BIT_B_r,&Cpu::getB,&Cpu::getD,8 };
	opcodes[0xCB43] = { "BIT_B_r",&Cpu::BIT_B_r,&Cpu::getB,&Cpu::getE,8 };
	opcodes[0xCB44] = { "BIT_B_r",&Cpu::BIT_B_r,&Cpu::getB,&Cpu::getH,8 };
	opcodes[0xCB45] = { "BIT_B_r",&Cpu::BIT_B_r,&Cpu::getB,&Cpu::getL,8 };
	opcodes[0xCB46] = { "BIT_B_r",&Cpu::BIT_B_r,&Cpu::getB,&Cpu::get$HL,16 };
	//SET_B_r
	opcodes[0xCBC7] = { "SET_B_r",&Cpu::SET_B_r,&Cpu::getB,&Cpu::getA,8 };
	opcodes[0xCBC0] = { "SET_B_r",&Cpu::SET_B_r,&Cpu::getB,&Cpu::getB,8 };
	opcodes[0xCBC1] = { "SET_B_r",&Cpu::SET_B_r,&Cpu::getB,&Cpu::getC,8 };
	opcodes[0xCBC2] = { "SET_B_r",&Cpu::SET_B_r,&Cpu::getB,&Cpu::getD,8 };
	opcodes[0xCBC3] = { "SET_B_r",&Cpu::SET_B_r,&Cpu::getB,&Cpu::getE,8 };
	opcodes[0xCBC4] = { "SET_B_r",&Cpu::SET_B_r,&Cpu::getB,&Cpu::getH,8 };
	opcodes[0xCBC5] = { "SET_B_r",&Cpu::SET_B_r,&Cpu::getB,&Cpu::getL,8 };
	opcodes[0xCBC6] = { "SET_B_r",&Cpu::SET_B_r,&Cpu::getB,&Cpu::get$HL,16 };
	//RES_B_r
	opcodes[0xCB87] = { "RES_B_r",&Cpu::RES_B_r,&Cpu::getB,&Cpu::getA,8 };
	opcodes[0xCB80] = { "RES_B_r",&Cpu::RES_B_r,&Cpu::getB,&Cpu::getB,8 };
	opcodes[0xCB81] = { "RES_B_r",&Cpu::RES_B_r,&Cpu::getB,&Cpu::getC,8 };
	opcodes[0xCB82] = { "RES_B_r",&Cpu::RES_B_r,&Cpu::getB,&Cpu::getD,8 };
	opcodes[0xCB83] = { "RES_B_r",&Cpu::RES_B_r,&Cpu::getB,&Cpu::getE,8 };
	opcodes[0xCB84] = { "RES_B_r",&Cpu::RES_B_r,&Cpu::getB,&Cpu::getH,8 };
	opcodes[0xCB85] = { "RES_B_r",&Cpu::RES_B_r,&Cpu::getB,&Cpu::getL,8 };
	opcodes[0xCB86] = { "RES_B_r",&Cpu::RES_B_r,&Cpu::getB,&Cpu::get$HL,16 };
	//JP_nn_n
	opcodes[0xC3] = { "JP_nn_n",&Cpu::JP_nn_n,NULL,NULL,12 };
	//JP_nz_nn
	opcodes[0xC2] = { "JP_nz_nn",&Cpu::JP_nz_nn,&Cpu::getF,NULL,12 };
	//JP_z_nn
	opcodes[0xCA] = { "JP_z_nn",&Cpu::JP_z_nn,&Cpu::getF,NULL,12 };
	//JP_nc_nn
	opcodes[0xD2] = { "JP_nc_nn",&Cpu::JP_nc_nn,&Cpu::getF,NULL,12 };
	//JP_c_nn
	opcodes[0xDA] = { "JP_c_nn",&Cpu::JP_c_nn,&Cpu::getC,NULL,12 };
	//JP_HL_n
	opcodes[0xE9] = { "JP_HL_n",&Cpu::JP_HL_n,&Cpu::get$HL,NULL,4 };
	//JR_n_n
	opcodes[0x18] = { "JR_n_n",&Cpu::JR_n_n,NULL,NULL,8 };
	//JR_cc_n
	opcodes[0x20] = { "JR_cc_n",&Cpu::JR_cc_n,&Cpu::getF,NULL,8 };
	opcodes[0x28] = { "JR_cc_n",&Cpu::JR_cc_n,&Cpu::getF,NULL,8 };
	opcodes[0x30] = { "JR_cc_n",&Cpu::JR_cc_n,&Cpu::getF,NULL,8 };
	opcodes[0x38] = { "JR_cc_n",&Cpu::JR_cc_n,&Cpu::getC,NULL,8 };
	//CALL_nn_n
	opcodes[0xCD] = { "CALL_nn_n",&Cpu::CALL_nn_n,NULL,NULL,12 };
	//CALL_cc_nn
	opcodes[0xC4] = { "CALL_cc_nn",&Cpu::CALL_cc_nn,&Cpu::getF,NULL,12 };
	opcodes[0xCC] = { "CALL_cc_nn",&Cpu::CALL_cc_nn,&Cpu::getF,NULL,12 };
	opcodes[0xD4] = { "CALL_cc_nn",&Cpu::CALL_cc_nn,&Cpu::getF,NULL,12 };
	opcodes[0xDC] = { "CALL_cc_nn",&Cpu::CALL_cc_nn,&Cpu::getC,NULL,12 };
	//RST_n_n
	//RET_n_n
	opcodes[0xC9] = { "RET_n_n",&Cpu::RET_n_n,NULL,NULL,8 };
	//RET_cc_n
	/*opcodes[0xC0] = { "RET_cc_n",&Cpu::RET_cc_n,&Cpu::getF,NULL,8 };
	opcodes[0xC8] = { "RET_cc_n",&Cpu::RET_cc_n,&Cpu::getF,NULL,8 };
	opcodes[0xD0] = { "RET_cc_n",&Cpu::RET_cc_n,&Cpu::getF,NULL,8 };
	opcodes[0xD8] = { "RET_cc_n",&Cpu::RET_cc_n,&Cpu::getC,NULL,8 };
	//RETI_n_n
	opcodes[0xD9] = { "RETI_n_n",&Cpu::RETI_n_n,NULL,NULL,8 };8*/



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
	char lsb = ram.read(PC++);
	char msb = ram.read(PC++);
	uint16_t nn = msb << 8 | lsb;
	return (char*)&nn;
}
char* Cpu::get$N()
{
	return (char*)&(ram.mem + (ram.read(PC++)));
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
void Cpu::LDH_$N_A(char* $N, char* A){
	ram.write(0xff00 + *$N, *A);
}
void Cpu::LDH_A_$N(char* A,char* $N) {
	*A=ram.read(0xff00 + *$N);
}
void Cpu::LD_n_nn(char* n, char* nn)
{
	*(uint16_t*)n = *(uint16_t*)nn;

}
void Cpu::LD_SP_HL(char* SP, char* HL)
{
	*(uint16_t*) SP = *(uint16_t*)HL;
}
void Cpu::LDHL_SP_n(char* SP, char* $N)
{
	HL = *(uint16_t*)SP + (unsigned char)*$N;
}
void Cpu::LDHL_$NN_SP(char* $NN,char* SP)
{
	*(uint16_t*)SP = *(uint16_t*)$NN;
}

void Cpu::PUSH_nn(char* nn, char* SP)
{
	uint16_t sp = *(uint16_t*)SP;
	sp--;
	uint16_t n = *(uint16_t*)nn;
	Word word = WordToBytes(n);	
	ram.write(sp--, word.msb);
	ram.write(sp--, word.lsb);
	*SP = sp;
}

void Cpu::POP_nn(char* SP, char* nn)
{
	uint16_t sp = *(uint16_t*)SP;
	uint8_t lsb=ram.read(sp++);
	uint8_t msb=ram.read(sp++);
	uint16_t word=BytesToWord(msb,lsb);
	*nn=word;
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
void Cpu::ADD_n(char* A, char* n)
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
void Cpu::CP_n(char* A, char* n)
{
}
void Cpu::INC_n(char* n, char* null)
{
}
void Cpu::DEC_n(char* n, char* null)
{
}
void Cpu::ADD_HL_n(char* HL, char* n)
{
}
void Cpu::ADD_SP_n(char* SP, char* n)
{
}
void Cpu::INC_nn(char* nn, char* null)
{
}
void Cpu::DEC_nn(char* nn, char* null)
{
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
void Cpu::SetWordIntoBytes(uint16_t* a, uint16_t* b) {
	Word word = WordToBytes(*a);
	
}
void Cpu::SetByte(uint16_t* word, uint8_t value) {
	uint8_t* b = (uint8_t*)word;
	*b = value;

}
