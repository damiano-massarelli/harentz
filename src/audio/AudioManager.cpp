#include "AudioManager.h"
#include <SDL.h>
#include <string>

void onChannelFinish(int channel) {
    AudioManager::getInstance()->detachChannel(channel);
}

std::unique_ptr<AudioManager> AudioManager::instance;

AudioManager::AudioManager()
{
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        SDL_Log("cannot open mixer");

    Mix_ChannelFinished(onChannelFinish);
}

AudioManager* AudioManager::getInstance()
{
    if (!instance)
        instance = std::make_unique<AudioManager>();

    return instance.get();
}

void AudioManager::loadSound(const std::string& fileName)
{
    Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());
    if (chunk == nullptr) {
        SDL_Log("Cannot read audio %s", fileName.c_str());
        return;
    }
    m_cachedSounds[fileName] = chunkUniquePtr{chunk};
}

int AudioManager::playSound(const std::string& fileName)
{
    // look in cache
    auto it = m_cachedSounds.find(fileName);
    if (it != m_cachedSounds.end())
        return Mix_PlayChannel(-1, it->second.get(), 0);

    // not in cache
    Mix_Chunk* chunkToPlay = Mix_LoadWAV(fileName.c_str());
    if (chunkToPlay == nullptr) {
        SDL_Log("Cannot read audio %s", fileName.c_str());
        return -1;
    }
    int channel = Mix_PlayChannel(-1, chunkToPlay, 0);
    attachChannel(channel, chunkToPlay);
    return channel;
}

void AudioManager::attachChannel(int channel, Mix_Chunk* chunk)
{
    m_channel2sound[channel] = chunkUniquePtr{chunk};
}

void AudioManager::detachChannel(int channel)
{
    m_channel2sound.erase(channel);
}

void AudioManager::playMusic(const std::string& fileName, int loops)
{
    Mix_HaltMusic();
    Mix_Music* music = Mix_LoadMUS(fileName.c_str());
    if (music == nullptr) {
        SDL_Log("Cannot read music %s", fileName.c_str());
        return;
    }
    m_currentMusic = musicUniquePtr{music};
    Mix_PlayMusic(music, loops);
}

AudioManager::~AudioManager()
{
    Mix_HaltMusic();
    Mix_HaltChannel(-1); // stops all
    m_currentMusic = nullptr; // calling music free after mix quit causes an error
    Mix_Quit();
}
