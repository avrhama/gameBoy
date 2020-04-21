#include <math.h>
#include <SDL.h>
#include <SDL_audio.h>
#include <iostream>

namespace AudioGen
{
    const int AMPLITUDE = 1;
    const int SAMPLE_RATE = 44000;

    // Globals
    float* in_buffer;
    SDL_atomic_t callback_sample_pos;
    SDL_Event event;
    SDL_bool running = SDL_TRUE;

    /**
     * Structure for holding audio metadata such as frequency
     */
    struct AudioData
    {
        int sampleNum;
        float frequency;
    };

    void audio_callback(void* user_data, Uint8* raw_buffer, int bytes)
    {
        float* buffer = (float*)raw_buffer;
        AudioData& audio_data(*static_cast<AudioData*>(user_data));
        int nSamples = bytes / 4; // For F32
        std::cout << nSamples << std::endl;
        for (int i = 0; i < nSamples; i++, audio_data.sampleNum++)
        {
            double time = (double)audio_data.sampleNum / (double)SAMPLE_RATE;
            buffer[i] = (float)(AMPLITUDE * sin(2.0f * M_PI * audio_data.frequency * time));
        }
    }

    int buffer_length;
    void callback(void* user_data, Uint8* raw_buffer, int bytes)
    {
        float* buffer = (float*)raw_buffer;
        int nSamples = bytes / 4;
        auto local_sample_pos = SDL_AtomicGet(&callback_sample_pos);
        for (int i = 0; i < nSamples; ++i)
        {
            // Stop running audio if all samples are finished playing
            if (buffer_length == local_sample_pos)
            {
                running = SDL_FALSE;
                break;
            }
            buffer[i] = in_buffer[local_sample_pos];
            ++local_sample_pos;
        }
        SDL_AtomicSet(&callback_sample_pos, local_sample_pos);
    }

    class Speaker
    {
    public:
        Speaker()
        {
            SDL_Init(SDL_INIT_AUDIO);
            SDL_AudioSpec ds;
            ds.freq = SAMPLE_RATE;
            ds.format = AUDIO_F32;
            ds.channels = 1;
            ds.samples = 4096;
            ds.callback = callback;
            ds.userdata = &ad; // metadata for frequency

            SDL_AudioSpec os;
            dev = SDL_OpenAudioDevice(NULL, 0, &ds, &os, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
        }

        ~Speaker()
        {
            SDL_CloseAudioDevice(dev);
            SDL_Quit();
        }
        void pushBeep(float frequency, int duration)
        {
            ad.frequency = frequency; // set the frequency for the beep
            SDL_PauseAudioDevice(dev, 0);
            SDL_Delay(duration); // wait while sound is playing
            SDL_PauseAudioDevice(dev, 1);
        }

        void pushBeep2(float frequency, int duration)
        {
            int nSamples = duration * SAMPLE_RATE / 1000;
            in_buffer = new float[nSamples];
            buffer_length = nSamples;
            for (int idx = 0; idx < nSamples; idx++) {
                double time = (double)idx / (double)SAMPLE_RATE;
                in_buffer[idx] = (float)(AMPLITUDE * std::sin(2.0f * M_PI * frequency * time));
            }
            SDL_QueueAudio(dev, in_buffer, nSamples * sizeof(float));
            SDL_PauseAudioDevice(dev, 0);
            while (running) {
                while (SDL_PollEvent(&event) != 0);
            }
            delete[] in_buffer;
        }

    private:
        SDL_AudioDeviceID dev;
        AudioData ad;
        int sampleNum = 0;
    };
} // End of namespace AudioGen

//int main(int argc, char* argv[])
//{
//    AudioGen::Speaker speaker;
//    //speaker.pushBeep(440, 1000);
//    speaker.pushBeep2(440.0f, 1000);
//    return 0;
//}