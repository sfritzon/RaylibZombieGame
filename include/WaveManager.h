#pragma once

#include "Enemy.h"
#include "Constants.h"
#include <array>


struct SpawnGroup 
{
    EnemyType type;
    int count;
    float interval; // Seconds between spawns
    float timer;
    int remaining;
    bool active;
};


class WaveManager 
{
public:
    void reset();
    bool update(float deltaTime);
    void draw() const;

    int getWave() const { return wave; }
    float getWaveTimer() const { return waveTimer; }
    bool isWaveComplete() const { return waveTimer <= 0.0f && allSpawned; }

    Enemy* getEnemies() { return enemies.data(); }
    int getEnemyCount() const { return MAX_ENEMIES; }

private:
    void spawnEnemy(EnemyType type);
    void startNextWave();
    Vector2 randomSpawnPos() const;

    int wave {0};
    float waveTimer {0};
    bool allSpawned {false};

    std::array<Enemy, MAX_ENEMIES> enemies;

    // Spawn scheduling
    static const int MAX_GROUPS = 4;
    SpawnGroup groups[MAX_GROUPS];
    float betweenGroupTimer {0};
    int groupIndex {0};
};
