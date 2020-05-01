#pragma once
#include "BUS.h"
#include "MMU.h"
#include "GPU.h"
#include <Windows.h>
class DMA
{
private:
	BUS* bus;
	uint16_t  FF46sourceAddress = 0;
	uint16_t sourceAddress=0;
	uint16_t destinationAddress=0;
	uint16_t transferLen=0;
	uint8_t hBlankDMAStep=0;
	enum class DMAMode:uint8_t{ General, hBlank, None};
	DMAMode dmaMode = DMAMode::None;
	uint16_t vRamBankOffset = 0;
public:
	uint8_t hDMA5 = 0;
	bool FF46transfering = false;
	bool transfer = false;
	//bool hBlankDMA = false;
	void connectToBus(BUS* bus);
	void FF46DMATransferStart(uint8_t sourceAddress);
	void FF46DMATransfer();
	void setSourceHi(uint8_t value);
	void setSourceLo(uint8_t value);
	void setDestinationHi(uint8_t value);
	void setDestinationLo(uint8_t value);
	void HDMATransfer(uint8_t value);
	void generalPurposeDMA();
	void hBlankDMA();
	void stopHDMATransfer();

};

