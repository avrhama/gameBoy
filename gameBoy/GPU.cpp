#include "GPU.h"
#include "pipeChannel.h"
void GPU::reset()
{
	for (int i = 0;i < 0x4000;i++)
		vRam[i] = 0;
	for (int i = 0;i < 0xa0;i++)
		oam[i] = 0;
	
}

void GPU::connectToBus(BUS* bus)
{
	this->bus = bus;
}
bool GPU::checkLCDStatus()
{
	bool useLockers = true;
	
	if (!((bus->interrupt->io[0x40] >> 7) & 0x1)) {//LCD Display Disable
		//bus->interrupt->io[0x41] &= 252;//web
		//bus->interrupt->io[0x41] &= 253;//me
		bus->interrupt->io[0x41] &= 0xFD;
		bus->interrupt->io[0x41] |= 0x01;
		bus->interrupt->io[0x44] = 0;
		cyclesPerScanline = 456;
		if (useLockers) {
			bus->mmu->vRamLock = false;
			bus->mmu->OAMLock = false;
		}
		
		//bus->dma->generalPurposeDMA();
		return false;
	}
	
	uint8_t currScanLine = bus->interrupt->io[0x44];//LY
	//uint8_t modeInterruptEnable = 0;
	uint8_t oldLCDmode = bus->interrupt->io[0x41] & 0x03;
	uint8_t mode=0;
	if (currScanLine >= 144) {//V-Blank 
		mode = 1;
		bus->interrupt->io[0x41] &= 0xFD;
		bus->interrupt->io[0x41] |= 0x01;

		if (useLockers) {
			bus->mmu->OAMLock = false;
			bus->mmu->vRamLock = false;
		}
		
			//bus->dma->generalPurposeDMA();
		
	
		/*	if (currScanLine != lastScanline) {
			bus->dma->generalPurposeDMA();
			lastScanline = currScanLine;
		}*/
		bus->display->setLock(false);
	}
	//cyclesPerScanline decrement from 456 to 0 split to 3 sections:
	else if (cyclesPerScanline>=376) {//mode 2:Searching Sprites Atts 80 [456->376] 
		mode = 2;
		if (lastScanline != currScanLine) {
			if (lastScanline == currScanLine)
				printf("lcdCtrlChanged\n");
			draw();
			lastScanline = currScanLine;
			lcdCtrlChanged = false;
		}
		bus->display->setLock(true);
		bus->interrupt->io[0x41] &= 0xFE;
		bus->interrupt->io[0x41] |= 0x02;
		if (useLockers) {
			bus->mmu->OAMLock = true;
			bus->mmu->vRamLock = false;
		}

	}
	else if (cyclesPerScanline >= 204) {//mode 3:Transfering Data to LCD Driver 172 [375->204] 
		mode = 3;
		bus->interrupt->io[0x41] |= 0x03;
		if (useLockers) {
			bus->mmu->OAMLock = true;
			bus->mmu->vRamLock = true;
		}
		bus->display->setLock(false);
	}
	else {//mode 0:H-Blank  204 [204->0)
		mode = 0;
		bus->interrupt->io[0x41] &= 0xFC;
	
		if (useLockers) {
			bus->mmu->OAMLock = false;
			bus->mmu->vRamLock = false;
		}
		if (mode != oldLCDmode) {

			bus->dma->hBlankDMA();
			//bus->dma->FF46DMATransfer();
		}
		
		bus->display->setLock(false);
	}
	//bus->interrupt->io[0x41] |= mode;
	if(mode!=oldLCDmode&& mode != 3)
	if ((bus->interrupt->io[0x41] >> (mode + 3)) & 0x01) {//checks mode interrupt requested and enabled
		bus->interrupt->setInterruptRequest(1);
	}

	if (currScanLine == bus->interrupt->io[0x45]) {//coincidence occured
		bus->interrupt->io[0x41] |= 0x4;//set coinciedence
		if ((bus->interrupt->io[0x41] >> 6) & 0x1)//coinciedence is set
			bus->interrupt->setInterruptRequest(1);
	}
	else
		bus->interrupt->io[0x41]&=0xfb;//reset coinciedence


	return true;
}
void GPU::tick()
{
	
	if (!checkLCDStatus())
		return;
	/*if (bus->cpu->cycelsCounter < 4)
		return;
	uint8_t cycels = bus->cpu->cycelsCounter / 4;*/
	
	
	cyclesPerScanline -= bus->cpu->lastOpcodeCycles;
	if (cyclesPerScanline <= 0) {
		cyclesPerScanline += 456*(bus->cpu->speedMode + 1);
		//cyclesPerScanline += 456 * (1/(bus->cpu->speedMode + 1));
		//cyclesPerScanline += 456 * (bus->cpu->speedMode + 1)*4;
		bus->interrupt->io[0x44]++;
		uint8_t currScanLine = bus->interrupt->io[0x44];
		//lastScanline = currScanLine;
		if (currScanLine == 144) {
			bus->interrupt->setInterruptRequest(0);
			//bus->display->render();
			//bus->display->setLock(false);
		}
		if (currScanLine > 153) {
			bus->interrupt->io[0x44] = 0;
			
			//bus->dma->FF46transfering = false;
			//bus->dma->generalPurposeDMA();
		}
		if (currScanLine < 144) {
			/*if (currScanLine == 0) {
			
			}*/
			
			//bus->display->setLock(true);

			//draw();
			
			//bus->dma->hBlankDMA();
			//bus->dma->FF46DMATransfer();
			
		/*	if (lastScanline != currScanLine) {
				draw();
				lastScanline = currScanLine;
			}*/
		}
		else {
			//bus->display->setLock(false);
		}

	}
	
}
void GPU::draw() {
	//bus->display->setLock(true);
	uint8_t BGpriority[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	uint32_t BGTransparents[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	uint8_t y = lastScanline;//currLine
	uint32_t* BGLine = bus->display->getLineSDL(y);

	bool render = false;
	render = drawBG(BGLine, BGpriority, BGTransparents);
	render=drawSprites(BGLine,BGpriority, BGTransparents)||render;
	//bus->display->setLock(false);
}
bool GPU::TestBit(BYTE n, int b) {
	return (n >> b) & 1;
}
int GPU::BitGetVal(BYTE n, int b) const {
	return (n >> b) & 1;
}
void GPU::getPalette(uint16_t paletteAddress, map<int, int>* palette) {
	uint8_t pletteMap = bus->mmu->read(paletteAddress);
	(*palette)[0] = pletteMap & 0x03;
	(*palette)[1] = (pletteMap>>2) & 0x03;
	(*palette)[2] = (pletteMap >> 4) & 0x03;
	(*palette)[3] = (pletteMap >> 6) & 0x03;

	/*(*palette)[pletteMap & 0x03] = pletteMap & 0x03;
	(*palette)[1] = (pletteMap >> 2) & 0x03;
	(*palette)[2] = (pletteMap >> 4) & 0x03;
	(*palette)[3] = (pletteMap >> 6) & 0x03;*/
}
void GPU::getPalette2(uint16_t paletteAddress, int * palette) {
	uint8_t pletteMap = bus->mmu->read(paletteAddress);
	  palette[0] = pletteMap & 0x03;
	palette[1] = (pletteMap >> 2) & 0x03;
	palette[2] = (pletteMap >> 4) & 0x03;
	palette[3] = (pletteMap >> 6) & 0x03;

	
}
uint8_t GPU::drawBGLine(uint8_t y, uint8_t* BGLine, uint8_t* BGcolos)
{
	map<int, int> palette;
	getPalette(0xff47, &palette);
	uint8_t offset = 0;

	for (uint8_t i = 0;i < 20;i++) {
		uint8_t index = 2 * i;
		uint8_t lsb = BGLine[index];
		uint8_t msb = BGLine[index + 1];
		uint8_t j = i * 8;
		for (int k = 0;k < 8;k++) {
			uint8_t colorPaletteDataIndex = (((msb >> (7 - k)) & 0x1) << 1) | ((lsb >> (7 - k)) & 0x1);
			uint8_t colorIndex = (bus->interrupt->io[0x47] >> (2 * colorPaletteDataIndex)) & 0x03;
			colorIndex = palette[colorPaletteDataIndex];
			//if(colorIndex==0)
				//*BGcolos |= 0x1 << (7 - (((i+k) + (offset - pixelIndex)) % 8));
			//printf("color id:%d\n",colorIndex);
			bus->display->setPixel(j + k, y, bus->display->colors[colorIndex]);
			//bus->display->setPixel(x + (offset - pixelIndex), y, bus->display->bgPalette[colorIndex]);
		}
	}

	return offset;

}
bool GPU::drawBG2(uint32_t* BGLine, uint8_t* BGpriorities, uint32_t* BGTransparents) {


	if (bus->interrupt->io[0x40] & 0x1) {//BG Display Enable
		uint16_t bgTileMap = 0x9800;//store locations of tiles to display	
		//map<int, int> palette;
		int palette[4];
		getPalette2(0xff47, palette);

		//uint8_t tileY = bus->interrupt->io[0x44];
		uint8_t tileY = lastScanline;
		uint8_t scrollY = bus->interrupt->io[0x42];
		uint8_t scrollX = bus->interrupt->io[0x43];
		uint8_t windowY = bus->interrupt->io[0x4A];
		uint8_t windowX = bus->interrupt->io[0x4B] - 7;
		Color* BGpalette = bus->display->bgPalette;
		bool windowDisplay = ((bus->interrupt->io[0x40] >> 5) & 0x1) && (tileY >= windowY);
		bool windowAdjusment;
		// windowDisplay = ((bus->interrupt->io[0x40] >> 5) & 0x1);
		if (windowDisplay) {
			tileY -= windowY;
		}
		else {
			tileY += scrollY;
		}
		/*if (tileY < 0)
			return false;*/
		if ((windowDisplay && ((bus->interrupt->io[0x40] >> 6) & 0x1)) ||//window display set and and Window Tile Map is 1 OR window not display and BG Tile Map is 1
			(!windowDisplay && ((bus->interrupt->io[0x40] >> 3) & 0x01))) {//BG & Window Tile Data Select 9c00-9fff
			bgTileMap = 0x9C00;
		}
		uint16_t tileDataTable = 0x8800;//signed indexing
		if ((bus->interrupt->io[0x40] >> 4) & 0x1) {
			tileDataTable = 0x8000;//unsigned indexing
		}

		//uint16_t  tileRow = ((uint8_t)(tileY / 8)) * 32;//number of tiles from 0 to currline. //num of tiles includes cols.
		uint16_t  tileRow = ((tileY / 8)) * 32;//number of tiles from 0 to currline. //num of tiles includes cols.
		uint16_t  tileIndexAddress;
		for (int x = 0;x < 160;) {


			uint8_t bgX = (scrollX + x);
			//int bgX = scrollX + x;
			if (scrollX + x > 250) {
				int test = 0;
			}
			windowAdjusment = false;
			if (windowDisplay && x >= windowX) {
				bgX = x - windowX;
				windowAdjusment = true;
			}
			uint8_t tileCol = bgX / 8;//number of tiles from 0 to scrollX.
			tileIndexAddress = bgTileMap + tileRow + tileCol;
			uint16_t tileAddress;
			if (tileDataTable == 0x8800) {
				uint8_t tileIndex = (int8_t)vRam[tileIndexAddress - 0x8000] + 128;
				tileAddress = 0x800 + tileIndex * 16;
			}
			else {
				uint8_t tileIndex = vRam[tileIndexAddress - 0x8000];
				tileAddress = (tileIndex) * 16;

			}


			//uint8_t yDisplay = bus->interrupt->io[0x44];//currLine
			//uint8_t tileLine = (tileY % 8) * 2;//each  bytes are a line
			uint8_t tileLine = (tileY % 8) * 2;
			uint16_t vRamBank = 0;

			uint8_t attributes = vRam[tileIndexAddress - 0x6000];//tile attributes in vRam bank 1
			bool priority;
			bool Xflip = false;
			if (bus->cartridge->header.colorGB) {

				if ((attributes >> 3) & 0x01)
					vRamBank = 0x2000;
				//Horizontal Flip
				if ((attributes >> 5) & 0x01) {
					bgX = 7 - bgX;
					//Xflip = true;
				}
				//Vertical Flip 
				if ((attributes >> 6) & 0x01)
					tileLine = ((7 - tileY) % 8) * 2;
				else
					tileLine = (tileY % 8) * 2;

				BGpalette = bus->display->BGPlatettes[attributes & 0x07];
				//priority = (attributes >> 7)&0x01 ? true : false;
				if ((attributes >> 7) & 0x01) {
					BGpriorities[x / 8] |= 0xff >> (x % 8);
					if (x % 8 != 0 && (x / 8) + 1 < 20)
						BGpriorities[(x / 8) + 1] |= (0xff << (7 - (x % 8)));
				}
				BGTransparents[x / 8] = BGpalette[0].val;
			}

			/*uint8_t lsbData = bus->mmu->read(tileAddress+ tileLine);
			uint8_t msbData = bus->mmu->read(tileAddress + tileLine+1);*/
			uint8_t lsbData = vRam[tileAddress + tileLine + vRamBank];
			uint8_t msbData = vRam[tileAddress + tileLine + 1 + vRamBank];
			/*if (Xflip) {

				lsbData = reverseByteLockup[lsbData];
				msbData = reverseByteLockup[msbData];
			}*/

			uint8_t pixelIndex = bgX % 8;
			//uint8_t lineBlock = 2 * (x / 8);

			//BGLineTile[lineBlock] |=((lsbData>> (7 - (x % 8)))&0x1) << (7 - (x % 8));
			//BGLineTile[lineBlock+1] |= ((msbData >> (7 - (x % 8))) & 0x1) << (7 - (x % 8));


			/*uint8_t colorPaletteDataIndex = (((msbData >> (7 - pixelIndex)) & 0x1) << 1) | ((lsbData >> (7 - pixelIndex)) & 0x1);
			uint8_t  colorIndex = palette[colorPaletteDataIndex];

			BGLine[x] = bus->display->bgPalette[colorIndex].val;
			x++;
			*/
			/*x++;
			continue;*/
			int k = pixelIndex;
			uint8_t shiftBit;
			int restoredX = 0;
			int8_t direction = 1;
			/*if (Xflip) {
				restoredX = x;
				x = 160 - x;
				direction = -1;
			}*/

			/*for (k = pixelIndex;k < 8&&x<160&&x>=0;k++) {
				shiftBit = 7 - k;
				uint8_t colorPaletteDataIndex = (((msbData >> shiftBit) & 0x1) << 1) | ((lsbData >> shiftBit) & 0x1);
				uint8_t  colorIndex = palette[colorPaletteDataIndex];
				BGLine[x] = BGpalette[colorIndex].val;
				x+=direction;
				if (windowDisplay && x >= windowX) {
					if(!windowAdjusment)
					break;
				}

			}*/
			int p = x;
			for (k = pixelIndex;k < 8;k++) {
				shiftBit = 7 - k;
				uint8_t colorPaletteDataIndex = (((msbData >> shiftBit) & 0x1) << 1) | ((lsbData >> shiftBit) & 0x1);
				uint8_t  colorIndex = palette[colorPaletteDataIndex];
				BGLine[p % 160] = BGpalette[colorIndex].val;
				p++;
				x += direction;
				if (windowDisplay && x >= windowX) {
					if (!windowAdjusment)
						break;
				}

			}
			/*if (Xflip) {
				x = restoredX + ((160 - restoredX)-x);
			}*/

		}

		//bus->display->drawBGLine(y,BGLine);
		return true;
	}
	return false;
}

bool GPU::drawBG(uint32_t* BGLine, uint8_t* BGpriorities, uint32_t* BGTransparents) {
	
	
	if (bus->interrupt->io[0x40] & 0x1) {//BG Display Enable
		uint16_t bgTileMap = 0x9800;//store locations of tiles to display	
		//map<int, int> palette;
		int palette[4];
		getPalette2(0xff47, palette);
		
		//uint8_t tileY = bus->interrupt->io[0x44];
		uint8_t tileY = lastScanline;
		uint8_t scrollY = bus->interrupt->io[0x42];
		uint8_t scrollX = bus->interrupt->io[0x43];
		uint8_t windowY = bus->interrupt->io[0x4A];
		uint8_t windowX = bus->interrupt->io[0x4B] - 7;
		Color* BGpalette= bus->display->bgPalette;
		bool windowDisplay = ((bus->interrupt->io[0x40] >> 5) & 0x1)&&(tileY>=windowY);
		//bool windowDisplay = ((bus->interrupt->io[0x40] >> 5) & 0x1)&&(143<= windowY&&159<=windowX&&windowX>=0);
		bool windowAdjusment;
		// windowDisplay = ((bus->interrupt->io[0x40] >> 5) & 0x1);
		if (windowDisplay) {
			tileY -= windowY;
		}
		else {
			tileY += scrollY;
		}
		if (bus->interrupt->io[0x43] < 0 || bus->interrupt->io[0x43]>159) {
			int test = 0;
		}

		/*if (tileY < 0)
			return false;*/
		if ((windowDisplay && ((bus->interrupt->io[0x40] >> 6) & 0x1)) ||//window display set and and Window Tile Map is 1 OR window not display and BG Tile Map is 1
			(!windowDisplay && ((bus->interrupt->io[0x40] >> 3) & 0x01))) {//BG & Window Tile Data Select 9c00-9fff
			bgTileMap = 0x9C00;
		}
		uint16_t tileDataTable = 0x8800;//signed indexing
		if ((bus->interrupt->io[0x40] >> 4) & 0x1) {
			tileDataTable = 0x8000;//unsigned indexing
		}
		
		//uint16_t  tileRow = ((uint8_t)(tileY / 8)) * 32;//number of tiles from 0 to currline. //num of tiles includes cols.
		uint16_t  tileRow = ((tileY / 8)) * 32;//number of tiles from 0 to currline. //num of tiles includes cols.
		uint16_t  tileIndexAddress;

		for (int x = 0;x < 160;x++) {
			
			
			uint8_t bgX = (scrollX + x);
			//int bgX = scrollX + x;
			if (bgX > 159) {
				//x++;
				//continue;
				
			}
			if (scrollX + x > 159) {
				
				//bgX = bgX % 256;
				//continue;
				int test = 0;
			}
			windowAdjusment = false;
			/*if (windowDisplay && x >= windowX) {
				bgX = x - windowX;
				windowAdjusment = true;
			}*/
			if (windowDisplay) {
				bgX = x - windowX;
				windowAdjusment = true;
			}


			/*else {
				continue;
			}*/
			uint8_t tileCol = bgX / 8;//number of tiles from 0 to scrollX.
			tileIndexAddress = bgTileMap+ tileRow+ tileCol;
			uint16_t tileAddress;
			uint8_t tileIndex;
			if (tileDataTable == 0x8800) {
				 tileIndex =(int8_t)vRam[tileIndexAddress -0x8000]+128;
				 tileAddress = 0x800 + tileIndex*16;
			}
			else {
				tileIndex = vRam[tileIndexAddress - 0x8000];
				tileAddress =   (tileIndex)*16;
				
			}

		
			//uint8_t yDisplay = bus->interrupt->io[0x44];//currLine
			//uint8_t tileLine = (tileY % 8) * 2;//each  bytes are a line
			uint8_t tileLine = (tileY % 8) * 2;
			uint16_t vRamBank = 0;
			
			uint8_t attributes = vRam[tileIndexAddress -0x6000];//tile attributes in vRam bank 1
			bool priority;
			bool Xflip = false;
			if (bus->cartridge->header.colorGB) {
			
				if((attributes>>3)&0x01)
					vRamBank = 0x2000;
				//Horizontal Flip
				if ((attributes >> 5) & 0x01) {
					bgX = 7 - bgX;
					//Xflip = true;
				}
				//Vertical Flip 
				if ((attributes >> 6) & 0x01) 
					tileLine = ((7-tileY) % 8) * 2;
				else
					tileLine=(tileY % 8) * 2;
				
				BGpalette = bus->display->BGPlatettes[attributes&0x07];
				//priority = (attributes >> 7)&0x01 ? true : false;
				
				if ((attributes >> 7) & 0x01) {
						BGpriorities[x / 8] |= 0xff>>(x%8);
					if(x % 8 != 0&&(x / 8) + 1<20)
						BGpriorities[(x / 8)+1] |= (0xff << (7-(x % 8)));
				}
				BGTransparents[x / 8] = BGpalette[0].val;
			}
			
			/*uint8_t lsbData = bus->mmu->read(tileAddress+ tileLine);
			uint8_t msbData = bus->mmu->read(tileAddress + tileLine+1);*/
			uint8_t lsbData =vRam[tileAddress + tileLine + vRamBank];
			uint8_t msbData = vRam[tileAddress + tileLine + 1 + vRamBank];
			/*if (Xflip) {

				lsbData = reverseByteLockup[lsbData];
				msbData = reverseByteLockup[msbData];
			}*/
			//bgX = bgX % 256;
			uint8_t pixelIndex = bgX % 8;
			//uint8_t lineBlock = 2 * (x / 8);

			//BGLineTile[lineBlock] |=((lsbData>> (7 - (x % 8)))&0x1) << (7 - (x % 8));
			//BGLineTile[lineBlock+1] |= ((msbData >> (7 - (x % 8))) & 0x1) << (7 - (x % 8));


			/*uint8_t colorPaletteDataIndex = (((msbData >> (7 - pixelIndex)) & 0x1) << 1) | ((lsbData >> (7 - pixelIndex)) & 0x1);
			uint8_t  colorIndex = palette[colorPaletteDataIndex];
			
			BGLine[x] = bus->display->bgPalette[colorIndex].val;
			x++;
			*/
			/*x++;
			continue;*/

			if (tileY == 0x38 && tileIndex == 0x15 && tileAddress == 0x8150) {
				int test = 0;
			}
			if (tileAddress + tileLine + vRamBank+0x8000== 0x8150) {
				int test = 0;
			}
			int k = pixelIndex;
			uint8_t shiftBit;
			int restoredX = 0;
			int8_t direction = 1;
			/*if (Xflip) {
				restoredX = x;
				x = 160 - x;
				direction = -1;
			}*/
			
			/*for (k = pixelIndex;k < 8&&x<160&&x>=0;k++) {
				shiftBit = 7 - k;
				uint8_t colorPaletteDataIndex = (((msbData >> shiftBit) & 0x1) << 1) | ((lsbData >> shiftBit) & 0x1);
				uint8_t  colorIndex = palette[colorPaletteDataIndex];
				BGLine[x] = BGpalette[colorIndex].val;
				x+=direction;
				if (windowDisplay && x >= windowX) {
					if(!windowAdjusment)
					break;
				}
				
			}*/
			int p = x;
			for (k = pixelIndex;k <= pixelIndex ;k++) {
				shiftBit = 7 - k;
				uint8_t colorPaletteDataIndex = (((msbData >> shiftBit) & 0x1) << 1) | ((lsbData >> shiftBit) & 0x1);
				uint8_t  colorIndex = palette[colorPaletteDataIndex];
				BGLine[p] = BGpalette[colorIndex].val;
				p++;
				//x += direction;
				/*if (windowDisplay && x >= windowX) {
					if (!windowAdjusment)
						break;
				}*/

			}
			/*if (Xflip) {
				x = restoredX + ((160 - restoredX)-x);
			}*/
			
		}
		
		//bus->display->drawBGLine(y,BGLine);
		return true;
	}
	else {
		if (bus->consoleModel == ConsoleModel::DMG||!bus->cartridge->header.colorGB) {
		
		}
		else {
		
		}
}
	return false;
}
bool GPU::drawSprites(uint32_t* BGLine, uint8_t* BGpriorities, uint32_t* BGTransparents) {
	if ((bus->interrupt->io[0x40] >> 1) & 0x1) {//OBJ (Sprite) Display Enable
		uint8_t spriteSize = ((bus->interrupt->io[0x40] >> 2) & 0x01) ? 16 : 8;
		//uint currentScanLine = bus->interrupt->io[0x44];
		uint currentScanLine = lastScanline;
		uint32_t transparent = bus->display->bgPalette[0].val;
		Color* OBpalette= bus->display->bgPalette;
		uint16_t vRamBankOffset = 0;
		uint8_t bgPriority = 0x0;
		int palette[4] = {0x0,0x1,0x2,0x3};
		for (uint8_t i = 0;i < 160;i += 4) {//loop 4 byte(sprite) in OAM
			uint8_t posY = oam[i] - 16;//if there are any bugs with drawing consider to change uint8_t to int16.
			
									   //if(posY <= currentScanLine && posY + spriteSize >= currentScanLine) {//check if the sprite is crossing the current scanline
				uint8_t posX = oam[i + 1] - 8;
				/*if (posX < 0 || posX>160 || posY > currentScanLine || posY + spriteSize <= currentScanLine)
					continue;*/
				if (posY > currentScanLine|| posY + spriteSize <= currentScanLine)
					continue;
				
				uint16_t patternNum = oam[i + 2];
				if (spriteSize == 16)
					patternNum &= 0xfffe;
				
				uint8_t attributes = oam[i + 3];
				uint8_t priority = attributes >> 7 & 0x01;
				
				bool Yflip = (attributes >> 6) & 0x01;
				bool Xflip = (attributes >> 5) & 0x01;
				uint8_t spriteDataLine;
				if (!Yflip)
					spriteDataLine = currentScanLine - posY;
				else
					spriteDataLine = spriteSize - 1 - (currentScanLine - posY);
				
				if (bus->cartridge->header.colorGB) {
					vRamBankOffset = (attributes >> 3)&0x01 ? 0x2000 : 0;
					OBpalette = bus->display->OBPlatettes[attributes&0x07];
					//transparent = OBpalette[0].val;
					transparent = BGTransparents[posX / 8];
					if (posX < 160 && posX >= 0) {
						bgPriority = BGpriorities[posX / 8] >> (posX % 8);
						if (posX % 8 != 0)
							bgPriority |= BGpriorities[(posX / 8) + 1] << (7 - (posX % 8));
					}
				//OBpalette=bus
				}
				else {
					uint16_t paletteAddress = ((attributes >> 4) & 0x01) ? 0xff49 : 0xff48;
					getPalette2(paletteAddress, palette);
				}

				

				uint16_t tileAddress =  16 * (patternNum)+2 * spriteDataLine+vRamBankOffset;
				uint8_t lsbData =vRam[tileAddress];
				uint8_t msbData = vRam[tileAddress + 1];
				
				
				
				if (Xflip) {
					
					lsbData = reverseByteLockup[lsbData];
					msbData = reverseByteLockup[msbData];
				}
				if (posX>= 160|| posX<0)
					int b = 0;
				//posX = posX % 160;
				for(uint8_t x = 0;x <8;x++) {
					if (posX + x >= 160) {
						continue;
					}
					//uint8_t isBgWinWhitePixel = (currBGLine[(posX + x) / 8] >> (7 - (posX + x) % 8)) & 0x01;
					
					if ((!priority&&!((bgPriority >> (7 - x)) & 0x01))|| BGLine[posX + x] == transparent) {//|| (!priority )&& 
						uint8_t colorPaletteDataIndex = (((msbData >> (7 - x)) & 0x1) << 1) | ((lsbData >> (7 - x)) & 0x1);
						uint8_t  colorIndex = palette[colorPaletteDataIndex];
						//BGLine[x + k] = bus->display->bgPalette[colorIndex].getHex();
						
						
						if (colorPaletteDataIndex != 0)
							BGLine[posX + x] = OBpalette[colorIndex].val;
						//BGLine[posX + x]= bus->display->spritesPalette[colorIndex].getHex();
					}
						//setPixel(posX + x, currentScanLine, lsbData, msbData, x, palette);
				}
			//}
		}
		return true;
	}
	return false;

	
}


//uint8_t GPU::setPixel(int x, int y, uint8_t lsb, uint8_t msb,uint8_t pixelIndex, map<int, int> palette)
//{	
//	//uint8_t offset = 0;
//	//for (offset = pixelIndex;offset < 8 && (offset + x) < 160;offset++) {
//		uint8_t colorPaletteDataIndex = (((msb >> (7 - pixelIndex)) & 0x1) << 1) | ((lsb >> (7 - pixelIndex)) & 0x1);
//		uint8_t colorIndex = (bus->interrupt->io[0x47] >> (2 * colorPaletteDataIndex)) & 0x03;
//		colorIndex = palette[colorPaletteDataIndex];
//		//printf("color id:%d\n",colorIndex);
//	 	//bus->display->setPixel(x+ (offset- pixelIndex), y, bus->display->colors[colorIndex]);
//		//bus->display->setPixel(x , y, bus->display->bgPalette[colorIndex]);
//		bus->display->setPixel(x, y, bus->display->colors[colorIndex]);
//	//}
//	
//		return colorIndex;
//}
uint8_t GPU::setPixel(int x, int y, uint8_t lsb, uint8_t msb, uint8_t pixelIndex, map<int, int> palette,bool BGTile,uint8_t *  BGcolos)
{
	uint8_t offset = 0;
	if(BGTile)
	for (offset = pixelIndex;offset < 8 && (offset + x) < 160;offset++) {
		uint8_t colorPaletteDataIndex = (((msb >> (7 - offset)) & 0x1) << 1) | ((lsb >> (7 - offset)) & 0x1);
		uint8_t colorIndex = (bus->interrupt->io[0x47] >> (2 * colorPaletteDataIndex)) & 0x03;
		colorIndex = palette[colorPaletteDataIndex];
		*BGcolos|= 0x1 << (7 - ((x + (offset - pixelIndex)) % 8));
		//printf("color id:%d\n",colorIndex);
		bus->display->setPixel(x+ (offset- pixelIndex), y, bus->display->colors[colorIndex]);
		//bus->display->setPixel(x + (offset - pixelIndex), y, bus->display->bgPalette[colorIndex]);
	}
	else {
		uint8_t colorPaletteDataIndex = (((msb >> (7 - pixelIndex)) & 0x1) << 1) | ((lsb >> (7 - pixelIndex)) & 0x1);
		uint8_t colorIndex = (bus->interrupt->io[0x47] >> (2 * colorPaletteDataIndex)) & 0x03;
		colorIndex = palette[colorPaletteDataIndex];
		//printf("color id:%d\n",colorIndex);
		//bus->display->setPixel(x+ (offset- pixelIndex), y, bus->display->colors[colorIndex]);
		//bus->display->setPixel(x , y, bus->display->bgPalette[colorIndex]);
		bus->display->setPixel(x, y, bus->display->colors[colorIndex]);
	}
	return offset;

}

void GPU::drawTest()
{
	uint8_t heart[16] = {
		0x66,0x66, 0x99,0xff,
		0x81,0xff,0x81,0xff,
		0x42,0x7e,0x24,0x3c,
		0x18,0x18,0x0,0x0 };
	int x = 0;
	int y = 0;
	
	bool flipy = false;
	int vflip = flipy?14:0;
	for (;y < 8;y++) {

		uint8_t lsb = heart[!flipy?(2 * y):(15-2*y)];
		uint8_t msb = heart[!flipy ? (2 * y+1) : (15 - (2 * y+1))];
		for (x = 0;x < 8;x++) {
			uint8_t colorIndex = (((msb >> (7 - x)) & 0x1) << 1) |( (lsb >> (7 - x))&0x1);
			//bus->display->setPixel(x, y, bus->display->colors[colorIndex]);
			bus->display->setPixel(x, y, bus->display->bgPalette[colorIndex]);
		}
	}
	
}


