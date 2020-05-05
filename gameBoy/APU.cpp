#include "APU.h"
#include "pipeChannel.h"
using namespace std;
int audio_pos = 0;
void soundTick(void* user_data, Uint8* stream, int len) {



	//printf("len:%d\n",len);

	//Sint16* buf = (Sint16*)stream;


	APU* apu = (APU*)user_data;

	/*steady_clock::time_point end = steady_clock::now();
	duration<double> elapsed_seconds = end - apu->startTimer;
	apu->startTimer = end;
	double elapse = elapsed_seconds.count();
	apu->tickElapse = elapse;
	apu->time += elapse;*/

	/*if (len != apu->adc.have.samples) {
		printf("len:%d", len);
	}*/
	BUS* bus = apu->bus;

	uint16_t opcode;
	uint8_t sampleCounter = 32;
	uint16_t squencerCounter = 256;
	float y1, y2, y3;

	apu->cyclesInSoundFrameCounter = 0;
	//apu->cyclesInSoundFrame= 4194304 * elapse;
	//4194304/44100=~95 cycles per sample. 97280 cycles for 1024 samples.
//	apu->cyclesInSoundFrame = (4194304/apu->adc.have.freq)* apu->adc.have.samples;


	bool rendered = false;
	apu->adc.have.samples = len;
	apu->adc.hasSamples = false;
	do {

		//cpu->Execute(opcode);
		//pipeRecive(bus, opcode, lastopcode, steps,"Execute");
		if (!bus->cpu->halt) {
			opcode = bus->cpu->getOpcode();
			opcode = (opcode == 0xCB) ? 0XCB00 | bus->cpu->getOpcode() : opcode;
			//cpu->Execute(opcode);
			bus->cpu->ExecuteOpcode(opcode);
			//cpu->steps++;
			//pipeRecive(bus, opcode, lastopcode, cpu->steps, "Execute");
			//cpu->Execute(opcode);
		}
		else {
			bus->cpu->lastOpcodeCycles = 1;
		}



		bus->cpu->lastOpcodeCycles *= (4 * (bus->cpu->speedMode + 1));
		apu->cyclesInSoundFrameCounter += bus->cpu->lastOpcodeCycles;

		apu->tick();
		bus->gpu->tick();
		bus->cpu->updateTimers();
		bus->joypad->updateKeys();

		bus->cpu->lastOpcodeCycles += bus->interrupt->InterruptsHandler() * (4 * (bus->cpu->speedMode + 1));;

		//bus->cpu->time.addMCycles(bus->cpu->lastOpcodeCycles / 4);
	//	bus->cpu->time.print(2);

		bus->display->tick(bus->cpu->lastOpcodeCycles);
		bus->cpu->steps += bus->cpu->lastOpcodeCycles;
		sampleCounter -= bus->cpu->lastOpcodeCycles;
		if (apu->cyclesInSoundFrameCounter >= 69905 && !rendered) {
			bus->display->render();
			rendered = true;
		}

	} while (!apu->adc.hasSamples);
	
	//while (apu->cyclesInSoundFrameCounter < apu->cyclesInSoundFrame);
	//stream = (Uint8*)apu->channels[0]->leftSamplesData;
	for (int i = 0;i < len;i++) {
	stream[i]= apu->channels[0]->leftSamplesData[i];
	}

}
void APU::createAudioDeviceControl(bool closeOld)
{
	return;
	if (bus->pipeEnable) {



		bus->p->read(20);
		if (bus->p->rBuffer[0] == 1) {
			if (closeOld) {
				SDL_CloseAudioDevice(adc.dev);
			}
			uint32_t freq = bus->p->rBuffer[1] << 24 | bus->p->rBuffer[2] << 16 | bus->p->rBuffer[3] << 8 | bus->p->rBuffer[4];
			uint32_t samples = bus->p->rBuffer[5] << 24 | bus->p->rBuffer[6] << 16 | bus->p->rBuffer[7] << 8 | bus->p->rBuffer[8];
			uint32_t harmonices = bus->p->rBuffer[9] << 24 | bus->p->rBuffer[10] << 16 | bus->p->rBuffer[11] << 8 | bus->p->rBuffer[12];
			uint32_t timeFactor = bus->p->rBuffer[13] << 24 | bus->p->rBuffer[14] << 16 | bus->p->rBuffer[15] << 8 | bus->p->rBuffer[16];
			bool samplesUint = bus->p->rBuffer[17];
			bool flatWave = bus->p->rBuffer[18];
			bool skipRest = bus->p->rBuffer[19];
			printf("freq:%d samples:%d harmonices:%d timeFactor:%d samplesUint:%d flatWave:%d skipRest:d\n", freq, samples, harmonices, timeFactor, samplesUint, flatWave, skipRest);


			adc.audioPosition = 0;
			adc.audioFrequency = 1.0 * adc.FREQ / 2048; /* 1.0 to make it a float */
			adc.audioVolume = 6000; /* ~1/5 max volume */

			SDL_zero(adc.want);


			adc.want.freq = freq;
			adc.want.format = AUDIO_S8;
			adc.want.channels = 1;
			adc.want.samples = samples;
			adc.H = harmonices;
			adc.samplesUint = samplesUint;
			adc.flatWave = flatWave;
			adc.skipRest = skipRest;
			adc.want.callback = NULL;
			adc.want.userdata = this;

			//adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_SAMPLES_CHANGE);
			adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

			if (!adc.dev) {
				printf("[SDL] Failed to open audio device: %s\n", SDL_GetError());
				SDL_Quit();
				return;
			}
			//SDL_PauseAudioDevice(dev, p); /* play! */
			SDL_PauseAudioDevice(adc.dev, 0);


		}


		bus->p->write(1);
	}
	else {
		if (!bus->modified)
			return;
		bus->modified = false;
		int nums[18] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072 };
		adc.devOld = adc.dev;
		adc.wantOld = adc.want;
		adc.haveOld = adc.have;
		SDL_AudioDeviceID old = adc.dev;
		adc.audioPosition = 0;
		adc.audioFrequency = 1.0 * adc.FREQ / 2048; /* 1.0 to make it a float */
		adc.audioVolume = 6000; /* ~1/5 max volume */

		SDL_zero(adc.want);


		//adc.want.freq = nums[bus->f];
		adc.want.freq = bus->f;
		adc.want.format = AUDIO_S8;
		adc.want.channels = 1;
		//adc.want.samples = nums[bus->s];
		adc.want.samples = bus->s;
		adc.want.callback = adc.want.callback;
		adc.want.userdata = this;

		//adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_SAMPLES_CHANGE);
		/*adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
		SDL_PauseAudioDevice(adc.dev, 0);
		if (!adc.dev) {
			printf("[SDL] Failed to open audio device: %s\n", SDL_GetError());
			SDL_Quit();
			return;
		}*/

		restart = true;
		//SDL_PauseAudioDevice(dev, p); /* play! */
		/*if (closeOld) {
			SDL_CloseAudioDevice(old);
				adc.devOld = adc.dev;
		adc.wantOld = adc.want;
		adc.haveOld = adc.have;
		}*/
		//	std::thread displayThread(closeDevice, old);


	}


}

APU::APU() {
	for (int i = 0;i < 4;i++) {
		channels[i]->adc = &adc;
		channels[i]->soundCtrl = &soundCtrl;
	}
}
void APU::start() {
	if (SDL_Init(SDL_INIT_AUDIO)) {
		printf("[SDL] Failed to initialize: %s\n", SDL_GetError());
		return;
	}
	SDL_zero(adc.want);
	adc.want.freq = 44100;
	//adc.want.freq = 16384;
	adc.want.format = AUDIO_S8;
	adc.want.channels = 1;
	adc.want.samples = 1024;
	//adc.want.samples = 128;
	adc.want.callback = NULL;
	adc.want.userdata = this;
	adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

	bus->f = adc.have.freq;
	bus->s = adc.want.samples;
	cyclesInSoundFrame = adc.have.freq;

	//cyclesInSoundFrame = 4194304/adc.have.freq;
	if (!adc.dev) {
		printf("[SDL] Failed to open audio device: %s\n", SDL_GetError());
		SDL_Quit();
		return;
	}

	adc.leftSamplesData = (Sint8*)calloc(adc.have.samples, sizeof(Sint8));
	adc.rightSamplesData = (Sint8*)calloc(adc.have.samples, sizeof(Sint8));
	adc.samplesData = (Sint8*)calloc(((Uint16)adc.have.channels) * adc.have.samples, sizeof(Sint8));
	for (int i = 0;i < 4;i++) {
		channels[i]->channelIndex = 0;
		channels[i]->leftSamplesData = (Sint8*)calloc(adc.have.samples, sizeof(Sint8));
		channels[i]->rightSamplesData = (Sint8*)calloc(adc.have.samples, sizeof(Sint8));
		channels[i]->samplesData = (Sint8*)calloc(((Uint16)adc.have.channels) * adc.have.samples, sizeof(Sint8));
		channels[i]->samplePosition = 0;
		channels[i]->sampleGenerateCounterReload = 4194304 / adc.have.freq;
		channels[i]->sampleGenerateCounter = 4194304 / adc.have.freq;
	}

	startTimer = steady_clock::now();
	lastTick = startTimer;

	adc.audioPosition = 0;
	adc.audioFrequency = 1.0 * adc.FREQ / adc.have.freq;
	//adc.audioFrequency = 1.0 * adc.FREQ; /* 1.0 to make it a float */
	adc.audioVolume = 6000; /* ~1/5 max volume */
	adc.audioLen = 60 * adc.have.freq;//1 min.

	SDL_PauseAudioDevice(adc.dev, 0);



	adc.sampleDurationSec = 0.001953125;//4096 samples steps in terms of seconds 
	adc.sampleDurationMS = 1.953125;//4096 samples steps in terms of mili seconds
	float m = adc.timeFactor;
	adc.sampleDurationSec = 1 / (double)adc.have.freq;


	createChannels();


}
void APU::createChannels() {

	//Channel 1


	adc.sampleGenerateCounterReload = 4194304 / adc.have.freq;
	adc.sampleGenerateCounter = 4194304 / adc.have.freq;
}

uint8_t APU::read(uint8_t address)
{
	return 0;
}

void APU::write(uint8_t address, uint8_t value)
{
	switch (address) {
	case 0x10:
		chann1->loadNRX0(value);
		break;
	case 0x11:
		chann1->loadNRX1(value);
		break;
	case 0x12:
		chann1->loadNRX2(value);
		break;
	case 0x13:
		chann1->loadNRX3(value);
		break;
	case 0x14:
		chann1->loadNRX4(value);
		break;
	case 0x16:
		chann2->loadNRX1(value);
		break;
	case 0x17:
		chann2->loadNRX2(value);
		break;
	case 0x18:
		chann2->loadNRX3(value);
		break;
	case 0x19:
		chann2->loadNRX4(value);
		break;
	case 0x1a:
		chann3->loadNRX0(value);
		break;
	case 0x1b:
		chann3->loadNRX1(value);
		break;
	case 0x1c:
		chann3->loadNRX2(value);
		break;
	case 0x1d:
		chann3->loadNRX3(value);
		break;
	case 0x1e:
		chann3->loadNRX4(value);
		break;
	case 0x20:
		chann4->loadNRX1(value);
		break;
	case 0x21:
		chann4->loadNRX2(value);
		break;
	case 0x22:
		chann4->loadNRX3(value);
		break;
	case 0x23:
		chann4->loadNRX4(value);
		break;
	case 0x24:
		feedChannelCtrlRegister(value);
		break;
	case 0x25:
		setSoundOutputTerminal(value);
		break;
	case 0x26:
		soundCtrl.soundState = (bus->apu->soundCtrl.soundState & 0x7f) | (value & 0x80);
		break;
	case 0x30:
	case 0x31:
	case 0x32:
	case 0x33:
	case 0x34:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x38:
	case 0x39:
	case 0x3a:
	case 0x3b:
	case 0x3c:
	case 0x3d:
	case 0x3e:
	case 0x3f:
		if (!chann3->enable) {
			chann3->wavePatternRam[address - 0x30] = value;
			chann3->write(address - 0x30,value);
		}
		break;
	}

}


void APU::connectToBus(BUS* bus)
{
	this->bus = bus;
}

void APU::tick()
{
	//int i = 2;
	//chann1->tick(bus->cpu->lastOpcodeCycles);
	channels[0]->tick(bus->cpu->lastOpcodeCycles);
	channels[1]->tick(bus->cpu->lastOpcodeCycles);
	channels[2]->tick(bus->cpu->lastOpcodeCycles);
	channels[3]->tick(bus->cpu->lastOpcodeCycles);
	if (adc.hasSample) {
		adc.hasSample = false;
		adc.audioPosition++;
		adc.sampleCounter++;

		if (adc.sampleCounter == adc.have.samples) {
			adc.hasSamples = true;
			adc.sampleCounter = 0;
			//SDL_QueueAudio(apu->adc.dev, ch->samplesData, apu->adc.have.channels * apu->adc.have.samples);
			//SDL_QueueAudio(apu->adc.dev, ch->samplesData,  apu->adc.have.samples);

			SDL_MixAudioFormat((Uint8*)channels[0]->leftSamplesData, (Uint8*)channels[1]->leftSamplesData, AUDIO_S8, adc.have.samples, SDL_MIX_MAXVOLUME);
			SDL_MixAudioFormat((Uint8*)channels[0]->leftSamplesData, (Uint8*)channels[2]->leftSamplesData, AUDIO_S8, adc.have.samples, SDL_MIX_MAXVOLUME);
			SDL_MixAudioFormat((Uint8*)channels[0]->leftSamplesData, (Uint8*)channels[3]->leftSamplesData, AUDIO_S8, adc.have.samples, SDL_MIX_MAXVOLUME);
			//if(!mute)
			if(!adc.have.callback)
			SDL_QueueAudio(adc.dev, channels[0]->leftSamplesData, adc.have.samples);
			for (int i = 0;i < adc.have.channels * adc.have.samples;i++) {
				if (!adc.have.callback)
				channels[0]->leftSamplesData[i] = 0;
				channels[1]->leftSamplesData[i] = 0;
				channels[2]->leftSamplesData[i] = 0;
				channels[3]->leftSamplesData[i] = 0;
			}


			//SDL_QueueAudio(apu->adc.dev, ch->leftSamplesData, apu->adc.have.samples);
			//SDL_QueueAudio(apu->adc.dev, ch->samplesData, apu->adc.have.channels * apu->adc.have.samples);
		}
	}
}
void APU::setSoundOutputTerminal(uint8_t value) {
	/*
		(FF25)
		Bit 7 - Output sound 4 to SO2 terminal
		Bit 6 - Output sound 3 to SO2 terminal
		Bit 5 - Output sound 2 to SO2 terminal
		Bit 4 - Output sound 1 to SO2 terminal
		Bit 3 - Output sound 4 to SO1 terminal
		Bit 2 - Output sound 3 to SO1 terminal
		Bit 1 - Output sound 2 to SO1 terminal
		Bit 0 - Output sound 1 to SO1 terminal
		*/
	for (uint8_t i = 0;i < 4;i++) {
		channels[i]->outputTerminal = (((value >> i + 4) & 0x01) << 1) | (value >> i) & 0x01;
	}
}
void APU::feedChannelCtrlRegister(uint8_t value)
{
	/*
	Bit 7   - Output Vin to SO2 terminal (1=Enable)
	Bit 6-4 - SO2 output level (volume)  (0-7)
	Bit 3   - Output Vin to SO1 terminal (1=Enable)
	Bit 2-0 - SO1 output level (volume)  (0-7)
	*/
	soundCtrl.VinS02 = (((value >> 7) & 0x01) == 0x01);
	soundCtrl.S02Volume = (value >> 4) & 0x07;
	soundCtrl.VinS01 = (((value >> 3) & 0x01) == 0x01);
	soundCtrl.S02Volume = value & 0x07;
}




void APU::close(bool closeSDL) {
	SDL_CloseAudioDevice(adc.dev);
	if(closeSDL)
	SDL_Quit();
}

void APU::play()
{
	restart = false;
	adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

	if (!adc.dev) {
		printf("[SDL] Failed to open audio device: %s\n", SDL_GetError());
		//SDL_Quit();
		adc.want = adc.wantOld;
		adc.have = adc.haveOld;
		adc.dev = adc.devOld;
		return;
	}
	SDL_CloseAudioDevice(adc.devOld);
	SDL_PauseAudioDevice(adc.dev, 0);

	/*if (closeOld) {
			SDL_CloseAudioDevice(old);
				adc.devOld = adc.dev;
		adc.wantOld = adc.want;
		adc.haveOld = adc.have;
		}*/

}

