#include "HealthPackManager.h"
#include "raylib.h"
#include <cmath>


HealthPackManager& HealthPackManager::instance()
{
    static HealthPackManager hPmanager;;
    return hPmanager;
}


void HealthPackManager::reset()
{
    for (auto& p : packs) p = HealthPack{};
}


void HealthPackManager::trySpawn(Vector2 pos, int activeTankCount)
{
    // Fewer active tanks == higher drop chance
    int roll = GetRandomValue(1, 100);
    int threshold;

    if (activeTankCount >= 4) threshold = 20;
    else if (activeTankCount == 2) threshold = 40;
    else if (activeTankCount == 1) threshold = 60;
    else threshold = 100; //Last tank always drops packs

    if (roll > threshold) return;

    // Find a free slot
    for (auto& p : packs)
    {
        if (!p.active)
        {
            p.position = pos;
            p.healAmount = (float)GetRandomValue(15, 35);
            p.lifetime = HEALTH_PACK_LIFETIME;
            p.active = true;
            return;
        }
    }
}


void HealthPackManager::update(float deltaTime, Vector2 playerPos, float playerRadius, float &playerHealth, float playerMaxHealth)
{
    for (auto& p : packs)
    {
        if (!p.active) continue;

        p.lifetime -= deltaTime;
        if (p.lifetime <= 0.0f)
        {
            p.active = false;
            continue;
        }

        // Check the pickup by player
        float distX = p.position.x - playerPos.x;
        float distY = p.position.y - playerPos.y;
        float dist = sqrtf(distX * distX + distY * distY);

        if (dist < HEALTH_PACK_RADIUS + playerRadius)
        {
            playerHealth += p.healAmount;
            if (playerHealth > playerMaxHealth) playerHealth = playerMaxHealth;
            p.active = false;
        }
    }
}


void HealthPackManager::draw() const
{
    for (const auto& p : packs)
    {
        if (!p.active) continue;

        float alpha = (p.lifetime < 2.0f) ? (p.lifetime / 2.0f) : 1.0f;
        unsigned char a = (unsigned char)(alpha * 255);

        float half = HEALTH_PACK_RADIUS;

        // Yellow health pack
        DrawRectangle(
            (int)(p.position.x - half), (int)(p.position.y - half),
            (int)(half * 2), (int)(half *2),
            { 240, 200, 20, a });

        //White border
        DrawRectangleLines(
            (int)(p.position.x - half), (int)(p.position.y - half),
            (int)(half * 2), (int)(half * 2),
            { 255, 255, 255, a});

        // White + sign
        DrawText("+", (int)(p.position.x - 7), (int)(p.position.y - 10), 24, { 255, 255, 255, a});
    }
}