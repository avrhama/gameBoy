#include "APU.h"
#include "pipeChannel.h"
using namespace std;
void MyAudioCallback(void* user_data, Uint8* stream, int len) {
	//len /= 2; /* 16 bit */

	APU * apu = (APU*)user_data;
	/*if (apu->adc.paused)
		return;*/
	/*
	double =0.00762939453125;//ms each period sound
	double f=0.003814697265625;//ms each sample calculate
	*/
	Sint8* buf = (Sint8*)stream;
	float duty = apu->channels[0].duty;
	float p = duty * 2.0f * M_PI;
	float a = 0;
	float b = 0;
	int H = 88;
	float y1, y2, y3;
	float P = apu->channels[0].duty;
	for (int i = 0; i < len; i++) {//each iteration is 1/samples of second
		//y1 = 0;
		//y2 = 0;

		//for (int n = 1;n <= H;n++) {
		//	y1 += apu->channels[0].envelopeVolume * sin(((float)( apu->adc.audioPosition))  * (1/(float)apu->channels[0].freq) * 2 * M_PI * n) / n;
		//	y2 += apu->channels[0].envelopeVolume * sin(((float)( apu->adc.audioPosition)  * (float)(1/(float)apu->channels[0].freq) - apu->channels[0].duty) * 2 * M_PI * n) / n;
		//	//y1 += sin(((float)(apu.channels[0].envelopeVolume * apu.adc.audioPosition)) * apu.channels[0].freq * 2 * M_PI * n) / n;
		//	//y2 += sin(((float)(apu.channels[0].envelopeVolume * apu.adc.audioPosition) * apu.channels[0].freq - apu.channels[0].duty) * 2 * M_PI * n) / n;
		//}
		//y3 = y1 - y2;
		y3 = apu->getChannelSample(0);
		//y3 = getSquareWave(0.5, i, 0.5);
		buf[i] = apu->adc.audioVolume * y3;
		//buf[i] = y3;
		apu->adc.audioPosition = (apu->adc.audioPosition + 1) % apu->adc.have.samples;
		apu->channels[0].sampleRatePos++;
		apu->channels[0].sampleRatePos = apu->channels[0].sampleRatePos % apu->adc.have.freq;
		//if (channels[0].counterEnable) {
		//-0.5
		//}
	}
	apu->adc.audioLen -= len;
	
}
void soundTick(void* user_data, Uint8* stream, int len) {
	//len /= 2; /* 16 bit */
	//printf("len:%d\n",len);

	Sint8* buf = (Sint8*)stream;
	
	APU* apu = (APU*)user_data;

	steady_clock::time_point end = steady_clock::now();
	duration<double> elapsed_seconds = end - apu->startTimer;
	apu->startTimer = end;
	double elapse = elapsed_seconds.count();
	apu->tickElapse = elapse;
	apu->time += elapse;


	BUS* bus = apu->bus;
		
		uint16_t opcode;
		uint8_t sampleCounter = 32;
		uint16_t squencerCounter = 256;
		float y1, y2, y3;
		
		apu->cyclesInSoundFrameCounter = 0;
	    apu->cyclesInSoundFrame= 4194304 * elapse;
		if (len != apu->adc.have.samples) {
			//printf("len:%d\n", len);
		}
		//len /= 2;
		//apu->cyclesInSoundFrame = 4194304;
	
		
		//apu->cyclesInSoundFrame = (4194304 / (double)apu->adc.have.samples) * len* 8192;
		//apu->cyclesInSoundFrame =8192* len;
		
		int sampleCount = (apu->cyclesInSoundFrame)/ 8192;
		//sampleCount= apu->cyclesInSoundFrame/((4194304 / (double)apu->adc.have.samples) * 8192);

		sampleCount = len/ (SDL_AUDIO_BITSIZE(apu->adc.have.format) / 8);
		/*do {

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
		   //apu->fs.tick(buf);

			//apu->tick();
			bus->gpu->tick();
			bus->cpu->updateTimers();
			bus->joypad->updateKeys();
		
			bus->cpu->lastOpcodeCycles += bus->interrupt->InterruptsHandler() * (4 * (bus->cpu->speedMode + 1));;

			bus->cpu->time.addMCycles(bus->cpu->lastOpcodeCycles / 4);
			bus->cpu->time.print(2);
			//bus->cpu->time.print(1);
			apu->cyclesInSoundFrameCounter += bus->cpu->lastOpcodeCycles;
			bus->display->tick(bus->cpu->lastOpcodeCycles);
			bus->cpu->steps += bus->cpu->lastOpcodeCycles;
			sampleCounter-= bus->cpu->lastOpcodeCycles;
		
			
		} while (apu->cyclesInSoundFrameCounter < apu->cyclesInSoundFrame);*/
		
		apu->cyclesInSoundFrameCounter = apu->cyclesInSoundFrameCounter%apu->cyclesInSoundFrame;
		bus->cpu->steps = bus->cpu->steps%4194304;
		//apu->adc.audioPosition = 0;
		//bus->display->tick(1);
		//len /= 2;
	/*	for (int i = 0; i < len; i++) {//each iteration is 1/samples of second
			apu->channels[0].samplesData[i]=0;
		}*/

		
		for (int i = 0; i < sampleCount; i++) {//each iteration is 1/samples of second
			//y3 = apu->adc.audioVolume*apu->getChannelSample(0);
			
		//	buf[apu->adc.audioPosition+i*4] = apu->adc.audioVolume * y3;
			//apu->channels[0].samplesData[apu->adc.audioPosition] = apu->adc.audioVolume * y3;
			//buf[i] = apu->adc.audioVolume * y3;
			
			y3 = apu->adc.audioVolume * apu->getChannelSample(0);
			/*float* p =(float*)buf + i;
			*p = y3;*/
			//y3 = apu->adc.audioVolume * apu->channels[0].sequencer.tick();
			unsigned int fltInt32;
			unsigned short fltInt16;
			fltInt32 = y3;
			fltInt16 = (fltInt32 >> 31) << 5;
			unsigned short tmp = (fltInt32 >> 23) & 0xff;
			tmp = (tmp - 0x70) & ((unsigned int)((int)(0x70 - tmp) >> 4) >> 27);
			fltInt16 = (fltInt16 | tmp) << 10;
			fltInt16 |= (fltInt32 >> 13) & 0x3ff;
			
			
			//unsigned short* p = (unsigned short*)buf+i;
			unsigned short* p = (unsigned short*)buf + i;
			*p = fltInt16;
			//buf[i] = y3;
			
			apu->adc.audioPosition++;
			apu->adc.audioPosition = apu->adc.audioPosition % apu->adc.have.samples;
			apu->channels[0].sampleRatePos++;
			apu->channels[0].sampleRatePos = apu->channels[0].sampleRatePos % apu->adc.have.freq;
			apu->channels[0].samplePosition++;
			apu->channels[0].samplePosition = apu->channels[0].samplePosition % apu->adc.have.samples;
	 	
		
		}
		
		/*for (int i = 0; i < len; i++) {//each iteration is 1/samples of second
			buf[i] = apu->channels[0].samplesData[i];
		}
		//buf[0] = apu->adc.audioVolume*apu->fs.out();
		//y3 = apu->getChannelSample(0);
		//buf[0] = apu->adc.audioVolume * y3;
		*/
		




	return;
	
	/*if (apu->adc.paused)
		return;*/
		/*
		double =0.00762939453125;//ms each period sound
		double f=0.003814697265625;//ms each sample calculate
		*/
	
	float duty = apu->channels[0].duty;
	float p = duty * 2.0f * M_PI;
	float a = 0;
	float b = 0;
	int H = 88;
	
	float P = apu->channels[0].duty;
	for (int i = 0; i < len; i++) {//each iteration is 1/samples of second
		//y1 = 0;
		//y2 = 0;

		//for (int n = 1;n <= H;n++) {
		//	y1 += apu->channels[0].envelopeVolume * sin(((float)( apu->adc.audioPosition))  * (1/(float)apu->channels[0].freq) * 2 * M_PI * n) / n;
		//	y2 += apu->channels[0].envelopeVolume * sin(((float)( apu->adc.audioPosition)  * (float)(1/(float)apu->channels[0].freq) - apu->channels[0].duty) * 2 * M_PI * n) / n;
		//	//y1 += sin(((float)(apu.channels[0].envelopeVolume * apu.adc.audioPosition)) * apu.channels[0].freq * 2 * M_PI * n) / n;
		//	//y2 += sin(((float)(apu.channels[0].envelopeVolume * apu.adc.audioPosition) * apu.channels[0].freq - apu.channels[0].duty) * 2 * M_PI * n) / n;
		//}
		//y3 = y1 - y2;
		y3 = apu->getChannelSample(0);
		//y3 = getSquareWave(0.5, i, 0.5);
		buf[i] = apu->adc.audioVolume * y3;
		//buf[i] = y3;
		apu->adc.audioPosition = (apu->adc.audioPosition + 1) % apu->adc.have.samples;
		apu->channels[0].sampleRatePos++;
		apu->channels[0].sampleRatePos = apu->channels[0].sampleRatePos % apu->adc.have.freq;
		//if (channels[0].counterEnable) {
		//-0.5
		//}
	}
	apu->adc.audioLen -= len;

}
void APU::createAudioDeviceControl(bool closeOld)
{
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
			adc.want.callback = MyAudioCallback;
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


		adc.want.freq = nums[bus->f];
		adc.want.format = AUDIO_S8;
		adc.want.channels = 1;
		adc.want.samples = nums[bus->s];
		adc.want.callback = soundTick;
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

	
}
void APU::start() {
	if (SDL_Init(SDL_INIT_AUDIO)) {
		printf("[SDL] Failed to initialize: %s\n", SDL_GetError());
		return;
	}
	adc.audioPosition = 0;
	adc.audioFrequency = 1.0 * adc.FREQ / 2048; /* 1.0 to make it a float */
	adc.audioVolume = 6000; /* ~1/5 max volume */

	SDL_zero(adc.want);
	//queue device
	/*adc.want.freq = 44100;
	adc.want.format = AUDIO_S8;
	adc.want.channels = 1;
	adc.want.samples = 512;
	adc.want.callback = 0;
	adc.H = 20;
	adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	SDL_PauseAudioDevice(adc.dev, 0);
	channels[0].channelIndex = 0;
	channels[0].samplesData = (Sint8*)calloc(adc.want.samples, sizeof(Sint8));
	channels[0].samplePosition = 0;
	return ;*/
	//callback device


	//adc.want.freq = 224000;
    //adc.want.freq = 131072;
	//adc.want.freq = 88200;
   // adc.want.freq = 44100;// 44100&8192
	// adc.want.freq = 22050;
	//adc.want.freq = 65536;
	adc.want.freq = 32768;
	
	//adc.want.freq = 16384;
	
    //adc.want.freq = 8192;
	//adc.want.freq = 4096;
   // adc.want.freq = 2048;
	//adc.want.freq = 1024;
   //adc.want.freq = 512;
	//adc.want.freq = 256;
	//adc.want.freq = 128;
	//adc.want.format = AUDIO_S32;
	//adc.want.format = AUDIO_S32MSB;
	adc.want.format = AUDIO_S16MSB;
	//adc.want.format = AUDIO_S8;
	//adc.want.channels = 1;
	//adc.want.samples = 32768;
	//adc.want.samples = 16384;

	//adc.want.samples = 8192;
	//adc.want.samples = 4096;
   // adc.want.samples = 2048;
    adc.want.samples = 1024;
	//adc.want.samples = 512;
	//adc.want.samples = 256;
	//adc.want.samples = 128;
    //adc.want.samples = 64;
	//adc.want.samples = 32;
	//adc.want.samples = 1;
	adc.want.callback = soundTick;
	adc.want.userdata = this;
	adc.H = 88;
	//2048:32 ,8192:256 
	//adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_SAMPLES_CHANGE);
	
	int nums[18] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072 };
	//adc.want.freq = 131072;
	//adc.want.samples = 32;
	adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

	cyclesInSoundFrame = adc.have.freq;
	
	//cyclesInSoundFrame = 4194304/adc.have.freq;
	if (!adc.dev) {
		printf("[SDL] Failed to open audio device: %s\n", SDL_GetError());
		SDL_Quit();
		return;
	}
	//SDL_PauseAudioDevice(dev, p); /* play! */
	channels[0].channelIndex = 0;
	channels[0].samplesData = (Sint8*)calloc(adc.have.samples, sizeof(Sint8));
	channels[0].samplePosition = 0;
	
	startTimer = steady_clock::now();
	lastTick = startTimer;
	SDL_PauseAudioDevice(adc.dev, 0);
	fs.apu = this;
	fs.ch = channels;
}
void APU::connectToBus(BUS* bus)
{
	this->bus = bus;
}
float getSquareWave(double f, int t,double duty) {
	float y1 = 0, y2 = 0, y3 = 0;

	//double t = (float)(channels[channelIndex].sampleRatePos) * sampleDurationSec;//point on time line
	//t = (float)(channels[channelIndex].sampleRatePos) * (1 / adc.have.freq);//point on time line
	//t = (float)(adc.audioPosition) * sampleDuration;//point on time line
	//t = sampleDuration * ((double)bus->cpu->steps)/1000;
	float p = duty *2.0f * M_PI;
	//phase = channels[channelIndex].duty;
	float c = 0;
	for (int n = 1;n <= 20;n++) {
		//c = n * ((float)channels[channelIndex].freq) * 2.0 * M_PI * t;
		c = n * f * 2.0 * M_PI * t;
		y1 += sin(c) / n;
		y2 += sin(c - p * (double)n) / n;
	}
	return (y1 - y2) * (2.0f / M_PI);
}
void APU::tick()
{
	apuCounter -= bus->cpu->lastOpcodeCycles;
	if (apuCounter <= 0) {
		apuCounter += apuReloadCounter;
		/*if (channels[0].enable) {
			int steps = 0;
			do {
				channels[0].samplesData[channels[0].samplePosition++] = getChannelSample(0);
				channels[0].samplePosition = channels[0].samplePosition % adc.have.samples;
				steps++;
			} while (channels[0].enable && steps < 256);
			SDL_QueueAudio(adc.dev, channels[0].samplesData, steps);
		}*/

			
		/*if (channels[0].enable) {
			int steps = 0;
			do {
				channels[0].samplesData[channels[0].samplePosition++] = getChannelSample(0);
				channels[0].samplePosition = channels[0].samplePosition % adc.have.samples;
				channels[0].sampleRatePos++;
				channels[0].sampleRatePos = channels[0].sampleRatePos % adc.have.freq;
				
				steps++;
			} while (channels[0].enable && steps < 2);
			SDL_QueueAudio(adc.dev, channels[0].samplesData, adc.have.samples);
			printf("queue push! %d,freq:%d timer:%s vol:%d\n", channels[0].samplePosition, channels[0].freq, channels[0].counterEnable?"yes":"no", channels[0].envelopeVolume);
		}*/
		//if (channels[0].enable) {
			int steps = 0;
			do {
				channels[0].samplesData[channels[0].samplePosition++] =getSquareWave(440.0f,bus->cpu->steps,0.5f);
				channels[0].samplePosition = channels[0].samplePosition % adc.have.samples;
				channels[0].sampleRatePos++;
				channels[0].sampleRatePos = channels[0].sampleRatePos % adc.have.freq;

				steps++;
			} while (steps < 512);
			SDL_QueueAudio(adc.dev, channels[0].samplesData, adc.have.samples);
			printf("queue push! %d,freq:%d timer:%s vol:%d\n", channels[0].samplePosition, channels[0].freq, channels[0].counterEnable ? "yes" : "no", channels[0].envelopeVolume);
	//	}
	//	SDL_QueueAudio(adc.dev, channels[0].samplesData, adc.have.samples);
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
		channels[i].outputTerminal = (((value >> i + 4) & 0x01)<<1) |(value >> i)&0x01;
	}
}
void APU::setSoundState(uint8_t channelIndex, bool set) {
	/*
	FF26
	Bit 7 - All sound on/off  (0: stop all sound circuits) (Read/Write)
	Bit 3 - Sound 4 ON flag (Read Only)
	Bit 2 - Sound 3 ON flag (Read Only)
	Bit 1 - Sound 2 ON flag (Read Only)
	Bit 0 - Sound 1 ON flag (Read Only)
	The flags get set when sound output is restarted by setting the Initial flag
	(Bit 7 in NR14-NR44), the flag remains set until the sound length has expired (if enabled).
	A volume envelopes which has decreased to zero volume will NOT cause the sound flag to go off.
	*/
	if (set) 
		soundState |= (0x01 << channelIndex);
	else
		soundState &= 0xff^(0x01 << channelIndex);
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
	soundCtrl.S02Volume = value&0x07;
}

void APU::feedTriggerRegister(uint8_t channelIndex, uint8_t value)
{
}

void APU::feedSweepRegister(uint8_t channelIndex, uint8_t value)
{
	/*
		Bit 6-4 - Sweep Time
		Bit 3   - Sweep Increase/Decrease
		0: Addition    (frequency increases)
		1: Subtraction (frequency decreases)
		Bit 2-0 - Number of sweep shift (n: 0-7)
	*/
	channels[channelIndex].sweepTime = sweepTimes[(value>>4)&0x07];
	channels[channelIndex].sweepInc = (value >> 3) & 0x01 ? -1 : 1;
	channels[channelIndex].nSweep = value & 0x07;
}

void APU::feedLenAndDutyRegister(uint8_t channelIndex, uint8_t value)
{
	/*
		Bit 7-6 - Wave Pattern Duty (Read/Write)
		Bit 5-0 - Sound length data (Write Only) (t1: 0-63)
	*/
	channels[channelIndex].soundLenData = value & 0x3f;
	channels[channelIndex].duty = duites[(value >> 6)&0x04];
	channels[channelIndex].sequencer.sequence = waveForms[(value >> 6) & 0x04];
	channels[channelIndex].soundLen = ((64 - (double)channels[channelIndex].soundLenData) / (256)) * 1000;// for ms

}

void APU::feedVolumeEnvelopeRegister(uint8_t channelIndex, uint8_t value)
{
	/*
		Bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
		Bit 3   - Envelope Direction (0=Decrease, 1=Increase)
		Bit 2-0 - Number of envelope sweep (n: 0-7)
		(If zero, stop envelope operation.)
		Length of 1 step = n*(1/64) seconds
	*/
	channels[channelIndex].nEnvelopeSweep = value & 0x07;
	channels[channelIndex].envelopeDirection = (value >> 3) & 0x01 ? 1 : -1;
	channels[channelIndex].envelopeVolume= (value >> 4) & 0x0f;
	channels[channelIndex].loadedVolumeEnvelopeLen = ((double)channels[channelIndex].nEnvelopeSweep / 64);//*1000;//ms
	//channels[channelIndex].loadedVolumeEnvelopeLen = ((double)channels[channelIndex].nEnvelopeSweep / 64) ;//sec
	channels[channelIndex].envelopeEnable = (channels[channelIndex].nEnvelopeSweep != 0)
		&&(channels[channelIndex].envelopeVolume >0x00&& channels[channelIndex].envelopeVolume <0x0f);
	if (channels[channelIndex].envelopeEnable) {
		int y = 0;
	}
}

void APU::feedFrequencyLoRegister(uint8_t channelIndex, uint8_t value)
{
	//Lower 8 bits of 11 bit frequency (x). Next 3 bit are in NR14 ($FF14)
	channels[channelIndex].loadedFreq = (channels[channelIndex].loadedFreq & 0x700) | (value & 0xff);
}

void APU::feedFrequencyHiCtlRegister(uint8_t channelIndex, uint8_t value)
{
	/*
		Bit 7   - Initial (1=Restart Sound)     (Write Only)
		Bit 6   - Counter/consecutive selection (Read/Write)
				(1=Stop output when length in NR11 expires)
		Bit 2-0 - Frequency's higher 3 bits (x) (Write Only)
	*/
	channels[channelIndex].loadedFreq = (value & 0x07) << 8 |(channels[channelIndex].loadedFreq & 0xff);
	channels[channelIndex].counterEnable = (((value >> 6) & 0x01) == 0x01);
	if (!channels[channelIndex].counterEnable) {
		channels[channelIndex].soundLen = 0;
	}
	fs.dutyTimerReload = 4194304/(4 * (2048 - channels[channelIndex].loadedFreq));
	fs.dutyTimer = fs.dutyTimerReload;
	if ((value >> 7) & 0x01) {//trigger 
		
		channels[channelIndex].enable = true;
		/*if (channels[channelIndex].counter == 0)
			channels[channelIndex].counter = channelIndex != 2 ? 64 : 256;*/

		if (channels[channelIndex].soundLen == 0) {
			//channels[channelIndex].soundLen = channelIndex != 2 ? 64 : 256;
			channels[channelIndex].soundLen = ((64 - (double)channels[channelIndex].soundLenData) / (256));//*1000;//ms
			//channels[channelIndex].soundLen = ((64 - (double)channels[channelIndex].soundLenData) / (256));//sec
		}
		channels[channelIndex].freq = 131072 / (2048 - channels[channelIndex].loadedFreq);
		channels[channelIndex].frequencySweepLen = channels[channelIndex].sweepTime;
		channels[channelIndex].volumeEnvelopeLen = channels[channelIndex].loadedVolumeEnvelopeLen;
		setSoundState(channelIndex, true);
		//channels[channel]. = channels[channel].nEnvelopeSweep / 64;//sec

		/*if (channels[0].enable) {
			int steps = 0;
			do {
				channels[0].samplesData[channels[0].samplePosition++] = getChannelSample(0);
				channels[0].samplePosition = channels[0].samplePosition % adc.have.samples;
				steps++;
			} while (channels[0].enable && steps < 256);
			SDL_QueueAudio(adc.dev, channels[0].samplesData, adc.have.samples);
		}*/
		/*if (channels[0].enable) {
			int steps = 0;
			do {
				channels[0].samplesData[channels[0].samplePosition++] = getChannelSample(0);
				channels[0].samplePosition = channels[0].samplePosition % adc.have.samples;
				channels[0].sampleRatePos++;
				channels[0].sampleRatePos = channels[0].sampleRatePos % adc.have.freq;

				steps++;
			} while (channels[0].enable && steps < 1);
			SDL_QueueAudio(adc.dev, channels[0].samplesData, adc.have.samples);
			printf("queue push! %d,freq:%d timer:%s vol:%d\n", channels[0].samplePosition, channels[0].freq, channels[0].counterEnable ? "yes" : "no", channels[0].envelopeVolume);
		}*/


		if (adc.paused) {
			
			adc.paused = false;
		//	printf("sound trigged!\n");
		}
		
	}
	else {
		if (!adc.paused) {
			//printf("sound stoped!\n");
			createAudioDeviceControl(true);
			adc.paused = true;
		}
		
	}
}
double approxsin(double t) {
	double j = t * 0.15915;
	j = j - (int)j;
	return 20.785 * j * (j - 0.5) * (j - 1.0f);

}
double APU::getSample(double f, double t, double d,int h) {
	float y1 = 0, y2 = 0, y3 = 0;
	float phase = 2.0f * M_PI * d;
	float c = 0;
	for (int n = 1;n <= h;n++) {
		c = n * f * 2.0 * M_PI * t;
		y1 += -approxsin(c) / n;
		y2 += -approxsin(c - phase * (double)n) / n;
	}
	y3 = (y1 - y2);
	if (y3 > 0)y3 = 1;else if (y3 < 0)y3 = -1;
	double amplitude = 1;
	y3 = y3 * (2.0 * amplitude / M_PI);
	return y3;
}
float APU::getChannelSample(uint8_t channelIndex)
{
	//sould the sweepFreq timer still running?
	if (!channels[channelIndex].enable)
		return 0;
	float m = adc.timeFactor;

	//double sampleDuration = 0.003814697265625;//ms each sample calculate 11
	// double sampleDuration = 0.00762939453125;//ms each sample calculate
	//double sampleDuration = 0.244140625*m;//ms each sample calculate
	//double sampleDuration = 0.00011920928955078125;
	// double sampleDurationSec = 0.000244140625 * m;//sec each sample calculate 131072 hz

	double sampleDurationSec = 0.001953125 * m;//4096 samples steps in terms of seconds 
	double sampleDuration = 1.953125 * m;//4096 samples steps in terms of mili seconds

	int d;
	if (adc.samplesUint)
		d = adc.have.samples;
	else
		d = adc.have.freq;
	//d = 1024;
	d = adc.have.freq;
	switch (d) {
	case 128:
		sampleDurationSec = 0.0078125 * m;
		sampleDuration = 7.8125 * m;
		break;
	case 256:
		sampleDurationSec = 0.00390625 * m;
		sampleDuration = 3.90625 * m;
		break;
	case 512:
		sampleDurationSec = 0.001953125 * m;
		sampleDuration = 1.953125 * m;
		break;
	case 1024:
		sampleDurationSec = 0.0009765625 * m;
		sampleDuration = 0.9765625 * m;
		break;
	case 2048:
		sampleDurationSec = 0.00048828125 * m;
		sampleDuration = 0.48828125 * m;
		break;
	case 4026:
		sampleDurationSec = 0.000244140625 * m;
		sampleDuration = 0.244140625 * m;
		break;
	case 8192:
		sampleDurationSec = 0.0001220703125 * m;
		sampleDuration = 0.1220703125 * m;
		break;
	case 16384:
		sampleDurationSec = 0.00006103515625 * m;
		sampleDuration = 0.06103515625 * m;
		break;
	case 32768:
		sampleDurationSec = 0.000030517578125 * m;
		sampleDuration = 0.030517578125 * m;
		break;
	case 65536:
		sampleDurationSec = 0.0000152587890625 * m;
		sampleDuration = 0.0152587890625 * m;
		break;
	case 131072:
		sampleDurationSec = 0.00000762939453125 * m;
		sampleDuration = 0.00762939453125 * m;
		break;
	}
	// sampleDurationSec = 1 / (double)adc.have.freq;
	// sampleDuration = 1000 / (double)adc.have.freq;
	double sampleRateDurationSec = 0.0000226757369614512471;
	sampleRateDurationSec = sampleDurationSec;
	sampleDuration = sampleDurationSec;
	float y1 = 0, y2 = 0, y3 = 0;

	double t = (float)(adc.audioPosition) * sampleDurationSec;//point on time line
	t = ((double)bus->cpu->steps / 4194304) * sampleRateDurationSec;// +(channels[channelIndex].sampleRatePos) * sampleRateDurationSec;
	t = ((double)bus->cpu->steps / 4194304) * sampleRateDurationSec + (channels[channelIndex].sampleRatePos) * sampleRateDurationSec;

	t = time + (channels[channelIndex].sampleRatePos) * sampleRateDurationSec;
	t = time + (channels[channelIndex].samplePosition) * sampleDurationSec;


	float phase = 2.0f * M_PI * channels[channelIndex].duty;
	//phase = channels[channelIndex].duty;
	float c = 0;
	adc.H = bus->h;
	for (int n = 1;n <= adc.H;n++) {
		//c = n * ((float)channels[channelIndex].freq) * 2.0 * M_PI * t;
		c = n * ((float)channels[channelIndex].freq) * 2.0 * M_PI * t;
		y1 += -approxsin(c) / n;
		y2 += -approxsin(c - phase * (double)n) / n;


	}
	y3 = (y1 - y2);
	double amplitude = 16;
	double r = 0;
	r = bus->r;
	/*if (adc.flatWave) {
		if (y3 > r) {
			y3 = 1;
		}
		else if (y3 < r) {
			y3 = -1;
		}
	}*/

	//y3 = channels[channelIndex].sequencer.output;
	//channels[channelIndex].sequencer.tick();
	//y3 = (y1 - y2);//*0.56;

	y3 = y3 * (2.0 * amplitude / M_PI);
	//y3 = getSample(channels[channelIndex].freq, t, channels[channelIndex].duty, adc.H);

	//return y3*channels[channelIndex].envelopeVolume;
	if (adc.skipRest) {
		y3 *= channels[channelIndex].envelopeVolume;
		return y3;
	}
	
	if (channels[channelIndex].sweepTime && channels[channelIndex].nSweep) {
		channels[channelIndex].frequencySweepLen -= ((tickElapse)+sampleDuration * channels[channelIndex].sampleRatePos);
		//channels[channelIndex].frequencySweepLen -= sampleDurationSec;
		if (channels[channelIndex].frequencySweepLen <= 0) {//change frequency
			channels[channelIndex].frequencySweepLen = channels[channelIndex].sweepTime;
			uint16_t freqData = channels[channelIndex].loadedFreq + channels[channelIndex].sweepInc * (channels[channelIndex].loadedFreq >> channels[channelIndex].nSweep);

			uint16_t newFreq = 131072 / (2048 - freqData);
			if (newFreq > 2047) {
				channels[channelIndex].enable = false;
				setSoundState(channels[channelIndex].channelIndex, false);
			}
			else if (newFreq >= 0) {
				channels[channelIndex].loadedFreq = freqData;
				channels[channelIndex].freq = newFreq;
				//Lower 8 bits of 11 bit frequency (x). Next 3 bit are in NR14 ($FF14)
				bus->interrupt->io[0x13] = freqData & 0xff;
				bus->interrupt->io[0x14] = (bus->interrupt->io[0x14] & 0xf8) | ((freqData >> 8) & 0x07);
				//write freqDatato nn14 and nn13
			}
		}
	}
	if (channels[channelIndex].counterEnable) {

		channels[channelIndex].soundLen = channels[channelIndex].soundLen - ((tickElapse)+sampleDuration * channels[channelIndex].sampleRatePos);
		if (channels[channelIndex].soundLen <= 0) {
			//stop channel output
			channels[channelIndex].enable = false;
			setSoundState(channels[channelIndex].channelIndex, false);
		}
	}
	//return y3;
	if (channels[channelIndex].envelopeEnable) {

		//channels[channelIndex].volumeEnvelopeLen -= sampleDurationSec;

		if (channels[channelIndex].volumeEnvelopeLen <= 0) {
			channels[channelIndex].volumeEnvelopeLen = channels[channelIndex].loadedVolumeEnvelopeLen;
		}
		if (channels[channelIndex].volumeEnvelopeLen == channels[channelIndex].loadedVolumeEnvelopeLen) {

			channels[channelIndex].envelopeVolume += channels[channelIndex].envelopeDirection;
			if (channels[channelIndex].envelopeVolume == 0x0f || channels[channelIndex].envelopeVolume == 0) {
				channels[channelIndex].envelopeEnable = false;
			}
		}
		channels[channelIndex].volumeEnvelopeLen -= sampleDuration;
	}
	y3 *= channels[channelIndex].envelopeVolume;
	return y3;
}


/*
float APU::getChannelSample(uint8_t channelIndex)
{
	//sould the sweepFreq timer still running?
	if (!channels[channelIndex].enable)
		return 0;
	float m = adc.timeFactor;

	//double sampleDuration = 0.003814697265625;//ms each sample calculate 11
	// double sampleDuration = 0.00762939453125;//ms each sample calculate
	//double sampleDuration = 0.244140625*m;//ms each sample calculate
	//double sampleDuration = 0.00011920928955078125;
	// double sampleDurationSec = 0.000244140625 * m;//sec each sample calculate 131072 hz

	double sampleDurationSec = 0.001953125 * m;//4096 samples steps in terms of seconds 
	double sampleDuration = 1.953125 * m;//4096 samples steps in terms of mili seconds

	int d;
	if (adc.samplesUint)
		d = adc.have.samples;
	else
		d = adc.have.freq;
	//d = 1024;
	d = adc.have.freq;
	switch (d) {
	case 128:
		sampleDurationSec = 0.0078125 * m;
		sampleDuration = 7.8125 * m;
		break;
	case 256:
		sampleDurationSec = 0.00390625 * m;
		sampleDuration = 3.90625 * m;
		break;
	case 512:
		sampleDurationSec = 0.001953125 * m;
		sampleDuration = 1.953125 * m;
		break;
	case 1024:
		sampleDurationSec = 0.0009765625 * m;
		sampleDuration = 0.9765625 * m;
		break;
	case 2048:
		sampleDurationSec = 0.00048828125 * m;
		sampleDuration = 0.48828125 * m;
		break;
	case 4026:
		sampleDurationSec = 0.000244140625 * m;
		sampleDuration = 0.244140625 * m;
		break;
	case 8192:
		sampleDurationSec = 0.0001220703125 * m;
		sampleDuration = 0.1220703125 * m;
		break;
	case 16384:
		sampleDurationSec = 0.00006103515625 * m;
		sampleDuration = 0.06103515625 * m;
		break;
	case 32768:
		sampleDurationSec = 0.000030517578125 * m;
		sampleDuration = 0.030517578125 * m;
		break;
	case 65536:
		sampleDurationSec = 0.0000152587890625 * m;
		sampleDuration = 0.0152587890625 * m;
		break;
	case 131072:
		sampleDurationSec = 0.00000762939453125 * m;
		sampleDuration = 0.00762939453125 * m;
		break;
	}
	// sampleDurationSec = 1 / (double)adc.have.freq;
	// sampleDuration = 1000 / (double)adc.have.freq;
	double sampleRateDurationSec = 0.0000226757369614512471;
	sampleRateDurationSec = sampleDurationSec;
	sampleDuration = sampleDurationSec;
	float y1 = 0, y2 = 0, y3 = 0;

	double t = (float)(adc.audioPosition) * sampleDurationSec;//point on time line
	t = ((double)bus->cpu->steps / 4194304) * sampleRateDurationSec;// +(channels[channelIndex].sampleRatePos) * sampleRateDurationSec;
	t = ((double)bus->cpu->steps / 4194304) * sampleRateDurationSec + (channels[channelIndex].sampleRatePos) * sampleRateDurationSec;

	t = time + (channels[channelIndex].sampleRatePos) * sampleRateDurationSec;
	t = time + (channels[channelIndex].samplePosition) * sampleDurationSec;
	

	float phase = 2.0f * M_PI * channels[channelIndex].duty;
	//phase = channels[channelIndex].duty;
	float c = 0;
	adc.H = bus->h;
	for (int n = 1;n <= adc.H;n++) {
		//c = n * ((float)channels[channelIndex].freq) * 2.0 * M_PI * t;
		c = n * ((float)channels[channelIndex].freq) * 2.0 * M_PI * t;
		y1 += -approxsin(c) / n;
		y2 += -approxsin(c - phase * (double)n) / n;


	}
	y3 = (y1 - y2);
	double amplitude = 16;
	double r = 0;
	r = bus->r;
	if (adc.flatWave) {
		if (y3 > r) {
			y3 = 1;
		}
		else if (y3 < r) {
			y3 = -1;
		}
	}

	//y3 = channels[channelIndex].sequencer.output;
	//channels[channelIndex].sequencer.tick();
	//y3 = (y1 - y2);//*0.56;

	y3 = y3 * (2.0 * amplitude / M_PI);
	//y3 = getSample(channels[channelIndex].freq, t, channels[channelIndex].duty, adc.H);

	//return y3*channels[channelIndex].envelopeVolume;
	if (adc.skipRest) {
		y3 *= channels[channelIndex].envelopeVolume;
		return y3;
	}
	if (channels[channelIndex].counterEnable) {

		channels[channelIndex].soundLen = channels[channelIndex].soundLen - ((tickElapse) + sampleDuration * channels[channelIndex].sampleRatePos);
		if (channels[channelIndex].soundLen <= 0) {
			//stop channel output
			channels[channelIndex].enable = false;
			setSoundState(channels[channelIndex].channelIndex, false);
		}
	}
	if (channels[channelIndex].sweepTime && channels[channelIndex].nSweep) {
		channels[channelIndex].frequencySweepLen -= ((tickElapse) + sampleDuration * channels[channelIndex].sampleRatePos);
		//channels[channelIndex].frequencySweepLen -= sampleDurationSec;
		if (channels[channelIndex].frequencySweepLen <= 0) {//change frequency
			channels[channelIndex].frequencySweepLen = channels[channelIndex].sweepTime;
			uint16_t freqData = channels[channelIndex].loadedFreq + channels[channelIndex].sweepInc * (channels[channelIndex].loadedFreq >> channels[channelIndex].nSweep);

			uint16_t newFreq = 131072 / (2048 - freqData);
			if (newFreq > 2047) {
				channels[channelIndex].enable = false;
				setSoundState(channels[channelIndex].channelIndex, false);
			}
			else if (newFreq >= 0) {
				channels[channelIndex].loadedFreq = freqData;
				channels[channelIndex].freq = newFreq;
				//Lower 8 bits of 11 bit frequency (x). Next 3 bit are in NR14 ($FF14)
				bus->interrupt->io[0x13] = freqData & 0xff;
				bus->interrupt->io[0x14] = (bus->interrupt->io[0x14] & 0xf8) | ((freqData >> 8) & 0x07);
				//write freqDatato nn14 and nn13
			}
		}
	}

	//return y3;
	if (channels[channelIndex].envelopeEnable) {

		//channels[channelIndex].volumeEnvelopeLen -= sampleDurationSec;

		if (channels[channelIndex].volumeEnvelopeLen <= 0) {
			channels[channelIndex].volumeEnvelopeLen = channels[channelIndex].loadedVolumeEnvelopeLen;
		}
		if (channels[channelIndex].volumeEnvelopeLen == channels[channelIndex].loadedVolumeEnvelopeLen) {

			channels[channelIndex].envelopeVolume += channels[channelIndex].envelopeDirection;
			if (channels[channelIndex].envelopeVolume == 0x0f || channels[channelIndex].envelopeVolume == 0) {
				channels[channelIndex].envelopeEnable = false;
			}
		}
		channels[channelIndex].volumeEnvelopeLen -= sampleDuration;
	}
	y3 *= channels[channelIndex].envelopeVolume;
	return y3;
}*/


/*
float APU::getChannelSample(uint8_t channelIndex)
{
	//sould the sweepFreq timer still running?
	if (!channels[channelIndex].enable)
		return 0;
	float m = adc.timeFactor;

	//double sampleDuration = 0.003814697265625;//ms each sample calculate 11
	// double sampleDuration = 0.00762939453125;//ms each sample calculate
	//double sampleDuration = 0.244140625*m;//ms each sample calculate
	//double sampleDuration = 0.00011920928955078125;
	// double sampleDurationSec = 0.000244140625 * m;//sec each sample calculate 131072 hz
	
	double sampleDurationSec = 0.001953125 * m;//4096 samples steps in terms of seconds 
	 double sampleDuration = 1.953125*m;//4096 samples steps in terms of mili seconds
	
	 int d;
	 if (adc.samplesUint)
		 d = adc.have.samples;
	 else
		 d = adc.have.freq;
	 //d = 1024;
	 d = adc.have.freq;
	 switch (d) {
	 case 128:
		 sampleDurationSec = 0.0078125 * m;
		 sampleDuration = 7.8125 * m;
		 break;
	 case 256:
		 sampleDurationSec = 0.00390625 * m;
		 sampleDuration = 3.90625 * m;
		 break;
	 case 512:
		 sampleDurationSec = 0.001953125 * m;
		 sampleDuration = 1.953125 * m;
		 break;
	 case 1024:
		 sampleDurationSec = 0.0009765625 * m;
		 sampleDuration = 0.9765625 * m;
		 break;
	 case 2048:
		 sampleDurationSec = 0.00048828125 * m;
		 sampleDuration = 0.48828125 * m;
		 break;
	 case 4026:
		 sampleDurationSec = 0.000244140625 * m;
		 sampleDuration = 0.244140625 * m;
		 break;
	 case 8192:
		 sampleDurationSec = 0.0001220703125 * m;
		 sampleDuration = 0.1220703125 * m;
		 break;
	 case 16384:
		 sampleDurationSec =  0.00006103515625 * m;
		 sampleDuration = 0.06103515625 * m;
		 break;
	 case 32768:
		 sampleDurationSec = 0.000030517578125 * m;
		 sampleDuration = 0.030517578125 * m;
		 break;
	 case 65536:
		 sampleDurationSec = 0.0000152587890625 * m;
		 sampleDuration = 0.0152587890625 * m;
		 break;
	 case 131072:
		 sampleDurationSec = 0.00000762939453125 * m;
		 sampleDuration = 0.00762939453125 * m;
		 break;
	 }
	// sampleDurationSec = 1 / (double)adc.have.freq;
	// sampleDuration = 1000 / (double)adc.have.freq;
	 double sampleRateDurationSec = 0.0000226757369614512471;
	 sampleRateDurationSec = sampleDurationSec;
	
	float y1 = 0, y2 = 0, y3 = 0;
	
	
	y3 = getSample(channels[channelIndex].freq,time, channels[channelIndex].duty, adc.H);
	
	//return y3*channels[channelIndex].envelopeVolume;
	if (adc.skipRest) {
		y3 *= channels[channelIndex].envelopeVolume;
		return y3;
	}
	if (channels[channelIndex].counterEnable) {

		channels[channelIndex].soundLen = channels[channelIndex].soundLen - ((tickElapse * 1000) + sampleDuration * channels[channelIndex].sampleRatePos);
		if (channels[channelIndex].soundLen <= 0) {
			//stop channel output
			channels[channelIndex].enable = false;
			setSoundState(channels[channelIndex].channelIndex, false);
		}
	}
	if (channels[channelIndex].sweepTime && channels[channelIndex].nSweep) {
	    channels[channelIndex].frequencySweepLen -= ((tickElapse * 1000)+sampleDuration*channels[channelIndex].sampleRatePos);
		//channels[channelIndex].frequencySweepLen -= sampleDurationSec;
		if (channels[channelIndex].frequencySweepLen <= 0) {//change frequency
			channels[channelIndex].frequencySweepLen = channels[channelIndex].sweepTime;
			uint16_t freqData = channels[channelIndex].loadedFreq + channels[channelIndex].sweepInc * (channels[channelIndex].loadedFreq >> channels[channelIndex].nSweep);
			
			uint16_t newFreq = 131072 / (2048 - freqData);
			if (newFreq > 2047) {
				channels[channelIndex].enable = false;
				setSoundState(channels[channelIndex].channelIndex, false);
			}
			else if (newFreq >= 0) {
				channels[channelIndex].loadedFreq = freqData;
				channels[channelIndex].freq = newFreq;
				//Lower 8 bits of 11 bit frequency (x). Next 3 bit are in NR14 ($FF14)
				bus->interrupt->io[0x13] = freqData & 0xff;
				bus->interrupt->io[0x14] = (bus->interrupt->io[0x14] & 0xf8) | ((freqData >> 8) & 0x07);
				//write freqDatato nn14 and nn13
			}
		}
	}

	//return y3;
	if (channels[channelIndex].envelopeEnable) {
		
		//channels[channelIndex].volumeEnvelopeLen -= sampleDurationSec;
		
		 if (channels[channelIndex].volumeEnvelopeLen <= 0) {
			 channels[channelIndex].volumeEnvelopeLen = channels[channelIndex].loadedVolumeEnvelopeLen;
		}
	if (channels[channelIndex].volumeEnvelopeLen == channels[channelIndex].loadedVolumeEnvelopeLen) {
		
		channels[channelIndex].envelopeVolume += channels[channelIndex].envelopeDirection;
		if (channels[channelIndex].envelopeVolume == 0x0f || channels[channelIndex].envelopeVolume == 0) {
			channels[channelIndex].envelopeEnable = false;
		}
		}
		channels[channelIndex].volumeEnvelopeLen -= sampleDuration;
	}
	y3 *= channels[channelIndex].envelopeVolume;
	return y3;
}

*/
void APU::updateChannelFreqData(uint8_t channelIndex, uint16_t newFreqData)
{
	bus->interrupt->io[0x13] = newFreqData & 0xff;
	bus->interrupt->io[0x14] = (bus->interrupt->io[0x14] & 0xf8) | ((newFreqData >> 8) & 0x07);
}
void APU::close() {
	SDL_CloseAudioDevice(adc.dev);
	SDL_Quit();
}

void APU::play()
{
	restart = false;
	adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	
	if (!adc.dev) {
		printf("[SDL] Failed to open audio device: %s\n", SDL_GetError());
		//SDL_Quit();
		adc.want=adc.wantOld;
		adc.have=adc.haveOld;
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

