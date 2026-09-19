#pragma once
#include <SDL3/SDL.h>

// Looping background music. SDL mixes on its own audio thread, so playback
// runs in parallel with the game loop without any threads of our own.
class Music
{
public:
    Music() = default;
    ~Music();
    Music(const Music&) = delete;
    Music& operator=(const Music&) = delete;

    bool Load(const char* path);
    void Start();
    void Stop();
    void Shutdown();

private:
    static void SDLCALL OnStreamNeedsData(void* userdata, SDL_AudioStream* stream, int additionalAmount, int totalAmount);

    SDL_AudioStream* stream = nullptr;
    Uint8* buffer = nullptr;
    Uint32 length = 0;
};
