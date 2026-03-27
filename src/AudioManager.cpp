#include "AudioManager.h"
#include "raylib.h"


AudioManager& AudioManager::instance()
{
    static AudioManager audioManager;
    return audioManager;
}


void AudioManager::init()
{
    if (initialized) return;
    InitAudioDevice();

    if (FileExists("resources/Audio/menu.mp3"))
    menuMusic = LoadMusicStream("resources/Audio/menu.mp3");

    if (FileExists("resources/Audio/game.mp3"))
    gameMusic = LoadMusicStream("resources/Audio/game.mp3");

    if (FileExists("resources/Audio/gameOver.mp3"))
    gameOverMusic = LoadMusicStream("resources/Audio/gameOver.mp3");

    menuMusic.looping = true;
    gameMusic.looping = true;
    gameOverMusic.looping = true;

    initialized = true;
}


void AudioManager::shutdown()
{
    stopMusic();

    if (initialized)
    {
        if (menuMusic.stream.buffer != nullptr)
            UnloadMusicStream(menuMusic);

        if (gameMusic.stream.buffer != nullptr)
            UnloadMusicStream(gameMusic);

        if (gameOverMusic.stream.buffer != nullptr)
            UnloadMusicStream(gameOverMusic);

        CloseAudioDevice();
        initialized = false;
    }
}


void AudioManager::playMenuMusic() 
{
    if (!initialized) return;
    if (current == &menuMusic && IsMusicStreamPlaying(menuMusic)) return;

    if (current && IsMusicStreamPlaying(*current))
        StopMusicStream(*current);

    current = &menuMusic;
    SetMusicVolume(menuMusic, ducked ? duckVolume : fullVolume);
    PlayMusicStream(menuMusic);
}


void AudioManager::playGameMusic() 
{
    if (!initialized) return;
    if (current == &gameMusic && IsMusicStreamPlaying(gameMusic)) return;

    if (current && IsMusicStreamPlaying(*current))
        StopMusicStream(*current);

    current = &gameMusic;
    SetMusicVolume(gameMusic, ducked ? duckVolume : fullVolume);
    PlayMusicStream(gameMusic);
}


void AudioManager::playGameOverMusic()
{
    if (!initialized) return;
    if (current == &gameOverMusic && IsMusicStreamPlaying(gameOverMusic)) return;

    if (current && IsMusicStreamPlaying(*current))
    StopMusicStream(*current);

    current = &gameOverMusic;
    SetMusicVolume(gameOverMusic, ducked ? duckVolume : fullVolume);
    PlayMusicStream(gameOverMusic);
}


void AudioManager::stopMusic() 
{
    if (!initialized) return;
    if (current) {
        StopMusicStream(*current);
        current = nullptr;
    }
}


void AudioManager::setDucked(bool ducked) 
{
    if (ducked == ducked) return;
    ducked = ducked;
    applyVolume();
}


void AudioManager::applyVolume() 
{
    if (!initialized || !current) return;
    SetMusicVolume(*current, ducked ? duckVolume : fullVolume);
}


void AudioManager::update() 
{
    if (!initialized || !current) return;
    UpdateMusicStream(*current);
}

