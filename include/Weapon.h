#pragma once

#include "raylib.h"


enum class WeaponType { PISTOL, LASER };


class Weapon 
{
public:
    explicit Weapon(WeaponType type);

    void fire(Vector2 origin, Vector2 targetDir);
    void update(float deltaTime);

    WeaponType getType() const { return w_type; }
    const char* getName() const;
    float getCooldown() const { return cooldown; }
    float getFireRate() const { return fireRate; }
    bool canFire() const { return cooldown <= 0.0f; }

    bool isBeamActive() const { return beamActive; }
    float getBeamTimer() const { return beamTimer; }
    Vector2 getBeamOrigin() const { return beamOrigin; }
    Vector2 getBeamDir() const { return beamDir; }

private:
    WeaponType w_type;
    float fireRate;
    float damage;
    float bulletSpeed;
    float bulletRadius;
    float cooldown {0.0f};

    bool beamActive {false};
    float beamTimer {0.0f};
    Vector2 beamOrigin {0, 0};
    Vector2 beamDir {0, 0};
};
