#include "AudioManager.h"
#include "raylib.h"


AudioManager& AudioManager::instance()
{
    static AudioManager audioManager;
    return audioManager;
}


void AudioManager::init()
{
    if (m_initialized) return;
    InitAudioDevice();

    if (FileExists("resources/Audio/menu.mp3"))
    m_menuMusic = LoadMusicStream("resources/Audio/menu.mp3");

    if (FileExists("resources/Audio/game.mp3"))
    m_gameMusic = LoadMusicStream("resources/Audio/game.mp3");

    if (FileExists("resources/Audio/gameOver.mp3"))
    m_gameOverMusic = LoadMusicStream("resources/Audio/gameOver.mp3");

    m_menuMusic.looping = true;
    m_gameMusic.looping = true;
    m_gameOverMusic.looping = true;

    m_initialized = true;
}


void AudioManager::shutdown()
{
    stopMusic();

    if (m_initialized)
    {
        if (m_menuMusic.stream.buffer != nullptr)
            UnloadMusicStream(m_menuMusic);

        if (m_gameMusic.stream.buffer != nullptr)
            UnloadMusicStream(m_gameMusic);

        if (m_gameOverMusic.stream.buffer != nullptr)
            UnloadMusicStream(m_gameOverMusic);

        CloseAudioDevice();
        m_initialized = false;
    }
}


void AudioManager::playMenuMusic() 
{
    if (!m_initialized) return;
    if (m_current == &m_menuMusic && IsMusicStreamPlaying(m_menuMusic)) return;

    if (m_current && IsMusicStreamPlaying(*m_current))
        StopMusicStream(*m_current);

    m_current = &m_menuMusic;
    SetMusicVolume(m_menuMusic, m_ducked ? m_duckVolume : m_fullVolume);
    PlayMusicStream(m_menuMusic);
}


void AudioManager::playGameMusic() 
{
    if (!m_initialized) return;
    if (m_current == &m_gameMusic && IsMusicStreamPlaying(m_gameMusic)) return;

    if (m_current && IsMusicStreamPlaying(*m_current))
        StopMusicStream(*m_current);

    m_current = &m_gameMusic;
    SetMusicVolume(m_gameMusic, m_ducked ? m_duckVolume : m_fullVolume);
    PlayMusicStream(m_gameMusic);
}


void AudioManager::playGameOverMusic()
{
    if (!m_initialized) return;
    if (m_current == &m_gameOverMusic && IsMusicStreamPlaying(m_gameOverMusic)) return;

    if (m_current && IsMusicStreamPlaying(*m_current))
    StopMusicStream(*m_current);

    m_current = &m_gameOverMusic;
    SetMusicVolume(m_gameOverMusic, m_ducked ? m_duckVolume : m_fullVolume);
    PlayMusicStream(m_gameOverMusic);
}


void AudioManager::stopMusic() 
{
    if (!m_initialized) return;
    if (m_current) {
        StopMusicStream(*m_current);
        m_current = nullptr;
    }
}


void AudioManager::setDucked(bool ducked) 
{
    if (m_ducked == ducked) return;
    m_ducked = ducked;
    applyVolume();
}


void AudioManager::applyVolume() 
{
    if (!m_initialized || !m_current) return;
    SetMusicVolume(*m_current, m_ducked ? m_duckVolume : m_fullVolume);
}


void AudioManager::update() 
{
    if (!m_initialized || !m_current) return;
    UpdateMusicStream(*m_current);
}
