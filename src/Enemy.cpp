#include "Enemy.h"
#include "Constants.h"
#include "raylib.h"
#include <cmath>


void Enemy::init(EnemyType t, Vector2 pos) 
{
    type = t;
    position = pos;
    active = true;
    attackTimer = 0.0f;

    if (t == EnemyType::ZOMBIE) 
    {
        health = maxHealth = ZOMBIE_HEALTH;
        speed  = ZOMBIE_SPEED;
        damage = ZOMBIE_DAMAGE;
        radius = ZOMBIE_RADIUS;
    } 
    else 
    {
        health = maxHealth = TANK_HEALTH;
        speed = TANK_SPEED;
        damage = TANK_DAMAGE;
        radius = TANK_RADIUS;
    }
}


void Enemy::update(float deltaTime, Vector2 playerPos, float playerRadius, float& playerHealth) 
{
    if (!active) return;

    // Enemies move towards the player
    float dirX = playerPos.x - position.x;
    float dirY = playerPos.y - position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);

    if (dist > 0.001f) 
    {
        position.x += (dirX / dist) * speed * deltaTime;
        position.y += (dirY / dist) * speed * deltaTime;
    }

    if (dist < radius + playerRadius) 
    {
        attackTimer -= deltaTime;

        if (attackTimer <= 0.0f) 
        {
            playerHealth -= damage;
            if (playerHealth < 0) playerHealth = 0;
            attackTimer = ENEMY_ATTACK_INTERVAL;
        }
    }
}


void Enemy::draw() const 
{
    if (!active) return;

    Color body, outline;
    if (type == EnemyType::ZOMBIE) 
    {
        body = { 80, 160, 80, 255 };
        outline = { 150, 255, 150, 200 };
    } 
    else 
    {
        body = { 160, 60, 60, 255 };
        outline = { 255, 130, 130, 200 };
    }

    DrawCircleV(position, radius, body);
    DrawCircleLines((int)position.x, (int)position.y, radius, outline);

    // Enemy health bar
    float barW = radius * 2.0f;
    float barH = 5.0f;
    float barX = position.x - radius;
    float barY = position.y - radius - 10.0f;
    float hpRatio = health / maxHealth;

    DrawRectangle((int)barX, (int)barY, (int)barW, (int)barH, { 60, 0, 0, 200 });
    DrawRectangle((int)barX, (int)barY, (int)(barW * hpRatio), (int)barH,
                  hpRatio > 0.5f ? GREEN : (hpRatio > 0.25f ? ORANGE : RED));

    // Emeny label on body
    const char* label = (type == EnemyType::ZOMBIE) ? "Z" : "T";
    DrawText(label, (int)(position.x - 5), (int)(position.y - 7), 14, WHITE);
}
