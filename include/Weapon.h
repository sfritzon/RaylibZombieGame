#pragma once

#include "raylib.h"


enum class WeaponType { PISTOL, SNIPER };


class Weapon 
{
public:
    explicit Weapon(WeaponType type);

    void fire(Vector2 origin, Vector2 targetDir);
    void update(float dt);

    WeaponType  getType() const { return m_type; }
    const char* getName() const;
    float getCooldown() const { return cooldown; }
    float getFireRate() const { return fireRate; }
    bool canFire() const { return cooldown <= 0.0f; }

private:
    WeaponType m_type;
    float fireRate;
    float damage;
    float bulletSpeed;
    float bulletRadius;
    float cooldown {0.0f};
};
