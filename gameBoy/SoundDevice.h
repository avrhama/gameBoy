

#include "SDL.h"
#include <math.h>
#include <stdio.h>
//class APU;
//#pragma once
//struct AudioDeviceControl {
//	int FREQ = 200; /* the frequency we want */
//	unsigned int audioPosition; /* which sample we are up to */
//	int audioLen; /* how many samples left to play, stops when <= 0 */
//	float audioFrequency; /* audio frequency in cycles per sample */
//	float audioVolume; /* audio volume, 0 - ~32000 */
//	uint16_t sectorPosition = 0;//divid 262144 samples to secors of 128 size each.
//	SDL_AudioSpec want, have;
//	SDL_AudioDeviceID dev;
//};
//void MyAudioCallback(void* user_data, Uint8* stream, int len) {
//	len /= 2; /* 16 bit */
//	int i;
//
//	/*APU * apu = *(APU*)user_data;
//	Sint16* buf = (Sint16*)stream;
//	float duty = apu.channels[0].duty;
//	float p = duty * 2.0f * M_PI;
//	float a = 0;
//	float b = 0;
//	int H = 88;
//	float y1, y2, y3;
//	float P = apu.channels[0].duty;
//	for (i = 0; i < len; i++) {//each iteration is 1/samples of second
//		y1 = 0;
//		y2 = 0;
//
//		for (int n = 1;n <= H;n++) {
//			y1 += sin(apu.channels[0].envelopeVolume * apu.adc.audioPosition * apu.adc.audioVolume * apu.channels[0].freq * 2 * M_PI * n) / n;
//			y2 += sin((apu.channels[0].envelopeVolume * apu.adc.audioPosition * apu.adc.audioVolume * apu.channels[0].freq - apu.channels[0].duty) * 2 * M_PI * n) / n;
//		}
//		y3 = y1 - y2;
//		float c= (2 * M_PI * audio_position * audio_frequency);
//		a = audio_volume * sin(c);
//		b = audio_volume * sin(p - c);
//
//		buf[i] = apu.adc.audioVolume * y3;
//		//buf[i] = audio_volume * sin(2 * M_PI * audio_position * audio_frequency);
//		apu.adc.audioPosition = (apu.adc.audioPosition + 1) % apu.adc.have.samples;
//		//if (channels[0].counterEnable) {
//		//-0.5
//		//}
//	}
//	apu.adc.audioLen -= len;
//	*/
//}