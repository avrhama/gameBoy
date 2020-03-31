#include "CPU.h"
#include "MMU.h"
CPU::CPU()
{
	
	//LD nn,n
	opcodes[0x06] = { "LD_nn_N",&CPU::LD_nn_N,&CPU::getB,&CPU::getN,8 };
	opcodes[0x0E] = { "LD_nn_N",&CPU::LD_nn_N,&CPU::getC,&CPU::getN,8 };
	opcodes[0x16] = { "LD_nn_N",&CPU::LD_nn_N,&CPU::getD,&CPU::getN,8 };
	opcodes[0x1E] = { "LD_nn_N",&CPU::LD_nn_N,&CPU::getE,&CPU::getN,8 };
	opcodes[0x26] = { "LD_nn_N",&CPU::LD_nn_N,&CPU::getH,&CPU::getN,8 };
	opcodes[0x2E] = { "LD_nn_N",&CPU::LD_nn_N,&CPU::getL,&CPU::getN,8 };
	//LD r1,r2
	opcodes[0x7F] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getA,&CPU::getA,4 };
	opcodes[0x78] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getA,&CPU::getB,4 };
	opcodes[0x79] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getA,&CPU::getC,4 };
	opcodes[0x7A] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getA,&CPU::getD,4 };
	opcodes[0x7B] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getA,&CPU::getE,4 };
	opcodes[0x7C] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getA,&CPU::getH,4 };
	opcodes[0x7D] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getA,&CPU::getL,4 };
	opcodes[0x7E] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getA,&CPU::get$HL,8 };
	opcodes[0x40] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getB,&CPU::getB,4 };
	opcodes[0x41] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getB,&CPU::getC,4 };
	opcodes[0x42] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getB,&CPU::getD,4 };
	opcodes[0x43] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getB,&CPU::getE,4 };
	opcodes[0x44] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getB,&CPU::getH,4 };
	opcodes[0x45] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getB,&CPU::getL,4 };
	opcodes[0x46] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getB,&CPU::get$HL,8 };
	opcodes[0x48] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getC,&CPU::getB,4 };
	opcodes[0x49] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getC,&CPU::getC,4 };
	opcodes[0x4A] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getC,&CPU::getD,4 };
	opcodes[0x4B] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getC,&CPU::getE,4 };
	opcodes[0x4C] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getC,&CPU::getH,4 };
	opcodes[0x4D] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getC,&CPU::getL,4 };
	opcodes[0x4E] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getC,&CPU::get$HL,8 };
	opcodes[0x50] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getD,&CPU::getB,4 };
	opcodes[0x51] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getD,&CPU::getC,4 };
	opcodes[0x52] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getD,&CPU::getD,4 };
	opcodes[0x53] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getD,&CPU::getE,4 };
	opcodes[0x54] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getD,&CPU::getH,4 };
	opcodes[0x55] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getD,&CPU::getL,4 };
	opcodes[0x56] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getD,&CPU::get$HL,8 };
	opcodes[0x58] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getE,&CPU::getB,4 };
	opcodes[0x59] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getE,&CPU::getC,4 };
	opcodes[0x5A] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getE,&CPU::getD,4 };
	opcodes[0x5B] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getE,&CPU::getE,4 };
	opcodes[0x5C] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getE,&CPU::getH,4 };
	opcodes[0x5D] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getE,&CPU::getL,4 };
	opcodes[0x5E] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getE,&CPU::get$HL,8 };
	opcodes[0x60] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getH,&CPU::getB,4 };
	opcodes[0x61] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getH,&CPU::getC,4 };
	opcodes[0x62] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getH,&CPU::getD,4 };
	opcodes[0x63] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getH,&CPU::getE,4 };
	opcodes[0x64] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getH,&CPU::getH,4 };
	opcodes[0x65] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getH,&CPU::getL,4 };
	opcodes[0x66] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getH,&CPU::get$HL,8 };
	opcodes[0x68] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getL,&CPU::getB,4 };
	opcodes[0x69] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getL,&CPU::getC,4 };
	opcodes[0x6A] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getL,&CPU::getD,4 };
	opcodes[0x6B] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getL,&CPU::getE,4 };
	opcodes[0x6C] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getL,&CPU::getH,4 };
	opcodes[0x6D] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getL,&CPU::getL,4 };
	opcodes[0x6E] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::getL,&CPU::get$HL,8 };
	opcodes[0x70] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::get$HL,&CPU::getB,8 };
	opcodes[0x71] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::get$HL,&CPU::getC,8 };
	opcodes[0x72] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::get$HL,&CPU::getD,8 };
	opcodes[0x73] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::get$HL,&CPU::getE,8 };
	opcodes[0x74] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::get$HL,&CPU::getH,8 };
	opcodes[0x75] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::get$HL,&CPU::getL,8 };
	opcodes[0x36] = { "LD_r1_r2",&CPU::LD_r1_r2,&CPU::get$HL,&CPU::getN,12 };
	//LD A,n
	opcodes[0x0A] = { "LD_A_n",&CPU::LD_A_n,&CPU::getA,&CPU::get$BC,8 };
	opcodes[0x1A] = { "LD_A_n",&CPU::LD_A_n,&CPU::getA,&CPU::get$DE,8 };
	opcodes[0xFA] = { "LD_A_n",&CPU::LD_A_n,&CPU::getA,&CPU::get$NN,16 };
	opcodes[0x3E] = { "LD_A_n",&CPU::LD_A_n,&CPU::getA,&CPU::get$N,8 };
	//LD n,A
	opcodes[0x47] = { "LD_n_A",&CPU::LD_n_A,&CPU::getB,&CPU::getA,4 };
	opcodes[0x4F] = { "LD_n_A",&CPU::LD_n_A,&CPU::getC,&CPU::getA,4 };
	opcodes[0x57] = { "LD_n_A",&CPU::LD_n_A,&CPU::getD,&CPU::getA,4 };
	opcodes[0x5F] = { "LD_n_A",&CPU::LD_n_A,&CPU::getE,&CPU::getA,4 };
	opcodes[0x67] = { "LD_n_A",&CPU::LD_n_A,&CPU::getH,&CPU::getA,4 };
	opcodes[0x6F] = { "LD_n_A",&CPU::LD_n_A,&CPU::getL,&CPU::getA,4 };
	opcodes[0x02] = { "LD_n_A",&CPU::LD_n_A,&CPU::get$BC,&CPU::getA,8 };
	opcodes[0x12] = { "LD_n_A",&CPU::LD_n_A,&CPU::get$DE,&CPU::getA,8 };
	opcodes[0x77] = { "LD_n_A",&CPU::LD_n_A,&CPU::get$HL,&CPU::getA,8 };
	opcodes[0xEA] = { "LD_n_A",&CPU::LD_n_A,&CPU::get$NN,&CPU::getA,16 };
	//LD A,(C)
	opcodes[0xF2] = { "LD_A_C",&CPU::LD_A_C,&CPU::getA,&CPU::getC,8 };
	//LD (C),A
	opcodes[0xE2] = { "LD_C_A",&CPU::LD_C_A,&CPU::getC,&CPU::getA,8 };
	//LD A,(HLD)
	//LD A,(HLminus)
	//LDD A,(HL)
	opcodes[0x3A] = { "LDD_A_$HL",&CPU::LDD_A_$HL,&CPU::getA,&CPU::get$HL,8 };
	//LD (HLD),A
	//LD (HLminus),A
	//LDD (HL),A
	opcodes[0x32] = { "LDD_$HL_A",&CPU::LDD_$HL_A,&CPU::get$HL,&CPU::getA,8 };
	//LD A,(HLI)
	//LD A,(HLplus)
	//LDI A,(HL)
	opcodes[0x2A] = { "LDI_A_$HL",&CPU::LDI_A_$HL,&CPU::getA,&CPU::get$HL,8 };
	//LD (HLI),A
	//LD (HLplus),A
	//LDI (HL),A
	opcodes[0x22] = { "LDI_$HL_A",&CPU::LDI_$HL_A,&CPU::get$HL,&CPU::getA,8 };
	//LDH n,A
	opcodes[0xE0] = { "LDH_n_A",&CPU::LDH_n_A,&CPU::getN,&CPU::getA,12 };
	
	//LDH A,n
	opcodes[0xF0] = { "LDH_A_n",&CPU::LDH_A_n,&CPU::getA,&CPU::getN,12 };
	//LD n,nn
	opcodes[0x01] = { "LD_n_nn",&CPU::LD_n_nn,&CPU::getBC,&CPU::getNN,12 };
	opcodes[0x11] = { "LD_n_nn",&CPU::LD_n_nn,&CPU::getDE,&CPU::getNN,12 };
	opcodes[0x21] = { "LD_n_nn",&CPU::LD_n_nn,&CPU::getHL,&CPU::getNN,12 };
	opcodes[0x31] = { "LD_n_nn",&CPU::LD_n_nn,&CPU::getSP,&CPU::getNN,12 };
	//LD SP,HL
	opcodes[0xF9] = { "LD_SP_HL",&CPU::LD_SP_HL,&CPU::getSP,&CPU::getHL,8 };
	//LD_HL_SP
	//LDHL SP,(n)
	opcodes[0xF8] = { "LDHL_SP_$n",&CPU::LDHL_SP_$n,&CPU::getSP,&CPU::get$N,12 };
	//LD nn,SP
	opcodes[0x08] = { "LD_$NN_SP",&CPU::LD_$NN_SP,&CPU::get$NN,&CPU::getSP,20 };
	//PUSH nn
	opcodes[0xF5] = { "PUSH_nn",&CPU::PUSH_nn,&CPU::getAF,&CPU::getSP,16 };
	opcodes[0xC5] = { "PUSH_nn",&CPU::PUSH_nn,&CPU::getBC,&CPU::getSP,16 };
	opcodes[0xD5] = { "PUSH_nn",&CPU::PUSH_nn,&CPU::getDE,&CPU::getSP,16 };
	opcodes[0xE5] = { "PUSH_nn",&CPU::PUSH_nn,&CPU::getHL,&CPU::getSP,16 };
	//POP nn
	opcodes[0xF1] = { "POP_nn",&CPU::POP_nn,&CPU::getSP,&CPU::getAF,12 };
	opcodes[0xC1] = { "POP_nn",&CPU::POP_nn,&CPU::getSP,&CPU::getBC,12 };
	opcodes[0xD1] = { "POP_nn",&CPU::POP_nn,&CPU::getSP,&CPU::getDE,12 };
	opcodes[0xE1] = { "POP_nn",&CPU::POP_nn,&CPU::getSP,&CPU::getHL,12 };
	//ADD A,n
	opcodes[0x87] = { "ADD_A_n",&CPU::ADD_A_n,&CPU::getA,&CPU::getA,4 };
	opcodes[0x80] = { "ADD_A_n",&CPU::ADD_A_n,&CPU::getA,&CPU::getB,4 };
	opcodes[0x81] = { "ADD_A_n",&CPU::ADD_A_n,&CPU::getA,&CPU::getC,4 };
	opcodes[0x82] = { "ADD_A_n",&CPU::ADD_A_n,&CPU::getA,&CPU::getD,4 };
	opcodes[0x83] = { "ADD_A_n",&CPU::ADD_A_n,&CPU::getA,&CPU::getE,4 };
	opcodes[0x84] = { "ADD_A_n",&CPU::ADD_A_n,&CPU::getA,&CPU::getH,4 };
	opcodes[0x85] = { "ADD_A_n",&CPU::ADD_A_n,&CPU::getA,&CPU::getL,4 };
	opcodes[0x86] = { "ADD_A_n",&CPU::ADD_A_n,&CPU::getA,&CPU::get$HL,8 };
	opcodes[0xC6] = { "ADD_A_n",&CPU::ADD_A_n,&CPU::getA,&CPU::get$N,8 };
	//ADC A,n
	opcodes[0x8F] = { "ADC_A_n",&CPU::ADC_A_n,&CPU::getA,&CPU::getA,4 };
	opcodes[0x88] = { "ADC_A_n",&CPU::ADC_A_n,&CPU::getA,&CPU::getB,4 };
	opcodes[0x89] = { "ADC_A_n",&CPU::ADC_A_n,&CPU::getA,&CPU::getC,4 };
	opcodes[0x8A] = { "ADC_A_n",&CPU::ADC_A_n,&CPU::getA,&CPU::getD,4 };
	opcodes[0x8B] = { "ADC_A_n",&CPU::ADC_A_n,&CPU::getA,&CPU::getE,4 };
	opcodes[0x8C] = { "ADC_A_n",&CPU::ADC_A_n,&CPU::getA,&CPU::getH,4 };
	opcodes[0x8D] = { "ADC_A_n",&CPU::ADC_A_n,&CPU::getA,&CPU::getL,4 };
	opcodes[0x8E] = { "ADC_A_n",&CPU::ADC_A_n,&CPU::getA,&CPU::get$HL,8 };
	opcodes[0xCE] = { "ADC_A_n",&CPU::ADC_A_n,&CPU::getA,&CPU::get$N,8 };
	//SUB n
	opcodes[0x97] = { "SUB_n",&CPU::SUB_n,&CPU::getA,&CPU::getA,4 };
	opcodes[0x90] = { "SUB_n",&CPU::SUB_n,&CPU::getA,&CPU::getB,4 };
	opcodes[0x91] = { "SUB_n",&CPU::SUB_n,&CPU::getA,&CPU::getC,4 };
	opcodes[0x92] = { "SUB_n",&CPU::SUB_n,&CPU::getA,&CPU::getD,4 };
	opcodes[0x93] = { "SUB_n",&CPU::SUB_n,&CPU::getA,&CPU::getE,4 };
	opcodes[0x94] = { "SUB_n",&CPU::SUB_n,&CPU::getA,&CPU::getH,4 };
	opcodes[0x95] = { "SUB_n",&CPU::SUB_n,&CPU::getA,&CPU::getL,4 };
	opcodes[0x96] = { "SUB_n",&CPU::SUB_n,&CPU::getA,&CPU::get$HL,8 };
	opcodes[0xD6] = { "SUB_n",&CPU::SUB_n,&CPU::getA,&CPU::get$N,8 };
	//SBC A,n
	opcodes[0x9F] = { "SBC_A_n",&CPU::SBC_A_n,&CPU::getA,&CPU::getA,4 };
	opcodes[0x98] = { "SBC_A_n",&CPU::SBC_A_n,&CPU::getA,&CPU::getB,4 };
	opcodes[0x99] = { "SBC_A_n",&CPU::SBC_A_n,&CPU::getA,&CPU::getC,4 };
	opcodes[0x9A] = { "SBC_A_n",&CPU::SBC_A_n,&CPU::getA,&CPU::getD,4 };
	opcodes[0x9B] = { "SBC_A_n",&CPU::SBC_A_n,&CPU::getA,&CPU::getE,4 };
	opcodes[0x9C] = { "SBC_A_n",&CPU::SBC_A_n,&CPU::getA,&CPU::getH,4 };
	opcodes[0x9D] = { "SBC_A_n",&CPU::SBC_A_n,&CPU::getA,&CPU::getL,4 };
	opcodes[0x9E] = { "SBC_A_n",&CPU::SBC_A_n,&CPU::getA,&CPU::get$HL,8 };
	//AND n
	opcodes[0xA7] = { "AND_n",&CPU::AND_n,&CPU::getA,&CPU::getA,4 };
	opcodes[0xA0] = { "AND_n",&CPU::AND_n,&CPU::getA,&CPU::getB,4 };
	opcodes[0xA1] = { "AND_n",&CPU::AND_n,&CPU::getA,&CPU::getC,4 };
	opcodes[0xA2] = { "AND_n",&CPU::AND_n,&CPU::getA,&CPU::getD,4 };
	opcodes[0xA3] = { "AND_n",&CPU::AND_n,&CPU::getA,&CPU::getE,4 };
	opcodes[0xA4] = { "AND_n",&CPU::AND_n,&CPU::getA,&CPU::getH,4 };
	opcodes[0xA5] = { "AND_n",&CPU::AND_n,&CPU::getA,&CPU::getL,4 };
	opcodes[0xA6] = { "AND_n",&CPU::AND_n,&CPU::getA,&CPU::get$HL,8 };
	opcodes[0xE6] = { "AND_n",&CPU::AND_n,&CPU::getA,&CPU::get$N,8 };
	//OR n
	opcodes[0xB7] = { "OR_n",&CPU::OR_n,&CPU::getA,&CPU::getA,4 };
	opcodes[0xB0] = { "OR_n",&CPU::OR_n,&CPU::getA,&CPU::getB,4 };
	opcodes[0xB1] = { "OR_n",&CPU::OR_n,&CPU::getA,&CPU::getC,4 };
	opcodes[0xB2] = { "OR_n",&CPU::OR_n,&CPU::getA,&CPU::getD,4 };
	opcodes[0xB3] = { "OR_n",&CPU::OR_n,&CPU::getA,&CPU::getE,4 };
	opcodes[0xB4] = { "OR_n",&CPU::OR_n,&CPU::getA,&CPU::getH,4 };
	opcodes[0xB5] = { "OR_n",&CPU::OR_n,&CPU::getA,&CPU::getL,4 };
	opcodes[0xB6] = { "OR_n",&CPU::OR_n,&CPU::getA,&CPU::get$HL,8 };
	opcodes[0xF6] = { "OR_n",&CPU::OR_n,&CPU::getA,&CPU::get$N,8 };
	//XOR n
	opcodes[0xAF] = { "XOR_n",&CPU::XOR_n,&CPU::getA,&CPU::getA,4 };
	opcodes[0xA8] = { "XOR_n",&CPU::XOR_n,&CPU::getA,&CPU::getB,4 };
	opcodes[0xA9] = { "XOR_n",&CPU::XOR_n,&CPU::getA,&CPU::getC,4 };
	opcodes[0xAA] = { "XOR_n",&CPU::XOR_n,&CPU::getA,&CPU::getD,4 };
	opcodes[0xAB] = { "XOR_n",&CPU::XOR_n,&CPU::getA,&CPU::getE,4 };
	opcodes[0xAC] = { "XOR_n",&CPU::XOR_n,&CPU::getA,&CPU::getH,4 };
	opcodes[0xAD] = { "XOR_n",&CPU::XOR_n,&CPU::getA,&CPU::getL,4 };
	opcodes[0xAE] = { "XOR_n",&CPU::XOR_n,&CPU::getA,&CPU::get$HL,8 };
	opcodes[0xEE] = { "XOR_n",&CPU::XOR_n,&CPU::getA,&CPU::get$N,8 };
	//CP n
	opcodes[0xBF] = { "CP_n",&CPU::CP_n,&CPU::getA,&CPU::getA,4 };
	opcodes[0xB8] = { "CP_n",&CPU::CP_n,&CPU::getA,&CPU::getB,4 };
	opcodes[0xB9] = { "CP_n",&CPU::CP_n,&CPU::getA,&CPU::getC,4 };
	opcodes[0xBA] = { "CP_n",&CPU::CP_n,&CPU::getA,&CPU::getD,4 };
	opcodes[0xBB] = { "CP_n",&CPU::CP_n,&CPU::getA,&CPU::getE,4 };
	opcodes[0xBC] = { "CP_n",&CPU::CP_n,&CPU::getA,&CPU::getH,4 };
	opcodes[0xBD] = { "CP_n",&CPU::CP_n,&CPU::getA,&CPU::getL,4 };
	opcodes[0xBE] = { "CP_n",&CPU::CP_n,&CPU::getA,&CPU::get$HL,8 };
	opcodes[0xFE] = { "CP_n",&CPU::CP_n,&CPU::getA,&CPU::get$N,8 };
	//INC n
	opcodes[0x3C] = { "INC_n",&CPU::INC_n,&CPU::getA,&CPU::getNULL,4 };
	opcodes[0x04] = { "INC_n",&CPU::INC_n,&CPU::getB,&CPU::getNULL,4 };
	opcodes[0x0C] = { "INC_n",&CPU::INC_n,&CPU::getC,&CPU::getNULL,4 };
	opcodes[0x14] = { "INC_n",&CPU::INC_n,&CPU::getD,&CPU::getNULL,4 };
	opcodes[0x1C] = { "INC_n",&CPU::INC_n,&CPU::getE,&CPU::getNULL,4 };
	opcodes[0x24] = { "INC_n",&CPU::INC_n,&CPU::getH,&CPU::getNULL,4 };
	opcodes[0x2C] = { "INC_n",&CPU::INC_n,&CPU::getL,&CPU::getNULL,4 };
	opcodes[0x34] = { "INC_n",&CPU::INC_n,&CPU::get$HL,&CPU::getNULL,12 };
	//DEC n
	opcodes[0x3D] = { "DEC_n",&CPU::DEC_n,&CPU::getA,&CPU::getNULL,4 };
	opcodes[0x05] = { "DEC_n",&CPU::DEC_n,&CPU::getB,&CPU::getNULL,4 };
	opcodes[0x0D] = { "DEC_n",&CPU::DEC_n,&CPU::getC,&CPU::getNULL,4 };
	opcodes[0x15] = { "DEC_n",&CPU::DEC_n,&CPU::getD,&CPU::getNULL,4 };
	opcodes[0x1D] = { "DEC_n",&CPU::DEC_n,&CPU::getE,&CPU::getNULL,4 };
	opcodes[0x25] = { "DEC_n",&CPU::DEC_n,&CPU::getH,&CPU::getNULL,4 };
	opcodes[0x2D] = { "DEC_n",&CPU::DEC_n,&CPU::getL,&CPU::getNULL,4 };
	opcodes[0x35] = { "DEC_n",&CPU::DEC_n,&CPU::get$HL,&CPU::getNULL,12 };
	//ADD HL,n
	opcodes[0x09] = { "ADD_HL_n",&CPU::ADD_HL_n,&CPU::getHL,&CPU::getBC,8 };
	opcodes[0x19] = { "ADD_HL_n",&CPU::ADD_HL_n,&CPU::getHL,&CPU::getDE,8 };
	opcodes[0x29] = { "ADD_HL_n",&CPU::ADD_HL_n,&CPU::getHL,&CPU::getHL,8 };
	opcodes[0x39] = { "ADD_HL_n",&CPU::ADD_HL_n,&CPU::getHL,&CPU::getSP,8 };
	//ADD SP,n
	opcodes[0xE8] = { "ADD_SP_n",&CPU::ADD_SP_n,&CPU::getSP,&CPU::getN,16 };
	//INC nn
	opcodes[0x03] = { "INC_nn",&CPU::INC_nn,&CPU::getBC,&CPU::getNULL,8 };
	opcodes[0x13] = { "INC_nn",&CPU::INC_nn,&CPU::getDE,&CPU::getNULL,8 };
	opcodes[0x23] = { "INC_nn",&CPU::INC_nn,&CPU::getHL,&CPU::getNULL,8 };
	opcodes[0x33] = { "INC_nn",&CPU::INC_nn,&CPU::getSP,&CPU::getNULL,8 };
	//DEC nn
	opcodes[0x0B] = { "DEC_nn",&CPU::DEC_nn,&CPU::getBC,&CPU::getNULL,8 };
	opcodes[0x1B] = { "DEC_nn",&CPU::DEC_nn,&CPU::getDE,&CPU::getNULL,8 };
	opcodes[0x2B] = { "DEC_nn",&CPU::DEC_nn,&CPU::getHL,&CPU::getNULL,8 };
	opcodes[0x3B] = { "DEC_nn",&CPU::DEC_nn,&CPU::getSP,&CPU::getNULL,8 };
	//SWAP n
	opcodes[0xCB37] = { "SWAP_n",&CPU::SWAP_n,&CPU::getA,&CPU::getNULL,8 };
	opcodes[0xCB30] = { "SWAP_n",&CPU::SWAP_n,&CPU::getB,&CPU::getNULL,8 };
	opcodes[0xCB31] = { "SWAP_n",&CPU::SWAP_n,&CPU::getC,&CPU::getNULL,8 };
	opcodes[0xCB32] = { "SWAP_n",&CPU::SWAP_n,&CPU::getD,&CPU::getNULL,8 };
	opcodes[0xCB33] = { "SWAP_n",&CPU::SWAP_n,&CPU::getE,&CPU::getNULL,8 };
	opcodes[0xCB34] = { "SWAP_n",&CPU::SWAP_n,&CPU::getH,&CPU::getNULL,8 };
	opcodes[0xCB35] = { "SWAP_n",&CPU::SWAP_n,&CPU::getL,&CPU::getNULL,8 };
	opcodes[0xCB36] = { "SWAP_n",&CPU::SWAP_n,&CPU::get$HL,&CPU::getNULL,16 };
	//DAA
	opcodes[0x27] = { "DAA",&CPU::DAA,&CPU::getNULL,&CPU::getNULL,4 };
	//CPL
	opcodes[0x2F] = { "CPL",&CPU::CPL,&CPU::getNULL,&CPU::getNULL,4 };
	//CCF
	opcodes[0x3F] = { "CCF",&CPU::CCF,&CPU::getNULL,&CPU::getNULL,4 };
	//SCF
	opcodes[0x37] = { "SCF",&CPU::SCF,&CPU::getNULL,&CPU::getNULL,4 };
	//NOP
	opcodes[0x00] = { "NOP",&CPU::NOP,&CPU::getNULL,&CPU::getNULL,4 };
	//HALT
	opcodes[0x76] = { "HALT",&CPU::HALT,&CPU::getNULL,&CPU::getNULL,4 };
	//STOP
	opcodes[0x1000] = { "STOP",&CPU::STOP,&CPU::getNULL,&CPU::getNULL,4 };
	//DI
	opcodes[0xF3] = { "DI",&CPU::DI,&CPU::getNULL,&CPU::getNULL,4 };
	//EI
	opcodes[0xFB] = { "EI",&CPU::EI,&CPU::getNULL,&CPU::getNULL,4 };
	//RLCA
	opcodes[0x07] = { "RLCA",&CPU::RLCA,&CPU::getNULL,&CPU::getNULL,4 };
	//RLA
	opcodes[0x17] = { "RLA",&CPU::RLA,&CPU::getNULL,&CPU::getNULL,4 };
	//RRCA
	opcodes[0x0F] = { "RRCA",&CPU::RRCA,&CPU::getNULL,&CPU::getNULL,4 };
	//RRA
	opcodes[0x1F] = { "RRA",&CPU::RRA,&CPU::getNULL,&CPU::getNULL,4 };
	//RLC n
	opcodes[0xCB07] = { "RLC_n",&CPU::RLC_n,&CPU::getA,&CPU::getNULL,8 };
	opcodes[0xCB00] = { "RLC_n",&CPU::RLC_n,&CPU::getB,&CPU::getNULL,8 };
	opcodes[0xCB01] = { "RLC_n",&CPU::RLC_n,&CPU::getC,&CPU::getNULL,8 };
	opcodes[0xCB02] = { "RLC_n",&CPU::RLC_n,&CPU::getD,&CPU::getNULL,8 };
	opcodes[0xCB03] = { "RLC_n",&CPU::RLC_n,&CPU::getE,&CPU::getNULL,8 };
	opcodes[0xCB04] = { "RLC_n",&CPU::RLC_n,&CPU::getH,&CPU::getNULL,8 };
	opcodes[0xCB05] = { "RLC_n",&CPU::RLC_n,&CPU::getL,&CPU::getNULL,8 };
	opcodes[0xCB06] = { "RLC_n",&CPU::RLC_n,&CPU::get$HL,&CPU::getNULL,16 };
	//RL n
	opcodes[0xCB17] = { "RL_n",&CPU::RL_n,&CPU::getA,&CPU::getNULL,8 };
	opcodes[0xCB10] = { "RL_n",&CPU::RL_n,&CPU::getB,&CPU::getNULL,8 };
	opcodes[0xCB11] = { "RL_n",&CPU::RL_n,&CPU::getC,&CPU::getNULL,8 };
	opcodes[0xCB12] = { "RL_n",&CPU::RL_n,&CPU::getD,&CPU::getNULL,8 };
	opcodes[0xCB13] = { "RL_n",&CPU::RL_n,&CPU::getE,&CPU::getNULL,8 };
	opcodes[0xCB14] = { "RL_n",&CPU::RL_n,&CPU::getH,&CPU::getNULL,8 };
	opcodes[0xCB15] = { "RL_n",&CPU::RL_n,&CPU::getL,&CPU::getNULL,8 };
	opcodes[0xCB16] = { "RL_n",&CPU::RL_n,&CPU::get$HL,&CPU::getNULL,16 };
	//RRC n
	opcodes[0xCB0F] = { "RRC_n",&CPU::RRC_n,&CPU::getA,&CPU::getNULL,8 };
	opcodes[0xCB08] = { "RRC_n",&CPU::RRC_n,&CPU::getB,&CPU::getNULL,8 };
	opcodes[0xCB09] = { "RRC_n",&CPU::RRC_n,&CPU::getC,&CPU::getNULL,8 };
	opcodes[0xCB0A] = { "RRC_n",&CPU::RRC_n,&CPU::getD,&CPU::getNULL,8 };
	opcodes[0xCB0B] = { "RRC_n",&CPU::RRC_n,&CPU::getE,&CPU::getNULL,8 };
	opcodes[0xCB0C] = { "RRC_n",&CPU::RRC_n,&CPU::getH,&CPU::getNULL,8 };
	opcodes[0xCB0D] = { "RRC_n",&CPU::RRC_n,&CPU::getL,&CPU::getNULL,8 };
	opcodes[0xCB0E] = { "RRC_n",&CPU::RRC_n,&CPU::get$HL,&CPU::getNULL,16 };
	//RR n
	opcodes[0xCB1F] = { "RR_n",&CPU::RR_n,&CPU::getA,&CPU::getNULL,8 };
	opcodes[0xCB18] = { "RR_n",&CPU::RR_n,&CPU::getB,&CPU::getNULL,8 };
	opcodes[0xCB19] = { "RR_n",&CPU::RR_n,&CPU::getC,&CPU::getNULL,8 };
	opcodes[0xCB1A] = { "RR_n",&CPU::RR_n,&CPU::getD,&CPU::getNULL,8 };
	opcodes[0xCB1B] = { "RR_n",&CPU::RR_n,&CPU::getE,&CPU::getNULL,8 };
	opcodes[0xCB1C] = { "RR_n",&CPU::RR_n,&CPU::getH,&CPU::getNULL,8 };
	opcodes[0xCB1D] = { "RR_n",&CPU::RR_n,&CPU::getL,&CPU::getNULL,8 };
	opcodes[0xCB1E] = { "RR_n",&CPU::RR_n,&CPU::get$HL,&CPU::getNULL,16 };
	//SLA n
	opcodes[0xCB27] = { "SLA_n",&CPU::SLA_n,&CPU::getA,&CPU::getNULL,8 };
	opcodes[0xCB20] = { "SLA_n",&CPU::SLA_n,&CPU::getB,&CPU::getNULL,8 };
	opcodes[0xCB21] = { "SLA_n",&CPU::SLA_n,&CPU::getC,&CPU::getNULL,8 };
	opcodes[0xCB22] = { "SLA_n",&CPU::SLA_n,&CPU::getD,&CPU::getNULL,8 };
	opcodes[0xCB23] = { "SLA_n",&CPU::SLA_n,&CPU::getE,&CPU::getNULL,8 };
	opcodes[0xCB24] = { "SLA_n",&CPU::SLA_n,&CPU::getH,&CPU::getNULL,8 };
	opcodes[0xCB25] = { "SLA_n",&CPU::SLA_n,&CPU::getL,&CPU::getNULL,8 };
	opcodes[0xCB26] = { "SLA_n",&CPU::SLA_n,&CPU::get$HL,&CPU::getNULL,16 };
	//SRA n
	opcodes[0xCB2F] = { "SRA_n",&CPU::SRA_n,&CPU::getA,&CPU::getNULL,8 };
	opcodes[0xCB28] = { "SRA_n",&CPU::SRA_n,&CPU::getB,&CPU::getNULL,8 };
	opcodes[0xCB29] = { "SRA_n",&CPU::SRA_n,&CPU::getC,&CPU::getNULL,8 };
	opcodes[0xCB2A] = { "SRA_n",&CPU::SRA_n,&CPU::getD,&CPU::getNULL,8 };
	opcodes[0xCB2B] = { "SRA_n",&CPU::SRA_n,&CPU::getE,&CPU::getNULL,8 };
	opcodes[0xCB2C] = { "SRA_n",&CPU::SRA_n,&CPU::getH,&CPU::getNULL,8 };
	opcodes[0xCB2D] = { "SRA_n",&CPU::SRA_n,&CPU::getL,&CPU::getNULL,8 };
	opcodes[0xCB2E] = { "SRA_n",&CPU::SRA_n,&CPU::get$HL,&CPU::getNULL,16 };
	//SRL n
	opcodes[0xCB3F] = { "SRL_n",&CPU::SRL_n,&CPU::getA,&CPU::getNULL,8 };
	opcodes[0xCB38] = { "SRL_n",&CPU::SRL_n,&CPU::getB,&CPU::getNULL,8 };
	opcodes[0xCB39] = { "SRL_n",&CPU::SRL_n,&CPU::getC,&CPU::getNULL,8 };
	opcodes[0xCB3A] = { "SRL_n",&CPU::SRL_n,&CPU::getD,&CPU::getNULL,8 };
	opcodes[0xCB3B] = { "SRL_n",&CPU::SRL_n,&CPU::getE,&CPU::getNULL,8 };
	opcodes[0xCB3C] = { "SRL_n",&CPU::SRL_n,&CPU::getH,&CPU::getNULL,8 };
	opcodes[0xCB3D] = { "SRL_n",&CPU::SRL_n,&CPU::getL,&CPU::getNULL,8 };
	opcodes[0xCB3E] = { "SRL_n",&CPU::SRL_n,&CPU::get$HL,&CPU::getNULL,16 };
	//BIT (n),r
	opcodes[0xCB47] = { "BIT_$n_r",&CPU::BIT_$n_r,&CPU::get$N,&CPU::getA,8 };
	opcodes[0xCB40] = { "BIT_$n_r",&CPU::BIT_$n_r,&CPU::get$N,&CPU::getB,8 };
	opcodes[0xCB41] = { "BIT_$n_r",&CPU::BIT_$n_r,&CPU::get$N,&CPU::getC,8 };
	opcodes[0xCB42] = { "BIT_$n_r",&CPU::BIT_$n_r,&CPU::get$N,&CPU::getD,8 };
	opcodes[0xCB43] = { "BIT_$n_r",&CPU::BIT_$n_r,&CPU::get$N,&CPU::getE,8 };
	opcodes[0xCB44] = { "BIT_$n_r",&CPU::BIT_$n_r,&CPU::get$N,&CPU::getH,8 };
	opcodes[0xCB45] = { "BIT_$n_r",&CPU::BIT_$n_r,&CPU::get$N,&CPU::getL,8 };
	opcodes[0xCB46] = { "BIT_$n_r",&CPU::BIT_$n_r,&CPU::get$N,&CPU::get$HL,16 };
	//SET (n),r
	opcodes[0xCBC7] = { "SET_$n_r",&CPU::SET_$n_r,&CPU::get$N,&CPU::getA,8 };
	opcodes[0xCBC0] = { "SET_$n_r",&CPU::SET_$n_r,&CPU::get$N,&CPU::getB,8 };
	opcodes[0xCBC1] = { "SET_$n_r",&CPU::SET_$n_r,&CPU::get$N,&CPU::getC,8 };
	opcodes[0xCBC2] = { "SET_$n_r",&CPU::SET_$n_r,&CPU::get$N,&CPU::getD,8 };
	opcodes[0xCBC3] = { "SET_$n_r",&CPU::SET_$n_r,&CPU::get$N,&CPU::getE,8 };
	opcodes[0xCBC4] = { "SET_$n_r",&CPU::SET_$n_r,&CPU::get$N,&CPU::getH,8 };
	opcodes[0xCBC5] = { "SET_$n_r",&CPU::SET_$n_r,&CPU::get$N,&CPU::getL,8 };
	opcodes[0xCBC6] = { "SET_$n_r",&CPU::SET_$n_r,&CPU::get$N,&CPU::get$HL,16 };
	//RES (n),r
	opcodes[0xCB87] = { "RES_$n_r",&CPU::RES_$n_r,&CPU::get$N,&CPU::getA,8 };
	opcodes[0xCB80] = { "RES_$n_r",&CPU::RES_$n_r,&CPU::get$N,&CPU::getB,8 };
	opcodes[0xCB81] = { "RES_$n_r",&CPU::RES_$n_r,&CPU::get$N,&CPU::getC,8 };
	opcodes[0xCB82] = { "RES_$n_r",&CPU::RES_$n_r,&CPU::get$N,&CPU::getD,8 };
	opcodes[0xCB83] = { "RES_$n_r",&CPU::RES_$n_r,&CPU::get$N,&CPU::getE,8 };
	opcodes[0xCB84] = { "RES_$n_r",&CPU::RES_$n_r,&CPU::get$N,&CPU::getH,8 };
	opcodes[0xCB85] = { "RES_$n_r",&CPU::RES_$n_r,&CPU::get$N,&CPU::getL,8 };
	opcodes[0xCB86] = { "RES_$n_r",&CPU::RES_$n_r,&CPU::get$N,&CPU::get$HL,16 };
	//JP nn
	opcodes[0xC3] = { "JP_nn",&CPU::JP_nn,&CPU::getNN,&CPU::getNULL,12 };
	//JP cc,nn
	opcodes[0xC2] = { "JP_cc_nn",&CPU::JP_cc_nn,&CPU::NZFlag,&CPU::getNN,12 };
	opcodes[0xCA] = { "JP_cc_nn",&CPU::JP_cc_nn,&CPU::ZFlag,&CPU::getNN,12 };
	opcodes[0xD2] = { "JP_cc_nn",&CPU::JP_cc_nn,&CPU::NCFlag,&CPU::getNN,12 };
	opcodes[0xDA] = { "JP_cc_nn",&CPU::JP_cc_nn,&CPU::CFlag,&CPU::getNN,12 };
	//JP HL
	opcodes[0xE9] = { "JP_HL",&CPU::JP_HL,&CPU::get$HL,&CPU::getNULL,4 };
	//JR n
	opcodes[0x18] = { "JR_n",&CPU::JR_n,&CPU::getN,&CPU::getNULL,8 };
	//JR cc,n
	opcodes[0x20] = { "JR_cc_n",&CPU::JR_cc_n,&CPU::NZFlag,&CPU::getN,8 };
	opcodes[0x28] = { "JR_cc_n",&CPU::JR_cc_n,&CPU::ZFlag,&CPU::getN,8 };
	opcodes[0x30] = { "JR_cc_n",&CPU::JR_cc_n,&CPU::NCFlag,&CPU::getN,8 };
	opcodes[0x38] = { "JR_cc_n",&CPU::JR_cc_n,&CPU::CFlag,&CPU::getN,8 };
	//CALL nn
	opcodes[0xCD] = { "CALL_nn",&CPU::CALL_nn,&CPU::getNN,&CPU::getNULL,12 };
	//CALL cc,nn
	opcodes[0xC4] = { "CALL_cc_nn",&CPU::CALL_cc_nn,&CPU::NZFlag,&CPU::getNN,12 };
	opcodes[0xCC] = { "CALL_cc_nn",&CPU::CALL_cc_nn,&CPU::ZFlag,&CPU::getNN,12 };
	opcodes[0xD4] = { "CALL_cc_nn",&CPU::CALL_cc_nn,&CPU::NCFlag,&CPU::getNN,12 };
	opcodes[0xDC] = { "CALL_cc_nn",&CPU::CALL_cc_nn,&CPU::CFlag,&CPU::getNN,12 };
	//RST n
	opcodes[0xC7] = { "RST_n",&CPU::RST_n,&CPU::getRST00,&CPU::getNULL,32 };
	opcodes[0xCF] = { "RST_n",&CPU::RST_n,&CPU::getRST08,&CPU::getNULL,32 };
	opcodes[0xD7] = { "RST_n",&CPU::RST_n,&CPU::getRST10,&CPU::getNULL,32 };
	opcodes[0xDF] = { "RST_n",&CPU::RST_n,&CPU::getRST18,&CPU::getNULL,32 };
	opcodes[0xE7] = { "RST_n",&CPU::RST_n,&CPU::getRST20,&CPU::getNULL,32 };
	opcodes[0xEF] = { "RST_n",&CPU::RST_n,&CPU::getRST28,&CPU::getNULL,32 };
	opcodes[0xF7] = { "RST_n",&CPU::RST_n,&CPU::getRST30,&CPU::getNULL,32 };
	opcodes[0xFF] = { "RST_n",&CPU::RST_n,&CPU::getRST38,&CPU::getNULL,32 };
	//RET
	opcodes[0xC9] = { "RET",&CPU::RET,&CPU::getNULL,&CPU::getNULL,8 };
	//RET cc
	opcodes[0xC0] = { "RET_cc",&CPU::RET_cc,&CPU::NZFlag,&CPU::getNULL,8 };
	opcodes[0xC8] = { "RET_cc",&CPU::RET_cc,&CPU::ZFlag,&CPU::getNULL,8 };
	opcodes[0xD0] = { "RET_cc",&CPU::RET_cc,&CPU::NCFlag,&CPU::getNULL,8 };
	opcodes[0xD8] = { "RET_cc",&CPU::RET_cc,&CPU::CFlag,&CPU::getNULL,8 };
	//RETI
	opcodes[0xD9] = { "RETI",&CPU::RETI,&CPU::getNULL,&CPU::getNULL,8 };

	reset();
}

uint8_t* CPU::getA()
{
	return (uint8_t*)&AF + 1;

}
uint8_t* CPU::getF()
{
	return (uint8_t*)&AF;
}
uint8_t* CPU::getB()
{
	return (uint8_t*)&BC + 1;
}
uint8_t* CPU::getC()
{
	return (uint8_t*)&BC;
}
uint8_t* CPU::getD()
{
	return (uint8_t*)&DE + 1;
}
uint8_t* CPU::getE()
{
	return (uint8_t*)&DE;
}

uint8_t* CPU::getH()
{
	return (uint8_t*)&HL + 1;
}
uint8_t* CPU::getL()
{
	return (uint8_t*)&HL;
}
uint8_t* CPU::getAF()
{
	return (uint8_t*)&AF;
}
uint8_t* CPU::getBC()
{
	return(uint8_t*)&BC;
}
uint8_t* CPU::getHL()
{
	return (uint8_t*)&HL;
}
uint8_t* CPU::getDE()
{
	return (uint8_t*)&DE;
}
uint8_t* CPU::get$BC()
{
	n=bus->mmu->getMemCell(BC);
	return n;
}
uint8_t* CPU::get$DE()
{
	n = bus->mmu->getMemCell(DE);
	return n;
}
uint8_t* CPU::get$HL()
{
	n = bus->mmu->getMemCell(HL);
	return n;
}
uint8_t* CPU::getN()
{
	n= bus->mmu->read(PC++);
	return &n;
}
uint8_t* CPU::getNN()
{
	uint8_t lsb = bus->mmu->read(PC++);
	uint8_t msb =bus->mmu->read(PC++);
	
	nn = msb << 8 | lsb;
	return (uint8_t*)&nn;
}
uint8_t* CPU::get$N()
{
	n = bus->mmu->read(bus->mmu->read(PC++));
	return &n;
}
uint8_t* CPU::get$NN()
{
	
	uint8_t lsb = bus->mmu->read(PC++);
	uint8_t msb = bus->mmu->read(PC++);
	n = bus->mmu->read(msb << 8 | lsb);
	return &n;
}

uint8_t* CPU::getImmediate()
{
	n=*getN();
	return &n;
}
uint8_t* CPU::getSP()
{
	return(uint8_t*)&SP;
}

uint8_t* CPU::getNULL()
{
	return NULL;
}







void CPU::LD_nn_N(uint8_t* nn, uint8_t* N)
{
	*nn = *N;
}
void CPU::LD_r1_r2(uint8_t* r1, uint8_t* r2)
{
	*r1 = *r2;
}
void CPU::LD_A_n(uint8_t* A, uint8_t* n)
{
	*A = *n;
}
void CPU::LD_n_A(uint8_t* n, uint8_t* A)
{
	*n = *A;
}
void CPU::LD_A_C(uint8_t* A, uint8_t* C)
{
	//?
	*A=bus->mmu->read(0xff00 + *C);
}
void CPU::LD_C_A(uint8_t* C, uint8_t* A)
{
	//?
	bus->mmu->write(0xff00 + *C, *A);
}

void CPU::LDD_A_$HL(uint8_t* A, uint8_t* $HL)
{
	*A = *$HL;
	HL--;
}
void CPU::LDD_$HL_A(uint8_t* $HL, uint8_t* A)
{
	*$HL = *A;
	HL--;
}
void CPU::LDI_A_$HL(uint8_t* A, uint8_t* $HL)
{
	*A = *$HL;
	HL++;
}
void CPU::LDI_$HL_A(uint8_t* $HL, uint8_t* A)
{
	uint16_t* hl = (uint16_t*)$HL;
	bus->mmu->write(*hl, *A);
	HL++;
}
void CPU::LDH_n_A(uint8_t* N, uint8_t* A){
	bus->mmu->write(0xff00 + *N, *A);
}
void CPU::LDH_A_n(uint8_t* A,uint8_t* N) {
	*A=bus->mmu->read(0xff00 + *N);
}
void CPU::LD_n_nn(uint8_t* n, uint8_t* nn)
{
	*(uint16_t*)n = *(uint16_t*)nn;

}
void CPU::LD_SP_HL(uint8_t* SP, uint8_t* HL)
{
	*(uint16_t*) SP = *(uint16_t*)HL;
}
void CPU::LDHL_SP_$n(uint8_t* SP, uint8_t* $N)
{
	//HL = *(uint16_t*)SP + (signed uint8_t)*N;
	HL = *(uint16_t*)SP + *$N;
	resetFlag('Z');
	resetFlag('N');
	if (*(uint16_t*)SP + *$N > 0xFFFF)
		setFlag('C');
	else
		resetFlag('C');
	if ((*(uint16_t*)SP & 0xF) + (*$N & 0xF) > 0xF)
		setFlag('H');
	else
		resetFlag('H');
	
}
void CPU::LD_$NN_SP(uint8_t* $NN,uint8_t* SP)
{
	//?
	*(uint16_t*)$NN = *(uint16_t*)SP;
}

void CPU::PUSH_nn(uint8_t* nn, uint8_t* SP)
{
	//?
	uint16_t sp = *(uint16_t*)SP;
	uint16_t n = *(uint16_t*)nn;
	Word word = WordToBytes(n);	
	sp--;
	bus->mmu->write(sp--, word.msb);
	bus->mmu->write(sp--, word.lsb);
	*(uint16_t*)SP = sp;
}

void CPU::POP_nn(uint8_t* SP, uint8_t* nn)
{
	uint16_t sp = *(uint16_t*)SP;
	uint8_t lsb=bus->mmu->read(sp++);
	uint8_t msb=bus->mmu->read(sp++);
	uint16_t word=BytesToWord(msb,lsb);
	*(uint16_t*)nn=word;
}

void CPU::ADD_A_n(uint8_t* A, uint8_t* n)
{
	Byte byteR = getByte(*A);
	uint8_t oldA = *A;
	*A += *n;
	if (*A == 0)
		setFlag('Z');
	resetFlag('N');
	if (byteR.lsn + (*n &0xf) > 0xf)
		setFlag('H');
	if (oldA + *n > 0xff)
		setFlag('C');
}

void CPU::ADC_A_n(uint8_t* A, uint8_t* n)
{
	Byte byteR = getByte(*A);
	
	uint8_t oldA = *A;
	*A += *n + getFlag('C');
	if (*A == 0)
		setFlag('Z');
	resetFlag('N');
	if (byteR.lsn + (*n &0xf) + getFlag('C') > 0xf)
		setFlag('H');
	if (oldA + *n + getFlag('C') > 0xff)
		setFlag('C');
}
void CPU::SUB_n(uint8_t* A, uint8_t* n) {
	Byte byteR = getByte(*A);
	
	uint8_t oldA = *A;
	
	*A -= *n & 0xf;
	if (*A == 0)
		setFlag('Z');
	setFlag('N');
	if (byteR.lsn < (*n&0xf))
		setFlag('H');
	if (oldA < *n)
		setFlag('C');

}
void CPU::SBC_A_n(uint8_t* A, uint8_t* n) {
	Byte byteR = getByte(*A);
	
	uint8_t oldA = *A;

	*A -= (*n + getFlag('C'));
	if (*A == 0)
		setFlag('Z');
	setFlag('N');
	if (byteR.lsn < (*n &0xf) + getFlag('C'))
		setFlag('H');
	if (oldA < *n + getFlag('C'))
		setFlag('C');

}
void CPU::AND_n(uint8_t* A, uint8_t* n)
{
	Byte byteR = getByte(*A);
	
	*A &= *n;
	if (*A == 0)
		setFlag('Z');
	resetFlag('N');
	setFlag('H');
	resetFlag('C');
}
void CPU::OR_n(uint8_t* A, uint8_t* n)
{
	Byte byteR = getByte(*A);
	*A |= *n;
	if (*A == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
	resetFlag('C');
}
void CPU::XOR_n(uint8_t* A, uint8_t* n)
{
	//? *?
	Byte byteR = getByte(*A);
	*A ^= *n;
	if (*A == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
	resetFlag('C');
}
void CPU::CP_n(uint8_t* n, uint8_t* none) {
	uint8_t A = *getA();
	Byte byteR = getByte(A);
	uint8_t res = A - *n;
	if (res == 0)
		setFlag('Z');
	setFlag('N');
	if (byteR.lsn < (*n & 0xf))
		setFlag('H');
	if (A < *n)
		setFlag('C');
}
void CPU::INC_n(uint8_t* n, uint8_t* none) {
	Byte byteN = getByte(*n);
	(*n)++;
	if (*n == 0)
		setFlag('Z');
	resetFlag('N');
	if (byteN.lsn +1  > 0xf)
		setFlag('H');
}
void CPU::DEC_n(uint8_t* n, uint8_t* none) {
	Byte byteN = getByte(*n);
	(*n)--;
	if (*n == 0)
		setFlag('Z');
	setFlag('N');
	//? if (byteN.lsn < 0xf)
	if (byteN.lsn < 1)
		setFlag('H');
}
void CPU::ADD_HL_n(uint8_t* HL, uint8_t* n) {

	uint16_t oldHL=* (uint16_t*)HL;
	uint16_t n_ = *(uint16_t*)n;
	*HL += n_;

	resetFlag('N');
	if((oldHL&0xfff) +(n_&0xfff)> 0xfff)
		setFlag('H');
	if (oldHL + n_ > 0xffff)
		setFlag('C');
}
void CPU::ADD_SP_n(uint8_t* SP, uint8_t* n) {
	uint16_t oldSP = *(uint16_t*)SP;
	*(uint16_t*)SP += *n;
	resetFlag('Z');
	resetFlag('N');
	if (oldSP + *n > 0xFFFF)
		setFlag('C');
	else
		resetFlag('C');
	if ((oldSP & 0xF) + (*n & 0xF) > 0xF)
		setFlag('H');
	else
		resetFlag('H');
}
void CPU::INC_nn(uint8_t* nn, uint8_t* none) {
	(*(uint16_t*)nn)++;
}
void CPU::DEC_nn(uint8_t* nn, uint8_t* none) {
	(*(uint16_t*)nn)--;
}
void CPU::SWAP_n(uint8_t* n, uint8_t* none) {
	Byte oldByte = getByte(*n);
	Byte newByte{ oldByte.lsn,oldByte.msn };
	*n = getByte(newByte);
	if (*n == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
	resetFlag('C');

}
void CPU::DAA(uint8_t* none, uint8_t* none2) {
	uint8_t correction = 0;
	Byte A = getByte(*getA());
	if ((!getFlag('N') && A.lsn > 9) || getFlag('H'))
		correction += 0x06;
	if((!getFlag('N') && A.msn > 9) || getFlag('C'))
		correction += 0x60;
	*getA() += getFlag('N')?-correction:correction;
	if (*getA() == 0)
		setFlag('Z');
	resetFlag('H');

	if ((!getFlag('N')&&getByte(A)+correction> 0xFF)||(getFlag('N')&& getByte(A)<correction))
		setFlag('C');
	else
		resetFlag('C');
}
void CPU::CPL(uint8_t* none, uint8_t* none2) {
	*getA() ^= 0xff;
	setFlag('N');
	setFlag('H');
}
void CPU::CCF(uint8_t* none, uint8_t* none2) {

	getFlag('C') ? resetFlag('C') : setFlag('C');
	resetFlag('N');
	resetFlag('H');
}
void CPU::SCF(uint8_t* none, uint8_t* none2) {
	setFlag('C');
	resetFlag('N');
	resetFlag('H');
}
void CPU::NOP(uint8_t* none, uint8_t* none2) {
}
void CPU::HALT(uint8_t* none, uint8_t* none2) {
	haltCPU = 1;
}
void CPU::STOP(uint8_t* none, uint8_t* none2) {
	
	haltCPU = 1;
	haltDisplay = 1;
}
void CPU::DI(uint8_t* none, uint8_t* none2) {
	IME = 0;
}
void CPU::EI(uint8_t* none, uint8_t* none2) {
	IMEScheduled = true;
}
void CPU::RLCA(uint8_t* none, uint8_t* none2) {
	uint8_t A = *getA();
	uint8_t bitLeaving=(1 & (A >> 7));
	*getA()= (A<<1)|bitLeaving;
	bitLeaving ? setFlag('C') : resetFlag('C');

	if(*getA()==0)
	setFlag('Z');
	resetFlag('N');
	resetFlag('H');
}
void CPU::RLA(uint8_t* none, uint8_t* none2) {
	uint8_t A = *getA();
	uint8_t bitLeaving = (1 & (A >> 7));
	*getA() = (A << 1) | getFlag('C');
	bitLeaving ? setFlag('C') : resetFlag('C');

	if (*getA() == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
}
void CPU::RRCA(uint8_t* none, uint8_t* none2) {
	uint8_t A = *getA();
	uint8_t bitLeaving = 1 & A;
	*getA() = (bitLeaving<<7) |(A >> 1);
	bitLeaving ? setFlag('C') : resetFlag('C');

	if (*getA() == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
}
void CPU::RRA(uint8_t* none, uint8_t* none2) {
	uint8_t A = *getA();
	uint8_t bitLeaving = 1 & A;
	*getA() = (getFlag('C') << 7) | (A >> 1);
	bitLeaving ? setFlag('C') : resetFlag('C');

	if (*getA() == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
}
void CPU::RLC_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = (1 & (*n >> 7));
	*n = (*n << 1) | bitLeaving;
	bitLeaving ? setFlag('C') : resetFlag('C');

	if (*n == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
}
void CPU::RL_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = (1 & (*n >> 7));
	*n = (*n << 1) | getFlag('C');
	bitLeaving ? setFlag('C') : resetFlag('C');

	if (*n == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
}
void CPU::RRC_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = 1 & *n;
	*n = (bitLeaving << 7) | (*n >> 1);
	bitLeaving ? setFlag('C') : resetFlag('C');

	if (*n == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
}
void CPU::RR_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = 1 & *n;
	*n = getFlag('C') | (*n >> 1);
	bitLeaving ? setFlag('C') : resetFlag('C');

	if (*n == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
}
void CPU::SLA_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = (1 & (*n >> 7));
	*n <<= 1;
	bitLeaving ? setFlag('C') : resetFlag('C');

	if (*n == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');

}
void CPU::SRA_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = (1 &*n);
	*n >>= 1;
	bitLeaving ? setFlag('C') : resetFlag('C');
	bitLeaving = (1 & (*n>>6));
	*n &= bitLeaving << 7;

	if (*n == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
}
void CPU::SRL_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = (1 & *n);
	*n >>= 1;
	bitLeaving ? setFlag('C') : resetFlag('C');
	*n &= 0x7f;

	if (*n == 0)
		setFlag('Z');
	resetFlag('N');
	resetFlag('H');
}
void CPU::BIT_$n_r(uint8_t* $n, uint8_t* r) {
	uint8_t bit =1 & *r>>*$n;
	bit ? resetFlag('Z') : setFlag('Z');

	resetFlag('N');
	setFlag('H');
}
void CPU::SET_$n_r(uint8_t* $n, uint8_t* r) {
	//?
	*r |= 1 << *$n;
	
}
void CPU::RES_$n_r(uint8_t* $n, uint8_t* r) {
	//?
	*r &= 0xff^(1 << *$n);
}
void CPU::JP_nn(uint8_t* nn, uint8_t* none) {
	PC = *(uint16_t*)nn;
}
void CPU::JP_cc_nn(uint8_t* cc, uint8_t* nn) {
	if((*cc == NZ&&!getFlag('Z'))|| (*cc == Z && getFlag('Z'))|| (*cc == NC && !getFlag('C'))|| (*cc == C && getFlag('C')))
		PC = *(uint16_t*)nn;
}
void CPU::JP_HL(uint8_t* HL, uint8_t* none) {
	PC = *(uint16_t*)HL;
}
void CPU::JR_n(uint8_t* n, uint8_t* none) {
	PC +=(signed char)*n;
}
void CPU::JR_cc_n(uint8_t* cc, uint8_t* n) {
	if ((*cc == NZ && !getFlag('Z')) || (*cc == Z && getFlag('Z')) || (*cc == NC && !getFlag('C')) || (*cc == C && getFlag('C')))
		PC +=(signed char)*n;
}
void CPU::CALL_nn(uint8_t* nn, uint8_t* none) {
	uint16_t next = PC + 1;
	PUSH_nn((uint8_t*)&next, (uint8_t*)&SP);
	PC = *(uint16_t*)nn;
}
void CPU::CALL_cc_nn(uint8_t* cc, uint8_t* nn) {
	if ((*cc == NZ && !getFlag('Z')) || (*cc == Z && getFlag('Z')) || (*cc == NC && !getFlag('C')) || (*cc == C && getFlag('C'))) {
		uint16_t next = PC + 1;
		PUSH_nn((uint8_t*)&next, (uint8_t*)&SP);//?
		PC = *(uint16_t*)nn;
	}
}
void CPU::RST_n(uint8_t* n, uint8_t* none) {
	PUSH_nn((uint8_t*)&PC, (uint8_t*)&SP);
	PC = *n;
}
void CPU::RET(uint8_t* none, uint8_t* none2) {
	uint8_t lsb = bus->mmu->read(SP++);
	uint8_t msb = bus->mmu->read(SP++);
	PC=BytesToWord(msb, lsb);
	
}
void CPU::RET_cc(uint8_t* cc, uint8_t* none) {
	if ((*cc == NZ && !getFlag('Z')) || (*cc == Z && getFlag('Z')) || (*cc == NC && !getFlag('C')) || (*cc == C && getFlag('C'))) {
		uint8_t lsb = bus->mmu->read(SP++);
		uint8_t msb = bus->mmu->read(SP++);
		PC = BytesToWord(msb, lsb);
	}
}
void CPU::RETI(uint8_t* none, uint8_t* none2) {
	RET(none, none2);
	IME = 1;
}


uint8_t CPU::getFlag(uint8_t flag)
{
	uint8_t byte = 16;
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

void CPU::setFlag(uint8_t flag)
{
	uint8_t byte = 16;
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
	uint8_t* f = getF();
	*f = *f | byte;
	
}

void CPU::resetFlag(uint8_t flag)
{
	uint8_t byte = 16;
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
	uint8_t* f = getF();
	*f = *f & byte;
}
uint8_t* CPU::NZFlag() {

	return flagsNames;
}
uint8_t* CPU::ZFlag() {
	
	return flagsNames + 1;
}
uint8_t* CPU::NCFlag() {

	return flagsNames +2;
}
uint8_t* CPU::CFlag() {

	return flagsNames + 3;
}
uint8_t* CPU::getRST00() {

	return RSTOffsets;
}
uint8_t* CPU::getRST08() {

	return RSTOffsets + 1;
}
uint8_t* CPU::getRST10() {

	return RSTOffsets + 2;
}
uint8_t* CPU::getRST18() {

	return RSTOffsets + 3;
}
uint8_t* CPU::getRST20() {

	return RSTOffsets + 4;
}
uint8_t* CPU::getRST28() {

	return RSTOffsets + 5;
}
uint8_t* CPU::getRST30() {

	return RSTOffsets + 6;
}
uint8_t* CPU::getRST38() {

	return RSTOffsets + 7;
}

void CPU::setFlags(uint16_t r,uint16_t n)
{
	/*Word word = WordToBytes(r);
	if (word.lsb == 0xff) {
		setFlag('H');
		if (word.msb == 0xff)
			setFlag('C');
	}*/

}
uint16_t CPU::BytesToWord(uint8_t msb, uint8_t lsb) {
	uint16_t n = (msb << 8) | lsb;
	return n;
}
Word CPU::WordToBytes(uint16_t word) {
	uint8_t msb = 0xff && (word >> 8);
	uint8_t lsb = 0xff && word;
	return Word{msb,lsb};
}
Byte CPU::getByte(uint8_t byte) {
	uint8_t msn = 0xf && (byte >> 4);
	uint8_t lsn = 0xf && byte;
	return Byte{ msn,lsn };
}
uint8_t CPU::getByte(Byte byte) {
	return byte.msn << 4 | byte.msn;
}
void CPU::SetWordIntoBytes(uint16_t* a, uint16_t* b) {
	Word word = WordToBytes(*a);
	
}
void CPU::SetByte(uint16_t* word, uint8_t value) {
	uint8_t* b = (uint8_t*)word;
	*b = value;

}
void CPU::Execute(uint16_t opcode)
{

	Opcode  op = opcodes[opcode];
	uint8_t n = 19;
	//opcodes[opcode]
	uint8_t* param1 = (this->*op.param1)();
	uint8_t* param2 = (this->*op.param2)();
	(this->*op.operate)(param1, param2);
	//(this->*opcodes[opcode].operate)(op.arg1, &n);
	//op.operate(op.arg1, &n);
	lastOpcodeCycles = op.cycles;
	time += lastOpcodeCycles;
	/*if (op.name != "NOP")
		printf("pc:%d opcode:%d func:%s\n", PC, opcode, op.name);*/
}
void CPU::reset()
{
	AF = 0x01;//-GB/SGB, $FF-GBP, $11-GBC
	//F = 0xB0;?
	BC = 0x0013;
	DE = 0x00D8;
	HL = 0x014D;
	SP=0xFFFE;
	PC = 0x100;
	//PC = 0;
	AF = 0;
	BC = 0;
	DE = 0;
	HL = 0;
	//SP = 0;
}

void CPU::connectToBus(BUS* bus)
{
	this->bus = bus;
}
