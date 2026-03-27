#pragma once

#include <raylib.h>

static const int MAX_HEALTH_PACKS = 20;
static const float HEALTH_PACK_RADIUS = 12.0f;
static const float HEALTH_PACK_LIFETIME = 8.0f;


struct HealthPack
{
    Vector2 position {0,0};
    float healAmount {0};
    float lifetime {0};
    bool active {false};
};


class HealthPackManager
{
public:
    static HealthPackManager& instance();

    void reset();
    void trySpawn(Vector2 pos, int activeTankCount);
    void update(float delaTime, Vector2 playerPos, float playerRadius, float &playerHealth, float playerMaxHealth);
    void draw() const;

private:
    HealthPackManager() = default;
    HealthPackManager(const HealthPackManager&) = delete;
    HealthPackManager& operator=(const HealthPackManager&) = delete;

    HealthPack packs[MAX_HEALTH_PACKS];
};