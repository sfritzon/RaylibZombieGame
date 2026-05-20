#pragma once

#include "raylib.h"
#include "Weapon.h"
#include <memory>


class Player 
{
public:
    Player();

    void reset();
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw() const;

    void takeDamage(float amount);
    bool isDead() const { return health <= 0.0f; }
    float getHealth() const { return health; }
    float& getHealthRef() { return health; }
    float getMaxHealth() const;
    Vector2 getPosition() const { return position; }
    float getRadius() const;

    WeaponType getCurrentWeaponType() const;
    const char* getCurrentWeaponName() const;
    float getWeaponCooldown() const;
    float getWeaponFireRate() const;

    bool isLaserBeamActive() const;
    Vector2 getLaserBeamOrigin() const;
    Vector2 getLaserBeamDir() const;

private:
    Vector2 position {640, 360};
    float health;

    std::unique_ptr<Weapon> pistol;
    std::unique_ptr<Weapon> laser;
    Weapon* currentWeapon {nullptr};

    float invisibleTimer {0.0f};
};
