#include "Weapon.h"
#include "BulletPool.h"
#include "Constants.h"
#include "AudioManager.h"
#include <cmath>


Weapon::Weapon(WeaponType type) : m_type(type) 
{
    switch (type) 
    {
        case WeaponType::PISTOL:
            fireRate = PISTOL_FIRE_RATE;
            damage = PISTOL_DAMAGE;
            bulletSpeed = PISTOL_BULLET_SPEED;
            bulletRadius = PISTOL_BULLET_RADIUS;
            break;
            
        case WeaponType::SNIPER:
            fireRate = SNIPER_FIRE_RATE;
            damage = SNIPER_DAMAGE;
            bulletSpeed = SNIPER_BULLET_SPEED;
            bulletRadius = SNIPER_BULLET_RADIUS;
            break;
    }
}


void Weapon::fire(Vector2 origin, Vector2 targetDir) 
{
    if (cooldown > 0.0f) return;

    float len = sqrtf(targetDir.x * targetDir.x + targetDir.y * targetDir.y);
    if (len < 0.001f) return;
    Vector2 dir = { targetDir.x / len, targetDir.y / len };

    int typeInt = (m_type == WeaponType::PISTOL) ? 0 : 1;

    BulletPool::instance().spawn(origin, dir, bulletSpeed, bulletRadius, damage, typeInt);

    if (m_type == WeaponType::PISTOL)
        AudioManager::instance().playSoundPistol();
    else
        AudioManager::instance().playSoundSniper();

    cooldown = fireRate;
}


void Weapon::update(float dt) 
{
    if (cooldown > 0.0f) cooldown -= dt;
    if (cooldown < 0.0f) cooldown = 0.0f;
}


const char* Weapon::getName() const 
{
    return (m_type == WeaponType::PISTOL) ? "Pistol" : "Sniper";
}
