#include "BulletPool.h"
#include "raylib.h"


BulletPool& BulletPool::instance()
{
    static BulletPool pool;
    return pool;
}


void BulletPool::reset() 
{
    for (int i = 0; i < MAX_BULLETS; ++i) bullets[i] = Bullet{};
}


void BulletPool::spawn(Vector2 pos, Vector2 dir, float speed, float radius, float damage, int weaponType) 
{
    for (int i = 0; i < MAX_BULLETS; ++i) 
    {
        if (!bullets[i].active) 
        {
            bullets[i].position = pos;
            bullets[i].velocity = { dir.x * speed, dir.y * speed };
            bullets[i].radius = radius;
            bullets[i].damage = damage;
            bullets[i].lifetime = BULLET_LIFETIME;
            bullets[i].active = true;
            bullets[i].weaponType = weaponType;
            return;
        }
    }
}


void BulletPool::update(float dt) 
{
    for (int i = 0; i < MAX_BULLETS; ++i) 
    {
        if (!bullets[i].active) continue;

        bullets[i].position.x += bullets[i].velocity.x * dt;
        bullets[i].position.y += bullets[i].velocity.y * dt;
        bullets[i].lifetime -= dt;

        if (bullets[i].lifetime <= 0.0f || bullets[i].position.x < -50 || 
            bullets[i].position.x > SCREEN_WIDTH  + 50 || bullets[i].position.y < -50 || 
            bullets[i].position.y > SCREEN_HEIGHT + 50)
        {
            bullets[i].active = false;
        }
    }
}

void BulletPool::draw() const 
{
    for (int i = 0; i < MAX_BULLETS; ++i) 
    {
        if (!bullets[i].active) continue;
        Color c = (bullets[i].weaponType == 0)
                ? Color{ 200, 0, 200, 255 }   // Pistol is purple
                : Color{ 255, 100, 20, 255 };  // Sniper is orange

        DrawCircleV(bullets[i].position, bullets[i].radius, c);
        DrawCircleV(bullets[i].position, bullets[i].radius + 3.0f, { c.r, c.g, c.b, 50 });
    }
}