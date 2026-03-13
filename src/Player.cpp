#include "Player.h"
#include "BulletPool.h"
#include "Constants.h"
#include "raylib.h"
#include <cmath>


Player::Player() 
{
    m_pistol = std::make_unique<Weapon>(WeaponType::PISTOL);
    m_sniper = std::make_unique<Weapon>(WeaponType::SNIPER);
    m_currentWeapon = m_pistol.get();
    m_health = PLAYER_MAX_HEALTH;
}


void Player::reset() 
{
    m_position = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };
    m_health = PLAYER_MAX_HEALTH;
    m_invincibleTimer = 0.0f;
    m_pistol = std::make_unique<Weapon>(WeaponType::PISTOL);
    m_sniper = std::make_unique<Weapon>(WeaponType::SNIPER);
    m_currentWeapon = m_pistol.get();
}


void Player::handleInput(float deltaTime) 
{
    // Movement
    Vector2 dir = {0, 0};
    if (IsKeyDown(KEY_W)) dir.y -= 1;
    if (IsKeyDown(KEY_S)) dir.y += 1;
    if (IsKeyDown(KEY_A)) dir.x -= 1;
    if (IsKeyDown(KEY_D)) dir.x += 1;

    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.001f) 
    {
        dir.x /= len; dir.y /= len;
        m_position.x += dir.x * PLAYER_SPEED * deltaTime;
        m_position.y += dir.y * PLAYER_SPEED * deltaTime;
    }

    const float radius = 18.0f; // same value as PLAYER_RADIUS in Constants.h
    if (m_position.x < radius) m_position.x = radius;
    if (m_position.x > SCREEN_WIDTH  - radius) m_position.x = SCREEN_WIDTH  - radius;
    if (m_position.y < radius) m_position.y = radius;
    if (m_position.y > SCREEN_HEIGHT - radius) m_position.y = SCREEN_HEIGHT - radius;


    // Swap weapon
    if (IsKeyPressed(KEY_Q)) 
    {
        m_currentWeapon = (m_currentWeapon == m_pistol.get()) ? m_sniper.get() : m_pistol.get();
    }

    // Shoot
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
    {
        Vector2 mouse = GetMousePosition();
        Vector2 fireDir = { mouse.x - m_position.x, mouse.y - m_position.y };
        m_currentWeapon->fire(m_position, fireDir);
    }

    (void)deltaTime;
}


void Player::update(float deltaTime) 
{

    m_pistol->update(deltaTime);
    m_sniper->update(deltaTime);
    if (m_invincibleTimer > 0.0f) m_invincibleTimer -= deltaTime;
}


void Player::draw() const 
{
    // Shadow
    DrawEllipse((int)m_position.x, (int)(m_position.y + PLAYER_RADIUS + 4),
                (int)(PLAYER_RADIUS * 0.9f), 6, { 0, 0, 0, 60 });

    // Body
bool isDamaged = m_invincibleTimer > 0.0f;
bool blinkOn = isDamaged && ((int)(m_invincibleTimer * 10) % 2 == 0);

Color bodyColor = blinkOn ? Color{ 255, 50, 50, 255 } : Color{ 80, 180, 255, 255 };
Color rimColor = blinkOn ? Color{ 255, 180, 180, 255 } : Color{ 200, 230, 255, 255 };

DrawCircleV(m_position, PLAYER_RADIUS, bodyColor);
DrawCircleLines((int)m_position.x, (int)m_position.y, PLAYER_RADIUS, rimColor);

// Red glow pulse when hit
if (blinkOn) 
{
    DrawCircleV(m_position, PLAYER_RADIUS + 5.0f, { 255, 0, 0, 60 });
}


    // Aim line toward mouse
    Vector2 mouse = GetMousePosition();
    Vector2 aimDir = { mouse.x - m_position.x, mouse.y - m_position.y };
    float len = sqrtf(aimDir.x * aimDir.x + aimDir.y * aimDir.y);

    if (len > 0.001f) 
    {
        aimDir.x /= len; aimDir.y /= len;
        Vector2 tip = { m_position.x + aimDir.x * (PLAYER_RADIUS + 14),
                        m_position.y + aimDir.y * (PLAYER_RADIUS + 14) };

        DrawLineV(m_position, tip, { 255, 255, 255, 180 });
    }
}


void Player::takeDamage(float amount) 
{
    if (m_invincibleTimer > 0.0f) return;
    m_health -= amount;
    if (m_health < 0) m_health = 0;
    m_invincibleTimer = 0.4f;
}

float Player::getMaxHealth() const { return PLAYER_MAX_HEALTH; }
float Player::getRadius() const { return PLAYER_RADIUS; }

WeaponType Player::getCurrentWeaponType() const { return m_currentWeapon->getType(); }
const char* Player::getCurrentWeaponName() const { return m_currentWeapon->getName(); }
float Player::getWeaponCooldown() const { return m_currentWeapon->getCooldown(); }
float Player::getWeaponFireRate() const { return m_currentWeapon->getFireRate(); }
