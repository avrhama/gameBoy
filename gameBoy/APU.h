#pragma once
#include "BUS.h"
#include "MMU.h"
#include "CPU.h"
#include "JOYPAD.h"
#include "SDL.h"
#include "SoundDevice.h"
#include<stdlib.h>
#include <math.h>
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <iostream>
using namespace std::chrono;
class APU
{
private:


	int apuReloadCounter = 4194304;
	int apuCounter = 4194304;//32;
	struct Sequencer {
		bool enable = true;
		uint32_t sequence = 0;
		double counter = 0;
		double counterReload = 0;
		double len = 0;
		uint32_t output = 0;
		uint32_t tick() {
			counter--;
			if (counter <= 0) {
				counter += counterReload;
				//counter = len+1;
				sequence = ((sequence & 0x0001) << 7) | ((sequence & 0x00FE) >> 1);
				output = sequence & 0x01;
			}
			return output;
		}
	};
	/*
	 000: sweep off - no freq change
	 001: 7.8 ms  (1/128Hz)
	 010: 15.6 ms (2/128Hz)
	 011: 23.4 ms (3/128Hz)
	 100: 31.3 ms (4/128Hz)
	 101: 39.1 ms (5/128Hz)
	 110: 46.9 ms (6/128Hz)
	 111: 54.7 ms (7/128Hz)
	*/

	float sweepTimes[8] = { 0,7.8125,15.625,23.4375,31.25,39.0625,46.875,54.6875 };
	//float duites[8] = { 12.5,25,50,75 };
	float duites[4] = { 0.125,0.25,0.5,0.75 };
	uint8_t waveForms[4] = { 0b00000001,0b10000001,0b10000111,0b01111110 };
	struct Channel {
		uint8_t channelIndex;
		bool enable = false;
		double sweepTime;
		double frequencySweepLen;
		//Lower 8 bits of 11 bit frequency in ($FF14). Next 3 bit are in NR14 ($FF14)
		uint16_t freq;
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

		Sequencer sequencer;
		uint32_t samplePosition = 0;
		Sint8* samplesData;
		int sampleRatePos = 0;
	};
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

	};

	SoundControl soundCtrl;

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
	};

	void createAudioDeviceControl(bool closeOld);


public:
	steady_clock::time_point startTimer;
	steady_clock::time_point lastTick;
	bool restart = false;
	double time = 0;
	double tickElapse = 0;
	BUS* bus;
	int cyclesInSoundFrameCounter = 0;
	//int cyclesInSoundFrame = 4194304;
	int cyclesInSoundFrame = 32;
	uint8_t soundState = 0x0;
	APU();
	void start();
	void connectToBus(BUS* bus);
	AudioDeviceControl adc;
	Channel channels[4];
	void tick();
	void setSoundOutputTerminal(uint8_t value);
	void setSoundState(uint8_t channelIndex, bool set);
	void feedChannelCtrlRegister(uint8_t value);
	void feedTriggerRegister(uint8_t channelIndex, uint8_t value);
	void feedSweepRegister(uint8_t channelIndex, uint8_t value);
	void feedLenAndDutyRegister(uint8_t channelIndex, uint8_t value);
	void feedVolumeEnvelopeRegister(uint8_t channelIndex, uint8_t value);
	void feedFrequencyLoRegister(uint8_t channelIndex, uint8_t value);
	void feedFrequencyHiCtlRegister(uint8_t channelIndex, uint8_t value);
	float getChannelSample(uint8_t channelIndex);
	void updateChannelFreqData(uint8_t channelIndex, uint16_t newFreqData);
	void close();
	void play();
	double getSample(double f, double t, double d, int h);

	struct FrameSequencer {
		APU* apu;
		//timerReload freq is 512 hz==4194304/512 clocks cycels.
		int timerReload = 8192;
		int timer = 8192;
		//lenCounterReload freq is 256 hz.
		int lenTimerReload = 16384;//16384
		int lenTimer = 16384;//16384
		//volEnvelopeTimerReload freq is 64 hz.
		int volEnvelopeTimerReload = 65536;//65536
		int volEnvelopeTimer = 65536;//65536
		//sweepTimerReload freq is 128 hz.
		int sweepTimerReload = 32768;//32768
		int sweepTimer = 32768;//32768
		int dutyTimerReload;
		int dutyTimer;
		uint8_t vol = 0;
		uint8_t waveformInput = 0;
		float output = 0;
		double sampleCounterReload = 65536;
		double sampleCounter = 65536;
	public:

		Channel* ch;
		void tick() {
			//sampleCounterReload = 4194304 * (1 / (double)apu->adc.have.samples);
			timer -= apu->bus->cpu->lastOpcodeCycles;
			//return;

			//sweepTimer -= apu->bus->cpu->lastOpcodeCycles;
			
			if (!ch->enable)
				return;
			
			envelope();
			//getSample();
			lenCounter();
			bool first = false;
			
			if (first)
				duty();
			if (timer <= 0) {

				timer += timerReload;
				//output = getSample();


				if (!first)
					duty();
				return;
				if (!ch->enable)
					return;
				//duty(buf);
				 //duty(buf);
				sweep();

				lenCounter();

			}

		}
		float getSample() {
			sampleCounter -= apu->bus->cpu->lastOpcodeCycles;
			if (sampleCounter <= 0) {
				sampleCounter += sampleCounterReload;
				//ch->sequencer.tick();


				Sint16 sample;
				float phase = 2.0f * M_PI * apu->channels[0].duty;
				float d = 1 / (float)apu->adc.have.freq;
				float f = apu->adc.audioFrequency;
				f = 1.0 * (float)apu->adc.FREQ / apu->channels[0].freq;
				f = (float)apu->adc.FREQ / apu->channels[0].freq;
				f = (float)apu->channels[0].freq;
			
					double g = 0;
					double c = 0, y1 = 0, y2 = 0;
					double t =  apu->channels[0].samplePosition *  apu->adc.sampleDurationSec;
					t = (double)apu->adc.audioPosition * 2 * M_PI * f * apu->adc.sampleDurationSec;
					t =  ((double)apu->bus->cpu->steps / 4194304);
					t += ((double)apu->adc.audioPosition) * apu->adc.sampleDurationSec;
					//t = audio_pos;
				/*	for (double k = 1;k < apu->bus->h;k += 2) {

						g += sin(t * k - phase * k) / k;
					}*/
					c = f * 2 * M_PI  * t;
					for (int n = 1;n <= apu->bus->h;n++) {
					
						y1 += -sin(c*n) / n;
						y2 += -sin(c*n - phase * (double)n) / n;
					}
					g = (y1 - y2)* (2.0/ M_PI);
					sample = g * apu->adc.audioVolume * apu->channels[0].envelopeVolume;
					//buf[i] = apu->getChannelSample(0)* apu->adc.audioVolume;
					//audio_pos++;
					apu->adc.audioPosition++;
					apu->channels[0].samplePosition++;
					apu->channels[0].samplePosition = apu->channels[0].samplePosition % apu->adc.have.samples;
					SDL_QueueAudio(apu->adc.dev, &sample, 2);
				return 0;
				/*float y1 = 0, y2 = 0, y3 = 0;
				double t;// = apu->time + (channels[channelIndex].samplePosition) * adc.sampleDurationSec;
				t = apu->time + ((double)apu->bus->cpu->steps / 4194304);
				t += (ch->samplePosition) * apu->adc.sampleDurationSec;

				float phase = 2.0f * M_PI * ch->duty;
				//phase = channels[channelIndex].duty;
				float c2 = 0;

				for (int n = 1;n <= apu->adc.H;n++) {
					c = n * ((float)ch->freq) * 2.0 * M_PI * t;
					y1 += -sin(c) / n;
					y2 += -sin(c - phase * (double)n) / n;
				}
				double amplitude = 1;
				y3 = (y1 - y2);
				y3 *= (2.0 * amplitude / M_PI);
				output = y3;
				return y3;*/
			}
			return 0;
		}
		void sweep() {
			//sweepTimer--;
			if (sweepTimer <= 0 && ch->sweepTime && ch->nSweep) {
				sweepTimer += sweepTimerReload;
				ch->frequencySweepLen--;
				if (ch->frequencySweepLen <= 0) {//change frequency
					ch->frequencySweepLen = ch->sweepTime;
					uint16_t freqData = ch->loadedFreq + ch->sweepInc * (ch->loadedFreq >> ch->nSweep);

					uint16_t newFreq = 131072 / (2048 - freqData);
					if (newFreq > 2047) {
						ch->enable = false;
						apu->setSoundState(ch->channelIndex, false);
					}
					else if (newFreq >= 0) {
						ch->loadedFreq = freqData;
						ch->freq = newFreq;
						//Lower 8 bits of 11 bit frequency (x). Next 3 bit are in NR14 ($FF14)
						apu->updateChannelFreqData(ch->channelIndex, freqData);
						//write freqDatato nn14 and nn13
					}
				}
			}

		}
		void duty() {

			sampleCounter += apu->bus->cpu->lastOpcodeCycles;
			if (sampleCounter >= sampleCounterReload) {
				//incSample();
				int g = 0;
			}
			bool first = true;
			/*if(first)
				ch->sequencer.tick();*/
			dutyTimer--;
			if (dutyTimer <= 0) {
				/*	if(!first)
					ch->sequencer.tick();*/


				dutyTimer = dutyTimerReload;

				return;
				if (!ch->enable)
					return;
				//output=ch->sequencer.tick();



				return;






			}
		}
		void incSample() {
			apu->adc.audioPosition++;
			apu->adc.audioPosition = apu->adc.audioPosition % (apu->adc.have.samples / 2);
			ch->sampleRatePos++;
			ch->sampleRatePos = ch->sampleRatePos % apu->adc.have.freq;
			ch->samplePosition++;
			ch->samplePosition = ch->samplePosition % apu->adc.have.samples;
			sampleCounter = 0;
		}
		void lenCounter() {
			if (ch->counterEnable) {
				lenTimer -= apu->bus->cpu->lastOpcodeCycles;
				if (lenTimer <= 0) {


					//lenTimer += lenTimerReload;
					//stop channel output
					ch->enable = false;
					//printf("stop channel\n");
					apu->setSoundState(ch->channelIndex, false);


					/*//ch->soundLen--;
					if (ch->soundLen <= 0) {
						//stop channel output
						ch->enable = false;
						printf("stop channel\n");
						apu->setSoundState(ch->channelIndex, false);
					}*/
				}
			}
		}
		void envelope() {

			if (ch->envelopeEnable) {
				if (volEnvelopeTimer <= 0) {
					volEnvelopeTimer += volEnvelopeTimerReload;
				}
				if (volEnvelopeTimer == volEnvelopeTimerReload) {

					ch->envelopeVolume += ch->envelopeDirection;
					if (ch->envelopeVolume == 0x0f || ch->envelopeVolume == 0) {
						ch->envelopeEnable = false;
					}
					else {
						if (output != 0)
							vol = ch->envelopeVolume;
						else vol = 0;
					}
				}
				volEnvelopeTimer -= apu->bus->cpu->lastOpcodeCycles;
			}



		}
		void envelope2() {

			if (ch->envelopeEnable) {
				if (volEnvelopeTimer <= 0) {
					volEnvelopeTimer += volEnvelopeTimerReload;
				}
				if (volEnvelopeTimer == volEnvelopeTimerReload) {

					ch->envelopeVolume += ch->envelopeDirection;
					if (ch->envelopeVolume == 0x0f || ch->envelopeVolume == 0) {
						ch->envelopeEnable = false;
					}
					else {
						if (output != 0)
							vol = ch->envelopeVolume;
						else vol = 0;
					}
				}
				volEnvelopeTimer -= apu->bus->cpu->lastOpcodeCycles;
			}



		}
		double out() {
			if (output * vol != 0) {
				int f = 0;
			}
			return output * vol;
		}
	};
	FrameSequencer fs;
};

