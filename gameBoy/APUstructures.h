#pragma once
#include "SDL.h"
#include<cstdint>
#include <math.h>
#include <stdio.h>



struct SoundControl {
	/*
	 (FF24)
	 Bit 7   - Output Vin to SO2 terminal (1=Enable)
	 Bit 6-4 - SO2 output level (volume)  (0-7)
	 Bit 3   - Output Vin to SO1 terminal (1=Enable)
	 Bit 2-0 - SO1 output level (volume)  (0-7)
	*/
	bool VinS02;//left headphone
	uint8_t S02Volume;
	bool VinS01;//right hearphone
	uint8_t S01Volume;
	uint8_t soundState = 0x0;
	void setSoundState(uint8_t channelIndex, bool set) {
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
			soundState &= 0xff ^ (0x01 << channelIndex);
	}
};
struct AudioDeviceControl {
	int FREQ = 200; /* the frequency we want */
	unsigned int audioPosition; /* which sample we are up to */
	int audioLen; /* how many samples left to play, stops when <= 0 */
	float audioFrequency; /* audio frequency in cycles per sample */
	float audioVolume; /* audio volume, 0 - ~32000 */
	uint32_t H = 88;//harmonies count.
	uint32_t timeFactor = 1;
	bool samplesUint = true;
	bool flatWave = true;
	bool skipRest = false;
	uint16_t sectorPosition = 0;//divid 262144 samples to secors of 128 size each.
	SDL_AudioSpec want, have;
	SDL_AudioDeviceID dev;
	SDL_AudioDeviceID devOld;
	SDL_AudioSpec wantOld, haveOld;
	double sampleDurationSec;
	double sampleDurationMS;
	bool paused = true;

	int sampleGenerateCounterReload;
	int sampleGenerateCounter;
	bool hasSample;
	int sampleCounter = 0;
	Sint8* leftSamplesData;
	Sint8* rightSamplesData;
	Sint8* samplesData;
};
struct Sequencer {
	bool enable = true;
	uint8_t sequence = 0;
	double counter = 0;
	double counterReload = 0;
	double len = 0;
	uint8_t output = 0;
	uint32_t wave = 0;
	uint8_t step = 0;
	uint8_t dutiesWaves[4] = { 0b00000001,0b10000001,0b10000111,0b01111110 };
	const char* waveForms[4] =
	{
		"12.5% ( _-------_-------_------- )",
		"25%   ( __------__------__------ )",
		"50%   ( ____----____----____---- )",
		"75%   ( ______--______--______-- )"
	};
	const char* waveForm;
	double timerReload = 8192;
	double timer = 8192;
	int stages = 7;
	void setWave(uint8_t i) {
		sequence = dutiesWaves[i];
		wave = sequence;
		waveForm = waveForms[i];
		step = 0;
		output = sequence & 0x01;
	}
	void setTimer(double frequency) {
		timerReload = (1 / frequency) * 524288;
		timer = timerReload;
	}
	void tick(int cycles) {
		//counter-=cycles;
		timer -= cycles;
		if (timer <= 0) {
			timer += timerReload;
			sequence = ((sequence & 0x01) << 7) | ((sequence & 0xFE) >> 1);
			output = sequence & 0x01;
			step++;
			step = step %8;
		}

	}
};
class Channel {
public:
	struct AudioDeviceControl* adc;
	struct SoundControl* soundCtrl;
	struct Sequencer sequencer;
	uint8_t channelIndex;
	bool enable = false;
	double frequencySweepLenReload;
	double frequencySweepLen;
	//Lower 8 bits of 11 bit frequency in ($FF14). Next 3 bit are in NR14 ($FF14)
	uint32_t freq;
	uint16_t freqData;
	uint16_t loadedFreq;
	int8_t sweepInc = 1;//sweep Increase(=1)/Decrease(=-1) 
	uint8_t nSweep;//number of sweep shift (n: 0-7)
	float duty;
	//(t1: 0-63)
	//Sound Length = (64-t1)*(1/256) seconds The Length value is used only if Bit 6 in NR14 is set.
	//If length counter is zero, it is set to 64 (256 for wave channel).
	double soundLen;
	uint8_t soundLenData;//t1:0-63
	bool envelopeEnable;
	//Initial Volume of envelope (0-0Fh) (0=No Sound)
	uint8_t envelopeVolume;
	uint8_t envelopeVolumeReload;
	//Envelope Direction (0=Decrease, 1=Increase)
	int8_t envelopeDirection = 1;
	//Number of envelope sweep (n: 0-7) (If zero, stop envelope operation.)
	//Length of 1 step = n*(1/64) seconds
	uint8_t nEnvelopeSweep;
	double volumeEnvelopeLen;
	double loadedVolumeEnvelopeLen;
	//Counter/consecutive selection (1=Stop output when length in NR11 expires)
	bool counterEnable;
	uint8_t outputTerminal;//0 none 1 left 2 right 3 both(sum).


	uint32_t samplePosition = 0;
	Sint8* leftSamplesData;
	Sint8* rightSamplesData;
	Sint8* samplesData;
	int sampleRatePos = 0;


	int timerReload = 8192;
	int timer = 8192;
	uint8_t frameStep = 0;
	uint8_t vol = 0;
	uint8_t waveformInput = 0;
	float output = 0;
	int sampleGenerateCounterReload = 95;//4194304/44100
	int sampleGenerateCounter = 95;
	const char* lastwaveform;
	double time = 0;
	bool volSaved = false;
	//	float squareLimits[4] = {
	//-0.25, // 12.5% ( _-------_-------_------- )
	// -0.5,  // 25%   ( __------__------__------ )
	// 0,     // 50%   ( ____----____----____---- ) (normal)
	// 0.5,   // 75%   ( ______--______--______-- )
	//	};

	bool hasSamples = false;
	uint8_t freqLo;
	uint8_t freqHi;
	virtual void tick(int cycles) = 0;
	virtual void getSample() {}
	virtual void sweep() {}
	virtual void envelope() {}
	virtual void loadNRX1(uint8_t  value) = 0;
	virtual void loadNRX2(uint8_t  value) = 0;
	virtual void loadNRX3(uint8_t  value) = 0;
	virtual void loadNRX4(uint8_t  value) = 0;
	float out() {
		return 0;
		//return (((float)ch->sequencer->output / 100) * vol);
	}
	void updateChannelFreqData(uint16_t newFreqData)
	{
		freqLo = newFreqData & 0xff;
		freqHi = (freqHi & 0xf8) | ((newFreqData >> 8) & 0x07);
	}
};
class SquareWave :public Channel {
protected:
	float squareLimits[4] = {
		-0.25, // 12.5% ( _-------_-------_------- )
		 -0.5,  // 25%   ( __------__------__------ )
		 0,     // 50%   ( ____----____----____---- ) (normal)
		 0.5,   // 75%   ( ______--______--______-- )
	};
	float squareLimit;

public:
	void setSquareLimit(uint8_t i) {
		squareLimit = squareLimits[i];
	}
	int tempsteps = 0;
	
	void loadNRX1(uint8_t  value) {
		/*
		Bit 7-6 - Wave Pattern Duty (Read/Write)
		Bit 5-0 - Sound length data (Write Only) (t1: 0-63)
	*/
		soundLenData = value & 0x3f;
		//duty = duties[(value >> 6) & 0x03];
		sequencer.setWave((value >> 6) & 0x03);
		soundLen = ((64 - (double)soundLenData));
		setSquareLimit((value >> 6) & 0x03);
	}
	void loadNRX2(uint8_t  value) {
		/*
			Bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
			Bit 3   - Envelope Direction (0=Decrease, 1=Increase)
			Bit 2-0 - Number of envelope sweep (n: 0-7)
			(If zero, stop envelope operation.)
			Length of 1 step = n*(1/64) seconds
		*/
		nEnvelopeSweep = value & 0x07;
		envelopeDirection = (value >> 3) & 0x01 ? 1 : -1;
		envelopeVolumeReload = (value >> 4) & 0x0f;
		envelopeVolume = envelopeVolumeReload;
		loadedVolumeEnvelopeLen = nEnvelopeSweep;
		envelopeEnable = (nEnvelopeSweep != 0) && (0x100 - envelopeVolume > 0);

	}
	void loadNRX3(uint8_t value) {
		//Lower 8 bits of 11 bit frequency (x). Next 3 bit are in NR14 ($FF14)
		freqLo = value;
		loadedFreq = (loadedFreq & 0x700) | (value & 0xff);
	}
	void loadNRX4(uint8_t value) {
		/*
		Bit 7   - Initial (1=Restart Sound)     (Write Only)
		Bit 6   - Counter/consecutive selection (Read/Write)
				(1=Stop output when length in NR11 expires)
		Bit 2-0 - Frequency's higher 3 bits (x) (Write Only)
	*/
		freqHi = value;
		loadedFreq = (value & 0x07) << 8 | (loadedFreq & 0xff);
		counterEnable = (((value >> 6) & 0x01) == 0x01);
		if (!counterEnable) {
			soundLen = 0;
		}

		sequencer.counterReload = (4 * (2048 - freqData));
		sequencer.counter = sequencer.counterReload;
		if ((value >> 7) & 0x01) {//trigger 
			enable = true;
			if (soundLen == 0) 
				soundLen = 64;
	
			freq = 131072 / (2048 - loadedFreq);
			frequencySweepLen = frequencySweepLenReload;
			volumeEnvelopeLen = loadedVolumeEnvelopeLen;
			sequencer.setTimer(freq);
			soundCtrl->setSoundState(channelIndex, true);
		}
	}
	virtual void sweep() {}
	void tick(int cycles) override {
		timer -= cycles;
		sampleGenerateCounter -= cycles;
		tempsteps += cycles;
		if (timer <= 0) {
			vol = envelopeVolume;
			timer += timerReload;

			//return;
			switch (frameStep) {//prpblem with len counter.
			case 0:
				lenCounter();
				break;
			case 1:
				break;
			case 2:
				sweep();
				lenCounter();
				break;
			case 3:
				break;
			case 4:
				lenCounter();
				break;
			case 5:
				break;
			case 6:
				sweep();
				lenCounter();
				break;
			case 7:
				envelope();
				break;
			}
			frameStep = frameStep < 7 ? frameStep + 1 : 0;



		}

		
		getSample();
		sequencer.tick(cycles);
	}
	void getSample() {
		if (sampleGenerateCounter <= 0) {
			sampleGenerateCounter += sampleGenerateCounterReload;

			/*auto approxsin = [](float t) {
				double j = t * 0.15915;
				j = j - (int)j;
				return 20.785 * j * (j - 0.5) * (j - 1.0f);
			};*/

			Uint8 rightSample;
			Uint8 leftSample;
			float phase = 2.0f * M_PI * duty;

			float f = (float)freq;

			double g = 0;
			double c = 0, y1 = 0, y2 = 0;
			double t = adc->audioPosition * adc->sampleDurationSec;
			//t = (double)tempsteps / 4194304+(adc->audioPosition * adc->sampleDurationSec);
			time += freq * M_PI / (float)adc->have.freq;
			time = 0;
			double x = t * 2 * M_PI * f;

			//g += sin(x * k - (double)phase * k) / k;
		//for (int k = 1;k < 20;k += 2) {	
		//		g += approxsin(x * k - (double)phase * k) / k;
		//	//	g += -approxsin(x * k) / k;
		//	}
		/*for (int n = 1;n <= 20;n++) {
				c = n * x;
				y1 += -approxsin(c) / n;
				y2 += -approxsin(c - phase * (double)n) / n;
			}
			g = (y1 - y2)* (2.0/ M_PI);*/
			//  vol= ch->envelopeVolume;
			  /*if (!ch->enable)
				  vol = 0;	*/

				  //sample = ((g*apu->adc.audioVolume * vol))/100;
			if (false) {
				/*if (ch->enable && (ch->sequencer->waveForm != lastwaveform || false)) {
					printf("g:%f out:%d sequence:%d step:%d wave:%s\n", g, ch->sequencer->output, ch->sequencer->sequence, ch->sequencer->step, ch->sequencer->waveForm);
					lastwaveform = ch->sequencer->waveForm;
				}*/

				float p = adc->audioVolume * vol;
				p = sequencer.output * duty * adc->audioVolume * vol;
				p = g * adc->audioVolume * vol * sequencer.output;
				p = ((float)p) / (float)32768;
				if (p > 1) p = 1;
				if (p < -1) p = -1;
				/*if (p < 0.5)
					p = -p;*/
					//p *= apu->adc.audioVolume * vol;

				rightSample = p;
			}



			/*g = sin(time);
			time += (2 * M_PI * ch->freq) / (float)apu->adc.have.samples;
			sample = (g * ch->sequencer.output * apu->adc.audioVolume * vol);*/
			//sample = ((g * apu->adc.audioVolume * vol));

			//time =SDL_GetTicks()* ch->freq * M_PI;
			//time =(apu->bus->cpu->steps/419433)*2.0f*M_PI;
			/*float h = sin(x);
			Sint8 temp = sequencer.output;
			
			if (h > squareLimit)
				h = 0xff;
			else
				h = 0;
			if (h != h2) {
				int test = 0;
			}
			else if (h == h2) {
				int test = 0;
			}*/

			//h = h2;
			float h = 0xff * (1 - sequencer.output);

				//apu->adc.audioVolume


			//sample = ((g * ((double)vol / 15) * v));
			//sample = ((g * apu->adc.audioVolume * vol));

				//sample = 0.5;

			//ch->samplesData[sampleCounter] = sample;
			//ch->samplesData[2*sampleCounter+1] = 1;
			if (!enable) {
				vol = 0;
			}

			float v = (((float)(soundCtrl->S01Volume + soundCtrl->S02Volume) / 7)) / 10;

			leftSample = ((h * ((double)vol / 15) * v));
			//float v = (((float)(apu->soundCtrl.S01Volume + apu->soundCtrl.S02Volume) / 7)) / 10;

			//rightSample = ((h * ((double)vol / 15) * (double)apu->soundCtrl.S01Volume / 70));
			//leftSample = ((h * ((double)vol / 15) * (double)apu->soundCtrl.S02Volume / 70));




			//ch->samplesData[2*sampleCounter] = rightSample;
			//ch->samplesData[2*sampleCounter+1] = leftSample;
			//ch->rightSamplesData[sampleCounter] = leftSample;
			//ch->rightSamplesData[sampleCounter] = 0;
			leftSamplesData[adc->sampleCounter] = leftSample;
			//ch->samplesData[2*sampleCounter] = 128;
			volSaved = true;
			if (rightSamplesData[adc->sampleCounter] != 0)
				int test = 0;
			//sampleCounter++;
			adc->hasSample = true;
			//TODO
			/*if (sampleCounter == apu->adc.have.samples) {
				hasSamples = true;
				sampleCounter = 0;
				//SDL_QueueAudio(apu->adc.dev, ch->samplesData, apu->adc.have.channels * apu->adc.have.samples);
				//SDL_QueueAudio(apu->adc.dev, ch->samplesData,  apu->adc.have.samples);
				SDL_QueueAudio(apu->adc.dev, ch->leftSamplesData, apu->adc.have.samples);
				//SDL_QueueAudio(apu->adc.dev, ch->leftSamplesData, apu->adc.have.samples);
				//SDL_QueueAudio(apu->adc.dev, ch->samplesData, apu->adc.have.channels * apu->adc.have.samples);
			}*/

		}

	}
	void lenCounter() {
		if (counterEnable) {
			if (soundLen <= 0) {
				enable = false;
				soundCtrl->setSoundState(channelIndex, false);
			}
			soundLen--;
		}
	}
	void envelope() {
		if (envelopeEnable) {
			if (volumeEnvelopeLen == 0) {
				volumeEnvelopeLen = loadedVolumeEnvelopeLen;
				uint8_t newVolume = envelopeVolume + envelopeDirection;
				if (newVolume >= 0x0f || newVolume < 0) {
					envelopeEnable = false;
				}
				else {
					if (!volSaved) {
						int test = 0;
					}
					envelopeVolume = newVolume;
					volSaved = false;

					/*if (sequencer.output != 0)
						vol = ch->envelopeVolume;
					else vol = 0;*/
				}

			}
			else {
				//vol = 0;
			}
			//}
			volumeEnvelopeLen--;
		}
	}
};
class Channel1 :public SquareWave {
public:
	void loadNRX0(uint8_t value) {
		/*
			Bit 6-4 - Sweep Time
			Bit 3   - Sweep Increase/Decrease
			0: Addition    (frequency increases)
			1: Subtraction (frequency decreases)
			Bit 2-0 - Number of sweep shift (n: 0-7)
		*/
		frequencySweepLenReload = (value >> 4) & 0x07;
		sweepInc = ((value >> 3) & 0x01) ? -1 : 1;
		nSweep = value & 0x07;
	}
	void sweep() override{
		//return;
		if (frequencySweepLenReload && nSweep) {

			if (frequencySweepLen <= 0) {//change frequency
				frequencySweepLen = frequencySweepLenReload;
				uint16_t freqData = loadedFreq + sweepInc * (loadedFreq >> nSweep);
				//uint16_t freqData = ch->freq + ch->sweepInc * (ch->freq >> ch->nSweep);


				//uint16_t newFreq = 131072 / (2048 - freqData);
				if (freqData > 2047) {
					enable = false;
					soundCtrl->setSoundState(channelIndex, false);
				}
				else if (freqData >= 0) {
					loadedFreq = freqData;
					uint16_t newFreq = 131072 / (2048 - freqData);
					//uint16_t newFreq = ch->freqData + ch->sweepInc * (ch->freqData >> ch->nSweep);
					freq = newFreq;
					sequencer.setTimer(freq);
					//Lower 8 bits of 11 bit frequency (x). Next 3 bit are in NR14 ($FF14)
					updateChannelFreqData(freqData);
					//write freqDatato nn14 and nn13
				}

			}
			frequencySweepLen--;
		}

	}
};
class Wave :public Channel {
protected:
	float outputLevels[4]={ 0,1,0.5,0.25 };
	float outputLevel = 0;
	uint8_t wavePatternRamPosition = 0;
	float squencerTimerReload;
	float squencerTimer;
public:
	uint8_t wavePatternRam[0x10] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	int tempsteps = 0;
	void loadNRX0(uint8_t value) {
		/*
			Bit 7 - Sound Channel 3 Off  (0=Stop, 1=Playback)  (Read/Write)
		*/
		enable = (value >> 7) & 0x01;
	}
	void loadNRX1(uint8_t  value) {
		/*
		Bit 7-0 - Sound length (t1: 0 - 255)
	*/
		soundLenData = value;
		soundLen = ((256 - (double)soundLenData));
	}
	void loadNRX2(uint8_t  value) {
		/*
			Bit 6-5 - Select output level (Read/Write)
		*/
		outputLevel = outputLevels[(value >> 5) & 0x03];
	}
	void loadNRX3(uint8_t value) {
		//Lower 8 bits of 11 bit frequency (x). Next 3 bit are in NR14 ($FF14)
		freqLo = value;
		loadedFreq = (loadedFreq & 0x700) | (value & 0xff);
	}
	void loadNRX4(uint8_t value) {
		/*
		Bit 7   - Initial (1=Restart Sound)     (Write Only)
		Bit 6   - Counter/consecutive selection (Read/Write)
           (1=Stop output when length in NR31 expires)
		Bit 2-0 - Frequency's higher 3 bits (x) (Write Only)
	*/
		freqHi = value;
		loadedFreq = (value & 0x07) << 8 | (loadedFreq & 0xff);
		counterEnable = (((value >> 6) & 0x01) == 0x01);
		if (!counterEnable) {
			soundLen = 0;
		}
		//squencerTimerReload = (2 * (2048 - (float)freqData));
		//timerReload = (1 / frequency) * 524288;
		//4194304/32=131072 cycles per ram sample pormote
		
		//squencerTimer = squencerTimerReload;
		if ((value >> 7) & 0x01) {//trigger 
			enable = true;
			if (soundLen == 0)
				soundLen = 256;

			freq = 65536 / (2048 - loadedFreq);
			//freq= (2 * (2048 - (float)loadedFreq));
			volumeEnvelopeLen = loadedVolumeEnvelopeLen;
			squencerTimerReload = (1 / (float)freq) * 65536;
			//squencerTimerReload = (1 / (float)freq) * 131072;
			//squencerTimerReload = (1 / (float)freq) * 262144;
			//squencerTimerReload = (1 / (float)freq) * 524288;
			//float x =14;
			//squencerTimerReload = (1 / (float)freq)*(pow(2,x));
			squencerTimer = squencerTimerReload;
			timer = timerReload;
			wavePatternRamPosition = 0;
			soundCtrl->setSoundState(channelIndex, true);
		}
	}
	virtual void sweep() {}
	void tick(int cycles) override {
		timer -= cycles;

		squencerTimer -= cycles;
		if (squencerTimer <= 0) {
			squencerTimer += squencerTimerReload;
			wavePatternRamPosition++;
			wavePatternRamPosition = wavePatternRamPosition % 32;
		}


		sampleGenerateCounter -= cycles;
		tempsteps += cycles;
		if (timer <= 0) {
			vol = envelopeVolume;
			timer += timerReload;

			//return;
			switch (frameStep) {//prpblem with len counter.
			case 0:
				lenCounter();
				break;
			case 1:
				break;
			case 2:
				lenCounter();
				break;
			case 3:
				break;
			case 4:
				lenCounter();
				break;
			case 5:
				break;
			case 6:
				lenCounter();
				break;
			}
			frameStep = frameStep < 7 ? frameStep + 1 : 0;



		}


		getSample();
		
	}
	void getSample() {
		if (sampleGenerateCounter <= 0) {
			sampleGenerateCounter += sampleGenerateCounterReload;

			Uint8 rightSample;
			Uint8 leftSample;
			float h = 0xff * (1 - sequencer.output);
			uint8_t sampleByte = wavePatternRam[wavePatternRamPosition / 2];
			h = ((sampleByte >> (4-4*(wavePatternRamPosition%2))))&0x0f;
			
			h *= outputLevel;
			
		
			//apu->adc.audioVolume


		//sample = ((g * ((double)vol / 15) * v));
		//sample = ((g * apu->adc.audioVolume * vol));

			//sample = 0.5;

		//ch->samplesData[sampleCounter] = sample;
		//ch->samplesData[2*sampleCounter+1] = 1;
			if (!enable) {
				vol = 0;
			}
			vol = 15;
			float v = (((float)(soundCtrl->S01Volume + soundCtrl->S02Volume) / 7)) / 10;

			if (!enable) {
				h = 0;
			}
			//leftSample = (h/0xf)*0xff*v;
		    //h = (h / 8) - 1;
			//h = (h/0xf)*0xff;
			//h /= 0xf;
			//h = 1 - h;
			leftSample = h;
			//float v = (((float)(apu->soundCtrl.S01Volume + apu->soundCtrl.S02Volume) / 7)) / 10;

			//rightSample = ((h * ((double)vol / 15) * (double)apu->soundCtrl.S01Volume / 70));
			//leftSample = ((h * ((double)vol / 15) * (double)apu->soundCtrl.S02Volume / 70));




			//ch->samplesData[2*sampleCounter] = rightSample;
			//ch->samplesData[2*sampleCounter+1] = leftSample;
			//ch->rightSamplesData[sampleCounter] = leftSample;
			//ch->rightSamplesData[sampleCounter] = 0;
			leftSamplesData[adc->sampleCounter] = leftSample;
			//ch->samplesData[2*sampleCounter] = 128;
			volSaved = true;
			if (rightSamplesData[adc->sampleCounter] != 0)
				int test = 0;
			//sampleCounter++;
			adc->hasSample = true;
			//TODO
			/*if (sampleCounter == apu->adc.have.samples) {
				hasSamples = true;
				sampleCounter = 0;
				//SDL_QueueAudio(apu->adc.dev, ch->samplesData, apu->adc.have.channels * apu->adc.have.samples);
				//SDL_QueueAudio(apu->adc.dev, ch->samplesData,  apu->adc.have.samples);
				SDL_QueueAudio(apu->adc.dev, ch->leftSamplesData, apu->adc.have.samples);
				//SDL_QueueAudio(apu->adc.dev, ch->leftSamplesData, apu->adc.have.samples);
				//SDL_QueueAudio(apu->adc.dev, ch->samplesData, apu->adc.have.channels * apu->adc.have.samples);
			}*/

		}

	}
	void lenCounter() {
		if (counterEnable) {
			if (soundLen <= 0) {
				enable = false;
				soundCtrl->setSoundState(channelIndex, false);
			}
			soundLen--;
		}
	}
};
class Noise :public Channel {
protected:
	double squencerTimerReload = 0;
	double squencerTimer = 0;
public:
	
	void loadNRX1(uint8_t  value) {
		/*
		 Bit 5-0 - Sound length data (t1: 0-63)
	*/
		soundLenData = value & 0x3f;		
		soundLen = ((64 - (double)soundLenData));
	}
	void loadNRX2(uint8_t  value) {
		/*
			Bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
			Bit 3   - Envelope Direction (0=Decrease, 1=Increase)
			Bit 2-0 - Number of envelope sweep (n: 0-7)
			(If zero, stop envelope operation.)
			Length of 1 step = n*(1/64) seconds
		*/
		nEnvelopeSweep = value & 0x07;
		envelopeDirection = (value >> 3) & 0x01 ? 1 : -1;
		envelopeVolumeReload = (value >> 4) & 0x0f;
		envelopeVolume = envelopeVolumeReload;
		loadedVolumeEnvelopeLen = nEnvelopeSweep;
		envelopeEnable = (nEnvelopeSweep != 0) && (0x100 - envelopeVolume > 0);

	}
	void loadNRX3(uint8_t value) {
		/*
		Bit 7-4 - Shift Clock Frequency (s)
		Bit 3   - Counter Step/Width (0=15 bits, 1=7 bits)
		Bit 2-0 - Dividing Ratio of Frequencies (r)
		*/
		dividingROF = (value & 0x07);
		dividingROF = dividingROF ? dividingROF : 0.5;
		counterStep = ((value >> 3) & 0x01) ? 7 : 15;
		sClockFreq = (value >> 4) & 0xf;
		freq = (double)(524288 / dividingROF / pow(2, ((double)sClockFreq + 1)));
		/*squencerTimerReload = ((1 / (double)freq) * 4194304);
		squencerTimerReload = ((1 / (double)freq) * 4194304/(counterStep));
		squencerTimer = squencerTimerReload;*/


	}
	void loadNRX4(uint8_t value) {
		/*
		Bit 7   - Initial (1=Restart Sound)     (Write Only)
		Bit 6   - Counter/consecutive selection (Read/Write)
		   (1=Stop output when length in NR41 expires)
		*/
		counterEnable = ((value >> 6) & 0x01);
		enable = ((value >> 7) & 0x01);
		if (enable) {
			if (soundLen == 0) {
				soundLen = 64;
			}

			squencerTimerReload = ((1 / (double)freq) * 4194304);
			squencerTimerReload = ((1 / (double)freq) * 4194304 / (counterStep));
			squencerTimer = squencerTimerReload;
			volumeEnvelopeLen = loadedVolumeEnvelopeLen;
			envelopeVolume = envelopeVolumeReload;
			LFSR = 0xffff;
			soundCtrl->setSoundState(3, true);
		}
	}
	virtual void sweep() {}
	uint16_t LFSR;
	//Bit 7 - 4 - Shift Clock Frequency(s)
	uint8_t sClockFreq;
	//Bit 3 - Counter Step / Width(0 = 15 bits, 1 = 7 bits)
	uint8_t counterStep;
	//Bit 2 - 0 - Dividing Ratio of Frequencies(r)
	float dividingROF;
	void setPolynomialCounter(uint8_t value) {
		dividingROF = (value & 0x07);
		dividingROF = dividingROF ? dividingROF : 0.5;
		counterStep = ((value >> 3) & 0x01) ? 7 : 15;
		sClockFreq = (value >> 4) & 0xf;
		freq = (double)(524288 / dividingROF / pow(2,((double)sClockFreq + 1)));
		/*squencerTimerReload = ((1 / (double)freq) * 4194304);
		squencerTimerReload = ((1 / (double)freq) * 4194304/(counterStep));
		squencerTimer = squencerTimerReload;*/
	}
	void inital(uint8_t value) {
		counterEnable = ((value >> 6) & 0x01);
		enable = ((value >> 7) & 0x01);
		
		if (counterEnable) {
			//soundLen = counterStep;
		}
		if (enable) {
			if (soundLen == 0) {
				soundLen = 64;
			}

			squencerTimerReload = ((1 / (double)freq) * 4194304);
			squencerTimerReload = ((1 / (double)freq) * 4194304 / (counterStep));
			squencerTimer = squencerTimerReload;
			volumeEnvelopeLen = loadedVolumeEnvelopeLen;
			envelopeVolume = envelopeVolumeReload;
			LFSR = 0xffff;
			soundCtrl->setSoundState(3, true);
		}


	}
	void shiftLFSR() {
		//uint8_t xorResult = (LFSR & 0X01) ^ ((LFSR>>1) & 0X01);
		
		
		if (counterStep == 7) {
			uint8_t xorResult = (LFSR & 0X01) ^ ((LFSR >> 6) & 0X01);
			//uint8_t xorResult = (LFSR & 0X01) ^ ((LFSR >> 1) & 0X01);
			LFSR >>= 1;
			//LFSR &= 0xff3f;
			//LFSR &= 0xffbf;
			LFSR &= 0xbfff;
			//LFSR |= (xorResult << 6);
			//LFSR |= (xorResult << 7);
			LFSR |= (xorResult << 14);
		}
		else {
			uint8_t xorResult = (LFSR & 0X01) ^ ((LFSR >> 1) & 0X01);
			LFSR >>= 1;
			//LFSR &= 0x7fff;
			LFSR &= 0xbfff;
			LFSR |= (xorResult << 14);
		}
		
		output = LFSR & 0x01;

	}
	void tick(int cycles) override {
		timer -= cycles;
		sampleGenerateCounter -= cycles;
		squencerTimer -= cycles;
		if (squencerTimer <= 0) {
			squencerTimer += squencerTimerReload;
			//lenCounter();
			shiftLFSR();

		}
		if (timer <= 0) {
			vol = envelopeVolume;
			timer += timerReload;
			
			//return;
			switch (frameStep) {
			case 0:
				lenCounter();
				break;
			case 1:
				break;
			case 2:
				sweep();
				lenCounter();
				break;
			case 3:
				break;
			case 4:
				lenCounter();
				break;
			case 5:
				break;
			case 6:
				sweep();
				lenCounter();
				break;
			case 7:
				envelope();
				
				break;
			}
			frameStep = frameStep < 7 ? frameStep + 1 : 0;

			

		}

		
		getSample();

	}
	void getSample() {
		if (sampleGenerateCounter <= 0) {
			sampleGenerateCounter += sampleGenerateCounterReload;

			/*auto approxsin = [](float t) {
				double j = t * 0.15915;
				j = j - (int)j;
				return 20.785 * j * (j - 0.5) * (j - 1.0f);
			};*/

			Uint8 rightSample;
			Uint8 leftSample;
			float phase = 2.0f * M_PI * duty;

			float f = (float)freq;

			double g = 0;
			double c = 0, y1 = 0, y2 = 0;
			double t = adc->audioPosition * adc->sampleDurationSec;
			time += freq * M_PI / (float)adc->have.freq;
			time = 0;
			double x = t * 2 * M_PI * f;

			//g += sin(x * k - (double)phase * k) / k;
		//for (int k = 1;k < 20;k += 2) {	
		//		g += approxsin(x * k - (double)phase * k) / k;
		//	//	g += -approxsin(x * k) / k;
		//	}
		/*for (int n = 1;n <= 20;n++) {
				c = n * x;
				y1 += -approxsin(c) / n;
				y2 += -approxsin(c - phase * (double)n) / n;
			}
			g = (y1 - y2)* (2.0/ M_PI);*/
			//  vol= ch->envelopeVolume;
			  /*if (!ch->enable)
				  vol = 0;	*/

				  //sample = ((g*apu->adc.audioVolume * vol))/100;
			if (false) {
				/*if (ch->enable && (ch->sequencer->waveForm != lastwaveform || false)) {
					printf("g:%f out:%d sequence:%d step:%d wave:%s\n", g, ch->sequencer->output, ch->sequencer->sequence, ch->sequencer->step, ch->sequencer->waveForm);
					lastwaveform = ch->sequencer->waveForm;
				}*/

				float p = adc->audioVolume * vol;
				p = sequencer.output * duty * adc->audioVolume * vol;
				p = g * adc->audioVolume * vol * sequencer.output;
				p = ((float)p) / (float)32768;
				if (p > 1) p = 1;
				if (p < -1) p = -1;
				/*if (p < 0.5)
					p = -p;*/
					//p *= apu->adc.audioVolume * vol;

				rightSample = p;
			}



			/*g = sin(time);
			time += (2 * M_PI * ch->freq) / (float)apu->adc.have.samples;
			sample = (g * ch->sequencer.output * apu->adc.audioVolume * vol);*/
			//sample = ((g * apu->adc.audioVolume * vol));

			//time =SDL_GetTicks()* ch->freq * M_PI;
			//time =(apu->bus->cpu->steps/419433)*2.0f*M_PI;
			float h = sin(x);
			Sint8 temp = sequencer.output;
			//float h2 = 0xff * (1-output);
			float h2 =(output);
			h2 *= 0xff;
			h = h2;


				//apu->adc.audioVolume


			//sample = ((g * ((double)vol / 15) * v));
			//sample = ((g * apu->adc.audioVolume * vol));

				//sample = 0.5;

			//ch->samplesData[sampleCounter] = sample;
			//ch->samplesData[2*sampleCounter+1] = 1;
			if (!enable) {
				vol = 0;
			}

			float v = (((float)(soundCtrl->S01Volume + soundCtrl->S02Volume) / 7)) / 10;

			leftSample = ((h * ((double)vol / 15) * v));
			//float v = (((float)(apu->soundCtrl.S01Volume + apu->soundCtrl.S02Volume) / 7)) / 10;

			//rightSample = ((h * ((double)vol / 15) * (double)apu->soundCtrl.S01Volume / 70));
			//leftSample = ((h * ((double)vol / 15) * (double)apu->soundCtrl.S02Volume / 70));




			//ch->samplesData[2*sampleCounter] = rightSample;
			//ch->samplesData[2*sampleCounter+1] = leftSample;
			//ch->rightSamplesData[sampleCounter] = leftSample;
			//ch->rightSamplesData[sampleCounter] = 0;
			leftSamplesData[adc->sampleCounter] = leftSample;
			//ch->samplesData[2*sampleCounter] = 128;
			volSaved = true;
			if (rightSamplesData[adc->sampleCounter] != 0)
				int test = 0;
			//sampleCounter++;
			adc->hasSample = true;
			//TODO
			/*if (sampleCounter == apu->adc.have.samples) {
				hasSamples = true;
				sampleCounter = 0;
				//SDL_QueueAudio(apu->adc.dev, ch->samplesData, apu->adc.have.channels * apu->adc.have.samples);
				//SDL_QueueAudio(apu->adc.dev, ch->samplesData,  apu->adc.have.samples);
				SDL_QueueAudio(apu->adc.dev, ch->leftSamplesData, apu->adc.have.samples);
				//SDL_QueueAudio(apu->adc.dev, ch->leftSamplesData, apu->adc.have.samples);
				//SDL_QueueAudio(apu->adc.dev, ch->samplesData, apu->adc.have.channels * apu->adc.have.samples);
			}*/

		}

	}
	void lenCounter() {
		if (counterEnable) {
			if (soundLen <= 0) {
				enable = false;
				soundCtrl->setSoundState(channelIndex, false);
			}
			soundLen--;
		}
	}
	void envelope() {
		if (envelopeEnable) {
			if (volumeEnvelopeLen == 0) {
				volumeEnvelopeLen = loadedVolumeEnvelopeLen;
				uint8_t newVolume = envelopeVolume + envelopeDirection;
				if (newVolume >= 0x0f || newVolume < 0) {
					envelopeEnable = false;
				}
				else {
					if (!volSaved) {
						int test = 0;
					}
					envelopeVolume = newVolume;
					volSaved = false;

					/*if (sequencer.output != 0)
						vol = ch->envelopeVolume;
					else vol = 0;*/
				}

			}
			else {
				//vol = 0;
			}
			//}
			volumeEnvelopeLen--;
		}
	}
};


