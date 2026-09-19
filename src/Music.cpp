#include <iostream>
#include "Music.h"

Music::~Music() {
    Shutdown();
}

bool Music::Load(const char* path) {
    SDL_AudioSpec spec;
    if (!SDL_LoadWAV(path, &spec, &buffer, &length)) {
        std::cerr << "Failed to load WAV file: " << SDL_GetError() << std::endl;
        return false;
    }

    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, OnStreamNeedsData, this);
    if (!stream) {
        std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
        SDL_free(buffer);
        buffer = nullptr;
        return false;
    }
    return true;
}

void Music::Start() {
    if (!stream) return;
    SDL_ClearAudioStream(stream);
    SDL_PutAudioStreamData(stream, buffer, static_cast<int>(length));
    SDL_ResumeAudioStreamDevice(stream);
}

void Music::Stop() {
    if (!stream) return;
    SDL_PauseAudioStreamDevice(stream);
    SDL_ClearAudioStream(stream);
}

void Music::Shutdown() {
    if (stream) {
        SDL_DestroyAudioStream(stream);
        stream = nullptr;
    }
    if (buffer) {
        SDL_free(buffer);
        buffer = nullptr;
    }
}

// Runs on the audio thread whenever the stream wants more data: queue the track again to loop.
void SDLCALL Music::OnStreamNeedsData(void* userdata, SDL_AudioStream* audioStream, int additionalAmount, int) {
    if (additionalAmount <= 0) return;
    auto* music = static_cast<Music*>(userdata);
    SDL_PutAudioStreamData(audioStream, music->buffer, static_cast<int>(music->length));
}
