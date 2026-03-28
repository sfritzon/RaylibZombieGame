#include "TimeBoostManager.h"
#include "AudioManager.h"
#include <raylib.h>
#include <cmath>


TimeBoostManager& TimeBoostManager::instance()
{
    static TimeBoostManager boostManager;
    return boostManager;
}


void TimeBoostManager::reset()
{
    for (auto& b : boosts) b = TimeBoost{};
    animTimer = 0.0f;
}


void TimeBoostManager::trySpawn(Vector2 pos, int currentWave)
{

    if (currentWave <= TIME_BOOST_MIN_WAVE) return;

    if (GetRandomValue(1, 100) > TIME_BOOST_DROP_CHANCE) return;

    for (auto& b : boosts)
    {
        if (!b.active)
        {
            b.position   = pos;
            b.addSeconds = (float)GetRandomValue(10, 20);
            b.lifetime   = TIME_BOOST_LIFETIME;
            b.active     = true;
            return;
        }
    }
}


void TimeBoostManager::update(float deltaTime, Vector2 playerPos, float playerRadius, float& waveTimer)
{
    animTimer += deltaTime;

    for (auto& b : boosts)
    {
        if (!b.active) continue;

        b.lifetime -= deltaTime;
        if (b.lifetime <= 0.0f)
        {
            b.active = false;
            continue;
        }

        float distX = b.position.x - playerPos.x;
        float distY = b.position.y - playerPos.y;
        float dist = sqrtf(distX * distX + distY * distY);

        if (dist < TIME_BOOST_RADIUS + playerRadius)
        {
            waveTimer += b.addSeconds;
            AudioManager::instance().playPickup();
            b.active = false;
        }
    }
}


void TimeBoostManager::draw() const
{
    for (const auto& b : boosts)
    {
        if (!b.active) continue;

        float alpha = (b.lifetime < 2.0f) ? (b.lifetime / 2.0f) : 1.0f;
        unsigned char a = (unsigned char)(alpha * 255);

        float half = TIME_BOOST_RADIUS;
        int px = (int)(b.position.x - half);
        int py = (int)(b.position.y - half);
        int sz = (int)(half * 2);

        DrawRectangle(px, py, sz, sz, { 255, 130, 0, a });

        DrawRectangleLines(px, py, sz, sz, { 255, 255, 255, a });

        DrawText("+t", (int)(b.position.x - 9), (int)(b.position.y - 9), 16, { 255, 255, 255, a });
    }
}