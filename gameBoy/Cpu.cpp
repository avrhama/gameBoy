#include "Cpu.h"
Cpu::Cpu()
{

	//1. LD nn,n
	opcodes[0x06] = { "LD",&Cpu::LD_nn_n,&Cpu::getB,NULL,8 };
	//opcodes[0x06] = { "LD",&Cpu::LD_nn_n,(uint16_t*)&this->b,NULL,8 };
	//opcodes[0x0e] = { "LD",&Cpu::LD_nn_n,(uint16_t*)&this->c,NULL,8 };
	//opcodes[0x16] = { "LD",&Cpu::LD_nn_n,(uint16_t*)&this->d,NULL,8 };
	//opcodes[0x1e] = { "LD",&Cpu::LD_nn_n,(uint16_t*)&this->e,NULL,8 };
	//opcodes[0x26] = { "LD",&Cpu::LD_nn_n,(uint16_t*)&this->h,NULL,8 };
	//opcodes[0x2e] = { "LD",&Cpu::LD_nn_n,(uint16_t*)&this->l,NULL,8 };
	////2. LD r1,r2					
	//opcodes[0x7f] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->a,4 };
	//opcodes[0x78] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->b,4 };
	//opcodes[0x79] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->c,4 };
	//opcodes[0x7a] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->d,4 };
	//opcodes[0x7b] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->e,4 };
	//opcodes[0x7c] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->h,4 };
	//opcodes[0x7d] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->l,4 };
	//opcodes[0x7e] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->ram.mem+(this->h<<8|this->l),8 };
	//							
	//opcodes[0x40] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->b,(uint16_t*)&this->b,4 };
	//opcodes[0x41] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->b,(uint16_t*)&this->c,4 };
	//opcodes[0x42] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->b,(uint16_t*)&this->d,4 };
	//opcodes[0x43] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->b,(uint16_t*)&this->e,4 };
	//opcodes[0x44] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->b,(uint16_t*)&this->h,4 };
	//opcodes[0x45] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->b,(uint16_t*)&this->l,4 };
	//opcodes[0x46] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->b,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),8 };
	//							
	//opcodes[0x48] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->c,(uint16_t*)&this->b,4 };
	//opcodes[0x49] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->c,(uint16_t*)&this->c,4 };
	//opcodes[0x4a] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->c,(uint16_t*)&this->d,4 };
	//opcodes[0x4b] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->c,(uint16_t*)&this->e,4 };
	//opcodes[0x4c] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->c,(uint16_t*)&this->h,4 };
	//opcodes[0x4d] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->c,(uint16_t*)&this->l,4 };
	//opcodes[0x4e] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->c,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),8 };
	//							
	//opcodes[0x52] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->d,(uint16_t*)&this->d,4 };
	//opcodes[0x53] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->d,(uint16_t*)&this->e,4 };
	//opcodes[0x54] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->d,(uint16_t*)&this->h,4 };
	//opcodes[0x55] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->d,(uint16_t*)&this->l,4 };
	//opcodes[0x56] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->d,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),8 };
	//							
	//opcodes[0x58] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->e,(uint16_t*)&this->b,4 };
	//opcodes[0x59] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->e,(uint16_t*)&this->c,4 };
	//opcodes[0x5a] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->e,(uint16_t*)&this->d,4 };
	//opcodes[0x5b] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->e,(uint16_t*)&this->e,4 };
	//opcodes[0x5c] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->e,(uint16_t*)&this->h,4 };
	//opcodes[0x5d] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->e,(uint16_t*)&this->l,4 };
	//opcodes[0x5e] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->e,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),8 };
	//							
	//opcodes[0x60] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->h,(uint16_t*)&this->b,4 };
	//opcodes[0x61] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->h,(uint16_t*)&this->c,4 };
	//opcodes[0x62] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->h,(uint16_t*)&this->d,4 };
	//opcodes[0x63] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->h,(uint16_t*)&this->e,4 };
	//opcodes[0x64] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->h,(uint16_t*)&this->h,4 };
	//opcodes[0x65] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->h,(uint16_t*)&this->l,4 };
	//opcodes[0x66] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->h,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),8 };
	//							
	//opcodes[0x68] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->l,(uint16_t*)&this->b,4 };
	//opcodes[0x69] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->l,(uint16_t*)&this->c,4 };
	//opcodes[0x6a] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->l,(uint16_t*)&this->d,4 };
	//opcodes[0x6b] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->l,(uint16_t*)&this->e,4 };
	//opcodes[0x6c] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->l,(uint16_t*)&this->h,4 };
	//opcodes[0x6d] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->l,(uint16_t*)&this->l,4 };
	//opcodes[0x6e] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->l,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),8 };
	//							
	//opcodes[0x70] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),(uint16_t*)&this->b,8 };
	//opcodes[0x71] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),(uint16_t*)&this->c,8 };
	//opcodes[0x72] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),(uint16_t*)&this->d,8 };
	//opcodes[0x73] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),(uint16_t*)&this->e,8 };
	//opcodes[0x74] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),(uint16_t*)&this->h,8 };
	//opcodes[0x75] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),(uint16_t*)&this->l,8 };
	//opcodes[0x36] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->ram.mem + (this->h << 8 | this->l),NULL,12 };
	//
	////3. LD A,n
	//opcodes[0x7e] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->ram.mem + (this->b << 8 | this->c),8 };
	//opcodes[0x7e] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->ram.mem + (this->d << 8 | this->e),8 };
	//opcodes[0x7e] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,NULL,16 };
	//opcodes[0x7d] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,NULL,8 };

	////4. LD A,n
	//opcodes[0x47] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->h,(uint16_t*)&this->b,4 };
	//opcodes[0x7e] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,(uint16_t*)&this->ram.mem + (this->d << 8 | this->e),8 };
	//opcodes[0x7e] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,NULL,16 };
	//opcodes[0x7d] = { "LD",&Cpu::LD_r1_r2,(uint16_t*)&this->a,NULL,8 };



	//LD_nn_n
	opcodes[0x06] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getB,&Cpu::getN,8 };
	opcodes[0x0E] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getC,&Cpu::getN,8 };
	opcodes[0x16] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getD,&Cpu::getN,8 };
	opcodes[0x1E] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getE,&Cpu::getN,8 };
	opcodes[0x26] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getH,&Cpu::getN,8 };
	opcodes[0x2E] = { "LD_nn_n",&Cpu::LD_nn_n,&Cpu::getL,&Cpu::getN,8 };
	//LD_r1_r2
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
	//LD_A_n
	opcodes[0x0A] = { "LD_A_n",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::get$BC,8 };
	opcodes[0x1A] = { "LD_A_n",&Cpu::LD_r1_r2,&Cpu::getA,&Cpu::get$DE,8 };
	opcodes[0xFA] = { "LD_A_n",&Cpu::LD_r1_r2,&Cpu::getA,NULL,16 };
	opcodes[0x3E] = { "LD_A_n",&Cpu::LD_r1_r2,&Cpu::getA,NULL,8 };
	//LD_n_A
	opcodes[0x47] = { "LD_n_A",&Cpu::LD_r1_r2,&Cpu::getB,&Cpu::getA,4 };
	opcodes[0x4F] = { "LD_n_A",&Cpu::LD_r1_r2,&Cpu::getC,&Cpu::getA,4 };
	opcodes[0x57] = { "LD_n_A",&Cpu::LD_r1_r2,&Cpu::getD,&Cpu::getA,4 };
	opcodes[0x5F] = { "LD_n_A",&Cpu::LD_r1_r2,&Cpu::getE,&Cpu::getA,4 };
	opcodes[0x67] = { "LD_n_A",&Cpu::LD_r1_r2,&Cpu::getH,&Cpu::getA,4 };
	opcodes[0x6F] = { "LD_n_A",&Cpu::LD_r1_r2,&Cpu::getL,&Cpu::getA,4 };
	opcodes[0x02] = { "LD_n_A",&Cpu::LD_r1_r2,&Cpu::get$BC,&Cpu::getA,8 };
	opcodes[0x12] = { "LD_n_A",&Cpu::LD_r1_r2,&Cpu::get$DE,&Cpu::getA,8 };
	opcodes[0x77] = { "LD_n_A",&Cpu::LD_r1_r2,&Cpu::get$HL,&Cpu::getA,8 };
	opcodes[0xEA] = { "LD_n_A",&Cpu::LD_r1_r2,NULL,&Cpu::getA,16 };
	//LD_A_C
	opcodes[0xF2] = { "LD A,C",&Cpu::LD_A_C,NULL,NULL,8 };
	//LD_C_A
	opcodes[0xF2] = { "LD C,A",&Cpu::LD_C_A,NULL,NULL,8 };
	//LD_A_HLD
	//LD_A_HLminus
	//LDD_A_HL
	opcodes[0x3A] = { "LDD A,(HL)",&Cpu::LDD_A_$HL,&Cpu::getA,&Cpu::get$HL,8 };
	//LD_HLD_A
	//LD_HLminus_A
	//LDD_HL_A
	opcodes[0x32] = { "LDD_(HL)_A",&Cpu::LDD_$HL_A,&Cpu::get$HL,&Cpu::getA,8 };
	//LD_A_HLI
	//LD_A_HLplus
	//LDI_A_HL
	opcodes[0x2A] = { "LDI_A_(HL)",&Cpu::LDI_A_$HL,&Cpu::getA,&Cpu::get$HL,8 };
	//LD_HLI_A
	//LD_HLplus_A
	//LDI_HL_A
	opcodes[0x22] = { "LDI_HL_A",&Cpu::LDI_$HL_A,&Cpu::get$HL,&Cpu::getA,8 };
	//LDH_n_A
	//LDH_A_n
	//LD_n_nn
	opcodes[0x01] = { "LD_n_nn",&Cpu::LD_n_nn,&Cpu::getBC,NULL,12 };
	opcodes[0x11] = { "LD_n_nn",&Cpu::LD_n_nn,&Cpu::getDE,NULL,12 };
	opcodes[0x21] = { "LD_n_nn",&Cpu::LD_n_nn,&Cpu::getHL,NULL,12 };
	/*opcodes[0x31] = { "LD_n_nn",&Cpu::LD_n_nn,&this->sp,NULL,12 };
	//LD_SP_HL
	opcodes[0xF9] = { "LD_SP_HL",&Cpu::LD_SP_HL,&this->sp,&Cpu::getHL,8 };
	//LDHL SP_n
	opcodes[0xF8] = { "LDHL SP_n",&Cpu::LDHL SP_n,&this->sp,NULL,12 };
	//LD_nn_SP
	opcodes[0x08] = { "LD_nn_SP",&Cpu::LD_nn_SP,NULL,&this->sp,20 };
	//PUSH_nn_n
	opcodes[0xF5] = { "PUSH_nn_n",&Cpu::PUSH_nn_n,&Cpu::getAF,NULL,16 };
	opcodes[0xC5] = { "PUSH_nn_n",&Cpu::PUSH_nn_n,&Cpu::getBC,NULL,16 };
	opcodes[0xD5] = { "PUSH_nn_n",&Cpu::PUSH_nn_n,&Cpu::getDE,NULL,16 };
	opcodes[0xE5] = { "PUSH_nn_n",&Cpu::PUSH_nn_n,&Cpu::getHL,NULL,16 };
	//POP_nn_n
	opcodes[0xF1] = { "POP_nn_n",&Cpu::POP_nn_n,&Cpu::getAF,NULL,12 };
	opcodes[0xC1] = { "POP_nn_n",&Cpu::POP_nn_n,&Cpu::getBC,NULL,12 };
	opcodes[0xD1] = { "POP_nn_n",&Cpu::POP_nn_n,&Cpu::getDE,NULL,12 };
	opcodes[0xE1] = { "POP_nn_n",&Cpu::POP_nn_n,&Cpu::getHL,NULL,12 };
	//ADD_A_n
	opcodes[0x87] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x80] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x81] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x82] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x83] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x84] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x85] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x86] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xC6] = { "ADD_A_n",&Cpu::ADD_A_n,&Cpu::getA,NULL,8 };
	//ADC_A_n
	opcodes[0x8F] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x88] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x89] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x8A] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x8B] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x8C] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x8D] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x8E] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,&Cpu::get$HL,8 };
	opcodes[0xCE] = { "ADC_A_n",&Cpu::ADC_A_n,&Cpu::getA,NULL,8 };
	//SUB_n_n
	opcodes[0x97] = { "SUB_n_n",&Cpu::SUB_n_n,&Cpu::getA,NULL,4 };
	opcodes[0x90] = { "SUB_n_n",&Cpu::SUB_n_n,&Cpu::getB,NULL,4 };
	opcodes[0x91] = { "SUB_n_n",&Cpu::SUB_n_n,&Cpu::getC,NULL,4 };
	opcodes[0x92] = { "SUB_n_n",&Cpu::SUB_n_n,&Cpu::getD,NULL,4 };
	opcodes[0x93] = { "SUB_n_n",&Cpu::SUB_n_n,&Cpu::getE,NULL,4 };
	opcodes[0x94] = { "SUB_n_n",&Cpu::SUB_n_n,&Cpu::getH,NULL,4 };
	opcodes[0x95] = { "SUB_n_n",&Cpu::SUB_n_n,&Cpu::getL,NULL,4 };
	opcodes[0x96] = { "SUB_n_n",&Cpu::SUB_n_n,&Cpu::get$HL,NULL,8 };
	opcodes[0xD6] = { "SUB_n_n",&Cpu::SUB_n_n,NULL,NULL,8 };
	//SBC_A_n
	opcodes[0x9F] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getA,4 };
	opcodes[0x98] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getB,4 };
	opcodes[0x99] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getC,4 };
	opcodes[0x9A] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getD,4 };
	opcodes[0x9B] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getE,4 };
	opcodes[0x9C] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getH,4 };
	opcodes[0x9D] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::getL,4 };
	opcodes[0x9E] = { "SBC_A_n",&Cpu::SBC_A_n,&Cpu::getA,&Cpu::get$HL,8 };
	//AND_n_n
	opcodes[0xA7] = { "AND_n_n",&Cpu::AND_n_n,&Cpu::getA,NULL,4 };
	opcodes[0xA0] = { "AND_n_n",&Cpu::AND_n_n,&Cpu::getB,NULL,4 };
	opcodes[0xA1] = { "AND_n_n",&Cpu::AND_n_n,&Cpu::getC,NULL,4 };
	opcodes[0xA2] = { "AND_n_n",&Cpu::AND_n_n,&Cpu::getD,NULL,4 };
	opcodes[0xA3] = { "AND_n_n",&Cpu::AND_n_n,&Cpu::getE,NULL,4 };
	opcodes[0xA4] = { "AND_n_n",&Cpu::AND_n_n,&Cpu::getH,NULL,4 };
	opcodes[0xA5] = { "AND_n_n",&Cpu::AND_n_n,&Cpu::getL,NULL,4 };
	opcodes[0xA6] = { "AND_n_n",&Cpu::AND_n_n,&Cpu::get$HL,NULL,8 };
	opcodes[0xE6] = { "AND_n_n",&Cpu::AND_n_n,NULL,NULL,8 };
	//OR_n_n
	opcodes[0xB7] = { "OR_n_n",&Cpu::OR_n_n,&Cpu::getA,NULL,4 };
	opcodes[0xB0] = { "OR_n_n",&Cpu::OR_n_n,&Cpu::getB,NULL,4 };
	opcodes[0xB1] = { "OR_n_n",&Cpu::OR_n_n,&Cpu::getC,NULL,4 };
	opcodes[0xB2] = { "OR_n_n",&Cpu::OR_n_n,&Cpu::getD,NULL,4 };
	opcodes[0xB3] = { "OR_n_n",&Cpu::OR_n_n,&Cpu::getE,NULL,4 };
	opcodes[0xB4] = { "OR_n_n",&Cpu::OR_n_n,&Cpu::getH,NULL,4 };
	opcodes[0xB5] = { "OR_n_n",&Cpu::OR_n_n,&Cpu::getL,NULL,4 };
	opcodes[0xB6] = { "OR_n_n",&Cpu::OR_n_n,&Cpu::get$HL,NULL,8 };
	opcodes[0xF6] = { "OR_n_n",&Cpu::OR_n_n,NULL,NULL,8 };
	//XOR_n_n
	opcodes[0xAF] = { "XOR_n_n",&Cpu::XOR_n_n,&Cpu::getA,NULL,4 };
	opcodes[0xA8] = { "XOR_n_n",&Cpu::XOR_n_n,&Cpu::getB,NULL,4 };
	opcodes[0xA9] = { "XOR_n_n",&Cpu::XOR_n_n,&Cpu::getC,NULL,4 };
	opcodes[0xAA] = { "XOR_n_n",&Cpu::XOR_n_n,&Cpu::getD,NULL,4 };
	opcodes[0xAB] = { "XOR_n_n",&Cpu::XOR_n_n,&Cpu::getE,NULL,4 };
	opcodes[0xAC] = { "XOR_n_n",&Cpu::XOR_n_n,&Cpu::getH,NULL,4 };
	opcodes[0xAD] = { "XOR_n_n",&Cpu::XOR_n_n,&Cpu::getL,NULL,4 };
	opcodes[0xAE] = { "XOR_n_n",&Cpu::XOR_n_n,&Cpu::get$HL,NULL,8 };
	opcodes[0xEE] = { "XOR_n_n",&Cpu::XOR_n_n,NULL,NULL,8 };
	//CP_n_n
	opcodes[0xBF] = { "CP_n_n",&Cpu::CP_n_n,&Cpu::getA,NULL,4 };
	opcodes[0xB8] = { "CP_n_n",&Cpu::CP_n_n,&Cpu::getB,NULL,4 };
	opcodes[0xB9] = { "CP_n_n",&Cpu::CP_n_n,&Cpu::getC,NULL,4 };
	opcodes[0xBA] = { "CP_n_n",&Cpu::CP_n_n,&Cpu::getD,NULL,4 };
	opcodes[0xBB] = { "CP_n_n",&Cpu::CP_n_n,&Cpu::getE,NULL,4 };
	opcodes[0xBC] = { "CP_n_n",&Cpu::CP_n_n,&Cpu::getH,NULL,4 };
	opcodes[0xBD] = { "CP_n_n",&Cpu::CP_n_n,&Cpu::getL,NULL,4 };
	opcodes[0xBE] = { "CP_n_n",&Cpu::CP_n_n,&Cpu::get$HL,NULL,8 };
	opcodes[0xFE] = { "CP_n_n",&Cpu::CP_n_n,NULL,NULL,8 };
	//INC_n_n
	opcodes[0x3C] = { "INC_n_n",&Cpu::INC_n_n,&Cpu::getA,NULL,4 };
	opcodes[0x04] = { "INC_n_n",&Cpu::INC_n_n,&Cpu::getB,NULL,4 };
	opcodes[0x0C] = { "INC_n_n",&Cpu::INC_n_n,&Cpu::getC,NULL,4 };
	opcodes[0x14] = { "INC_n_n",&Cpu::INC_n_n,&Cpu::getD,NULL,4 };
	opcodes[0x1C] = { "INC_n_n",&Cpu::INC_n_n,&Cpu::getE,NULL,4 };
	opcodes[0x24] = { "INC_n_n",&Cpu::INC_n_n,&Cpu::getH,NULL,4 };
	opcodes[0x2C] = { "INC_n_n",&Cpu::INC_n_n,&Cpu::getL,NULL,4 };
	opcodes[0x34] = { "INC_n_n",&Cpu::INC_n_n,&Cpu::get$HL,NULL,12 };
	//DEC_n_n
	opcodes[0x3D] = { "DEC_n_n",&Cpu::DEC_n_n,&Cpu::getA,NULL,4 };
	opcodes[0x05] = { "DEC_n_n",&Cpu::DEC_n_n,&Cpu::getB,NULL,4 };
	opcodes[0x0D] = { "DEC_n_n",&Cpu::DEC_n_n,&Cpu::getC,NULL,4 };
	opcodes[0x15] = { "DEC_n_n",&Cpu::DEC_n_n,&Cpu::getD,NULL,4 };
	opcodes[0x1D] = { "DEC_n_n",&Cpu::DEC_n_n,&Cpu::getE,NULL,4 };
	opcodes[0x25] = { "DEC_n_n",&Cpu::DEC_n_n,&Cpu::getH,NULL,4 };
	opcodes[0x2D] = { "DEC_n_n",&Cpu::DEC_n_n,&Cpu::getL,NULL,4 };
	opcodes[0x35] = { "DEC_n_n",&Cpu::DEC_n_n,&Cpu::get$HL,NULL,12 };
	//ADD_HL_n
	opcodes[0x09] = { "ADD_HL_n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getBC,8 };
	opcodes[0x19] = { "ADD_HL_n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getDE,8 };
	opcodes[0x29] = { "ADD_HL_n",&Cpu::ADD_HL_n,&Cpu::getHL,&Cpu::getHL,8 };
	opcodes[0x39] = { "ADD_HL_n",&Cpu::ADD_HL_n,&Cpu::getHL,&this->sp,8 };
	//ADD SP,n
	opcodes[0xE8] = { "ADD SP,n",&Cpu::ADD SP,n,&this->sp,NULL,16 };
	//INC_nn_n
	opcodes[0x03] = { "INC_nn_n",&Cpu::INC_nn_n,&Cpu::getBC,NULL,8 };
	opcodes[0x13] = { "INC_nn_n",&Cpu::INC_nn_n,&Cpu::getDE,NULL,8 };
	opcodes[0x23] = { "INC_nn_n",&Cpu::INC_nn_n,&Cpu::getHL,NULL,8 };
	opcodes[0x33] = { "INC_nn_n",&Cpu::INC_nn_n,&this->sp,NULL,8 };
	//DEC_nn_n
	opcodes[0x0B] = { "DEC_nn_n",&Cpu::DEC_nn_n,&Cpu::getBC,NULL,8 };
	opcodes[0x1B] = { "DEC_nn_n",&Cpu::DEC_nn_n,&Cpu::getDE,NULL,8 };
	opcodes[0x2B] = { "DEC_nn_n",&Cpu::DEC_nn_n,&Cpu::getHL,NULL,8 };
	opcodes[0x3B] = { "DEC_nn_n",&Cpu::DEC_nn_n,&this->sp,NULL,8 };
	//SWAP_n_n
	opcodes[0xCB] = { "SWAP_n_n",&Cpu::SWAP_n_n,&Cpu::getA,NULL,37 };
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
	opcodes[0xC0] = { "RET_cc_n",&Cpu::RET_cc_n,&Cpu::getF,NULL,8 };
	opcodes[0xC8] = { "RET_cc_n",&Cpu::RET_cc_n,&Cpu::getF,NULL,8 };
	opcodes[0xD0] = { "RET_cc_n",&Cpu::RET_cc_n,&Cpu::getF,NULL,8 };
	opcodes[0xD8] = { "RET_cc_n",&Cpu::RET_cc_n,&Cpu::getC,NULL,8 };
	//RETI_n_n
	opcodes[0xD9] = { "RETI_n_n",&Cpu::RETI_n_n,NULL,NULL,8 };8*/



}
uint16_t* Cpu::getA()
{
	return (uint16_t*)((char*)&AF + 1);

}
uint16_t* Cpu::getF()
{
	return &AF;
}
uint16_t* Cpu::getB()
{
	return (uint16_t*)((char*)&BC + 1);
}
uint16_t* Cpu::getC()
{
	return &BC;
}
uint16_t* Cpu::getD()
{
	return (uint16_t*)((char*)&DE + 1);
}
uint16_t* Cpu::getE()
{
	return &DE;
}

uint16_t* Cpu::getH()
{
	return (uint16_t*)((char*)&HL + 1);
}
uint16_t* Cpu::getL()
{
	return &HL;
}
uint16_t* Cpu::getAF()
{
	return &AF;
}
uint16_t* Cpu::getHL()
{
	return &HL;
}
uint16_t* Cpu::getDE()
{
	return &DE;
}
uint16_t* Cpu::get$DE()
{
	return (uint16_t*)&ram.mem+ DE;
}
uint16_t* Cpu::get$HL()
{
	return (uint16_t*)&ram.mem + HL;
}
uint16_t* Cpu::getN()
{
	return (uint16_t*)&ram.mem + (pc++);
}
uint16_t* Cpu::getBC()
{
	return &BC;
}
uint16_t* Cpu::get$BC()
{
	return (uint16_t*)&ram.mem + BC;
}
void Cpu::Execute(uint16_t opcode)
{
	Opcode  op = opcodes[opcode];
	uint16_t n = 19;
	//opcodes[opcode]
	uint16_t* param1= (this->*op.param1)();
	(this->*op.operate)(param1, &n);
	//(this->*opcodes[opcode].operate)(op.arg1, &n);
	//op.operate(op.arg1, &n);
}
void Cpu::LD(uint16_t * nn, uint16_t* n)
{
	*((uint8_t*)n) == *nn;
	printf("welcome to LD!");
}
void Cpu::LD_nn_n(uint16_t* nn, uint16_t* n)
{
	*((uint8_t*)n) == *nn;
}
void Cpu::LD_r1_r2(uint16_t* r1, uint16_t* r2)
{
	*((uint8_t*)r1) == *r2;
}

void Cpu::LD_A_C(uint16_t* A, uint16_t* C)
{
	SetByte(getA(),ram.read(0xff00 + *getC()));
}
void Cpu::LD_C_A(uint16_t* C, uint16_t* A)
{
	ram.write(0xff00 + this->c, this->a);
}

void Cpu::LDD_A_$HL(uint16_t* A, uint16_t* $HL)
{
	uint16_t hl = *this->get$HL();
	a = ram.read(hl);
	hl--;
	Word word = WordToBytes(hl);
	h = word.msb;
	l = word.lsb;
}
void Cpu::LDD_$HL_A(uint16_t* $HL,uint16_t* A)
{
	uint16_t hl = *this->get$HL();
	ram.write(hl,a);
	hl--;
	Word word = WordToBytes(hl);
	h = word.msb;
	l = word.lsb;
}
void Cpu::LDI_A_$HL(uint16_t* A, uint16_t* $HL)
{
	uint16_t hl = *this->get$HL();
	a = ram.read(hl);
	hl++;
	Word word = WordToBytes(hl);
	h = word.msb;
	l = word.lsb;
}
void Cpu::LDI_$HL_A(uint16_t* $HL, uint16_t* A)
{
	uint16_t hl = *this->get$HL();
	ram.write(hl, a);
	hl++;
	Word word = WordToBytes(hl);
	h = word.msb;
	l = word.lsb;
}
void Cpu::LD_n_nn(uint16_t* n, uint16_t* nn)
{
	*n = *nn;
	

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
void Cpu::SetWordIntoBytes(uint16_t* a, uint16_t* b) {
	Word word = WordToBytes(*a);
	
}
void Cpu::SetByte(uint16_t* word, uint8_t value) {
	uint8_t* b = (uint8_t*)word;
	*b = value;

}
