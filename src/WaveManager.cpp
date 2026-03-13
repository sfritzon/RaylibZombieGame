#include "WaveManager.h"
#include "Constants.h"
#include "raylib.h"
#include <cmath>
#include <cstdlib>


void WaveManager::reset() 
{
    m_wave = 0;
    m_waveTimer = 0.0f;
    m_allSpawned = true;
    m_betweenGroupTimer = 0.0f;
    m_groupIndex = 0;
    for (auto& e : m_enemies) e.active = false;
    for (auto& g : m_groups)  g.active = false;
    startNextWave();
}


void WaveManager::startNextWave() 
{
    m_wave++;
    m_waveTimer = WAVE_DURATION;
    m_allSpawned = false;
    m_groupIndex = 0;
    m_betweenGroupTimer = 0.0f;

    // Build spawn groups based on wave number
    int zombiesTotal = 4 + m_wave * 3;
    int tanksTotal = (m_wave >= 2) ? (m_wave / 2) : 0;

    int numGroups = 0;
    if (zombiesTotal > 0) 
    {
        int perGroup = zombiesTotal / 3 + 1;
        for (int g = 0; g < 3 && numGroups < MAX_GROUPS && zombiesTotal > 0; ++g) 
        {
            int cnt = (zombiesTotal > perGroup) ? perGroup : zombiesTotal;
            zombiesTotal -= cnt;
            m_groups[numGroups] = { EnemyType::ZOMBIE, cnt, 0.3f, 0.0f, cnt, true };
            numGroups++;
        }
    }

    if (tanksTotal > 0 && numGroups < MAX_GROUPS) 
    {
        m_groups[numGroups] = { EnemyType::TANK, tanksTotal, 1.2f, 0.0f, tanksTotal, true };
        numGroups++;
    }

    for (int g = numGroups; g < MAX_GROUPS; ++g) m_groups[g].active = false;
}


Vector2 WaveManager::randomSpawnPos() const 
{
    // Spawn off-screen on random edge
    int edge = GetRandomValue(0, 3);
    switch (edge) 
    {
        case 0: return { (float)GetRandomValue(0, SCREEN_WIDTH), -SPAWN_MARGIN };
        case 1: return { (float)GetRandomValue(0, SCREEN_WIDTH), (float)SCREEN_HEIGHT + SPAWN_MARGIN };
        case 2: return { -SPAWN_MARGIN, (float)GetRandomValue(0, SCREEN_HEIGHT) };
        default:return { (float)SCREEN_WIDTH + SPAWN_MARGIN, (float)GetRandomValue(0, SCREEN_HEIGHT) };
    }
}


void WaveManager::spawnEnemy(EnemyType type) 
{
    for (auto& e : m_enemies) 
    {
        if (!e.active) 
        {
            e.init(type, randomSpawnPos());
            return;
        }
    }
}


void WaveManager::update(float dt) 
{
    // Update wave timer
    if (m_waveTimer > 0.0f) m_waveTimer -= dt;
    if (m_waveTimer < 0.0f) m_waveTimer = 0.0f;

    // Process spawn groups
    if (m_groupIndex < MAX_GROUPS) 
    {
        SpawnGroup& g = m_groups[m_groupIndex];
        if (g.active && g.remaining > 0) 
        {
            g.timer -= dt;
            if (g.timer <= 0.0f) 
            {
                spawnEnemy(g.type);
                g.remaining--;
                g.timer = g.interval;
            }

            if (g.remaining <= 0) 
            {
                m_groupIndex++;
                m_betweenGroupTimer = 1.5f;
            }

        } 

        else if (!g.active || m_betweenGroupTimer > 0.0f) 
        {
            m_betweenGroupTimer -= dt;
            if (m_betweenGroupTimer <= 0.0f) m_groupIndex++;
        } 
        
        else 
        {
            m_groupIndex++;
        }
    } 
    
    else 
    {
        m_allSpawned = true;
    }

    // Count living enemies
    int alive = 0;
    for (auto& e : m_enemies) if (e.active) alive++;

    // Start next wave when all enemies are dead
    if (alive == 0 && m_allSpawned) 
    {
        startNextWave();
    }

}


void WaveManager::draw() const 
{
    // Wave banner
    const char* waveText = TextFormat("WAVE %d", m_wave);
    int textWidth = MeasureText(waveText, 28);
    DrawText(waveText, SCREEN_WIDTH / 2 - textWidth / 2, 18, 28, { 255, 220, 80, 255 });

    // Timer bar
    float ratio = m_waveTimer / WAVE_DURATION;
    int barW = 300, barH = 12;
    int barX = SCREEN_WIDTH / 2 - barW / 2;
    int barY = 52;

    DrawRectangle(barX - 1, barY - 1, barW + 2, barH + 2, { 40, 40, 40, 200 });
    Color barColor;

    if (ratio > 0.5f) barColor = Color{ 80, 200, 80, 255 };
    else if (ratio > 0.25f) barColor = ORANGE;
    else barColor = RED;
    
    DrawRectangle(barX, barY, (int)(barW * ratio), barH, barColor);

    // Timer text
    const char* timerText = TextFormat("%.1f s", m_waveTimer);
    DrawText(timerText, SCREEN_WIDTH / 2 - MeasureText(timerText, 16) / 2, 68, 20, LIGHTGRAY);
}
