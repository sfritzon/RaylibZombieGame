#include "Player.h"
#include "BulletPool.h"
#include "Constants.h"
#include "WaveManager.h"
#include "AudioManager.h"
#include "raylib.h"
#include <cmath>


Player::Player() 
{
    pistol = std::make_unique<Weapon>(WeaponType::PISTOL);
    laser = std::make_unique<Weapon>(WeaponType::LASER);
    currentWeapon = pistol.get();
    health = PLAYER_MAX_HEALTH;
}


void Player::reset() 
{
    position = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };
    health = PLAYER_MAX_HEALTH;
    invisibleTimer = 0.0f;
    pistol = std::make_unique<Weapon>(WeaponType::PISTOL);
    laser = std::make_unique<Weapon>(WeaponType::LASER);
    currentWeapon = pistol.get();
}


void Player::handleInput(float deltaTime) 
{
    // Movement
    Vector2 dir = {0, 0};
    if (IsKeyDown(KEY_W)) dir.y -= 1;
    if (IsKeyDown(KEY_S)) dir.y += 1;
    if (IsKeyDown(KEY_A)) dir.x -= 1;
    if (IsKeyDown(KEY_D)) dir.x += 1;

    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

    if (length > 0.001f) 
    {
        dir.x /= length; dir.y /= length;
        position.x += dir.x * PLAYER_SPEED * deltaTime;
        position.y += dir.y * PLAYER_SPEED * deltaTime;
    }

    const float radius = 18.0f; // Same value as PLAYER_RADIUS in Constants.h
    if (position.x < radius) position.x = radius;
    if (position.x > SCREEN_WIDTH  - radius) position.x = SCREEN_WIDTH  - radius;
    if (position.y < radius) position.y = radius;
    if (position.y > SCREEN_HEIGHT - radius) position.y = SCREEN_HEIGHT - radius;


    // Swap weapon
    if (IsKeyPressed(KEY_Q)) 
    {
        currentWeapon = (currentWeapon == pistol.get()) ? laser.get() : pistol.get();
    }

    // Shoot
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
    Vector2 mouse = GetMousePosition();
    Vector2 fireDir = { mouse.x - position.x, mouse.y - position.y };
    currentWeapon->fire(position, fireDir);

    // Update beam direction every frame while holding fire so it tracks the player position
        if (currentWeapon->isBeamActive())
        {
            float fireLen = sqrtf(fireDir.x * fireDir.x + fireDir.y * fireDir.y);
            if (fireLen > 0.001f)
            {
            fireDir.x /= fireLen;
            fireDir.y /= fireLen;
            }
        }
    }
}


void Player::update(float deltaTime) 
{

    pistol->update(deltaTime);
    laser->update(deltaTime);
    if (invisibleTimer > 0.0f) invisibleTimer -= deltaTime;
}


void Player::draw() const 
{
    // Shadow
    DrawEllipse((int)position.x, (int)(position.y + PLAYER_RADIUS + 4),
                (int)(PLAYER_RADIUS * 0.9f), 6, { 0, 0, 0, 60 });

    // Body
    bool isDamaged = invisibleTimer > 0.0f;
    bool blinkOn = isDamaged && ((int)(invisibleTimer * 10) % 2 == 0);

    Color bodyColor = blinkOn ? Color{ 255, 50, 50, 255 } : Color{ 80, 180, 255, 255 };
    Color rimColor = blinkOn ? Color{ 255, 180, 180, 255 } : Color{ 200, 230, 255, 255 };

    DrawCircleV(position, PLAYER_RADIUS, bodyColor);
    DrawCircleLines((int)position.x, (int)position.y, PLAYER_RADIUS, rimColor);

    // Red glow pulse when hit
    if (blinkOn) 
    {
        DrawCircleV(position, PLAYER_RADIUS + 5.0f, { 255, 0, 0, 60 });
    }


    // Aim line toward mouse
    Vector2 mouse = GetMousePosition();
    Vector2 aimDir = { mouse.x - position.x, mouse.y - position.y };

    float fireLen = sqrtf(aimDir.x * aimDir.x + aimDir.y * aimDir.y);

    if (fireLen > 0.001f) 
    {
        aimDir.x /= fireLen; aimDir.y /= fireLen;
        Vector2 tip = { position.x + aimDir.x * (PLAYER_RADIUS + 14),
                        position.y + aimDir.y * (PLAYER_RADIUS + 14) };

        DrawLineV(position, tip, { 255, 255, 255, 180 });
    }


    //Draw laser beam if active
    if (currentWeapon->getType() == WeaponType::LASER && currentWeapon->isBeamActive())
    {
        float laserTime = currentWeapon->isBeamActive() / LASER_DURATION;
        unsigned char alpha = (unsigned char)(laserTime * 255);

        Vector2 beamEnd = 
        {
            position.x + currentWeapon->getBeamDir().x * 2000.0f,
            position.y + currentWeapon->getBeamDir().y * 2000.0f
        };

        // Outer glow
        DrawLineEx(position, beamEnd, LASER_WIDTH + 6.0f, { 255, 50, 50, (unsigned char)(alpha * 0.3f) });
        // Mid glow
        DrawLineEx(position, beamEnd, LASER_WIDTH + 2.0f, { 255, 120, 50, (unsigned char)(alpha * 0.6f) });
        // Core beam
        DrawLineEx(position, beamEnd, LASER_WIDTH, { 255, 220, 200, alpha });
    }
}


void Player::takeDamage(float amount) 
{
    if (invisibleTimer > 0.0f) return;
    health -= amount;
    if (health < 0) health = 0;
    AudioManager::instance().playDamageSound();
    invisibleTimer = 0.4f;
}

    float Player::getMaxHealth() const { return PLAYER_MAX_HEALTH; }
    float Player::getRadius() const { return PLAYER_RADIUS; }

    WeaponType Player::getCurrentWeaponType() const { return currentWeapon->getType(); }
    const char* Player::getCurrentWeaponName() const { return currentWeapon->getName(); }
    float Player::getWeaponCooldown() const { return currentWeapon->getCooldown(); }
    float Player::getWeaponFireRate() const { return currentWeapon->getFireRate(); }

    bool    Player::isLaserBeamActive()  const { return laser->isBeamActive(); }
    Vector2 Player::getLaserBeamOrigin() const { return laser->getBeamOrigin(); }
    Vector2 Player::getLaserBeamDir()    const { return laser->getBeamDir(); } 
