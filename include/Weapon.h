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
    float getCooldown() const { return m_cooldown; }
    float getFireRate() const { return m_fireRate; }
    bool canFire() const { return m_cooldown <= 0.0f; }

private:
    WeaponType m_type;
    float m_fireRate;
    float m_damage;
    float m_bulletSpeed;
    float m_bulletRadius;
    float m_cooldown {0.0f};
};
