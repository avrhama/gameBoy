#include "GPU.h"
#include "pipeChannel.h"
void GPU::reset()
{
	for (int i = 0;i < 0x2000;i++)
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
		bus->interrupt->io[0x41] &= 0xFC;
		bus->interrupt->io[0x41] |= 0x01;
		bus->interrupt->io[0x44] = 0;
		cyclesPerScanline = 456;
		return false;
	}
	
	uint8_t currScanLine = bus->interrupt->io[0x44];//LY
	//uint8_t modeInterruptEnable = 0;
	uint8_t oldLCDmode = bus->interrupt->io[0x41] & 0x03;
	uint8_t mode=0;
	if (currScanLine >= 144) {//V-Blank 
		mode = 1;
		bus->interrupt->io[0x41] &= 0xFC;
	}
	//cyclesPerScanline decrement from 456 to 0 split to 3 sections:
	//mode 2:Searching Sprites Atts 80 [456->376] 
	//mode 3:Transfering Data to LCD Driver 172 [375->204] 
	//mode 0:H-Blank  204 [204->0)
	else if (cyclesPerScanline>=376) {
		mode = 2;
		bus->interrupt->io[0x41] &= 0xFC;		
	}
	else if (cyclesPerScanline >= 204) {
		mode = 3;
		bus->interrupt->io[0x41] &= 0xFC;
	}
	else {
		mode = 0;
		bus->interrupt->io[0x41] &= 0xFC;
	
	}
	bus->interrupt->io[0x41] |= mode;
	if(mode!=oldLCDmode)
	if ((mode!=3) && ((bus->interrupt->io[0x41] >> (mode + 3)) & 0x01)) {//checks mode interrupt requested and enabled
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
	while (cyclesPerScanline <= 0) {
		cyclesPerScanline += 456;// *(bus->cpu->speedMode + 1);
		bus->interrupt->io[0x44]++;
		uint8_t currScanLine = bus->interrupt->io[0x44];
		if (currScanLine == 144) {
			bus->interrupt->setInterruptRequest(0);
			//bus->display->render();
		}
		if (currScanLine > 153)
			bus->interrupt->io[0x44] = 0;
		if (currScanLine < 144)
			draw();
			//drawTest();

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
ColorName GPU::GetColour(BYTE colourNum, WORD address) const
{
	ColorName res = WHITE;
	BYTE palette = bus->mmu->read(address);
	int hi = 0;
	int lo = 0;

	// which bits of the colour palette does the colour id map to?
	switch (colourNum)
	{
	case 0: hi = 1; lo = 0;break;
	case 1: hi = 3; lo = 2;break;
	case 2: hi = 5; lo = 4;break;
	case 3: hi = 7; lo = 6;break;
	}

	// use the palette to get the colour
	int colour = 0;
	colour = BitGetVal(palette, hi) << 1;
	colour |= BitGetVal(palette, lo);

	// convert the game colour to emulator colour
	switch (colour)
	{
	case 0: res = WHITE;break;
	case 1: res = LIGHT_GRAY;break;
	case 2: res = DARK_GRAY;break;
	case 3: res = BLACK;break;
	}

	return res;
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
void GPU::drawBG(uint8_t * currBGLine) {
	//uint8_t currBGLine[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	map<int, int> palette;
	getPalette(0xff47,&palette);
	if (bus->interrupt->io[0x40] & 0x1) {//BG Display Enable
		uint16_t bgTileMap = 0x9800;//store locations of tiles to display	
		
		bool windowDisplay = (bus->interrupt->io[0x40] >> 5) & 0x1;
		if ((windowDisplay && ((bus->interrupt->io[0x40] >> 6) & 0x1)) ||//window display set and and Window Tile Map is 1 OR window not display and BG Tile Map is 1
			(!windowDisplay && ((bus->interrupt->io[0x40] >> 3) & 0x01))) {//BG & Window Tile Data Select 9c00-9fff
			bgTileMap = 0x9C00;
		}
		uint16_t tileDataTable = 0x8800;//signed indexing
		if ((bus->interrupt->io[0x40] >> 4) & 0x1) {
			tileDataTable = 0x8000;//unsigned indexing
		}
		

		uint8_t tileY = bus->interrupt->io[0x44];
		uint8_t scrollY = bus->mmu->read(0xFF42);
		uint8_t scrollX = bus->mmu->read(0xFF43);
		uint8_t windowY = bus->mmu->read(0xFF4A);
		uint8_t windowX = bus->mmu->read(0xFF4B) - 7;
		if (windowDisplay)
			tileY -= windowY;
		else
			tileY += scrollY;

		uint16_t  tileRow = ((uint8_t)(tileY / 8)) * 32;//number of tiles from 0 to currline. //num of tiles includes cols.

		for (int x = 0;x < 160;) {
			uint8_t bgX = scrollX + x;
			if (windowDisplay)
				if (x >= windowX)
					bgX = x - windowX;
			uint8_t tileCol = bgX / 8;//number of tiles from 0 to scrollX.
			
			uint16_t tileAddress;
			if (tileDataTable == 0x8800) {
				uint8_t tileIndex =(int8_t)bus->mmu->read(bgTileMap + (tileCol + tileRow))+128;
				tileAddress = 0x8800 + 16 * tileIndex;
			}
			else {
				uint8_t tileIndex = bus->mmu->read(bgTileMap + (tileCol + tileRow));
				tileAddress = 0x8000 + 16 * (tileIndex);
			}

			uint8_t yDisplay = bus->interrupt->io[0x44];//currLine
			uint8_t tileLine = (tileY % 8) * 2;//each  bytes are a line
			uint8_t lsbData = bus->mmu->read(tileAddress+ tileLine);
			uint8_t msbData = bus->mmu->read(tileAddress + tileLine+1);
			uint8_t pixelIndex = bgX % 8;
			
			uint8_t colorIndex=setPixel(x, yDisplay, lsbData, msbData, pixelIndex,palette);//x: the leftmost x tile pos[0-159],pixelIndex: pos of pixel in the tile[0-7]
			//x += (offset - pixelIndex);
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
			x++;
		}
	}
}
void GPU::drawSprites(uint8_t* currBGLine) {
	if ((bus->interrupt->io[0x40] >> 1) & 0x1) {//OBJ (Sprite) Display Enable
		uint8_t spriteSize = ((bus->interrupt->io[0x40] >> 2) & 0x01) ? 16 : 8;
		uint currentScanLine = bus->interrupt->io[0x44];

		for (uint8_t i = 0;i < 160;i += 4) {//loop 4 byte(sprite) in OAM
			uint8_t posY = bus->mmu->read(0xFE00 + i) - 16;//if there are any bugs with drawing consider to change uint8_t to int16.
			if(posY <= currentScanLine && posY + spriteSize >= currentScanLine) {//check if the sprite is crossing the current scanline
				uint8_t posX = bus->mmu->read(0xFE00 + i + 1) - 8;
				uint16_t patternNum = bus->mmu->read(0xFE00 + i + 2);
				if (spriteSize == 16)
					patternNum &= 0xfffe;
				
				uint8_t attributes = bus->mmu->read(0xFE00 + i + 3);
				uint8_t priority = attributes >> 7 & 0x01;
				bool Yflip = (attributes >> 6) & 0x01;
				bool Xflip = (attributes >> 5) & 0x01;
				uint16_t paletteAddress = (attributes >> 4) & 0x01?0xff49:0xff48;
				map<int, int> palette;
				getPalette(paletteAddress, &palette);
				uint16_t tileAddress = 0x8000 + 16 * (patternNum)+2 * (currentScanLine - posY);
				for(uint8_t x = 0;x < 8;x++) {
					uint8_t lsbData = bus->mmu->read(tileAddress);
					uint8_t msbData = bus->mmu->read(tileAddress + 1);
					uint8_t isBgWinWhitePixel = (currBGLine[(posX + x) / 8] >> (7 - (posX + x) % 8)) & 0x01;
					if(!priority|| isBgWinWhitePixel)
						setPixel(posX + x, currentScanLine, lsbData, msbData, x, palette);
				}
			}
		}
	}

}

void GPU::draw() {
	bus->display->setLock(true);
	uint8_t currBGLine[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	drawBG(currBGLine);
	//drawSprites(currBGLine);
	bus->display->setLock(false);
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
uint8_t GPU::setPixel(int x, int y, uint8_t lsb, uint8_t msb,uint8_t pixelIndex, map<int, int> palette)
{	
	//uint8_t offset = 0;
	//for (offset = pixelIndex;offset < 8 && (offset + x) < 160;offset++) {
		uint8_t colorPaletteDataIndex = (((msb >> (7 - pixelIndex)) & 0x1) << 1) | ((lsb >> (7 - pixelIndex)) & 0x1);
		uint8_t colorIndex = (bus->interrupt->io[0x47] >> (2 * colorPaletteDataIndex)) & 0x03;
		colorIndex = palette[colorPaletteDataIndex];
		//printf("color id:%d\n",colorIndex);
	 	//bus->display->setPixel(x+ (offset- pixelIndex), y, bus->display->colors[colorIndex]);
		//bus->display->setPixel(x , y, bus->display->bgPalette[colorIndex]);
		bus->display->setPixel(x, y, bus->display->colors[colorIndex]);
	//}
	
		return colorIndex;
}
//uint8_t GPU::setPixel(int x, int y, uint8_t lsb, uint8_t msb, uint8_t pixelIndex, map<int, int> palette)
//{
//	uint8_t offset = 0;
//	for (offset = pixelIndex;offset < 8 && (offset + x) < 160;offset++) {
//		uint8_t colorPaletteDataIndex = (((msb >> (7 - offset)) & 0x1) << 1) | ((lsb >> (7 - offset)) & 0x1);
//		uint8_t colorIndex = (bus->interrupt->io[0x47] >> (2 * colorPaletteDataIndex)) & 0x03;
//		colorIndex = palette[colorPaletteDataIndex];
//		//printf("color id:%d\n",colorIndex);
//		//bus->display->setPixel(x+ (offset- pixelIndex), y, bus->display->colors[colorIndex]);
//		bus->display->setPixel(x + (offset - pixelIndex), y, bus->display->bgPalette[colorIndex]);
//	}
//	return offset;
//
//}

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


