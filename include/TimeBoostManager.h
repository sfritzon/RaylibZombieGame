#pragma once

#include <raylib.h>


static const int MAX_TIME_BOOSTS = 10;
static const float TIME_BOOST_RADIUS = 12.0f;
static const float TIME_BOOST_LIFETIME = 10.0f;
static const int TIME_BOOST_MIN_WAVE = 5;
static const int TIME_BOOST_DROP_CHANCE = 12;


struct TimeBoost
{
    Vector2 position {0, 0};
    float addSeconds {0};
    float lifetime {0};
    bool active {false};
};


class TimeBoostManager
{
public:
    static TimeBoostManager& instance();
    
    void reset();
    void trySpawn(Vector2 pos, int currentWave);
    void update(float deltaTime, Vector2 playerPos, float playerRadius, float& waveTimer);
    void draw() const;

private:
    TimeBoostManager() = default;
    TimeBoostManager(const TimeBoostManager&) = delete;
    TimeBoostManager& operator=(const TimeBoostManager&) = delete;

    TimeBoost boosts[MAX_TIME_BOOSTS];
    float animTimer {0};
};