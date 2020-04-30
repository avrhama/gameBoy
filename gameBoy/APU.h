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

	
	


	

	

	void createAudioDeviceControl(bool closeOld);

	
public:
	steady_clock::time_point startTimer;
	steady_clock::time_point lastTick;
	bool mute = false;
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
	Channel1* chann1 =new Channel1();
	SquareWave* chann2 = new SquareWave();
	Wave* chann3 = new Wave();
	Noise* chann4=new Noise();
	Channel* channels[4] = { chann1,chann2,chann3,chann4};
	
	void tick();
	void setSoundOutputTerminal(uint8_t value);
	void feedChannelCtrlRegister(uint8_t value);
	void close(bool closeSDL = false);
	void play();
	void createChannels();
	uint8_t read(uint8_t address);
	void write(uint8_t address,uint8_t value);
	~APU() {
		delete chann1;
		delete chann2;
		delete chann3;
		delete chann4;
		free(adc.leftSamplesData);
		free(adc.rightSamplesData);
		free(adc.samplesData);
	}
};

