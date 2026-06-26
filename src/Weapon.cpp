#include "Weapon.h"
#include "BulletPool.h"
#include "Constants.h"
#include "AudioManager.h"
#include <cmath>

Weapon::Weapon(WeaponType type) : w_type(type)
{
    switch (type)
    {
    case WeaponType::PISTOL:
        fireRate = PISTOL_FIRE_RATE;
        damage = PISTOL_DAMAGE;
        bulletSpeed = PISTOL_BULLET_SPEED;
        bulletRadius = PISTOL_BULLET_RADIUS;
        break;
    case WeaponType::LASER:
        fireRate = LASER_FIRE_RATE;
        damage = LASER_DAMAGE;
        bulletSpeed = 0.0f;
        bulletRadius = 0.0f;
        break;
    }
}


void Weapon::fire(Vector2 origin, Vector2 targetDir)
{
    if (cooldown > 0.0f) return;

    float len = sqrtf(targetDir.x * targetDir.x + targetDir.y * targetDir.y);
    if (len < 0.001f) return;

    Vector2 dir = { targetDir.x / len, targetDir.y / len };

    if (w_type == WeaponType::PISTOL)
    {
        BulletPool::instance().spawn(origin, dir, bulletSpeed, bulletRadius, damage, 0);
        AudioManager::instance().playSoundPistol();
        cooldown = fireRate;
    }
    else if (w_type == WeaponType::LASER)
    {
        if (!beamActive)
        {
            beamActive = true;
            beamTimer = LASER_DURATION;
            AudioManager::instance().playSoundSniper();
        }

        beamOrigin = origin;
        beamDir = dir;
    }
}


void Weapon::update(float deltaTime)
{
    if (beamActive)
    {
        beamTimer -= deltaTime;
        
        if (beamTimer <= 0.0f)
        {
            beamActive = false;
            beamTimer = 0.0f;
            cooldown = fireRate;  // Cooldown starts after beam ends
        }
    }
    else
    {
        if (cooldown > 0.0f) cooldown -= deltaTime;
        if (cooldown < 0.0f) cooldown = 0.0f;
    }
}


const char* Weapon::getName() const
{
    return (w_type == WeaponType::PISTOL) ? "Pistol" : "Laser";
}