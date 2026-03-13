#include "BulletPool.h"
#include "raylib.h"


BulletPool& BulletPool::instance()
{
    static BulletPool pool;
    return pool;
}


void BulletPool::reset() 
{
    for (int i = 0; i < MAX_BULLETS; ++i) m_bullets[i] = Bullet{};
}


void BulletPool::spawn(Vector2 pos, Vector2 dir, float speed, float radius, float damage, int weaponType) 
{
    for (int i = 0; i < MAX_BULLETS; ++i) 
    {
        if (!m_bullets[i].active) 
        {
            m_bullets[i].position = pos;
            m_bullets[i].velocity = { dir.x * speed, dir.y * speed };
            m_bullets[i].radius = radius;
            m_bullets[i].damage = damage;
            m_bullets[i].lifetime = BULLET_LIFETIME;
            m_bullets[i].active = true;
            m_bullets[i].weaponType = weaponType;
            return;
        }
    }
}


void BulletPool::update(float dt) 
{
    for (int i = 0; i < MAX_BULLETS; ++i) 
    {
        if (!m_bullets[i].active) continue;

        m_bullets[i].position.x += m_bullets[i].velocity.x * dt;
        m_bullets[i].position.y += m_bullets[i].velocity.y * dt;
        m_bullets[i].lifetime -= dt;

        if (m_bullets[i].lifetime <= 0.0f || m_bullets[i].position.x < -50 || 
            m_bullets[i].position.x > SCREEN_WIDTH  + 50 || m_bullets[i].position.y < -50 || 
            m_bullets[i].position.y > SCREEN_HEIGHT + 50)
        {
            m_bullets[i].active = false;
        }
    }
}

void BulletPool::draw() const 
{
    for (int i = 0; i < MAX_BULLETS; ++i) 
    {
        if (!m_bullets[i].active) continue;
        Color c = (m_bullets[i].weaponType == 0)
                  ? Color{ 200, 0, 200, 255 }   // Pistol is purple
                  : Color{ 255, 100, 20, 255 };  // Sniper is orange

        DrawCircleV(m_bullets[i].position, m_bullets[i].radius, c);
        DrawCircleV(m_bullets[i].position, m_bullets[i].radius + 3.0f, { c.r, c.g, c.b, 50 });
    }
}