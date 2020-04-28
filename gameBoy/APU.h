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
#include "APUstructures.h"
using namespace std::chrono;
class APU
{
private:


	int apuReloadCounter = 4194304;
	int apuCounter = 4194304;//32;
	
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
	float duties[4] = { 0.125,0.25,0.5,0.75 };
	
	


	

	

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
	
	SoundControl soundCtrl;
	APU();
	void start();
	void connectToBus(BUS* bus);
	AudioDeviceControl adc;

	Channel* channels[4] = {new Channel1(),new SquareWave(),new Channel1(),new Noise() };
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
	void updateChannelFreqData(uint8_t channelIndex, uint16_t newFreqData);
	void close();
	void play();
	void createChannels();
	uint8_t read(uint8_t address);
	void write(uint8_t address,uint8_t value);

};

