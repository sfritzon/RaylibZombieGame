#pragma once

#include "raylib.h"
#include "Constants.h"


struct Bullet 
{
    Vector2 position {0, 0};
    Vector2 velocity {0, 0};
    float radius {5.0f};
    float damage {0.0f};
    float lifetime {0.0f};
    bool active {false};
    int weaponType {0};
};


class BulletPool 
{
public:
    static BulletPool& instance();

    void reset();
    void spawn(Vector2 pos, Vector2 dir, float speed, float radius, float damage, int weaponType);
    void update(float dt);
    void draw() const;

    Bullet* getBullets() { return m_bullets; }
    int getCount() const { return MAX_BULLETS; }

private:
    BulletPool() = default;
    BulletPool(const BulletPool&) = delete;
    BulletPool& operator=(const BulletPool&) = delete;

    Bullet m_bullets[MAX_BULLETS];
};
