#include "Weapon.h"
#include "BulletPool.h"
#include "Constants.h"
#include <cmath>


Weapon::Weapon(WeaponType type) : m_type(type) 
{
    switch (type) 
    {
        case WeaponType::PISTOL:
            m_fireRate = PISTOL_FIRE_RATE;
            m_damage = PISTOL_DAMAGE;
            m_bulletSpeed = PISTOL_BULLET_SPEED;
            m_bulletRadius = PISTOL_BULLET_RADIUS;
            break;
            
        case WeaponType::SNIPER:
            m_fireRate = SNIPER_FIRE_RATE;
            m_damage = SNIPER_DAMAGE;
            m_bulletSpeed = SNIPER_BULLET_SPEED;
            m_bulletRadius = SNIPER_BULLET_RADIUS;
            break;
    }
}


void Weapon::fire(Vector2 origin, Vector2 targetDir) 
{
    if (m_cooldown > 0.0f) return;

    float len = sqrtf(targetDir.x * targetDir.x + targetDir.y * targetDir.y);
    if (len < 0.001f) return;
    Vector2 dir = { targetDir.x / len, targetDir.y / len };

    int typeInt = (m_type == WeaponType::PISTOL) ? 0 : 1;

BulletPool::instance().spawn(origin, dir, m_bulletSpeed, m_bulletRadius, m_damage, typeInt);

    m_cooldown = m_fireRate;
}


void Weapon::update(float dt) 
{
    if (m_cooldown > 0.0f) m_cooldown -= dt;
    if (m_cooldown < 0.0f) m_cooldown = 0.0f;
}


const char* Weapon::getName() const 
{
    return (m_type == WeaponType::PISTOL) ? "Pistol" : "Sniper";
}
