#include "mixer.hpp"

Mixer::Mixer() {
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        // TODO: error handling, could not initialize mixer
    }
}

Mixer::~Mixer() {
    for(std::pair<std::string, Mix_Music*> music: m_music) {
        Mix_FreeMusic(music.second);
    }

    for(std::pair<std::string, Mix_Chunk*> chunk: m_chunks) {
        Mix_FreeChunk(chunk.second);
    }

    Mix_CloseAudio();
}

void Mixer::loadAudio(const std::string &filePath, AudioType audioType) {
    auto music_idx = m_music.find(filePath);
    if(music_idx != m_music.end()) {
        // This has already been loaded
        return;
    }

    auto chunk_idx = m_chunks.find(filePath);
    if(chunk_idx != m_chunks.end()) {
        // This has already been loaded
        return;
    }

    switch(audioType) {
    case AudioType::Chunk:
        {
            Mix_Chunk *chunk = Mix_LoadWAV(filePath.c_str());
            if(chunk == NULL) {
                // TODO: error handling, chunk wasn't loaded
            }
            m_chunks[filePath] = chunk;
        }
        break;
    case AudioType::Music:
        {
            Mix_Music *music = Mix_LoadMUS(filePath.c_str());
            if(music == NULL) {
                // TODO: error handling, music wasn't loaded
            }
            m_music[filePath] = music;
        }
        break;
    }
}

/**
 * Plays audio that has already been loaded using loadAudio.
 * 
 * `filePath` is the filepath of the audio.
 * `loops` is the number of times to repeat the audio. -1 loops infinitely,
 *  0 plays the audio one time, 1 plays twice, etc.
 *  `volume` is the volume level to play the sound at. it should only be from 0 to 1. 1 is default.
 */
void Mixer::playAudio(const std::string &filePath, int loops, float volume) {
    auto music_idx = m_music.find(filePath);
    if(music_idx == m_music.end()) {
        auto chunk_idx = m_chunks.find(filePath);
        if(chunk_idx == m_chunks.end()) {
            // TODO: error handling, audio does not exist
        } else {
            // Attempt to play the chunk.
            Mix_Chunk *chunk = chunk_idx->second;
            Mix_VolumeChunk(chunk, MIX_MAX_VOLUME * volume);
            if(Mix_PlayChannel(-1, chunk, loops) == -1) {
                // TODO: error handling, couldn't play chunk
            }
        }
    } else {
        // Attempt to play the music.
        Mix_Music *music = music_idx->second;
        Mix_VolumeMusic(MIX_MAX_VOLUME * volume);
        if(Mix_PlayMusic(music, loops) == -1) {
            // TODO: error handling, couldn't play music
        }
    }
}

void Mixer::pauseAllAudio() {
    Mix_PauseMusic();
}

void Mixer::resumeAllAudio() {
    Mix_ResumeMusic();
}

/**
 * `ms` is the number of milliseconds before the music should finish
 */
void Mixer::fadeOutAllMusic(int ms) {
    Mix_FadeOutMusic(ms);
}

/**
 *  `ms` is the number of milliseconds before reaching full volume
 *  `loops` is the number of times to repeat the audio. -1 loops infinitely,
 *  0 plays the audio one time, 1 plays twice, etc.
 */
void Mixer::fadeInMusic(const std::string& filePath, int ms, int loops) {
    auto music_idx = m_music.find(filePath);
    if(music_idx == m_music.end()) return; // Music doesn't exist
    Mix_Music *music = music_idx->second;
    Mix_FadeInMusic(music, loops, ms);
}