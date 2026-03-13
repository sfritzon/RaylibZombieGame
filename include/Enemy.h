#pragma once

#include "raylib.h"


enum class EnemyType { ZOMBIE, TANK };

struct Enemy 
{
    Vector2 position {0, 0};
    float health {0};
    float maxHealth {0};
    float speed {0};
    float damage {0};
    float radius {0};
    float attackTimer{0};
    bool active {false};
    EnemyType type {EnemyType::ZOMBIE};

    void init(EnemyType t, Vector2 pos);
    void update(float dt, Vector2 playerPos, float playerRadius, float& playerHealth);
    void draw() const;
    bool isDead() const { return health <= 0.0f; }
};
