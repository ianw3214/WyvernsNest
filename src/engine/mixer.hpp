#pragma once

#include <unordered_map>
#include <string>

#include <SDL.h>
#include <SDL_mixer.h>

/**
 *  Chunk is used for short sounds (less than 10 seconds)
 *  Musis is used for long sounds (more than 10 seconds)
 */
enum class AudioType { Chunk, Music };

class Mixer {
public:
    Mixer();
    ~Mixer();

    void loadAudio(const std::string &filePath, AudioType audioType);
    void playAudio(const std::string &filePath, int loops, float volume = 1);

    void pauseAllAudio();
    void resumeAllAudio();

    void fadeOutAllMusic(int ms);
    void fadeInMusic(const std::string &filePath, int ms, int loops = 0);
private:
    std::unordered_map<std::string, Mix_Music*> m_music;
    std::unordered_map<std::string, Mix_Chunk*> m_chunks;
};