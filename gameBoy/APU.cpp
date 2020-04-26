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

	if (len != apu->adc.have.samples) {
		printf("len:%d", len);
	}
	BUS* bus = apu->bus;

		uint16_t opcode;
		uint8_t sampleCounter = 32;
		uint16_t squencerCounter = 256;
		float y1, y2, y3;

		apu->cyclesInSoundFrameCounter = 0;
		//apu->cyclesInSoundFrame= 4194304 * elapse;
		//4194304/44100=~95 cycles per sample. 97280 cycles for 1024 samples.
	//	apu->cyclesInSoundFrame = (4194304/apu->adc.have.freq)* apu->adc.have.samples;
		
		apu->channels[0].samplesData =(Sint8*)stream;
		
		bool rendered = false;
		apu->fs.hasSamples = false;
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
		    apu->fs.tick();

			//apu->tick();
			bus->gpu->tick();
			bus->cpu->updateTimers();
			bus->joypad->updateKeys();

			bus->cpu->lastOpcodeCycles += bus->interrupt->InterruptsHandler() * (4 * (bus->cpu->speedMode + 1));;

			//bus->cpu->time.addMCycles(bus->cpu->lastOpcodeCycles / 4);
		//	bus->cpu->time.print(2);
			
			bus->display->tick(bus->cpu->lastOpcodeCycles);
			bus->cpu->steps += bus->cpu->lastOpcodeCycles;
			sampleCounter-= bus->cpu->lastOpcodeCycles;
			if (apu->cyclesInSoundFrameCounter >= 69905 && !rendered) {
				bus->display->render();
				rendered = true;
			}

		} while (!apu->fs.hasSamples);
		//while (apu->cyclesInSoundFrameCounter < apu->cyclesInSoundFrame);

		
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

	
}
void APU::start() {
	if (SDL_Init(SDL_INIT_AUDIO)) {
		printf("[SDL] Failed to initialize: %s\n", SDL_GetError());
		return;
	}
	

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
    adc.want.freq = 44100;// 44100&8192
	// adc.want.freq = 22050;
	//adc.want.freq = 65536;
	//adc.want.freq = 32768;
	
	//adc.want.freq = 16384;
	
   //adc.want.freq = 8192;
	//adc.want.freq = 4096;
   // adc.want.freq = 2048;
	//adc.want.freq = 1024;
   //adc.want.freq = 512;
	//adc.want.freq = 256;
	//adc.want.freq = 128;
	//adc.want.freq = 64;
	//adc.want.freq = 4;
	//adc.want.format = AUDIO_S32;
	//adc.want.format = AUDIO_S32MSB;
	//adc.want.format = AUDIO_S16MSB;
	adc.want.format = AUDIO_S16;
	//adc.want.format = AUDIO_S8;
	adc.want.channels = 1;
	adc.want.samples = 32768;
	//adc.want.samples = 16384;

	//adc.want.samples = 8192;
	//adc.want.samples = 4096;
    //adc.want.samples = 2048;
    // adc.want.samples = 1024;
   //adc.want.samples = 512;
	adc.want.samples = 256;
	//adc.want.samples = 128;
   // adc.want.samples = 64;
	//adc.want.samples = 32;
	//adc.want.samples = 8;
	//adc.want.samples = 4;
	//adc.want.samples = 1;
	adc.want.callback = soundTick;
	adc.want.userdata = this;
	adc.H = 20;
	//H:50 freq:65536 samples:1024
	//adc.dev = SDL_OpenAudioDevice(NULL, 0, &adc.want, &adc.have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_SAMPLES_CHANGE);
	
	int nums[18] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072 };
	//adc.want.freq = 131072;
	//adc.want.samples = 32;
	adc.want.samples = 1024;
	adc.want.callback = NULL;
	//adc.want.format = AUDIO_F32SYS;
	//adc.want.format = AUDIO_S32;
	adc.want.format = AUDIO_S8;
	
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
	//SDL_PauseAudioDevice(dev, p); /* play! */
	channels[0].channelIndex = 0;
	channels[0].samplesData = (Sint8*)calloc(adc.have.samples, sizeof(Sint8));
	channels[0].samplePosition = 0;
	fs.sampleGenerateCounterReload = 4194304 / adc.have.freq;
	fs.sampleGenerateCounter = fs.sampleGenerateCounterReload;
	startTimer = steady_clock::now();
	lastTick = startTimer;

	adc.audioPosition = 0;
	adc.audioFrequency = 1.0 * adc.FREQ / adc.have.freq;
	//adc.audioFrequency = 1.0 * adc.FREQ; /* 1.0 to make it a float */
	adc.audioVolume = 6000; /* ~1/5 max volume */
	adc.audioLen = 60 * adc.have.freq;//1 min.

	SDL_PauseAudioDevice(adc.dev, 0);
	fs.apu = this;
	fs.ch = channels;


	adc.sampleDurationSec = 0.001953125;//4096 samples steps in terms of seconds 
	adc.sampleDurationMS = 1.953125;//4096 samples steps in terms of mili seconds
	float m = adc.timeFactor;
	switch (adc.have.freq) {
	case 128:
		adc.sampleDurationSec = 0.0078125 * m;
		adc.sampleDurationMS = 7.8125 * m;
		break;
	case 256:
		adc.sampleDurationSec = 0.00390625 * m;
		adc.sampleDurationMS = 3.90625 * m;
		break;
	case 512:
		adc.sampleDurationSec = 0.001953125 * m;
		adc.sampleDurationMS = 1.953125 * m;
		break;
	case 1024:
		adc.sampleDurationSec = 0.0009765625 * m;
		adc.sampleDurationMS = 0.9765625 * m;
		break;
	case 2048:
		adc.sampleDurationSec = 0.00048828125 * m;
		adc.sampleDurationMS = 0.48828125 * m;
		break;
	case 4026:
		adc.sampleDurationSec = 0.000244140625 * m;
		adc.sampleDurationMS = 0.244140625 * m;
		break;
	case 8192:
		adc.sampleDurationSec = 0.0001220703125 * m;
		adc.sampleDurationMS = 0.1220703125 * m;
		break;
	case 16384:
		adc.sampleDurationSec = 0.00006103515625 * m;
		adc.sampleDurationMS = 0.06103515625 * m;
		break;
	case 32768:
		adc.sampleDurationSec = 0.000030517578125 * m;
		adc.sampleDurationMS = 0.030517578125 * m;
		break;
	case 65536:
		adc.sampleDurationSec = 0.0000152587890625 * m;
		adc.sampleDurationMS = 0.0152587890625 * m;
		break;
	case 131072:
		adc.sampleDurationSec = 0.00000762939453125 * m;
		adc.sampleDurationMS = 0.00762939453125 * m;
		break;
	}

	adc.sampleDurationSec = 1/(double)adc.have.freq;

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
	//channels[channelIndex].frequencySweepLenReload = sweepTimes[(value>>4)&0x07];
	channels[channelIndex].frequencySweepLenReload =(value >> 4) & 0x07;
	channels[channelIndex].sweepInc = ((value >> 3) & 0x01) ? -1 : 1;
	channels[channelIndex].nSweep = value & 0x07;
}

void APU::feedLenAndDutyRegister(uint8_t channelIndex, uint8_t value)
{
	/*
		Bit 7-6 - Wave Pattern Duty (Read/Write)
		Bit 5-0 - Sound length data (Write Only) (t1: 0-63)
	*/
	channels[channelIndex].soundLenData = value & 0x3f;
	channels[channelIndex].duty = duties[(value >> 6)&0x03];
	channels[channelIndex].sequencer.sequence = waveForms[(value >> 6) & 0x03];
	channels[channelIndex].sequencer.wave = channels[channelIndex].sequencer.sequence;
	channels[channelIndex].sequencer.waveForm = channels[channelIndex].sequencer.waveForms[(value >> 6) & 0x03];
	
	channels[channelIndex].sequencer.step = 0;
	//channels[channelIndex].soundLen = ((64 - (double)channels[channelIndex].soundLenData) / (256)) * 1000;// for ms
	//channels[channelIndex].soundLen = ((64 - (double)channels[channelIndex].soundLenData) / (256));// for sec
	channels[channelIndex].soundLen = ((64 - (double)channels[channelIndex].soundLenData));//

	fs.squareLimit = fs.squareLimits[(value >> 6) & 0x03];
	fs.lenTimerReload = channels[channelIndex].soundLen * 4194304;
	fs.lenTimer =fs.lenTimerReload;
	
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
	//channels[channelIndex].loadedVolumeEnvelopeLen = ((double)channels[channelIndex].nEnvelopeSweep / 64);//*1000;//ms																									  //channels[channelIndex].loadedVolumeEnvelopeLen = ((double)channels[channelIndex].nEnvelopeSweep / 64) ;//sec
	channels[channelIndex].loadedVolumeEnvelopeLen = channels[channelIndex].nEnvelopeSweep;
	channels[channelIndex].envelopeEnable = (channels[channelIndex].nEnvelopeSweep != 0)
		&&(channels[channelIndex].envelopeVolume >0x00&& channels[channelIndex].envelopeVolume <0x0f);
	fs.volEnvelopeTimerReload = 4194304 * channels[channelIndex].loadedVolumeEnvelopeLen;
	fs.volEnvelopeTimer = fs.volEnvelopeTimerReload;
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
		fs.lenTimerReload =0;
		fs.lenTimer = fs.lenTimerReload;
	}
	
	fs.dutyTimerReload = 4194304/(4 * (2048 - channels[channelIndex].loadedFreq));
	fs.dutyTimer = fs.dutyTimerReload;
	channels[channelIndex].sequencer.counterReload = (4 * (2048 - channels[channelIndex].freqData));
	channels[channelIndex].sequencer.counter = channels[channelIndex].sequencer.counterReload;
	if ((value >> 7) & 0x01) {//trigger 
		
		channels[channelIndex].enable = true;
		/*if (channels[channelIndex].counter == 0)
			channels[channelIndex].counter = channelIndex != 2 ? 64 : 256;*/

		if (channels[channelIndex].soundLen == 0) {
			channels[channelIndex].soundLen = channelIndex != 2 ? 64 : 256;
			//channels[channelIndex].soundLen = ((64 - (double)channels[channelIndex].soundLenData) / (256));//sec
			fs.lenTimerReload = channels[channelIndex].soundLen * 4194304;
			fs.lenTimer = fs.lenTimerReload;
		}
		channels[channelIndex].freq = 131072 / (2048 - channels[channelIndex].loadedFreq);
		channels[channelIndex].frequencySweepLen = channels[channelIndex].frequencySweepLenReload;
		channels[channelIndex].volumeEnvelopeLen = channels[channelIndex].loadedVolumeEnvelopeLen;
		fs.volEnvelopeTimer = fs.volEnvelopeTimerReload;
		
		setSoundState(channelIndex, true);
		//channels[channel]. = channels[channel].nEnvelopeSweep / 64;//sec

		


		if (adc.paused) {
			
			adc.paused = false;
		//	printf("sound trigged!\n");
		}
		
	}
	else {
		if (!adc.paused) {
			//printf("sound stoped!\n");
			//createAudioDeviceControl(true);
			adc.paused = true;
		}
		
	}
}
double approxsin(double t) {
	double j = t * 0.15915;
	j = j - (int)j;
	return 20.785 * j * (j - 0.5) * (j - 1.0f);

}
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

