#pragma once

#include "raylib.h"
#include "Weapon.h"
#include <memory>


class Player 
{
public:
    Player();

    void reset();
    void handleInput(float dt);
    void update(float dt);
    void draw() const;

    void takeDamage(float amount);
    bool isDead() const { return m_health <= 0.0f; }
    float getHealth() const { return m_health; }
    float& getHealthRef() { return m_health; }
    float getMaxHealth() const;
    Vector2 getPosition() const { return m_position; }
    float getRadius() const;

    WeaponType getCurrentWeaponType() const;
    const char* getCurrentWeaponName() const;
    float getWeaponCooldown() const;
    float getWeaponFireRate() const;

private:
    Vector2 m_position {640, 360};
    float m_health;

    std::unique_ptr<Weapon> m_pistol;
    std::unique_ptr<Weapon> m_sniper;
    Weapon* m_currentWeapon {nullptr};

    float m_invincibleTimer {0.0f};
};
