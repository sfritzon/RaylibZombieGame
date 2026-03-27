#include "WaveManager.h"
#include "Constants.h"
#include "raylib.h"
#include <cmath>
#include <cstdlib>


void WaveManager::reset() 
{
    wave = 0;
    waveTimer = 0.0f;
    allSpawned = true;
    betweenGroupTimer = 0.0f;
    groupIndex = 0;
    for (auto& e : enemies) e.active = false;
    for (auto& g : groups)  g.active = false;
    startNextWave();
}


void WaveManager::startNextWave() 
{
    wave++;
    waveTimer = WAVE_DURATION;
    allSpawned = false;
    groupIndex = 0;
    betweenGroupTimer = 0.0f;

    // Build spawn groups based on wave number
    int zombiesTotal = 4 + wave * 3;
    int tanksTotal = (wave >= 2) ? (wave / 2) : 0;

    int numGroups = 0;
    
    if (zombiesTotal > 0) 
    {
        int perGroup = zombiesTotal / 3 + 1;
        for (int g = 0; g < 3 && numGroups < MAX_GROUPS && zombiesTotal > 0; ++g) 
        {
            int cnt = (zombiesTotal > perGroup) ? perGroup : zombiesTotal;
            zombiesTotal -= cnt;
            groups[numGroups] = { EnemyType::ZOMBIE, cnt, 0.3f, 0.0f, cnt, true };
            numGroups++;
        }
    }

    if (tanksTotal > 0 && numGroups < MAX_GROUPS) 
    {
        groups[numGroups] = { EnemyType::TANK, tanksTotal, 1.2f, 0.0f, tanksTotal, true };
        numGroups++;
    }

    for (int g = numGroups; g < MAX_GROUPS; ++g) groups[g].active = false;
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
    for (auto& e : enemies) 
    {
        if (!e.active) 
        {
            e.init(type, randomSpawnPos());
            return;
        }
    }
}


bool WaveManager::update(float deltaTime) 
{
    // Update wave timer
    if (waveTimer > 0.0f) waveTimer -= deltaTime;
    if (waveTimer < 0.0f) waveTimer = 0.0f;

    // Process spawn groups
    if (groupIndex < MAX_GROUPS) 
    {
        SpawnGroup& g = groups[groupIndex];
        if (g.active && g.remaining > 0) 
        {
            g.timer -= deltaTime;
            if (g.timer <= 0.0f) 
            {
                spawnEnemy(g.type);
                g.remaining--;
                g.timer = g.interval;
            }

            if (g.remaining <= 0) 
            {
                groupIndex++;
                betweenGroupTimer = 1.5f;
            }

        } 

        else if (!g.active || betweenGroupTimer > 0.0f) 
        {
            betweenGroupTimer -= deltaTime;
            if (betweenGroupTimer <= 0.0f) groupIndex++;
        } 
        
        else 
        {
            groupIndex++;
        }
    } 
    
    else 
    {
        allSpawned = true;
    }

    // Count living enemies
    int alive = 0;
    for (auto& e : enemies) if (e.active) alive++;

    // Start next wave when all enemies are dead
    if (alive == 0 && allSpawned) 
    {
        startNextWave();
    }

    if (waveTimer <= 0.0f && alive > 0)
    {
        return true;
    }

    return false;
}


void WaveManager::draw() const 
{
    // Wave banner
    const char* waveText = TextFormat("WAVE %d", wave);
    int textWidth = MeasureText(waveText, 28);
    DrawText(waveText, SCREEN_WIDTH / 2 - textWidth / 2, 18, 28, { 255, 220, 80, 255 });

    // Timer bar
    float ratio = waveTimer / WAVE_DURATION;
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
    const char* timerText = TextFormat("%.1f s", waveTimer);
    DrawText(timerText, SCREEN_WIDTH / 2 - MeasureText(timerText, 16) / 2, 68, 20, LIGHTGRAY);
}
