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

    Music m_menuMusic {};
    Music m_gameMusic {};
    Music m_gameOverMusic {};
    Music* m_current {nullptr};

    bool m_initialized {false};
    bool m_ducked {false};

    float m_fullVolume {0.7f};
    float m_duckVolume {0.2f};
};