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

    bool initialized {false};
    bool ducked {false};

    float fullVolume {0.7f};
    float duckVolume {0.2f};
};