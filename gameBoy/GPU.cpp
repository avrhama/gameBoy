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
	
	if (!((bus->interrupt->io[0x40] >> 7) & 0x1)) {//LCD Display Disable
		//bus->interrupt->io[0x41] &= 252;//web
		//bus->interrupt->io[0x41] &= 253;//me
		bus->interrupt->io[0x41] &= 0xFD;
		bus->interrupt->io[0x41] |= 0x01;
		bus->interrupt->io[0x44] = 0;
		cyclesPerScanline = 456;
		/*bus->mmu->vRamLock = false;
		bus->mmu->OAMLock = false;*/
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
	/*	bus->mmu->OAMLock = false;
		bus->mmu->vRamLock = false;*/
	}
	//cyclesPerScanline decrement from 456 to 0 split to 3 sections:
	//mode 2:Searching Sprites Atts 80 [456->376] 
	//mode 3:Transfering Data to LCD Driver 172 [375->204] 
	//mode 0:H-Blank  204 [204->0)
	else if (cyclesPerScanline>=376) {
		mode = 2;
		bus->interrupt->io[0x41] &= 0xFE;
		bus->interrupt->io[0x41] |= 0x02;
		/*bus->mmu->OAMLock = true;
		bus->mmu->vRamLock = false;*/
	}
	else if (cyclesPerScanline >= 204) {
		mode = 3;
		bus->interrupt->io[0x41] |= 0x03;
		/*bus->mmu->OAMLock = true;
		bus->mmu->vRamLock = true;*/
	}
	else {
		mode = 0;
		bus->interrupt->io[0x41] &= 0xFC;
		/*bus->mmu->OAMLock = false;
		bus->mmu->vRamLock = false;*/
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
		bus->interrupt->io[0x44]++;
		uint8_t currScanLine = bus->interrupt->io[0x44];
		if (currScanLine == 144) {
			bus->interrupt->setInterruptRequest(0);
			//bus->display->render();
		}
		if (currScanLine > 153) 
			bus->interrupt->io[0x44] = 0;
		if (currScanLine < 144) {
			draw();
			
		}

	}
	
	if (bus->pipeEnable) {
		bus->p->read(8);
		uint16_t otherPC = bus->p->rBuffer[1] << 8 | bus->p->rBuffer[0];
		uint32_t othercyclesPerScanline = bus->p->rBuffer[5] << 24 | bus->p->rBuffer[4] << 16 | bus->p->rBuffer[3] << 8 | bus->p->rBuffer[2];
		uint8_t othercycles = bus->p->rBuffer[6];
		uint8_t otherIO0x44 = bus->p->rBuffer[7];
		bus->p->wBuffer[0] = 0;
		int errorCode = -1;
		
	     if (otherPC != bus->cpu->PC) {
			errorCode = 1;
		}
		else if (othercyclesPerScanline != cyclesPerScanline) {
			errorCode = 2;
		}
		else if (othercycles != bus->cpu->lastOpcodeCycles) {
			errorCode = 3;
		}
		else if (otherIO0x44 != bus->interrupt->io[0x44]){
			errorCode = 4;
		}
		
		
		if (errorCode!=-1)
		{
			bus->p->wBuffer[0] = 1;
		}
		bus->p->write();
		if (bus->p->wBuffer[0] == 1) {
			bus->p->wBuffer[1] = 0;
		}

	}
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
bool GPU::drawBG(uint32_t* BGLine) {
	
	if (bus->interrupt->io[0x40] & 0x1) {//BG Display Enable
		uint16_t bgTileMap = 0x9800;//store locations of tiles to display	
		map<int, int> palette;
		getPalette(0xff47, &palette);
		//uint8_t BGLineTile[40] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		uint8_t tileY = bus->interrupt->io[0x44];
		uint8_t scrollY = bus->interrupt->io[0x42];
		uint8_t scrollX = bus->interrupt->io[0x43];
		uint8_t windowY = bus->interrupt->io[0x4A];
		uint8_t windowX = bus->interrupt->io[0x4B] - 7;
		bool windowDisplay = ((bus->interrupt->io[0x40] >> 5) & 0x1)&&(tileY>=windowY);
		// windowDisplay = ((bus->interrupt->io[0x40] >> 5) & 0x1);
		if (windowDisplay) {
			/*int p= tileY - windowY;
			
			p = p % 144;
			p = p > 0 ? p : p + 1*/44;
			//tileY = p;
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
		for (int x = 0;x < 160;) {
			uint8_t bgX = scrollX + x;
			if (windowDisplay&& x >= windowX)
					bgX = x - windowX;
			uint8_t tileCol = bgX / 8;//number of tiles from 0 to scrollX.
			
			uint16_t tileAddress;
			if (tileDataTable == 0x8800) {
				uint8_t tileIndex =(int8_t)vRam[bgTileMap + tileCol + tileRow-0x8000]+128;
				tileAddress = 0x800 + 16 * tileIndex;
			}
			else {
				uint8_t tileIndex = vRam[bgTileMap + tileCol + tileRow - 0x8000];
				tileAddress =  16 * (tileIndex);
			}

		
			//uint8_t yDisplay = bus->interrupt->io[0x44];//currLine
			//uint8_t tileLine = (tileY % 8) * 2;//each  bytes are a line
			uint8_t tileLine = (tileY % 8) * 2;
			uint16_t vRamBank = 0;
			
			uint8_t attributes = vRam[bgTileMap + (tileCol + tileRow)-0x6000];
			if (bus->cartridge->colorGB) {
			
				if((attributes>>3)&0x01)
					vRamBank = 0x2000;
				if ((attributes >> 5) & 0x01)
					bgX = 7 - bgX;
				if ((attributes >> 6) & 0x01) 
					tileLine = ((7-tileY) % 8) * 2;
				else
					tileLine=(tileY % 8) * 2;
			}
			
			/*uint8_t lsbData = bus->mmu->read(tileAddress+ tileLine);
			uint8_t msbData = bus->mmu->read(tileAddress + tileLine+1);*/
			uint8_t lsbData =vRam[tileAddress + tileLine + vRamBank];
			uint8_t msbData = vRam[tileAddress + tileLine + 1 + vRamBank];
			uint8_t pixelIndex = bgX % 8;
			uint8_t lineBlock = 2 * (x / 8);

			//BGLineTile[lineBlock] |=((lsbData>> (7 - (x % 8)))&0x1) << (7 - (x % 8));
			//BGLineTile[lineBlock+1] |= ((msbData >> (7 - (x % 8))) & 0x1) << (7 - (x % 8));


			uint8_t colorPaletteDataIndex = (((msbData >> (7 - pixelIndex)) & 0x1) << 1) | ((lsbData >> (7 - pixelIndex)) & 0x1);
			uint8_t  colorIndex = palette[colorPaletteDataIndex];
			if (pixelIndex + x >= 160)
				int u = 0;
			BGLine[x] = bus->display->bgPalette[colorIndex].val;
			x++;
			/*int k = pixelIndex;
			for (k = pixelIndex;k < 8;k++) {
				uint8_t colorPaletteDataIndex = (((msbData >> (7 - k)) & 0x1) << 1) | ((lsbData >> (7 - k)) & 0x1);
				uint8_t  colorIndex = palette[colorPaletteDataIndex];
				if (k + x >= 160)
					int u = 0;
				BGLine[x+k] = bus->display->bgPalette[colorIndex].getHex();
				
			}
			x += k;*/

			//uint8_t offset =setPixel(x, yDisplay, lsbData, msbData, pixelIndex,palette,true, currBGLine+(x / 8));//x: the leftmost x tile pos[0-159],pixelIndex: pos of pixel in the tile[0-7]
			//uint8_t colorIndex = setPixel(x, yDisplay, lsbData, msbData, pixelIndex, palette);//x: the leftmost x tile pos[0-159],pixelIndex: pos of pixel in the tile[0-7]
			//x += (offset - pixelIndex);
			//x++;
			/*continue;
			if (colorIndex == 0) {//set the bit if the pixel color is white.
				int  index_ = (x + pixelIndex) / 8;

				if (index_ > 19) {
					int p = 0;
				}
				int  c = currBGLine[x / 8] | 0x1 << (7 - (x% 8));
				if (c > 0xff) {
					int p = 0;
				}

				currBGLine[x / 8] |= 0x1 << (7 - (x % 8));
			}
			x++;*/
		}
		
		//bus->display->drawBGLine(y,BGLine);
		return true;
	}
	return false;
}
uint8_t GPU::drawBGLine(uint8_t y,uint8_t* BGLine, uint8_t* BGcolos)
{
	map<int, int> palette;
	getPalette(0xff47, &palette);
	uint8_t offset = 0;
	
	for (uint8_t i = 0;i < 20;i++) {
		uint8_t index = 2 * i;
		uint8_t lsb = BGLine[index];
		uint8_t msb = BGLine[index +1];
		uint8_t j = i * 8;
		for (int k = 0;k < 8 ;k++) {
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

bool GPU::drawSprites(uint32_t* BGLine) {
	if ((bus->interrupt->io[0x40] >> 1) & 0x1) {//OBJ (Sprite) Display Enable
		uint8_t spriteSize = ((bus->interrupt->io[0x40] >> 2) & 0x01) ? 16 : 8;
		uint currentScanLine = bus->interrupt->io[0x44];
		uint32_t transparent = bus->display->bgPalette[0].getHex();
		for (uint8_t i = 0;i < 160;i += 4) {//loop 4 byte(sprite) in OAM
			uint8_t posY = oam[i] - 16;//if there are any bugs with drawing consider to change uint8_t to int16.
			//if(posY <= currentScanLine && posY + spriteSize >= currentScanLine) {//check if the sprite is crossing the current scanline
				uint8_t posX = oam[i + 1] - 8;
				/*if (posX < 0 || posX>160 || posY > currentScanLine || posY + spriteSize <= currentScanLine)
					continue;*/
				if (posY > currentScanLine || posY + spriteSize <= currentScanLine)
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
				uint16_t paletteAddress = ((attributes >> 4) & 0x01)?0xff49:0xff48;
				//uint16_t paletteAddress = ((attributes >> 4) & 0x01) ? 0xff48 : 0xff49;

				map<int, int> palette;
				getPalette(paletteAddress, &palette);
				uint16_t tileAddress =  16 * (patternNum)+2 * spriteDataLine;
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
					
					if (!priority || BGLine[posX +x] == transparent) {
						uint8_t colorPaletteDataIndex = (((msbData >> (7 - x)) & 0x1) << 1) | ((lsbData >> (7 - x)) & 0x1);
						uint8_t  colorIndex = palette[colorPaletteDataIndex];
						//BGLine[x + k] = bus->display->bgPalette[colorIndex].getHex();
						
						if (posX + x >= 160)
							int u = 0;
						if (colorPaletteDataIndex != 0)
							BGLine[posX + x] = bus->display->bgPalette[colorIndex].val;
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

void GPU::draw() {
	bus->display->setLock(true);
	//uint8_t currBGLine[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	//uint32_t* BGLine[160];
	uint8_t y = bus->interrupt->io[0x44];//currLine
	//uint32_t * BGLine= bus->display->getLineCV(y);
	uint32_t* BGLine = bus->display->getLineSDL(y);
	//uint8_t currBGLine[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	bool render = false;
	//render=drawBG(BGLine);
	render=drawSprites(BGLine)||render;
	bus->display->setLock(false);
	
	if (bus->cpu->speed == -1) {
		//bus->display->update();
		//printf("line: %d\n", y);
		//Sleep(30);
	}
	

	//bus->display->update();
	//uint8_t currScanLine = bus->mmu->read(0xFF44);
	//for (uint8_t i = 0;i < 160;i+=4) {//loop 4 byte(sprite) in OAM
	//	uint8_t posY = bus->mmu->read(0xFE00+i);
	//	uint8_t posX = bus->mmu->read(0xFE00 + i+1);
	//	uint8_t tileNum = bus->mmu->read(0xFE00 + i + 2);
	//	uint8_t flags = bus->mmu->read(0xFE00 + i + 3);
	//	if(posY+posX+tileNum+flags>0)
	//	int x = 0;
	//}
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


