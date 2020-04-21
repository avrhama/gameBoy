#pragma once
#include "BUS.h"
#include "MMU.h"
#include "CPU.h"
#include "APU.h"
#include "SDL.h"
#include "SoundDevice.h"
#include <math.h>
#include <stdio.h>

class APU
{
private:
	
	BUS* bus;
	int apuCounter = 32;
	struct Sequencer {
		bool enable=true;
		uint32_t sequence=0;
		double counter=0;
		double len=0;
		uint8_t output=0;
		uint8_t tick() {
			if (enable) {
				counter--;
				if (counter == 0xffff) {
					counter = len+1;
					sequence = ((sequence & 0x0001) << 7) | ((sequence & 0x00FE) >> 1);
					output = sequence & 0x01;
				}
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
	  
	float sweepTimes[8] = {0,7.8125,15.625,23.4375,31.25,39.0625,46.875,54.6875};
	//float duites[8] = { 12.5,25,50,75 };
   float duites[4] = { 0.125,0.25,0.5,0.75 };
   uint8_t waveForms[4] = { 0b00000001,0b10000001,0b10000111,0b01111110 };
	struct Channel {
		uint8_t channelIndex;
		bool enable;
		double sweepTime;
		double frequencySweepLen;
		//Lower 8 bits of 11 bit frequency in ($FF14). Next 3 bit are in NR14 ($FF14)
		uint16_t freq;
		uint16_t freqData;
		uint16_t loadedFreq;
		int8_t sweepInc=1;//sweep Increase(=1)/Decrease(=-1) 
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
		uint8_t H=88;//harmonies count.
		uint16_t sectorPosition = 0;//divid 262144 samples to secors of 128 size each.
		SDL_AudioSpec want, have;
		SDL_AudioDeviceID dev;
		bool paused = true;
	};
	
	
public:
	uint8_t soundState=0x0;
	APU();
	void connectToBus(BUS* bus);
	AudioDeviceControl adc;
	Channel channels[4];
	void produceSound();
	void setSoundOutputTerminal(uint8_t value);
	void setSoundState(uint8_t channelIndex,bool set);
	void feedChannelCtrlRegister(uint8_t value);
	void feedTriggerRegister(uint8_t channelIndex,uint8_t value);
	void feedSweepRegister(uint8_t channelIndex, uint8_t value);
	void feedLenAndDutyRegister(uint8_t channelIndex, uint8_t value);
	void feedVolumeEnvelopeRegister(uint8_t channelIndex, uint8_t value);
	void feedFrequencyLoRegister(uint8_t channelIndex, uint8_t value);
	void feedFrequencyHiCtlRegister(uint8_t channelIndex, uint8_t value);
	float getChannelSample(uint8_t channelIndex);
	void close();
};

