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
    bool update(float dt);
    void draw() const;

    int getWave() const { return m_wave; }
    float getWaveTimer() const { return m_waveTimer; }
    bool isWaveComplete() const { return m_waveTimer <= 0.0f && m_allSpawned; }

    Enemy* getEnemies() { return m_enemies.data(); }
    int getEnemyCount() const { return MAX_ENEMIES; }

private:
    void spawnEnemy(EnemyType type);
    void startNextWave();
    Vector2 randomSpawnPos() const;

    int m_wave {0};
    float m_waveTimer {0};
    bool m_allSpawned {false};

    std::array<Enemy, MAX_ENEMIES> m_enemies;

    // Spawn scheduling
    static const int MAX_GROUPS = 4;
    SpawnGroup m_groups[MAX_GROUPS];
    float m_betweenGroupTimer {0};
    int m_groupIndex {0};
};
