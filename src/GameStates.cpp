#include "GameStates.h"
#include "Game.h"
#include "AudioManager.h"
#include "BulletPool.h"
#include "Constants.h"
#include "HealthPackManager.h"
#include "TimeBoostManager.h"
#include "raylib.h"
#include <cmath>
#include <cstring>


void MainMenuState::enter(Game&) 
{
    selected = 0;
    anim = 0.0f;
    AudioManager::instance().setDucked(false);
    AudioManager::instance().playMenuMusic();
}


void MainMenuState::handleInput(Game& game)
{
    // Mouse position
    Vector2 mouse = GetMousePosition();

    // Match the Y positions and sizes from draw
    const char* opts[] = { "START GAME", "QUIT" };
    for (int i = 0; i < 2; ++i)
    {
        int size = 34; // Use the selected size since hover acts as selected
        int y = 280 + i * 70;
        int w = MeasureText(opts[i], size) + 30;
        int x = SCREEN_WIDTH / 2 - w / 2;

        // Check if mouse is hovering over this option
        if (mouse.x >= x && mouse.x <= x + w &&
            mouse.y >= y - 6 && mouse.y <= y - 6 + size + 10)
        {
            selected = i;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (selected == 0)
                {
                    game.resetWorld();
                    game.changeState(GameStateID::PLAYING);
                }
                else
                {
                    CloseWindow();
                }
            }
        }
    }

    // Keyboard support
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) selected = 0;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) selected = 1;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        if (selected == 0) { game.resetWorld(); game.changeState(GameStateID::PLAYING); }
        else CloseWindow();
    }

    if (IsKeyPressed(KEY_ESCAPE)) CloseWindow();
}


void MainMenuState::update(Game&, float deltaTime) 
{
    anim += deltaTime;
}


void MainMenuState::draw(Game&) 
{
    ClearBackground({ 8, 8, 18, 255 });

    // Animated grid dots for the funz
    for (int x = 0; x < SCREEN_WIDTH; x += 50)
        for (int y = 0; y < SCREEN_HEIGHT; y += 50) 
        {
            float pulse = 0.3f + 0.15f * sinf(anim * 1.2f + x * 0.01f + y * 0.01f);
            DrawCircle(x, y, 1.5f, { 40, 80, 140, (unsigned char)(pulse * 255) });
        }


    // Title
    const char* title = "ZOMBIE ARENA";
    int textSize = 110;
    int textWidth = MeasureText(title, textSize);

    // Shadow
    DrawText(title, SCREEN_WIDTH / 2 - textWidth / 2 + 3, 103, textSize, { 0, 0, 0, 180 });
    DrawText(title, SCREEN_WIDTH / 2 - textWidth / 2, 100, textSize, { 80, 200, 255, 255 });

    // Sub-title
    const char* sub = "Survive the waves. Stay alive. Good luck.";
    DrawText(sub, SCREEN_WIDTH / 2 - MeasureText(sub, 25) / 2, 205
    , 25, { 140, 180, 220, 200 });

    // Menu options
    const char* opts[] = { "START GAME", "QUIT" };
    for (int i = 0; i < 2; ++i) 
    {
        bool select = (i == selected);
        float bounce = select ? sinf(anim * 4.0f) * 3.0f : 0.0f;
        int y = 280 + i * 70 + (int)bounce;

        Color c = select ? Color{ 255, 220, 60, 255 } : Color{ 160, 180, 200, 200 };
        
        int size = select ? 34 : 28;
        
        if (select) 
        {
            // Highlight box
            int w = MeasureText(opts[i], size) + 30;

            DrawRectangle(SCREEN_WIDTH / 2 - w / 2, y - 6, w, size + 10, { 255, 220, 60, 30 });

            DrawRectangleLines(SCREEN_WIDTH / 2 - w / 2, y - 6, w, size + 10, { 255, 220, 60, 120 });
        }

        DrawText(opts[i], SCREEN_WIDTH / 2 - MeasureText(opts[i], size) / 2, y, size, c);
    }


    // Menu instructions
    int intSpace = 450;
    const char* howTitle = "HOW TO PLAY";
    DrawText(howTitle, SCREEN_WIDTH / 2 - MeasureText(howTitle, 26) / 2, intSpace, 27, { 200, 200, 255, 255 });
    intSpace += 65;

    struct Tip { const char* key; const char* desc; };

    Tip tips[] = 
    {
        { "WASD",         "Move" },
        { "LMB",          "Shoot toward cursor" },
        { "Q",            "Swap weapon (Pistol / Sniper)" },
        { "ESC",          "Pause" },
        { "R",            "Reset game" },
        { "Pistol",       "Standard damage, fast fire rate" },
        { "Sniper",       "Heavy damage, slow fire rate" },
        { "Zombie (Z)",   "Fast, low health" },
        { "Tank   (T)",   "Slow, heavy armor" },
    };

    int colKeyRight = SCREEN_WIDTH / 2 - 20;  // All keys align to the right
    int colDash = SCREEN_WIDTH / 2 - 10;
    int colDesc = SCREEN_WIDTH / 2 + 9;

    for (auto& t : tips) 
    {
    int keyWidth = MeasureText(t.key, 18);

    DrawText(t.key, colKeyRight - keyWidth, intSpace, 18, { 255, 220, 80, 230 });
    DrawText("—", colDash, intSpace, 18, { 100, 120, 160, 200 });
    DrawText(t.desc, colDesc, intSpace, 18, { 180, 200, 220, 220 });
    intSpace += 22;
    }

    DrawText("Use W/S or Arrow Keys to navigate, ENTER to confirm",
             SCREEN_WIDTH / 2 - MeasureText("Use W/S or Arrow Keys to navigate, ENTER to confirm", 16) / 2,
             SCREEN_HEIGHT - 55, 18, { 80, 100, 140, 180 });
}


// Playing state
void PlayingState::enter(Game&) 
{
    AudioManager::instance().setDucked(false);
    AudioManager::instance().playGameMusic();
}


void PlayingState::handleInput(Game& game) 
{
    if (IsKeyPressed(KEY_ESCAPE)) game.changeState(GameStateID::PAUSED);

    if (IsKeyPressed(KEY_R)) 
    {
        game.resetWorld();
    }

    //DEBUG: kill all zombies instantly
    if (IsKeyPressed(KEY_K))
    {
        WaveManager& waveManager = game.getWaveManager();
        for (int i = 0; i < MAX_ENEMIES; ++i)
        {
            Enemy& e = waveManager.getEnemies()[i];
            if (e.active && e.type == EnemyType::ZOMBIE)
            e.active = false;
            if (e.active && e.type == EnemyType::TANK)
            e.active = false;
        }
    }
}


void PlayingState::update(Game& game, float deltaTime) 
{
    Player& player = game.getPlayer();
    WaveManager& waveManager = game.getWaveManager();

    player.handleInput(deltaTime);
    player.update(deltaTime);

    BulletPool::instance().update(deltaTime);

    HealthPackManager::instance().update(
        deltaTime,
        player.getPosition(),
        player.getRadius(),
        player.getHealthRef(),
        player.getMaxHealth()
    );

    TimeBoostManager::instance().update(
        deltaTime,
        player.getPosition(),
        player.getRadius(),
        waveManager.getWaveTimerRef()
    );

    // Update enemies, deal damage to player
    for (int i = 0; i < MAX_ENEMIES; ++i) 
    {
    Enemy& e = waveManager.getEnemies()[i];
    if (!e.active) continue;

    // Check if enemy is touching player
    float dx = e.position.x - player.getPosition().x;
    float distY = e.position.y - player.getPosition().y;
    float dist = sqrtf(dx * dx + distY * distY);

    if (dist < e.radius + player.getRadius()) 
    {
        e.attackTimer -= deltaTime;
        if (e.attackTimer <= 0.0f) 
        {
            player.takeDamage(e.damage);
            e.attackTimer = ENEMY_ATTACK_INTERVAL;
        }
    }

    // Still move enemy toward player
    float dummy = 0.0f;
    e.update(deltaTime, player.getPosition(), player.getRadius(), dummy);
}

    updateBulletEnemyCollisions(game);

    if (waveManager.update(deltaTime))
        game.changeState(GameStateID:: GAME_OVER);

    if (player.isDead()) game.changeState(GameStateID::GAME_OVER);
}


void PlayingState::updateBulletEnemyCollisions(Game& game) 
{
    BulletPool& pool = BulletPool::instance();
    WaveManager& waveManager = game.getWaveManager();

    for (int bi = 0; bi < MAX_BULLETS; ++bi) 
    {
        Bullet& b = pool.getBullets()[bi];
        if (!b.active) continue;

        for (int ei = 0; ei < MAX_ENEMIES; ++ei) 
        {
            Enemy& e = waveManager.getEnemies()[ei];
            if (!e.active) continue;

            float distX = b.position.x - e.position.x;
            float distY = b.position.y - e.position.y;
            float dist2 = distX * distX + distY * distY;
            float radSum = b.radius + e.radius;

            if (dist2 < radSum * radSum) 
            {
                e.health -= b.damage;
                b.active = false;

                if (e.isDead())
                {
                    e.active = false;
                    game.addScore(e.type == EnemyType::ZOMBIE ? 10 : 30);

                    if (e.type == EnemyType::ZOMBIE)
                    {
                        TimeBoostManager::instance().trySpawn( e.position, game.getWaveManager().getWave());
                    }

                    if (e.type == EnemyType::TANK)
                    {
                        // Counts the remaining active tanks before current dies
                        int activeTanks = 0;
                        for (int ti = 0; ti < MAX_ENEMIES; ++ti)
                        {
                            if (waveManager.getEnemies()[ti].active && waveManager.getEnemies()[ti].type == EnemyType::TANK)
                                activeTanks++;

                        }

                        HealthPackManager::instance().trySpawn(e.position, activeTanks);
                    }
                }
            }
        }
    }
}


void PlayingState::drawHUD(Game& game) const 
{
    Player& player = game.getPlayer();

    bool isSniper = player.getCurrentWeaponType() == WeaponType::SNIPER;
    Color wpnColor = isSniper ? Color{ 255, 160, 30, 255 } : Color{ 200, 0, 200, 255 }; // First: sniper, Second: pistol

    // Weapon name
    DrawText(TextFormat("Weapon: %s  [Q] Swap", player.getCurrentWeaponName()),
             20, SCREEN_HEIGHT - 115, 25, wpnColor);

    // Fire rate bar
    float cd = player.getWeaponCooldown();
    float maxCD = player.getWeaponFireRate();
    float ready = 1.0f - (maxCD > 0 ? cd / maxCD : 0.0f);

    DrawText("Fire Rate", 20, SCREEN_HEIGHT - 79, 18, { 200, 220, 200, 255 });
    DrawRectangle(20, SCREEN_HEIGHT - 62, 160, 8, { 30, 30, 30, 220 });
    DrawRectangle(20, SCREEN_HEIGHT - 62, (int)(160 * ready), 8, wpnColor);
    DrawRectangleLines(20, SCREEN_HEIGHT - 62, 160, 8, { 80, 80, 80, 200 });

    // Health label + bar
    float hpRatio = player.getHealth() / player.getMaxHealth();
    Color hpColor = hpRatio > 0.5f ? GREEN : (hpRatio > 0.25f ? ORANGE : RED);

    DrawText(TextFormat("HP: %.0f / %.0f", player.getHealth(), player.getMaxHealth()),
             20, SCREEN_HEIGHT - 44, 18, { 200, 220, 200, 255 });

    DrawRectangle(20, SCREEN_HEIGHT - 28, 200, 14, { 30, 30, 30, 220 });
    DrawRectangle(20, SCREEN_HEIGHT - 28, (int)(200 * hpRatio), 14, hpColor);
    DrawRectangleLines(20, SCREEN_HEIGHT - 28, 200, 14, { 80, 80, 80, 200 });

    // Controls hint
    DrawText("[R] Reset  [ESC] Pause",
             SCREEN_WIDTH - MeasureText("[R] Reset  [ESC] Pause", 13) - 14,
             SCREEN_HEIGHT - 24, 13, { 100, 120, 150, 160 });

    // Score counter
    const char* scoreText = TextFormat("Score: %d", game.getScore());
    DrawText(scoreText, SCREEN_WIDTH - MeasureText(scoreText, 25) - 14, 20, 22, { 255, 220, 80, 255 });
}


void PlayingState::draw(Game& game) 
{

    ClearBackground({ 12, 14, 22, 255 });


    // Grid
    for (int x = 0; x < SCREEN_WIDTH; x += 60)
        DrawLine(x, 0, x, SCREEN_HEIGHT, { 25, 30, 45, 255 });
    for (int y = 0; y < SCREEN_HEIGHT; y += 60)
        DrawLine(0, y, SCREEN_WIDTH, y, { 25, 30, 45, 255 });

    // World objects
    BulletPool::instance().draw();

    HealthPackManager::instance().draw();
    TimeBoostManager::instance().draw();

    WaveManager& waveManager = game.getWaveManager();
    for (int i = 0; i < MAX_ENEMIES; ++i)
        waveManager.getEnemies()[i].draw();

    game.getPlayer().draw();

    // HUD
    waveManager.draw();
    drawHUD(game);
}

// Paused state
void PausedState::handleInput(Game& game)
{
    Vector2 mouse = GetMousePosition();

    int pw = 380, ph = 240;
    int px = SCREEN_WIDTH / 2 - pw / 2, py = SCREEN_HEIGHT / 2 - ph / 2;

    const char* opts[] = { "CONTINUE", "MAIN MENU" };
    for (int i = 0; i < 2; ++i)
    {
        int size = 30;
        int y = py + 110 + i * 60;
        int w = MeasureText(opts[i], size) + 24;
        int x = SCREEN_WIDTH / 2 - w / 2;

        if (mouse.x >= x && mouse.x <= x + w &&
            mouse.y >= y - 4 && mouse.y <= y - 4 + size + 8)
        {
            selected = i;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (selected == 0) game.changeState(GameStateID::PLAYING);
                else game.changeState(GameStateID::MAIN_MENU);
            }
        }
    }

    // Keep keyboard support
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) selected = 0;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) selected = 1;
    if (IsKeyPressed(KEY_ENTER))
    {
        if (selected == 0) game.changeState(GameStateID::PLAYING);
        else game.changeState(GameStateID::MAIN_MENU);
    }

    if (IsKeyPressed(KEY_ESCAPE)) game.changeState(GameStateID::PLAYING);
}


void PausedState::enter(Game&)
{
    AudioManager::instance().setDucked(true);
}

void PausedState::draw(Game& game) 
{
    ClearBackground({ 12, 14, 22, 255 });

    for (int x = 0; x < SCREEN_WIDTH; x += 60)
        DrawLine(x, 0, x, SCREEN_HEIGHT, { 25, 30, 45, 255 });
    for (int y = 0; y < SCREEN_HEIGHT; y += 60)
        DrawLine(0, y, SCREEN_WIDTH, y, { 25, 30, 45, 255 });

    BulletPool::instance().draw();
    WaveManager& waveManager = game.getWaveManager();

    for (int i = 0; i < MAX_ENEMIES; ++i) waveManager.getEnemies()[i].draw(); 
    game.getPlayer().draw();

    // Overlay
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 140 });

    // Panel
    int pw = 380, ph = 240;
    int px = SCREEN_WIDTH / 2 - pw / 2, py = SCREEN_HEIGHT / 2 - ph / 2;
    DrawRectangle(px, py, pw, ph, { 10, 14, 28, 240 });
    DrawRectangleLines(px, py, pw, ph, { 80, 120, 200, 200 });

    const char* pauseTitle = "PAUSED";
    DrawText(pauseTitle, SCREEN_WIDTH / 2 - MeasureText(pauseTitle, 42) / 2, py + 24, 42, { 80, 200, 255, 255 });

    const char* opts[] = { "CONTINUE", "MAIN MENU" };

    for (int i = 0; i < 2; ++i)
    {
        bool select = (i == selected);

        Color c = select ? Color{ 255, 220, 60, 255 } : Color{ 160, 180, 200, 200 };

        int size = select ? 30 : 24;
        int y  = py + 110 + i * 60;

        if (select) 
        {
            int w = MeasureText(opts[i], size) + 24;
            DrawRectangle(SCREEN_WIDTH / 2 - w / 2, y - 4, w, size + 8, { 255, 220, 60, 25 });
            DrawRectangleLines(SCREEN_WIDTH / 2 - w / 2, y - 4, w, size + 8, { 255, 220, 60, 100 });
        }

        DrawText(opts[i], SCREEN_WIDTH / 2 - MeasureText(opts[i], size) / 2, y, size, c);
    }
    
    DrawText("W/S to selectect  •  ENTER to confirm  •  ESC to resume",
            SCREEN_WIDTH / 2 - MeasureText("W/S to selectect  •  ENTER to confirm  •  ESC to resume", 12) / 2,
            py + ph - 24, 12, { 80, 100, 140, 180 });
}


void PausedState::exit(Game&)
{
    AudioManager::instance().setDucked(false);
}

// Game over state
void GameOverState::enter(Game& game) 
{
    finalScore = game.getScore();
    finalWave  = game.getWaveManager().getWave();
    AudioManager::instance().setDucked(false);
    AudioManager::instance().playMenuMusic();
}

void GameOverState::handleInput(Game& game) 
{
    AudioManager::instance().setDucked(false);
    AudioManager::instance().playGameOverMusic();
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {}

    if (IsKeyPressed(KEY_R)) 
    {
        game.resetWorld();
        game.changeState(GameStateID::PLAYING);
    }

    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_M)) 
    {
        game.changeState(GameStateID::MAIN_MENU);
    }
}

void GameOverState::draw(Game&) 
{
    ClearBackground({ 8, 4, 10, 255 });

    // Dramatic red vignette
    for (int i = 0; i < 8; ++i) 
    {
        unsigned char alpha = (unsigned char)(30 - i * 3);
        DrawRectangleLinesEx({ (float)i * 12, (float)i * 8,
                               (float)(SCREEN_WIDTH - i * 24), (float)(SCREEN_HEIGHT - i * 16) },
                             12, { 180, 30, 30, alpha });
    }

    const char* goText = "GAME OVER";
    int siz = 110;
    DrawText(goText, SCREEN_WIDTH / 2 - MeasureText(goText, siz) / 2 + 3, 303, siz, { 80, 0, 0, 200 });
    DrawText(goText, SCREEN_WIDTH / 2 - MeasureText(goText, siz) / 2, 300, siz, { 220, 50, 50, 255 });

    // Stats
    const char* scoreText = TextFormat("Score: %d", finalScore);
    const char* waveText  = TextFormat("Reached Wave: %d", finalWave);
    DrawText(scoreText, SCREEN_WIDTH / 2 - MeasureText(scoreText, 32) / 2, 425, 32, { 255, 220, 80, 255 });
    DrawText(waveText,  SCREEN_WIDTH / 2 - MeasureText(waveText,  24) / 2, 465, 24, LIGHTGRAY);

    DrawText("[ R ]  Restart",
             SCREEN_WIDTH / 2 - MeasureText("[ R ]  Restart", 24) / 2, 530, 24, { 80, 200, 255, 255 });
    DrawText("[ ENTER / ESC ]  Main Menu",
             SCREEN_WIDTH / 2 - MeasureText("[ ENTER / ESC ]  Main Menu", 20) / 2, 560, 20, { 140, 160, 200, 200 });
}
