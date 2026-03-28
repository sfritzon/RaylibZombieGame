#pragma once

#include "raylib.h"


class AudioManager
{
public:
    static AudioManager& instance();

    void init();
    void shutdown();

    void playMenuMusic();
    void playGameMusic();
    void playGameOverMusic();
    void playSoundPistol();
    void playSoundSniper();
    void playPickup();
    void playDamageSound();
    void stopMusic();

    void setDucked(bool ducked);
    void update();

private:
    AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    void applyVolume();

    Music menuMusic {};
    Music gameMusic {};
    Music gameOverMusic {};
    Music* current {nullptr};
    Sound soundPistol{};
    Sound soundSniper{};
    Sound pickUp{};
    Sound damageSound{};

    bool initialized {false};
    bool ducked {false};

    float fullVolume {0.7f};
    float duckVolume {0.2f};
};