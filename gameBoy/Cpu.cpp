#include "CPU.h"
#include "MMU.h"
#include "pipeChannel.h"
CPU::CPU()
{

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
	immidiateN = bus->mmu->read(BC);
	return &immidiateN;
}
uint8_t* CPU::get$DE()
{
	immidiateN = bus->mmu->read(DE);
	return &immidiateN;
}
uint8_t* CPU::get$HL()
{


	immidiateN = bus->mmu->read(HL);
	return &immidiateN;
}
uint8_t* CPU::getN()
{
	immidiateN = bus->mmu->read(PC++);
	return &immidiateN;
}
uint8_t* CPU::getNN()
{
	uint8_t lsb = bus->mmu->read(PC++);
	uint8_t msb = bus->mmu->read(PC++);
	immidiateNN = msb << 8 | lsb;
	return (uint8_t*)&immidiateNN;
}
uint8_t* CPU::get$NN()
{
	//n=(msb|lsb) write(c,*(uint16*)n)-> memory[c]=lsb memory[c+1]=msb.
	uint8_t lsb = bus->mmu->read(PC++);
	uint8_t msb = bus->mmu->read(PC++);
	immidiateNN = bus->mmu->read(msb << 8 | lsb);
	return (uint8_t*)&immidiateNN;
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
	/*if (r1 == getA())
		AF &= 0xfff0;*/
}
void CPU::LD_n_$HL(uint8_t* none, uint8_t* r2)
{
	bus->mmu->write(HL, *r2);
}
void CPU::LD_A_n(uint8_t* A, uint8_t* n)
{
	*A = *n;
}
void CPU::LD_n_A(uint8_t* n, uint8_t* A)
{
	*n = *A;
}
void CPU::LD_$n_A(uint8_t* n, uint8_t* A)
{
	bus->mmu->write(*(uint16_t*)n, *A);
}
void CPU::LD_A_C(uint8_t* A, uint8_t* C)
{
	//?
	*A = bus->mmu->read(0xff00 + *C);
	//AF &= 0xfff0;
}
void CPU::LD_C_A(uint8_t* C, uint8_t* A)
{
	//?
	bus->mmu->write(0xff00 + *C, *A);
}

void CPU::LDD_A_$HL(uint8_t* A, uint8_t* none)
{
	*A = bus->mmu->read(HL);
	HL--;
	//AF &= 0xfff0;
}
void CPU::LDD_$HL_A(uint8_t* none, uint8_t* A)
{
	bus->mmu->write(HL, *A);
	HL--;
}
void CPU::LDI_A_$HL(uint8_t* A, uint8_t* none)
{
	*A = bus->mmu->read(HL);
	HL++;

	//AF &= 0xfff0;
}
void CPU::LDI_$HL_A(uint8_t* none, uint8_t* A)
{
	bus->mmu->write(HL, *A);
	HL++;
}
void CPU::LDH_n_A(uint8_t* N, uint8_t* A) {
	bus->mmu->write(0xff00 + *N, *A);
}
void CPU::LDH_A_n(uint8_t* A, uint8_t* N) {
	if (*N == 0x91) {
		int stop = 0;
	}
	*A = bus->mmu->read(0xff00 + *N);
	//AF &= 0xfff0;
}
void CPU::LD_n_nn(uint8_t* n, uint8_t* nn)
{
	*(uint16_t*)n = *(uint16_t*)nn;

}
void CPU::LD_SP_HL(uint8_t* none1, uint8_t* none2)
{
	SP = HL;
}
void CPU::LDHL_SP_$n(uint8_t* none1, uint8_t* none2)
{
	signed char N_ = bus->mmu->read(PC++);
	HL = SP + N_;

	uint16_t temp = HL ^ N_ ^ SP;
	flagsArray[Z] = 0;
	flagsArray[N] = 0;
	flagsArray[C] = ((temp & 0x100) == 0x100) ? 1 : 0;
	flagsArray[H] = ((temp & 0x10) == 0x10) ? 1 : 0;
	updateFlags();
	return;
	//resetFlag('Z');
	//resetFlag('N');
	//if (SP + N_ >= 0) {
	//	if (SP + N_ > 0xffff)
	//		//setFlag('C');
	//		flagsArray[C] = 1;
	//	else
	//		//resetFlag('C');
	//		flagsArray[C] = 0;
	//	if ((SP & 0xF) + (N_ & 0xF) > 0xF)
	//		//setFlag('H');
	//		flagsArray[H] = 1;
	//	else
	//		//resetFlag('H');
	//		flagsArray[H] = 0;
	//}
	//else {
	//	if (SP < N_)
	//		//setFlag('C');
	//		flagsArray[C] = 1;
	//	else
	//		//resetFlag('C');
	//		flagsArray[C] = 0;
	//	if ((SP & 0xF) < (N_ & 0xF))
	//		//setFlag('H');
	//		flagsArray[H] = 1;
	//	else
	//		//resetFlag('H');
	//		flagsArray[H] = 0;
	//}
	if (N_ >= 0) {
		if (SP + N_ > 0xffff)
			//setFlag('C');
			flagsArray[C] = 1;
		else
			//resetFlag('C');
			flagsArray[C] = 0;
		if ((SP & 0xF) + (N_ & 0xF) > 0xF)
			//setFlag('H');
			flagsArray[H] = 1;
		else
			//resetFlag('H');
			flagsArray[H] = 0;
	}
	else {
		if (SP < N_)
			//setFlag('C');
			flagsArray[C] = 1;
		else
			//resetFlag('C');
			flagsArray[C] = 0;
		if ((SP & 0xF) < (N_ & 0xF))
			//setFlag('H');
			flagsArray[H] = 1;
		else
			//resetFlag('H');
			flagsArray[H] = 0;
	}
	updateFlags();

}
void CPU::LD_$NN_SP(uint8_t* $NN, uint8_t* none)
{
	Word word = WordToBytes(SP);
	bus->mmu->write(*(uint16_t*)$NN, word.lsb);
	bus->mmu->write(*(uint16_t*)$NN + 1, word.msb);

}

void CPU::PUSH_nn(uint8_t* nn, uint8_t* none)
{
	uint16_t n = *(uint16_t*)nn;

	Word word = WordToBytes(n);
	/*bus->mmu->write(SP--, word.msb);
	bus->mmu->write(SP--, word.lsb);
	bus->mmu->write(SP, word.msb);
	bus->mmu->write(SP - 1, word.lsb);
	SP -= 2;*/
	/*SP--;
	bus->mmu->write(SP, word.msb);
	SP--;

	bus->mmu->write(SP, word.lsb);*/

	/*bus->mmu->write(SP, word.lsb);
	bus->mmu->write(SP - 1, word.msb);
	SP -= 2;*/

	bus->mmu->write(SP - 1, word.msb);
	bus->mmu->write(SP - 2, word.lsb);
	SP -= 2;


}

void CPU::POP_nn(uint8_t* none, uint8_t* nn)
{


	uint8_t lsb = bus->mmu->read(SP);
	uint8_t msb = bus->mmu->read(SP + 1);
	SP += 2;

	/*uint8_t lsb = bus->mmu->read(SP + 1);
	uint8_t msb = bus->mmu->read(SP + 2);
	SP += 2;*/
	//uint8_t lsb = bus->mmu->read(SP);
	//uint8_t msb = bus->mmu->read(SP+1);
	//SP += 2;
	/*uint8_t lsb = bus->mmu->read(SP+1);
	uint8_t msb = bus->mmu->read(SP + 2);
	SP += 2;
	//uint8_t lsb = bus->mmu->read(++SP);
	//uint8_t msb = bus->mmu->read(++SP);*/
	uint16_t word = BytesToWord(msb, lsb);
	*(uint16_t*)nn = word;

}

void CPU::ADD_A_n(uint8_t* A, uint8_t* n_)
{
	Byte byteR = getByte(*A);
	uint8_t n = *n_;
	uint8_t oldA = *A;
	if (n_ == A)
		int y = 0;
	*A += n;

	//resetFlag('N');

	flagsArray[N] = 0;
	if (*A == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	if (oldA + n > 0xff)
		//setFlag('C');

		flagsArray[C] = 1;
	else
		//resetFlag('C');

		flagsArray[C] = 0;

	if (byteR.lsn + (n & 0xf) > 0xf)
		//setFlag('H');

		flagsArray[H] = 1;
	else
		//resetFlag('H');
		flagsArray[H] = 0;
	updateFlags();

	//AF &= 0xfff0;
}

void CPU::ADC_A_n(uint8_t* A, uint8_t* n_)
{

	/**A += n + getFlag('C');

	resetFlag('N');
	if (*A == 0)
		setFlag('Z');
	else
		resetFlag('Z');


	if ((oldA & 0xF) + (n & 0xF) + getFlag('C') > 0xF)
		setFlag('H');
	else
		resetFlag('H');
	if (*A > 0xFF)
		setFlag('C');
	else
		resetFlag('C');*/



		/*BYTE before = *A;
		BYTE adding = *n+getFlag('C');


		*A += adding;


		resetFlag('Z');
		resetFlag('N');
		resetFlag('C');
		resetFlag('H');
		if (*A == 0)
			setFlag('Z');

		WORD htest = (before & 0xF);
		htest += (adding & 0xF);

		if (htest > 0xF)
			setFlag('H');

		if ((before + adding) > 0xFF)
			setFlag('C');*/
	if (n_ == getC()) {
		int d = 4;
	}


	uint8_t n = *n_;
	Byte byteR = getByte(*A);
	uint8_t oldA = *A;
	*A += n + getFlag('C');

	//resetFlag('N');

	flagsArray[N] = 0;
	if (*A == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	if (oldA + n + getFlag('C') > 0xff)
		//setFlag('C');

		flagsArray[C] = 1;
	else
		//resetFlag('C');

		flagsArray[C] = 0;
	if ((byteR.lsn + (n & 0xf) + getFlag('C')) > 0xf)
		//setFlag('H');

		flagsArray[H] = 1;
	else
		//resetFlag('H');
		flagsArray[H] = 0;
	updateFlags();

	//AF &= 0xfff0;
}
void CPU::SUB_n(uint8_t* A, uint8_t* n_) {
	uint8_t n = *n_;
	Byte byteR = getByte(*A);
	uint8_t oldA = *A;
	*A -= n;

	//setFlag('N');

	flagsArray[N] = 1;
	if (*A == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	if (oldA < n)
		//setFlag('C');

		flagsArray[C] = 1;
	else
		//resetFlag('C');

		flagsArray[C] = 0;
	if (byteR.lsn < (n & 0xf))
		//setFlag('H');

		flagsArray[H] = 1;
	else
		//resetFlag('H');
		flagsArray[H] = 0;
	updateFlags();

	//AF &= 0xfff0;

}
void CPU::SBC_A_n(uint8_t* A, uint8_t* n_) {
	uint8_t n = *n_;
	Byte byteR = getByte(*A);
	uint8_t oldA = *A;
	*A -= (n + getFlag('C'));

	//setFlag('N');

	flagsArray[N] = 1;
	if (*A == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	if (oldA < n + getFlag('C'))
		//setFlag('C');

		flagsArray[C] = 1;
	else
		//resetFlag('C');

		flagsArray[C] = 0;
	if (byteR.lsn < (n & 0xf) + getFlag('C'))
		//setFlag('H');

		flagsArray[H] = 1;
	else
		//resetFlag('H');
		flagsArray[H] = 0;
	updateFlags();

	//AF &= 0xfff0;
}
void CPU::AND_n(uint8_t* A, uint8_t* n)
{
	Byte byteR = getByte(*A);
	*A &= *n;

	//resetFlag('N');

	flagsArray[N] = 0;
	//setFlag('H');

	flagsArray[H] = 1;
	//resetFlag('C');

	flagsArray[C] = 0;
	if (*A == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');
		flagsArray[Z] = 0;
	updateFlags();

	//AF &= 0xfff0;
}
void CPU::OR_n(uint8_t* A, uint8_t* n)
{
	Byte byteR = getByte(*A);
	*A |= *n;

	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;
	//resetFlag('C');

	flagsArray[C] = 0;
	if (*A == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');
		flagsArray[Z] = 0;
	updateFlags();

	//AF &= 0xfff0;
}
void CPU::XOR_n(uint8_t* A, uint8_t* n)
{
	//? *?
	Byte byteR = getByte(*A);
	*A ^= *n;

	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;
	//resetFlag('C');

	flagsArray[C] = 0;
	if (*A == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');
		flagsArray[Z] = 0;
	updateFlags();

	//AF &= 0xfff0;
}
void CPU::CP_n(uint8_t* n_, uint8_t* none) {
	uint8_t n = *n_;
	uint8_t A = *getA();
	Byte byteR = getByte(A);
	uint8_t res = A - n;

	//setFlag('N');

	flagsArray[N] = 1;
	if (res == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	if (A < n)
		//setFlag('C');

		flagsArray[C] = 1;
	else
		//resetFlag('C');

		flagsArray[C] = 0;
	if (byteR.lsn < (n & 0xf))
		//setFlag('H');

		flagsArray[H] = 1;
	else
		//resetFlag('H');
		flagsArray[H] = 0;
	updateFlags();

	//AF &= 0xfff0;
}
void CPU::INC_n(uint8_t* n, uint8_t* none) {
	Byte byteN = getByte(*n);
	(*n)++;

	//resetFlag('N');

	flagsArray[N] = 0;
	if (*n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	if (byteN.lsn + 1 > 0xf)
		//setFlag('H');

		flagsArray[H] = 1;
	else
		//resetFlag('H');
		flagsArray[H] = 0;
	updateFlags();
	/*if (n == getA())
		AF &= 0xfff0;*/

}
void CPU::INC_$HL(uint8_t* none1, uint8_t* none2) {
	uint8_t n = bus->mmu->read(HL);
	Byte byteN = getByte(n);
	n++;
	bus->mmu->write(HL, n);

	//resetFlag('N');

	flagsArray[N] = 0;
	if (n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;

	if (byteN.lsn + 1 > 0xf)
		//setFlag('H');

		flagsArray[H] = 1;
	else
		//resetFlag('H');

		flagsArray[H] = 0;
	updateFlags();
}
void CPU::DEC_n(uint8_t* n, uint8_t* none) {
	Byte byteN = getByte(*n);
	(*n)--;

	//setFlag('N');

	flagsArray[N] = 1;
	if (*n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	if (byteN.lsn == 0)
		//setFlag('H');

		flagsArray[H] = 1;
	else
		//resetFlag('H');
		flagsArray[H] = 0;
	updateFlags();
	/*if (n == getA())
		AF &= 0xfff0;*/
}
void CPU::DEC_$HL(uint8_t* none1, uint8_t* none2) {
	uint8_t n = bus->mmu->read(HL);
	Byte byteN = getByte(n);
	n--;
	bus->mmu->write(HL, n);

	//setFlag('N');

	flagsArray[N] = 1;
	if (n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	if (byteN.lsn == 0)
		//setFlag('H');

		flagsArray[H] = 1;
	else
		//resetFlag('H');

		flagsArray[H] = 0;
	updateFlags();
}
void CPU::ADD_HL_n(uint8_t* none, uint8_t* n) {

	uint16_t oldHL = HL;
	uint16_t n_ = *(uint16_t*)n;
	HL += n_;

	//resetFlag('N');

	flagsArray[N] = 0;
	if (oldHL + n_ > 0xffff)
		//setFlag('C');

		flagsArray[C] = 1;
	else
		//resetFlag('C');

		flagsArray[C] = 0;
	if ((oldHL & 0xfff) + (n_ & 0xfff) > 0xfff)
		//setFlag('H');

		flagsArray[H] = 1;
	else
		//resetFlag('H');

		flagsArray[H] = 0;
	updateFlags();
}
int lastn = 0;
void CPU::ADD_SP_n(uint8_t* none, uint8_t* n) {
	uint16_t oldSP = SP;
	signed char n_ = *(signed char*)n;

	if (n_ == -1 && lastn == 1) {
		int k = 0;
	}
	lastn = n_;
	SP += n_;
	uint16_t temp = oldSP ^ n_ ^ SP;
	flagsArray[Z] = 0;
	flagsArray[N] = 0;
	flagsArray[C] = ((temp & 0x100) == 0x100) ? 1 : 0;
	flagsArray[H] = ((temp & 0x10) == 0x10) ? 1 : 0;
	updateFlags();
	return;
	if ((oldSP & 0xff) + n_ > 0xff)
		//setFlag('C');
		flagsArray[C] = 1;
	else
		//resetFlag('C');
		flagsArray[C] = 0;
	if ((oldSP & 0xF) + (n_ & 0xF) > 0xF)
		//setFlag('H');
		flagsArray[H] = 1;
	else
		//resetFlag('H');
		flagsArray[H] = 0;

	updateFlags();
	return;

	/*flagsArray[C] = ((temp & 0x100) == 0x100) ? 1 : 0;
	flagsArray[H] = ((temp & 0x10) == 0x10)?1 : 0;
	updateFlags();*/
	/*flagsArray[C] = ((temp & 0x100) == 0x100) ? 1 : 0;
	flagsArray[H] = ((temp & 0x10) == 0x10)?1 : 0;
	updateFlags();
	return;*/
	/*
	//resetFlag('Z');

	flagsArray[Z] = 0;
	//resetFlag('N');

	flagsArray[N] = 0;
	if (oldSP + n_ > 0xFFFF)
		//setFlag('C');

		flagsArray[C] = 1;
	else
		//resetFlag('C');

		flagsArray[C] = 0;
	if ((oldSP & 0xF) + (n_ & 0xF) > 0xF)
		//setFlag('H');
		flagsArray[H] = 1;
	else
		//resetFlag('H');
		flagsArray[H] = 0;
		*/
	flagsArray[Z] = 0;
	flagsArray[N] = 0;
	if (n_ >= 0) {
		if (oldSP & 0xff + n_ > 0xff)
			//setFlag('C');
			flagsArray[C] = 1;
		else
			//resetFlag('C');
			flagsArray[C] = 0;
		if ((oldSP & 0xF) + (n_ & 0xF) > 0xF)
			//setFlag('H');
			flagsArray[H] = 1;
		else
			//resetFlag('H');
			flagsArray[H] = 0;
	}
	else {
		if ((oldSP & 0xff) < n_)
			//setFlag('C');
			flagsArray[C] = 1;
		else
			//resetFlag('C');
			flagsArray[C] = 0;
		if ((oldSP & 0xF) < (n_ & 0xF))
			//setFlag('H');
			flagsArray[H] = 1;
		else
			//resetFlag('H');
			flagsArray[H] = 0;
	}
	updateFlags();
}
void CPU::INC_nn(uint8_t* nn, uint8_t* none) {
	(*(uint16_t*)nn)++;
	/*if ((uint16_t*)nn == &AF)
		AF &= 0xfff0;*/
}
void CPU::DEC_nn(uint8_t* nn, uint8_t* none) {
	(*(uint16_t*)nn)--;
	/*if ((uint16_t*)nn == &AF)
		AF &= 0xfff0;*/
}
void CPU::SWAP_n(uint8_t* n, uint8_t* none) {
	Byte oldByte = getByte(*n);
	Byte newByte{ oldByte.lsn,oldByte.msn };
	*n = getByte(newByte);

	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;
	//resetFlag('C');

	flagsArray[C] = 0;
	if (*n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;

	updateFlags();
	/*if (n == getA())
		AF &= 0xfff0;*/


}
void CPU::SWAP_$HL(uint8_t* none1, uint8_t* none2) {
	uint8_t n = bus->mmu->read(HL);
	Byte oldByte = getByte(n);
	Byte newByte{ oldByte.lsn,oldByte.msn };
	n = getByte(newByte);
	bus->mmu->write(HL, n);

	if (n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;
	//resetFlag('C');

	flagsArray[C] = 0;

	updateFlags();
}
void CPU::DAA(uint8_t* none, uint8_t* none2) {
	uint8_t* A_ = getA();

	if (getFlag('N')) {
		if (getFlag('C'))
			(*A_) -= 0x60;
		if (getFlag('H'))
			(*A_) -= 0x06;
	}
	else {
		if (*A_ > 0x99 || getFlag('C')) {
			(*A_) += 0x60;
			flagsArray[C] = 1;
		}
		if ((*A_ & 0xf) > 0x09 || getFlag('H'))
			(*A_) += 0x06;
	}
	if (*A_ == 0) {
		flagsArray[Z] = 1;
	}
	else {

		flagsArray[Z] = 0;
	}
	flagsArray[H] = 0;

	updateFlags();


}
void CPU::CPL(uint8_t* none, uint8_t* none2) {
	*getA() ^= 0xff;
	flagsArray[N] = 1;
	flagsArray[H] = 1;
	updateFlags();
}
void CPU::CCF(uint8_t* none, uint8_t* none2) {
	getFlag('C') ? flagsArray[C] = 0 : flagsArray[C] = 1;
	flagsArray[N] = 0;
	flagsArray[H] = 0;
	updateFlags();
}
void CPU::SCF(uint8_t* none, uint8_t* none2) {
	flagsArray[C] = 1;
	flagsArray[N] = 0;
	flagsArray[H] = 0;
	updateFlags();
}
void CPU::NOP(uint8_t* none, uint8_t* none2) {
}
void CPU::HALT(uint8_t* none, uint8_t* none2) {
	halt = true;
}
void CPU::STOP(uint8_t* none, uint8_t* none2) {
	halt = true;
	uint8_t prepareSpeedSwitch = bus->interrupt->io[0x4d];
	if (prepareSpeedSwitch & 0x01) {
		speedMode = 1 - speedMode;
		bus->interrupt->io[0x4d] = (speedMode << 7) | 0xfe;
		halt = false;
		lastOpcodeCycles += 32749;
		printf("speed changed");
	}
}
void CPU::DI(uint8_t* none, uint8_t* none2) {
	IME = false;
}
void CPU::EI(uint8_t* none, uint8_t* none2) {
	bus->cpu->setIME = true;
}
void CPU::RLCA(uint8_t* none, uint8_t* none2) {
	uint8_t A = *getA();
	uint8_t bitLeaving = (1 & (A >> 7));
	*getA() = (A << 1) | bitLeaving;
	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	flagsArray[Z] = 0;
	flagsArray[N] = 0;
	flagsArray[H] = 0;
	updateFlags();
}
void CPU::RLA(uint8_t* none, uint8_t* none2) {
	uint8_t A = *getA();
	uint8_t bitLeaving = (1 & (A >> 7));
	*getA() = (A << 1) | getFlag('C');
	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	flagsArray[N] = 0;
	flagsArray[H] = 0;
	flagsArray[Z] = 0;
	updateFlags();
}
void CPU::RRCA(uint8_t* none, uint8_t* none2) {
	uint8_t A = *getA();
	uint8_t bitLeaving = 1 & A;
	*getA() = (bitLeaving << 7) | (A >> 1);
	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	flagsArray[N] = 0;
	flagsArray[H] = 0;
	flagsArray[Z] = 0;
	updateFlags();
}
void CPU::RRA(uint8_t* none, uint8_t* none2) {
	uint8_t A = *getA();
	uint8_t bitLeaving = 1 & A;
	*getA() = (getFlag('C') << 7) | (A >> 1);
	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	flagsArray[Z] = 0;
	flagsArray[N] = 0;
	flagsArray[H] = 0;
	updateFlags();
}
void CPU::RLC_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = (1 & (*n >> 7));
	*n = (*n << 1) | bitLeaving;
	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	flagsArray[N] = 0;
	flagsArray[H] = 0;
	if (*n == 0)
		flagsArray[Z] = 1;
	else
		flagsArray[Z] = 0;
	updateFlags();
}
void CPU::RLC_$HL(uint8_t* none1, uint8_t* none2) {
	uint8_t n = bus->mmu->read(HL);
	uint8_t bitLeaving = (1 & (n >> 7));
	n = (n << 1) | bitLeaving;
	bus->mmu->write(HL, n);
	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	if (n == 0)
		flagsArray[Z] = 1;
	else
		flagsArray[Z] = 0;
	flagsArray[N] = 0;
	flagsArray[H] = 0;
	updateFlags();
}
void CPU::RL_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = (1 & (*n >> 7));
	*n = (*n << 1) | getFlag('C');
	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	flagsArray[N] = 0;
	flagsArray[H] = 0;
	if (*n == 0)
		flagsArray[Z] = 1;
	else
		flagsArray[Z] = 0;
	updateFlags();
}
void CPU::RL_$HL(uint8_t* none1, uint8_t* none2) {
	uint8_t n = bus->mmu->read(HL);
	uint8_t bitLeaving = (1 & (n >> 7));
	n = (n << 1) | getFlag('C');
	bus->mmu->write(HL, n);
	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	flagsArray[N] = 0;
	flagsArray[H] = 0;
	if (n == 0)
		flagsArray[Z] = 1;
	else
		flagsArray[Z] = 0;
	updateFlags();
}
void CPU::RRC_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = 1 & *n;
	*n = (bitLeaving << 7) | (*n >> 1);
	//bitLeaving ? //setFlag('C') : resetFlag('C');

	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;

	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;
	if (*n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;

	updateFlags();
	/*if (n == getA())
		AF &= 0xfff0;*/
}
void CPU::RRC_$HL(uint8_t* none1, uint8_t* none2) {
	uint8_t n = bus->mmu->read(HL);
	uint8_t bitLeaving = 1 & n;
	n = (bitLeaving << 7) | (n >> 1);
	bus->mmu->write(HL, n);
	//bitLeaving ? //setFlag('C') : resetFlag('C');

	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	if (n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');
	flagsArray[H] = 0;
	updateFlags();
}
void CPU::RR_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = 1 & *n;
	*n = (getFlag('C') << 7) | (*n >> 1);
	//bitLeaving ? //setFlag('C') : resetFlag('C');

	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;

	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;

	if (*n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');
		flagsArray[Z] = 0;
	updateFlags();


	/*if (n == getA())
		AF &= 0xfff0;*/
}
void CPU::RR_$HL(uint8_t* none1, uint8_t* none2) {
	uint8_t n = bus->mmu->read(HL);
	uint8_t bitLeaving = 0X01 & n;
	n = (getFlag('C') << 7) | (n >> 1);
	bus->mmu->write(HL, n);
	//bitLeaving ? //setFlag('C') : resetFlag('C');

	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	if (n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');
	flagsArray[H] = 0;
	updateFlags();
}
void CPU::SLA_n(uint8_t* n, uint8_t* none) {

	uint8_t bitLeaving = (1 & (*n >> 7));
	*n <<= 1;
	//bitLeaving ? //setFlag('C') : resetFlag('C');

	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;

	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;
	if (*n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');
		flagsArray[Z] = 0;
	updateFlags();


	//if (n == getA())
	//	AF &= 0xfff0;

}
void CPU::SLA_$HL(uint8_t* none1, uint8_t* none2) {
	uint8_t n = bus->mmu->read(HL);
	uint8_t bitLeaving = (1 & (n >> 7));
	n <<= 1;
	bus->mmu->write(HL, n);
	//bitLeaving ? //setFlag('C') : resetFlag('C');

	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;

	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;
	if (n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');
		flagsArray[Z] = 0;
	updateFlags();


}
void CPU::SRA_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = (0x01 & *n);
	*n >>= 1;
	//bitLeaving ? //setFlag('C') : resetFlag('C');

	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	bitLeaving = (0x01 & (*n >> 6));
	*n |= bitLeaving << 7;

	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;
	////resetFlag('C');//web table

	//flagsArray[C] = 0;//web table
	if (*n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');
		flagsArray[Z] = 0;
	updateFlags();


	/*if (n == getA())
		AF &= 0xfff0;*/
}
void CPU::SRA_$HL(uint8_t* none1, uint8_t* none2) {
	uint8_t n = bus->mmu->read(HL);
	uint8_t bitLeaving = (0x01 & n);
	n >>= 1;
	//bitLeaving ? //setFlag('C') : resetFlag('C');

	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	bitLeaving = (0x01 & (n >> 6));
	n |= bitLeaving << 7;
	bus->mmu->write(HL, n);
	if (n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');

		flagsArray[Z] = 0;
	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');
	flagsArray[H] = 0;
	updateFlags();
}
void CPU::SRL_n(uint8_t* n, uint8_t* none) {
	uint8_t bitLeaving = (1 & *n);
	*n >>= 1;
	//bitLeaving ? //setFlag('C') : resetFlag('C');

	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	*n &= 0x7f;

	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;
	if (*n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');
		flagsArray[Z] = 0;
	updateFlags();


	//if (n == getA())
	//	AF &= 0xfff0;
}
void CPU::SRL_$HL(uint8_t* none1, uint8_t* none2) {
	uint8_t n = bus->mmu->read(HL);
	uint8_t bitLeaving = (1 & n);
	n >>= 1;
	//bitLeaving ? //setFlag('C') : resetFlag('C');

	bitLeaving ? flagsArray[C] = 1 : flagsArray[C] = 0;
	n &= 0x7f;
	bus->mmu->write(HL, n);

	//resetFlag('N');

	flagsArray[N] = 0;
	//resetFlag('H');

	flagsArray[H] = 0;
	if (n == 0)
		//setFlag('Z');

		flagsArray[Z] = 1;
	else
		//resetFlag('Z');
		flagsArray[Z] = 0;
	updateFlags();

}
void CPU::BIT_$n_r(uint8_t* $n, uint8_t* r) {
	uint8_t bit = 1 & (*r >> *$n);
	//bit ? //resetFlag('Z') : setFlag('Z');

	bit ? flagsArray[Z] = 0 : flagsArray[Z] = 1;

	//resetFlag('N');

	flagsArray[N] = 0;
	//setFlag('H');
	flagsArray[H] = 1;
	updateFlags();

}
void CPU::SET_$n_r(uint8_t* $n, uint8_t* r) {
	//?
	*r |= 1 << *$n;
	/*if (r == getA())
		AF &= 0xfff0;*/

}
void CPU::SET_$n_$HL(uint8_t* $n, uint8_t* none) {
	bus->mmu->write(HL, bus->mmu->read(HL) | (1 << *$n));
}
void CPU::RES_$n_r(uint8_t* $n, uint8_t* r) {
	//?
	*r &= 0xff ^ (1 << *$n);
	//if (r == getA())
	//	AF &= 0xfff0;
}
void CPU::RES_$n_$HL(uint8_t* $n, uint8_t* none) {
	bus->mmu->write(HL, bus->mmu->read(HL) & (0xff ^ (1 << *$n)));
}
void CPU::JP_nn(uint8_t* nn, uint8_t* none) {
	PC = *(uint16_t*)nn;
}
void CPU::JP_cc_nn(uint8_t* cc_, uint8_t* nn) {
	ccFlag cc = (ccFlag)*cc_;
	if ((cc == ccFlag::NZ && !getFlag('Z')) || (cc == ccFlag::Z && getFlag('Z')) || (cc == ccFlag::NC && !getFlag('C')) || (cc == ccFlag::C && getFlag('C'))) {
		PC = *(uint16_t*)nn;
		lastOpcodeCycles += 1;
	}
}
void CPU::JP_HL(uint8_t* none1, uint8_t* none2) {
	PC = HL;
}
void CPU::JR_n(uint8_t* n, uint8_t* none) {
	PC += *(signed char*)n;
}
void CPU::JR_cc_n(uint8_t* cc_, uint8_t* n) {
	ccFlag cc = (ccFlag)*cc_;
	if ((cc == ccFlag::NZ && !getFlag('Z')) || (cc == ccFlag::Z && getFlag('Z')) || (cc == ccFlag::NC && !getFlag('C')) || (cc == ccFlag::C && getFlag('C'))) {
		PC += *(signed char*)n;
		lastOpcodeCycles += 1;
	}
}
void CPU::CALL_nn(uint8_t* nn, uint8_t* none) {

	//Word word = WordToBytes(PC);
	//bus->mmu->write(SP - 1, word.msb);
	//bus->mmu->write(SP - 2, word.lsb);
	//SP -= 2;
	///*bus->mmu->write(SP--, word.msb);
	//bus->mmu->write(SP--, word.lsb);*/
	//PC = *(uint16_t*)nn;
	PUSH_nn((uint8_t*)&PC, NULL);
	PC = *(uint16_t*)nn;
	if (PC == 0XC2E0) {//found in e30d
		int p = 0;
	}
}
void CPU::CALL_cc_nn(uint8_t* cc_, uint8_t* nn) {
	ccFlag cc = (ccFlag)*cc_;
	if ((cc == ccFlag::NZ && !getFlag('Z')) || (cc == ccFlag::Z && getFlag('Z')) || (cc == ccFlag::NC && !getFlag('C')) || (cc == ccFlag::C && getFlag('C'))) {

		//Word word = WordToBytes(PC);
		//bus->mmu->write(SP - 1, word.msb);
		//bus->mmu->write(SP - 2, word.lsb);
		//SP -= 2;
		///*bus->mmu->write(SP--, word.msb);
		//bus->mmu->write(SP--, word.lsb);*/
		//PC = *(uint16_t*)nn;
		PUSH_nn((uint8_t*)&PC, NULL);
		PC = *(uint16_t*)nn;
		lastOpcodeCycles += 3;
	}
}
void CPU::RST_n(uint8_t* n, uint8_t* none) {

	//Word word = WordToBytes(PC);
	/*bus->mmu->write(SP--, word.msb);
	bus->mmu->write(SP--, word.lsb);*/
	/*bus->mmu->write(SP - 1, word.msb);
	bus->mmu->write(SP - 2, word.lsb);
	SP -= 2;
	uint16_t d = *(uint16_t*)n;
	PC = *n;*/
	PUSH_nn((uint8_t*)&PC, NULL);
	PC = *n;
}
void CPU::RET(uint8_t* none, uint8_t* none2) {
	/*uint8_t lsb = bus->mmu->read(++SP);
	uint8_t msb = bus->mmu->read(++SP);*/
	/*uint8_t lsb = bus->mmu->read(SP);
	uint8_t msb = bus->mmu->read(SP + 1);
	SP += 2;*/

	//PC = BytesToWord(msb, lsb);
	POP_nn(NULL, (uint8_t*)&PC);

}
void CPU::RET_cc(uint8_t* cc_, uint8_t* none) {
	ccFlag cc = (ccFlag)*cc_;
	if ((cc == ccFlag::NZ && !getFlag('Z')) || (cc == ccFlag::Z && getFlag('Z')) || (cc == ccFlag::NC && !getFlag('C')) || (cc == ccFlag::C && getFlag('C'))) {
		/*uint8_t lsb = bus->mmu->read(++SP);
		uint8_t msb = bus->mmu->read(++SP);*/
		/*uint8_t lsb = bus->mmu->read(SP);
		uint8_t msb = bus->mmu->read(SP + 1);
		SP += 2;
		PC = BytesToWord(msb, lsb);*/
		POP_nn(NULL, (uint8_t*)&PC);
		lastOpcodeCycles += 3;
	}
}
void CPU::RETI(uint8_t* none, uint8_t* none2) {

	/*	uint8_t lsb = bus->mmu->read(++SP);
	uint8_t msb = bus->mmu->read(++SP);
	uint8_t lsb = bus->mmu->read(SP);
	uint8_t msb = bus->mmu->read(SP + 1);
	SP += 2;
	PC = BytesToWord(msb, lsb);*/

	POP_nn(NULL, (uint8_t*)&PC);
	IME = true;
	//setIME = true;
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
	byte ^= 0xffff;
	uint8_t* f = getF();
	*f = *f & byte;
}
void CPU::updateFlags()
{
	if (flagsArray[0] != -1) {
		flagsArray[0] ? setFlag('Z') : resetFlag('Z');
	}
	if (flagsArray[1] != -1) {
		flagsArray[1] ? setFlag('N') : resetFlag('N');
	}
	if (flagsArray[2] != -1) {
		flagsArray[2] ? setFlag('H') : resetFlag('H');
	}
	if (flagsArray[3] != -1) {
		flagsArray[3] ? setFlag('C') : resetFlag('C');
	}
	flagsArray[0] = -1;
	flagsArray[1] = -1;
	flagsArray[2] = -1;
	flagsArray[3] = -1;
}
uint8_t* CPU::NZFlag() {

	return (uint8_t*)flagsNames;
}
uint8_t* CPU::ZFlag() {

	return (uint8_t*)flagsNames + 1;
}
uint8_t* CPU::NCFlag() {

	return (uint8_t*)flagsNames + 2;
}
uint8_t* CPU::CFlag() {

	return (uint8_t*)flagsNames + 3;
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
uint8_t* CPU::getBIT0() {
	bit = 0;
	return &bit;
}
uint8_t* CPU::getBIT1() {
	bit = 1;
	return &bit;
}
uint8_t* CPU::getBIT2() {
	bit = 2;
	return &bit;
}
uint8_t* CPU::getBIT3() {
	bit = 3;
	return &bit;
}
uint8_t* CPU::getBIT4() {
	bit = 4;
	return &bit;
}
uint8_t* CPU::getBIT5() {
	bit = 5;
	return &bit;
}
uint8_t* CPU::getBIT6() {
	bit = 6;
	return &bit;
}
uint8_t* CPU::getBIT7() {
	bit = 7;
	return &bit;
}
void CPU::setFlags(uint16_t r, uint16_t n)
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
	uint8_t msb = 0xff & (word >> 8);
	uint8_t lsb = 0xff & word;
	return Word{ msb,lsb };
}
Byte CPU::getByte(uint8_t byte) {
	uint8_t msn = 0xf & (byte >> 4);
	uint8_t lsn = 0xf & byte;
	return Byte{ msn,lsn };
}
uint8_t CPU::getByte(Byte byte) {
	return byte.msn << 4 | byte.lsn;
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

	lastOpcode = opcode;
	uint8_t* param1 = (this->*op.param1)();
	uint8_t* param2 = (this->*op.param2)();
	lastOpcodeCycles = op.cycles;//need to be before execution. look at jp condition
	lastOpcodeCycles /= 4;
	if ((opcode & 0xFF00) == 0xCB00) {
		//lastOpcodeCycles += 1;
	}

	(this->*op.operate)(param1, param2);
	AF &= 0xfff0;
	//printf("%s\n",op.name);
}
void CPU::ExecuteOpcode(uint16_t opcode) {

	uint8_t msb, lsb;


	uint8_t* param1;
	uint8_t* param2;
	//opcodes[opcode]


	//uint8_t* param1 = (this->*op.param1)();
	//uint8_t* param2 = (this->*op.param2)();

	switch (opcode) {
	case 0x0:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		NOP(param1, param2);
		break;
	case 0x1:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)&BC;
		param2 = (uint8_t*)&immidiateNN;
		LD_n_nn(param1, param2);
		break;
	case 0x2:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = (uint8_t*)&AF + 1;
		LD_$n_A(param1, param2);
		break;
	case 0x3:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		INC_nn(param1, param2);
		break;
	case 0x4:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		INC_n(param1, param2);
		break;
	case 0x5:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		DEC_n(param1, param2);
		break;
	case 0x6:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&BC + 1;
		param2 = &immidiateN;
		LD_nn_N(param1, param2);
		break;
	case 0x7:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		RLCA(param1, param2);
		break;
	case 0x8:
		lastOpcodeCycles = 5;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		//immidiateN = bus->mmu->read(msb << 8 | lsb);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)&immidiateNN;
		param2 = NULL;
		LD_$NN_SP(param1, param2);
		break;
	case 0x9:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&BC;
		ADD_HL_n(param1, param2);
		break;
	case 0xa:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(BC);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		LD_A_n(param1, param2);
		break;
	case 0xb:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		DEC_nn(param1, param2);
		break;
	case 0xc:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		INC_n(param1, param2);
		break;
	case 0xd:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		DEC_n(param1, param2);
		break;
	case 0xe:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&BC;
		param2 = &immidiateN;
		LD_nn_N(param1, param2);
		break;
	case 0xf:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		RRCA(param1, param2);
		break;
	case 0x10:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		STOP(param1, param2);
		break;
	case 0x11:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)&DE;
		param2 = (uint8_t*)&immidiateNN;
		LD_n_nn(param1, param2);
		break;
	case 0x12:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = (uint8_t*)&AF + 1;
		LD_$n_A(param1, param2);
		break;
	case 0x13:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		INC_nn(param1, param2);
		break;
	case 0x14:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		INC_n(param1, param2);
		break;
	case 0x15:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		DEC_n(param1, param2);
		break;
	case 0x16:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&DE + 1;
		param2 = &immidiateN;
		LD_nn_N(param1, param2);
		break;
	case 0x17:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		RLA(param1, param2);
		break;
	case 0x18:
		lastOpcodeCycles = 3;
		immidiateN = bus->mmu->read(PC++);
		param1 = &immidiateN;
		param2 = NULL;
		JR_n(param1, param2);
		break;
	case 0x19:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&DE;
		ADD_HL_n(param1, param2);
		break;
	case 0x1a:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(DE);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		LD_A_n(param1, param2);
		break;
	case 0x1b:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		DEC_nn(param1, param2);
		break;
	case 0x1c:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		INC_n(param1, param2);
		break;
	case 0x1d:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		DEC_n(param1, param2);
		break;
	case 0x1e:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&DE;
		param2 = &immidiateN;
		LD_nn_N(param1, param2);
		break;
	case 0x1f:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		RRA(param1, param2);
		break;
	case 0x20:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)flagsNames;
		param2 = &immidiateN;
		JR_cc_n(param1, param2);
		break;
	case 0x21:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)&HL;
		param2 = (uint8_t*)&immidiateNN;
		LD_n_nn(param1, param2);
		break;
	case 0x22:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&AF + 1;
		LDI_$HL_A(param1, param2);
		break;
	case 0x23:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		INC_nn(param1, param2);
		break;
	case 0x24:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		INC_n(param1, param2);
		break;
	case 0x25:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		DEC_n(param1, param2);
		break;
	case 0x26:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&HL + 1;
		param2 = &immidiateN;
		LD_nn_N(param1, param2);
		break;
	case 0x27:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		DAA(param1, param2);
		break;
	case 0x28:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)flagsNames + 1;
		param2 = &immidiateN;
		JR_cc_n(param1, param2);
		break;
	case 0x29:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&HL;
		ADD_HL_n(param1, param2);
		break;
	case 0x2a:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		LDI_A_$HL(param1, param2);
		break;
	case 0x2b:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		DEC_nn(param1, param2);
		break;
	case 0x2c:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		INC_n(param1, param2);
		break;
	case 0x2d:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		DEC_n(param1, param2);
		break;
	case 0x2e:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&HL;
		param2 = &immidiateN;
		LD_nn_N(param1, param2);
		break;
	case 0x2f:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		CPL(param1, param2);
		break;
	case 0x30:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)flagsNames + 2;
		param2 = &immidiateN;
		JR_cc_n(param1, param2);
		break;
	case 0x31:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)&SP;
		param2 = (uint8_t*)&immidiateNN;
		LD_n_nn(param1, param2);
		break;
	case 0x32:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&AF + 1;
		LDD_$HL_A(param1, param2);
		break;
	case 0x33:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&SP;
		param2 = NULL;
		INC_nn(param1, param2);
		break;
	case 0x34:
		lastOpcodeCycles = 3;
		immidiateN = bus->mmu->read(HL);
		param1 = &immidiateN;
		param2 = NULL;
		INC_$HL(param1, param2);
		break;
	case 0x35:
		lastOpcodeCycles = 3;
		immidiateN = bus->mmu->read(HL);
		param1 = &immidiateN;
		param2 = NULL;
		DEC_$HL(param1, param2);
		break;
	case 0x36:
		lastOpcodeCycles = 3;
		immidiateN = bus->mmu->read(PC++);
		param1 = NULL;
		param2 = &immidiateN;
		LD_n_$HL(param1, param2);
		break;
	case 0x37:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		SCF(param1, param2);
		break;
	case 0x38:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)flagsNames + 3;
		param2 = &immidiateN;
		JR_cc_n(param1, param2);
		break;
	case 0x39:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&SP;
		ADD_HL_n(param1, param2);
		break;
	case 0x3a:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		LDD_A_$HL(param1, param2);
		break;
	case 0x3b:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&SP;
		param2 = NULL;
		DEC_nn(param1, param2);
		break;
	case 0x3c:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		INC_n(param1, param2);
		break;
	case 0x3d:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		DEC_n(param1, param2);
		break;
	case 0x3e:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		LD_A_n(param1, param2);
		break;
	case 0x3f:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		CCF(param1, param2);
		break;
	case 0x40:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC + 1;
		param2 = (uint8_t*)&BC + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x41:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC + 1;
		param2 = (uint8_t*)&BC;
		LD_r1_r2(param1, param2);
		break;
	case 0x42:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC + 1;
		param2 = (uint8_t*)&DE + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x43:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC + 1;
		param2 = (uint8_t*)&DE;
		LD_r1_r2(param1, param2);
		break;
	case 0x44:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC + 1;
		param2 = (uint8_t*)&HL + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x45:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC + 1;
		param2 = (uint8_t*)&HL;
		LD_r1_r2(param1, param2);
		break;
	case 0x46:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&BC + 1;
		param2 = &immidiateN;
		LD_r1_r2(param1, param2);
		break;
	case 0x47:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC + 1;
		param2 = (uint8_t*)&AF + 1;
		LD_n_A(param1, param2);
		break;
	case 0x48:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC;
		param2 = (uint8_t*)&BC + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x49:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC;
		param2 = (uint8_t*)&BC;
		LD_r1_r2(param1, param2);
		break;
	case 0x4a:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC;
		param2 = (uint8_t*)&DE + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x4b:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC;
		param2 = (uint8_t*)&DE;
		LD_r1_r2(param1, param2);
		break;
	case 0x4c:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC;
		param2 = (uint8_t*)&HL + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x4d:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC;
		param2 = (uint8_t*)&HL;
		LD_r1_r2(param1, param2);
		break;
	case 0x4e:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&BC;
		param2 = &immidiateN;
		LD_r1_r2(param1, param2);
		break;
	case 0x4f:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC;
		param2 = (uint8_t*)&AF + 1;
		LD_n_A(param1, param2);
		break;
	case 0x50:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE + 1;
		param2 = (uint8_t*)&BC + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x51:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE + 1;
		param2 = (uint8_t*)&BC;
		LD_r1_r2(param1, param2);
		break;
	case 0x52:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE + 1;
		param2 = (uint8_t*)&DE + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x53:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE + 1;
		param2 = (uint8_t*)&DE;
		LD_r1_r2(param1, param2);
		break;
	case 0x54:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE + 1;
		param2 = (uint8_t*)&HL + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x55:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE + 1;
		param2 = (uint8_t*)&HL;
		LD_r1_r2(param1, param2);
		break;
	case 0x56:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&DE + 1;
		param2 = &immidiateN;
		LD_r1_r2(param1, param2);
		break;
	case 0x57:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE + 1;
		param2 = (uint8_t*)&AF + 1;
		LD_n_A(param1, param2);
		break;
	case 0x58:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE;
		param2 = (uint8_t*)&BC + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x59:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE;
		param2 = (uint8_t*)&BC;
		LD_r1_r2(param1, param2);
		break;
	case 0x5a:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE;
		param2 = (uint8_t*)&DE + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x5b:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE;
		param2 = (uint8_t*)&DE;
		LD_r1_r2(param1, param2);
		break;
	case 0x5c:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE;
		param2 = (uint8_t*)&HL + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x5d:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE;
		param2 = (uint8_t*)&HL;
		LD_r1_r2(param1, param2);
		break;
	case 0x5e:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&DE;
		param2 = &immidiateN;
		LD_r1_r2(param1, param2);
		break;
	case 0x5f:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE;
		param2 = (uint8_t*)&AF + 1;
		LD_n_A(param1, param2);
		break;
	case 0x60:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL + 1;
		param2 = (uint8_t*)&BC + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x61:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL + 1;
		param2 = (uint8_t*)&BC;
		LD_r1_r2(param1, param2);
		break;
	case 0x62:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL + 1;
		param2 = (uint8_t*)&DE + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x63:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL + 1;
		param2 = (uint8_t*)&DE;
		LD_r1_r2(param1, param2);
		break;
	case 0x64:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL + 1;
		param2 = (uint8_t*)&HL + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x65:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL + 1;
		param2 = (uint8_t*)&HL;
		LD_r1_r2(param1, param2);
		break;
	case 0x66:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&HL + 1;
		param2 = &immidiateN;
		LD_r1_r2(param1, param2);
		break;
	case 0x67:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL + 1;
		param2 = (uint8_t*)&AF + 1;
		LD_n_A(param1, param2);
		break;
	case 0x68:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL;
		param2 = (uint8_t*)&BC + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x69:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL;
		param2 = (uint8_t*)&BC;
		LD_r1_r2(param1, param2);
		break;
	case 0x6a:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL;
		param2 = (uint8_t*)&DE + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x6b:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL;
		param2 = (uint8_t*)&DE;
		LD_r1_r2(param1, param2);
		break;
	case 0x6c:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL;
		param2 = (uint8_t*)&HL + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x6d:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL;
		param2 = (uint8_t*)&HL;
		LD_r1_r2(param1, param2);
		break;
	case 0x6e:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&HL;
		param2 = &immidiateN;
		LD_r1_r2(param1, param2);
		break;
	case 0x6f:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL;
		param2 = (uint8_t*)&AF + 1;
		LD_n_A(param1, param2);
		break;
	case 0x70:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&BC + 1;
		LD_n_$HL(param1, param2);
		break;
	case 0x71:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&BC;
		LD_n_$HL(param1, param2);
		break;
	case 0x72:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&DE + 1;
		LD_n_$HL(param1, param2);
		break;
	case 0x73:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&DE;
		LD_n_$HL(param1, param2);
		break;
	case 0x74:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&HL + 1;
		LD_n_$HL(param1, param2);
		break;
	case 0x75:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = (uint8_t*)&HL;
		LD_n_$HL(param1, param2);
		break;
	case 0x76:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		HALT(param1, param2);
		break;
	case 0x77:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = (uint8_t*)&AF + 1;
		LD_$n_A(param1, param2);
		break;
	case 0x78:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x79:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC;
		LD_r1_r2(param1, param2);
		break;
	case 0x7a:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x7b:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE;
		LD_r1_r2(param1, param2);
		break;
	case 0x7c:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x7d:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL;
		LD_r1_r2(param1, param2);
		break;
	case 0x7e:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		LD_r1_r2(param1, param2);
		break;
	case 0x7f:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&AF + 1;
		LD_r1_r2(param1, param2);
		break;
	case 0x80:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC + 1;
		ADD_A_n(param1, param2);
		break;
	case 0x81:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC;
		ADD_A_n(param1, param2);
		break;
	case 0x82:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE + 1;
		ADD_A_n(param1, param2);
		break;
	case 0x83:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE;
		ADD_A_n(param1, param2);
		break;
	case 0x84:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL + 1;
		ADD_A_n(param1, param2);
		break;
	case 0x85:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL;
		ADD_A_n(param1, param2);
		break;
	case 0x86:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		ADD_A_n(param1, param2);
		break;
	case 0x87:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&AF + 1;
		ADD_A_n(param1, param2);
		break;
	case 0x88:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC + 1;
		ADC_A_n(param1, param2);
		break;
	case 0x89:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC;
		ADC_A_n(param1, param2);
		break;
	case 0x8a:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE + 1;
		ADC_A_n(param1, param2);
		break;
	case 0x8b:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE;
		ADC_A_n(param1, param2);
		break;
	case 0x8c:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL + 1;
		ADC_A_n(param1, param2);
		break;
	case 0x8d:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL;
		ADC_A_n(param1, param2);
		break;
	case 0x8e:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		ADC_A_n(param1, param2);
		break;
	case 0x8f:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&AF + 1;
		ADC_A_n(param1, param2);
		break;
	case 0x90:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC + 1;
		SUB_n(param1, param2);
		break;
	case 0x91:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC;
		SUB_n(param1, param2);
		break;
	case 0x92:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE + 1;
		SUB_n(param1, param2);
		break;
	case 0x93:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE;
		SUB_n(param1, param2);
		break;
	case 0x94:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL + 1;
		SUB_n(param1, param2);
		break;
	case 0x95:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL;
		SUB_n(param1, param2);
		break;
	case 0x96:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		SUB_n(param1, param2);
		break;
	case 0x97:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&AF + 1;
		SUB_n(param1, param2);
		break;
	case 0x98:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC + 1;
		SBC_A_n(param1, param2);
		break;
	case 0x99:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC;
		SBC_A_n(param1, param2);
		break;
	case 0x9a:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE + 1;
		SBC_A_n(param1, param2);
		break;
	case 0x9b:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE;
		SBC_A_n(param1, param2);
		break;
	case 0x9c:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL + 1;
		SBC_A_n(param1, param2);
		break;
	case 0x9d:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL;
		SBC_A_n(param1, param2);
		break;
	case 0x9e:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		SBC_A_n(param1, param2);
		break;
	case 0x9f:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&AF + 1;
		SBC_A_n(param1, param2);
		break;
	case 0xa0:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC + 1;
		AND_n(param1, param2);
		break;
	case 0xa1:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC;
		AND_n(param1, param2);
		break;
	case 0xa2:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE + 1;
		AND_n(param1, param2);
		break;
	case 0xa3:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE;
		AND_n(param1, param2);
		break;
	case 0xa4:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL + 1;
		AND_n(param1, param2);
		break;
	case 0xa5:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL;
		AND_n(param1, param2);
		break;
	case 0xa6:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		AND_n(param1, param2);
		break;
	case 0xa7:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&AF + 1;
		AND_n(param1, param2);
		break;
	case 0xa8:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC + 1;
		XOR_n(param1, param2);
		break;
	case 0xa9:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC;
		XOR_n(param1, param2);
		break;
	case 0xaa:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE + 1;
		XOR_n(param1, param2);
		break;
	case 0xab:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE;
		XOR_n(param1, param2);
		break;
	case 0xac:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL + 1;
		XOR_n(param1, param2);
		break;
	case 0xad:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL;
		XOR_n(param1, param2);
		break;
	case 0xae:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		XOR_n(param1, param2);
		break;
	case 0xaf:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&AF + 1;
		XOR_n(param1, param2);
		break;
	case 0xb0:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC + 1;
		OR_n(param1, param2);
		break;
	case 0xb1:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC;
		OR_n(param1, param2);
		break;
	case 0xb2:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE + 1;
		OR_n(param1, param2);
		break;
	case 0xb3:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&DE;
		OR_n(param1, param2);
		break;
	case 0xb4:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL + 1;
		OR_n(param1, param2);
		break;
	case 0xb5:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&HL;
		OR_n(param1, param2);
		break;
	case 0xb6:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		OR_n(param1, param2);
		break;
	case 0xb7:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&AF + 1;
		OR_n(param1, param2);
		break;
	case 0xb8:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		CP_n(param1, param2);
		break;
	case 0xb9:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		CP_n(param1, param2);
		break;
	case 0xba:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		CP_n(param1, param2);
		break;
	case 0xbb:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		CP_n(param1, param2);
		break;
	case 0xbc:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		CP_n(param1, param2);
		break;
	case 0xbd:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		CP_n(param1, param2);
		break;
	case 0xbe:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = &immidiateN;
		param2 = NULL;
		CP_n(param1, param2);
		break;
	case 0xbf:
		lastOpcodeCycles = 1;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		CP_n(param1, param2);
		break;
	case 0xc0:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)flagsNames;
		param2 = NULL;
		RET_cc(param1, param2);
		break;
	case 0xc1:
		lastOpcodeCycles = 3;
		param1 = NULL;
		param2 = (uint8_t*)&BC;
		POP_nn(param1, param2);
		break;
	case 0xc2:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)flagsNames;
		param2 = (uint8_t*)&immidiateNN;
		JP_cc_nn(param1, param2);
		break;
	case 0xc3:
		lastOpcodeCycles = 4;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)&immidiateNN;
		param2 = NULL;
		JP_nn(param1, param2);
		break;
	case 0xc4:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)flagsNames;
		param2 = (uint8_t*)&immidiateNN;
		CALL_cc_nn(param1, param2);
		break;
	case 0xc5:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		PUSH_nn(param1, param2);
		break;
	case 0xc6:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		ADD_A_n(param1, param2);
		break;
	case 0xc7:
		lastOpcodeCycles = 4;
		param1 = RSTOffsets;
		param2 = NULL;
		RST_n(param1, param2);
		break;
	case 0xc8:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)flagsNames + 1;
		param2 = NULL;
		RET_cc(param1, param2);
		break;
	case 0xc9:
		lastOpcodeCycles = 4;
		param1 = NULL;
		param2 = NULL;
		RET(param1, param2);
		break;
	case 0xca:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)flagsNames + 1;
		param2 = (uint8_t*)&immidiateNN;
		JP_cc_nn(param1, param2);
		break;
	case 0xcc:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)flagsNames + 1;
		param2 = (uint8_t*)&immidiateNN;
		CALL_cc_nn(param1, param2);
		break;
	case 0xcd:
		lastOpcodeCycles = 6;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)&immidiateNN;
		param2 = NULL;
		CALL_nn(param1, param2);
		break;
	case 0xce:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		ADC_A_n(param1, param2);
		break;
	case 0xcf:
		lastOpcodeCycles = 4;
		param1 = RSTOffsets + 1;
		param2 = NULL;
		RST_n(param1, param2);
		break;
	case 0xd0:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)flagsNames + 2;
		param2 = NULL;
		RET_cc(param1, param2);
		break;
	case 0xd1:
		lastOpcodeCycles = 3;
		param1 = NULL;
		param2 = (uint8_t*)&DE;
		POP_nn(param1, param2);
		break;
	case 0xd2:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)flagsNames + 2;
		param2 = (uint8_t*)&immidiateNN;
		JP_cc_nn(param1, param2);
		break;
	case 0xd4:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)flagsNames + 2;
		param2 = (uint8_t*)&immidiateNN;
		CALL_cc_nn(param1, param2);
		break;
	case 0xd5:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		PUSH_nn(param1, param2);
		break;
	case 0xd6:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		SUB_n(param1, param2);
		break;
	case 0xd7:
		lastOpcodeCycles = 4;
		param1 = RSTOffsets + 2;
		param2 = NULL;
		RST_n(param1, param2);
		break;
	case 0xd8:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)flagsNames + 3;
		param2 = NULL;
		RET_cc(param1, param2);
		break;
	case 0xd9:
		lastOpcodeCycles = 4;
		param1 = NULL;
		param2 = NULL;
		RETI(param1, param2);
		break;
	case 0xda:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)flagsNames + 3;
		param2 = (uint8_t*)&immidiateNN;
		JP_cc_nn(param1, param2);
		break;
	case 0xdc:
		lastOpcodeCycles = 3;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)flagsNames + 3;
		param2 = (uint8_t*)&immidiateNN;
		CALL_cc_nn(param1, param2);
		break;
	case 0xde:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		SBC_A_n(param1, param2);
		break;
	case 0xdf:
		lastOpcodeCycles = 4;
		param1 = RSTOffsets + 3;
		param2 = NULL;
		RST_n(param1, param2);
		break;
	case 0xe0:
		if (steps == 6207) {
			int y = 0;
		}
		lastOpcodeCycles = 3;
		immidiateN = bus->mmu->read(PC++);
		param1 = &immidiateN;
		param2 = (uint8_t*)&AF + 1;
		LDH_n_A(param1, param2);
		break;
	case 0xe1:
		lastOpcodeCycles = 3;
		param1 = NULL;
		param2 = (uint8_t*)&HL;
		POP_nn(param1, param2);
		break;
	case 0xe2:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = (uint8_t*)&AF + 1;
		LD_C_A(param1, param2);
		break;
	case 0xe5:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		PUSH_nn(param1, param2);
		break;
	case 0xe6:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		AND_n(param1, param2);
		break;
	case 0xe7:
		lastOpcodeCycles = 4;
		param1 = RSTOffsets + 4;
		param2 = NULL;
		RST_n(param1, param2);
		break;
	case 0xe8:
		lastOpcodeCycles = 4;
		immidiateN = bus->mmu->read(PC++);
		param1 = NULL;
		param2 = &immidiateN;
		ADD_SP_n(param1, param2);
		break;
	case 0xe9:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		JP_HL(param1, param2);
		break;
	case 0xea:
		lastOpcodeCycles = 4;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateNN = msb << 8 | lsb;
		param1 = (uint8_t*)&immidiateNN;
		param2 = (uint8_t*)&AF + 1;
		LD_$n_A(param1, param2);
		break;
	case 0xee:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		XOR_n(param1, param2);
		break;
	case 0xef:
		lastOpcodeCycles = 4;
		param1 = RSTOffsets + 5;
		param2 = NULL;
		RST_n(param1, param2);
		break;
	case 0xf0:
		if (steps == 121735) {
			int h = 0;
		}
		lastOpcodeCycles = 3;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		LDH_A_n(param1, param2);
		break;
	case 0xf1:
		lastOpcodeCycles = 3;
		param1 = NULL;
		param2 = (uint8_t*)&AF;
		POP_nn(param1, param2);
		break;
	case 0xf2:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = (uint8_t*)&BC;
		LD_A_C(param1, param2);
		break;
	case 0xf3:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		DI(param1, param2);
		break;
	case 0xf5:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&AF;
		param2 = NULL;
		PUSH_nn(param1, param2);
		break;
	case 0xf6:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		OR_n(param1, param2);
		break;
	case 0xf7:
		lastOpcodeCycles = 4;
		param1 = RSTOffsets + 6;
		param2 = NULL;
		RST_n(param1, param2);
		break;
	case 0xf8:
		lastOpcodeCycles = 3;
		param1 = NULL;
		param2 = NULL;
		LDHL_SP_$n(param1, param2);
		break;
	case 0xf9:
		lastOpcodeCycles = 2;
		param1 = NULL;
		param2 = NULL;
		LD_SP_HL(param1, param2);
		break;
	case 0xfa:
		lastOpcodeCycles = 4;
		lsb = bus->mmu->read(PC++);
		msb = bus->mmu->read(PC++);
		immidiateN = bus->mmu->read(msb << 8 | lsb);
		param1 = (uint8_t*)&AF + 1;
		param2 = &immidiateN;
		LD_A_n(param1, param2);
		break;
	case 0xfb:
		lastOpcodeCycles = 1;
		param1 = NULL;
		param2 = NULL;
		EI(param1, param2);
		break;
	case 0xfe:
		lastOpcodeCycles = 2;
		immidiateN = bus->mmu->read(PC++);
		param1 = &immidiateN;
		param2 = NULL;
		CP_n(param1, param2);
		break;
	case 0xff:
		lastOpcodeCycles = 4;
		param1 = RSTOffsets + 7;
		param2 = NULL;
		RST_n(param1, param2);
		break;
	case 0xcb00:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		RLC_n(param1, param2);
		break;
	case 0xcb01:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		RLC_n(param1, param2);
		break;
	case 0xcb02:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		RLC_n(param1, param2);
		break;
	case 0xcb03:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		RLC_n(param1, param2);
		break;
	case 0xcb04:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		RLC_n(param1, param2);
		break;
	case 0xcb05:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		RLC_n(param1, param2);
		break;
	case 0xcb06:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		RLC_$HL(param1, param2);
		break;
	case 0xcb07:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		RLC_n(param1, param2);
		break;
	case 0xcb08:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		RRC_n(param1, param2);
		break;
	case 0xcb09:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		RRC_n(param1, param2);
		break;
	case 0xcb0a:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		RRC_n(param1, param2);
		break;
	case 0xcb0b:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		RRC_n(param1, param2);
		break;
	case 0xcb0c:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		RRC_n(param1, param2);
		break;
	case 0xcb0d:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		RRC_n(param1, param2);
		break;
	case 0xcb0e:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		RRC_$HL(param1, param2);
		break;
	case 0xcb0f:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		RRC_n(param1, param2);
		break;
	case 0xcb10:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		RL_n(param1, param2);
		break;
	case 0xcb11:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		RL_n(param1, param2);
		break;
	case 0xcb12:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		RL_n(param1, param2);
		break;
	case 0xcb13:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		RL_n(param1, param2);
		break;
	case 0xcb14:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		RL_n(param1, param2);
		break;
	case 0xcb15:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		RL_n(param1, param2);
		break;
	case 0xcb16:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		RL_$HL(param1, param2);
		break;
	case 0xcb17:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		RL_n(param1, param2);
		break;
	case 0xcb18:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		RR_n(param1, param2);
		break;
	case 0xcb19:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		RR_n(param1, param2);
		break;
	case 0xcb1a:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		RR_n(param1, param2);
		break;
	case 0xcb1b:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		RR_n(param1, param2);
		break;
	case 0xcb1c:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		RR_n(param1, param2);
		break;
	case 0xcb1d:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		RR_n(param1, param2);
		break;
	case 0xcb1e:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		RR_$HL(param1, param2);
		break;
	case 0xcb1f:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		RR_n(param1, param2);
		break;
	case 0xcb20:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		SLA_n(param1, param2);
		break;
	case 0xcb21:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		SLA_n(param1, param2);
		break;
	case 0xcb22:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		SLA_n(param1, param2);
		break;
	case 0xcb23:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		SLA_n(param1, param2);
		break;
	case 0xcb24:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		SLA_n(param1, param2);
		break;
	case 0xcb25:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		SLA_n(param1, param2);
		break;
	case 0xcb26:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		SLA_$HL(param1, param2);
		break;
	case 0xcb27:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		SLA_n(param1, param2);
		break;
	case 0xcb28:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		SRA_n(param1, param2);
		break;
	case 0xcb29:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		SRA_n(param1, param2);
		break;
	case 0xcb2a:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		SRA_n(param1, param2);
		break;
	case 0xcb2b:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		SRA_n(param1, param2);
		break;
	case 0xcb2c:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		SRA_n(param1, param2);
		break;
	case 0xcb2d:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		SRA_n(param1, param2);
		break;
	case 0xcb2e:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		SRA_$HL(param1, param2);
		break;
	case 0xcb2f:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		SRA_n(param1, param2);
		break;
	case 0xcb30:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		SWAP_n(param1, param2);
		break;
	case 0xcb31:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		SWAP_n(param1, param2);
		break;
	case 0xcb32:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		SWAP_n(param1, param2);
		break;
	case 0xcb33:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		SWAP_n(param1, param2);
		break;
	case 0xcb34:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		SWAP_n(param1, param2);
		break;
	case 0xcb35:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		SWAP_n(param1, param2);
		break;
	case 0xcb36:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		SWAP_$HL(param1, param2);
		break;
	case 0xcb37:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		SWAP_n(param1, param2);
		break;
	case 0xcb38:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC + 1;
		param2 = NULL;
		SRL_n(param1, param2);
		break;
	case 0xcb39:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&BC;
		param2 = NULL;
		SRL_n(param1, param2);
		break;
	case 0xcb3a:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE + 1;
		param2 = NULL;
		SRL_n(param1, param2);
		break;
	case 0xcb3b:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&DE;
		param2 = NULL;
		SRL_n(param1, param2);
		break;
	case 0xcb3c:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL + 1;
		param2 = NULL;
		SRL_n(param1, param2);
		break;
	case 0xcb3d:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		SRL_n(param1, param2);
		break;
	case 0xcb3e:
		lastOpcodeCycles = 4;
		param1 = (uint8_t*)&HL;
		param2 = NULL;
		SRL_$HL(param1, param2);
		break;
	case 0xcb3f:
		lastOpcodeCycles = 2;
		param1 = (uint8_t*)&AF + 1;
		param2 = NULL;
		SRL_n(param1, param2);
		break;
	case 0xcb40:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb41:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb42:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb43:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb44:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb45:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb46:
		lastOpcodeCycles = 3;
		bit = 0;
		immidiateN = bus->mmu->read(HL);
		param1 = &bit;
		param2 = &immidiateN;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb47:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb48:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb49:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb4a:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb4b:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb4c:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb4d:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb4e:
		lastOpcodeCycles = 3;
		bit = 1;
		immidiateN = bus->mmu->read(HL);
		param1 = &bit;
		param2 = &immidiateN;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb4f:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb50:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb51:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb52:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb53:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb54:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb55:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb56:
		lastOpcodeCycles = 3;
		bit = 2;
		immidiateN = bus->mmu->read(HL);
		param1 = &bit;
		param2 = &immidiateN;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb57:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb58:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb59:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb5a:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb5b:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb5c:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb5d:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb5e:
		lastOpcodeCycles = 3;
		bit = 3;
		immidiateN = bus->mmu->read(HL);
		param1 = &bit;
		param2 = &immidiateN;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb5f:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb60:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb61:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb62:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb63:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb64:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb65:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb66:
		lastOpcodeCycles = 3;
		bit = 4;
		immidiateN = bus->mmu->read(HL);
		param1 = &bit;
		param2 = &immidiateN;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb67:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb68:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb69:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb6a:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb6b:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb6c:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb6d:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb6e:
		lastOpcodeCycles = 3;
		bit = 5;
		immidiateN = bus->mmu->read(HL);
		param1 = &bit;
		param2 = &immidiateN;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb6f:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb70:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb71:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb72:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb73:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb74:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb75:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb76:
		lastOpcodeCycles = 3;
		bit = 6;
		immidiateN = bus->mmu->read(HL);
		param1 = &bit;
		param2 = &immidiateN;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb77:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb78:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb79:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb7a:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb7b:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb7c:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb7d:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb7e:
		lastOpcodeCycles = 3;
		bit = 7;
		immidiateN = bus->mmu->read(HL);
		param1 = &bit;
		param2 = &immidiateN;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb7f:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		BIT_$n_r(param1, param2);
		break;
	case 0xcb80:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb81:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		RES_$n_r(param1, param2);
		break;
	case 0xcb82:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb83:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		RES_$n_r(param1, param2);
		break;
	case 0xcb84:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb85:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		RES_$n_r(param1, param2);
		break;
	case 0xcb86:
		lastOpcodeCycles = 4;
		bit = 0;
		param1 = &bit;
		param2 = NULL;
		RES_$n_$HL(param1, param2);
		break;
	case 0xcb87:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb88:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb89:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		RES_$n_r(param1, param2);
		break;
	case 0xcb8a:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb8b:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		RES_$n_r(param1, param2);
		break;
	case 0xcb8c:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb8d:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		RES_$n_r(param1, param2);
		break;
	case 0xcb8e:
		lastOpcodeCycles = 4;
		bit = 1;
		param1 = &bit;
		param2 = NULL;
		RES_$n_$HL(param1, param2);
		break;
	case 0xcb8f:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb90:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb91:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		RES_$n_r(param1, param2);
		break;
	case 0xcb92:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb93:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		RES_$n_r(param1, param2);
		break;
	case 0xcb94:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb95:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		RES_$n_r(param1, param2);
		break;
	case 0xcb96:
		lastOpcodeCycles = 4;
		bit = 2;
		param1 = &bit;
		param2 = NULL;
		RES_$n_$HL(param1, param2);
		break;
	case 0xcb97:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb98:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb99:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		RES_$n_r(param1, param2);
		break;
	case 0xcb9a:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb9b:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		RES_$n_r(param1, param2);
		break;
	case 0xcb9c:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcb9d:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		RES_$n_r(param1, param2);
		break;
	case 0xcb9e:
		lastOpcodeCycles = 4;
		bit = 3;
		param1 = &bit;
		param2 = NULL;
		RES_$n_$HL(param1, param2);
		break;
	case 0xcb9f:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcba0:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcba1:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		RES_$n_r(param1, param2);
		break;
	case 0xcba2:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcba3:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		RES_$n_r(param1, param2);
		break;
	case 0xcba4:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcba5:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		RES_$n_r(param1, param2);
		break;
	case 0xcba6:
		lastOpcodeCycles = 4;
		bit = 4;
		param1 = &bit;
		param2 = NULL;
		RES_$n_$HL(param1, param2);
		break;
	case 0xcba7:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcba8:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcba9:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		RES_$n_r(param1, param2);
		break;
	case 0xcbaa:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbab:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		RES_$n_r(param1, param2);
		break;
	case 0xcbac:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbad:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		RES_$n_r(param1, param2);
		break;
	case 0xcbae:
		lastOpcodeCycles = 4;
		bit = 5;
		param1 = &bit;
		param2 = NULL;
		RES_$n_$HL(param1, param2);
		break;
	case 0xcbaf:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbb0:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbb1:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		RES_$n_r(param1, param2);
		break;
	case 0xcbb2:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbb3:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		RES_$n_r(param1, param2);
		break;
	case 0xcbb4:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbb5:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		RES_$n_r(param1, param2);
		break;
	case 0xcbb6:
		lastOpcodeCycles = 4;
		bit = 6;
		param1 = &bit;
		param2 = NULL;
		RES_$n_$HL(param1, param2);
		break;
	case 0xcbb7:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbb8:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbb9:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		RES_$n_r(param1, param2);
		break;
	case 0xcbba:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbbb:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		RES_$n_r(param1, param2);
		break;
	case 0xcbbc:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbbd:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		RES_$n_r(param1, param2);
		break;
	case 0xcbbe:
		lastOpcodeCycles = 4;
		bit = 7;
		param1 = &bit;
		param2 = NULL;
		RES_$n_$HL(param1, param2);
		break;
	case 0xcbbf:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		RES_$n_r(param1, param2);
		break;
	case 0xcbc0:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbc1:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		SET_$n_r(param1, param2);
		break;
	case 0xcbc2:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbc3:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		SET_$n_r(param1, param2);
		break;
	case 0xcbc4:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbc5:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_r(param1, param2);
		break;
	case 0xcbc6:
		lastOpcodeCycles = 4;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_$HL(param1, param2);
		break;
	case 0xcbc7:
		lastOpcodeCycles = 2;
		bit = 0;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbc8:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbc9:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		SET_$n_r(param1, param2);
		break;
	case 0xcbca:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbcb:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		SET_$n_r(param1, param2);
		break;
	case 0xcbcc:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbcd:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_r(param1, param2);
		break;
	case 0xcbce:
		lastOpcodeCycles = 4;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_$HL(param1, param2);
		break;
	case 0xcbcf:
		lastOpcodeCycles = 2;
		bit = 1;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbd0:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbd1:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		SET_$n_r(param1, param2);
		break;
	case 0xcbd2:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbd3:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		SET_$n_r(param1, param2);
		break;
	case 0xcbd4:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbd5:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_r(param1, param2);
		break;
	case 0xcbd6:
		lastOpcodeCycles = 4;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_$HL(param1, param2);
		break;
	case 0xcbd7:
		lastOpcodeCycles = 2;
		bit = 2;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbd8:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbd9:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		SET_$n_r(param1, param2);
		break;
	case 0xcbda:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbdb:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		SET_$n_r(param1, param2);
		break;
	case 0xcbdc:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbdd:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_r(param1, param2);
		break;
	case 0xcbde:
		lastOpcodeCycles = 4;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_$HL(param1, param2);
		break;
	case 0xcbdf:
		lastOpcodeCycles = 2;
		bit = 3;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbe0:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbe1:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		SET_$n_r(param1, param2);
		break;
	case 0xcbe2:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbe3:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		SET_$n_r(param1, param2);
		break;
	case 0xcbe4:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbe5:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_r(param1, param2);
		break;
	case 0xcbe6:
		lastOpcodeCycles = 4;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_$HL(param1, param2);
		break;
	case 0xcbe7:
		lastOpcodeCycles = 2;
		bit = 4;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbe8:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbe9:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		SET_$n_r(param1, param2);
		break;
	case 0xcbea:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbeb:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		SET_$n_r(param1, param2);
		break;
	case 0xcbec:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbed:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_r(param1, param2);
		break;
	case 0xcbee:
		lastOpcodeCycles = 4;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_$HL(param1, param2);
		break;
	case 0xcbef:
		lastOpcodeCycles = 2;
		bit = 5;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbf0:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbf1:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		SET_$n_r(param1, param2);
		break;
	case 0xcbf2:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbf3:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		SET_$n_r(param1, param2);
		break;
	case 0xcbf4:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbf5:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_r(param1, param2);
		break;
	case 0xcbf6:
		lastOpcodeCycles = 4;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_$HL(param1, param2);
		break;
	case 0xcbf7:
		lastOpcodeCycles = 2;
		bit = 6;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbf8:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&BC + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbf9:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&BC;
		SET_$n_r(param1, param2);
		break;
	case 0xcbfa:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&DE + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbfb:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&DE;
		SET_$n_r(param1, param2);
		break;
	case 0xcbfc:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&HL + 1;
		SET_$n_r(param1, param2);
		break;
	case 0xcbfd:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_r(param1, param2);
		break;
	case 0xcbfe:
		lastOpcodeCycles = 4;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&HL;
		SET_$n_$HL(param1, param2);
		break;
	case 0xcbff:
		lastOpcodeCycles = 2;
		bit = 7;
		param1 = &bit;
		param2 = (uint8_t*)&AF + 1;
		SET_$n_r(param1, param2);
		break;
	}


	AF &= 0xfff0;
	lastOpcode = opcode;
	if ((opcode & 0xFF00) == 0xCB00) {
		//lastOpcodeCycles += 1;
	}

	//time.addMCycles(lastOpcodeCycles);
	//time.print(2);
}
void CPU::reset()
{
	int stage = 4;
	//1 bad
	stage = 4;
	switch (stage) {
	case 0:
		//antonio reset
		AF = 0X1180;
		BC = 0x0000;
		DE = 0X0008;
		HL = 0X007C;
		SP = 0XFFFE;
		PC = 0X0100;
		//if (bus->cartridge->colorGB) {
		DE = 0xFF56;
		HL = 0X000D;
		//}
		break;
	case 1:
		//gbspec reset
		AF = 0X01B0;
		BC = 0x0013;
		DE = 0X00d8;
		HL = 0X014d;
		SP = 0XFFFE;
		PC = 0X0100;
		//if (bus->cartridge->colorGB) {
			//AF = 0X11B0;
		//}
		break;
	case 2:
		PC = 0x100;
		AF = 0x01B0;
		BC = 0x0000;
		DE = 0xFF56;
		HL = 0x000D;
		SP = 0xFFFE;
		if (bus->cartridge->header.colorGB) {
			AF = 0x1180;
		}

	case 3:
		PC = 0x100;
		AF = 0x01B0;
		BC = 0x0000;
		DE = 0xFF56;
		HL = 0x000D;
		SP = 0xFFFE;
		AF = 0X1180;
		/*if (bus->cartridge->colorGB) {
			AF = 0x1180;
		}*/
		break;
	case 4:

		AF = 0x1180;
		BC = 0x0000;
		DE = 0x0008;
		HL = 0x007C;
		SP = 0xFFFE;
		PC = 0x100;

		DE = 0xFF56;
		HL = 0x000D;
		if (bus->cartridge->header.colorGB) {
			DE = 0xFF56;
			HL = 0x000D;
		}
		break;
	}







	//PC = 0;

}
int CPU::getCycelPerIncrementTIMA(uint8_t freqIndex) {
	switch (freqIndex) {//read tac selected freq
	case 0:
		return cpuFreq / 4096;

	case 1:
		return cpuFreq / 262144;

	case 2:
		return cpuFreq / 65536;
	case 3:
		return cpuFreq / 16384;
	}

	return 0;
}
//if tima counter pass half of the counter target, tima incease.
void CPU::isCycelPerIncrementTIMAPassedHalf(int newCycelPerIncrementTIMA) {
	uint8_t timcont = 0;
	bool increaseTima = false;
	int stage = 1;
	switch (stage) {
	case 0:
		if (cyclesPerIncrementTIMA <= newCycelPerIncrementTIMA)
			cyclesPerIncrementTIMA = newCycelPerIncrementTIMA;
		else {
			int incrementTima = cyclesPerIncrementTIMACounter / newCycelPerIncrementTIMA;
			while (incrementTima > 0) {
				if (timaOverflow) {
					bus->mmu->write(0xff05, bus->mmu->read(0xff06));//load tma to tima
					bus->interrupt->setInterruptRequest(2);
					timaOverflow = false;
					//cyclesPerIncrementTIMACounter += 4;
				}
				bus->mmu->write(0xff05, bus->mmu->read(0xff05) + 1);//tima increment
				if (bus->mmu->read(0xff05) == 0xff) {//tima overflow
					timaOverflow = true;
				}
				incrementTima--;
			}
			//cyclesPerIncrementTIMACounter = 0;
			cyclesPerIncrementTIMACounter = cyclesPerIncrementTIMACounter % newCycelPerIncrementTIMA;
		}
		break;
	case 1:

		timcont = bus->mmu->read(0xff07);//timer controler reister
		increaseTima = (cyclesPerIncrementTIMA == 1024 && newCycelPerIncrementTIMA == 256 && ((timcont >> 2) & 0x1)) ? true : false;
		if (cyclesPerIncrementTIMA / 2 <= cyclesPerIncrementTIMACounter || (increaseTima)) {
			int freq = getCycelPerIncrementTIMA(timcont & 0x3);

			if (timaOverflow) {
				bus->mmu->write(0xff05, bus->mmu->read(0xff06));//load tma to tima
				bus->interrupt->setInterruptRequest(2);
				timaOverflow = false;
			}
			bus->mmu->write(0xff05, bus->mmu->read(0xff05) + 1);//tima increment
			if (bus->mmu->read(0xff05) == 0) {//tima overflow
				timaOverflow = true;
			}
			printf("counter:%d old_dst:%d new_dst:%d\n", cyclesPerIncrementTIMACounter, cyclesPerIncrementTIMA, newCycelPerIncrementTIMA);

			cyclesPerIncrementTIMACounter = 0;
		}
		cyclesPerIncrementTIMA = newCycelPerIncrementTIMA;

		cyclesPerIncrementTIMACounter = 0;

		break;
	case 2:
		cyclesPerIncrementTIMA = newCycelPerIncrementTIMA;
		cyclesPerIncrementTIMACounter = 0;
		break;
	}

}
void CPU::updateCycelPerIncrementTIMA(uint8_t freqIndex)
{
	int newCycelPerIncrementTIMA = getCycelPerIncrementTIMA(freqIndex);
	//isCycelPerIncrementTIMAPassedHalf(newCycelPerIncrementTIMA);
	cyclesPerIncrementTIMA = newCycelPerIncrementTIMA;
	cyclesPerIncrementTIMACounter = 0;
}

//void CPU::updateTimers()
//{
//
//	cyclesPerIncrementDIVIDER += lastOpcodeCycles;
//	if (cyclesPerIncrementDIVIDER >= 256) {
//		bus->interrupt->io[0x04]++;//divider increment
//		cyclesPerIncrementDIVIDER -= 256;
//	}
//
//
//	uint8_t timcont = bus->mmu->read(0xff07);//timer controler reister
//	if ((timcont >> 2) & 0x1) {//counting
//		//if (timaOverflow) {
//		//	bus->mmu->write(0xff05, bus->mmu->read(0xff06));//load tma to tima
//		//	bus->interrupt->setInterruptRequest(2);
//		//	timaOverflow = false;
//		//}
//		cyclesPerIncrementTIMACounter += lastOpcodeCycles;
//		int freq = getCycelPerIncrementTIMA(timcont & 0x3);
//		if (cyclesPerIncrementTIMACounter >= freq) {
//			//if (timaOverflow) {
//			//	bus->mmu->write(0xff05, bus->mmu->read(0xff06));//load tma to tima
//			//	bus->interrupt->setInterruptRequest(2);
//			//	timaOverflow = false;
//			//}
//			
//
//			if (bus->mmu->read(0xff05) == 0xff) {//tima overflow
//				
//				bus->mmu->write(0xff05, bus->mmu->read(0xff06));//load tma to tima
//				bus->interrupt->setInterruptRequest(2);
//			}
//			else {
//				bus->mmu->write(0xff05, bus->mmu->read(0xff05) + 1);//tima increment
//			}
//
//
//			cyclesPerIncrementTIMACounter -= freq;
//		}
//		//cyclesPerIncrementTIMA -= lastOpcodeCycles;
//
//		//if (cyclesPerIncrementTIMA <= 0) {
//		//	cyclesPerIncrementTIMA=getCycelPerIncrementTIMA(timcont & 0x3);
//		//	if (bus->mmu->read(0xff05)== 0xff) {//tima overflow
//		//		bus->mmu->write(0xff05, bus->mmu->read(0xff06));//load tma to tima
//		//		bus->interrupt->setInterruptRequest(2);
//		//	}
//		//	else {
//		//		bus->mmu->write(0xff05, bus->mmu->read(0xff05) + 1);//tima increment
//		//	}
//		//}
//	}
//	else {
//		//updateCycelPerIncrementTIMA(timcont & 0x03);
//	}
//
//}
void CPU::updateTimers()
{

	cyclesPerIncrementDIVIDER += lastOpcodeCycles;
	if (cyclesPerIncrementDIVIDER >= 256) {
		bus->interrupt->io[0x04]++;//divider increment
		cyclesPerIncrementDIVIDER -= 256;
	}

	if (timaOverflow) {
		bus->mmu->write(0xff05, bus->mmu->read(0xff06));//load tma to tima
		bus->interrupt->setInterruptRequest(2);
		timaOverflow = false;
	}
	uint8_t timcont = bus->mmu->read(0xff07);//timer controler reister
	if ((timcont >> 2) & 0x1) {//counting
		cyclesPerIncrementTIMACounter += lastOpcodeCycles;
		int freq = getCycelPerIncrementTIMA(timcont & 0x3);
		while (cyclesPerIncrementTIMACounter >= freq) {
			if (timaOverflow) {
				bus->mmu->write(0xff05, bus->mmu->read(0xff06));//load tma to tima
				bus->interrupt->setInterruptRequest(2);
				timaOverflow = false;
			}
			bus->mmu->write(0xff05, bus->mmu->read(0xff05) + 1);//tima increment
			if (bus->mmu->read(0xff05) == 0) {//tima overflow
				timaOverflow = true;
			}

			cyclesPerIncrementTIMACounter -= freq;
		}

	}

}
uint8_t CPU::getOpcode()
{

	/*if (halt)
		return 0x76;*/
	uint8_t opcode = *getN();
	/*steps++;
	if (bus->pipeEnable) {
		bus->p->read(1);
		bus->p->wBuffer[0] = 0;
		if (bus->p->rBuffer[0] != opcode||steps==35257) {

			bus->p->wBuffer[0] = 1;
		}
		bus->p->write();
		if (bus->p->wBuffer[0]==1) {
			int x = 0;
		}

	}*/
	return opcode;
}

void CPU::connectToBus(BUS* bus)
{
	this->bus = bus;
}