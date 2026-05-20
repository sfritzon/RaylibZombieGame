#include "GameStates.h"
#include "Game.h"
#include "AudioManager.h"
#include "BulletPool.h"
#include "Constants.h"
#include "HealthPackManager.h"
#include "TimeBoostManager.h"
#include "raylib.h"
#include "Scoreboard.h"
#include <cmath>
#include <cstring>


//---------------------------------------------------------------------------MENU STATE---------------------------------------------------------------------------//

void MainMenuState::enter(Game&) 
{
    selected = 0;
    anim = 0.0f;
    confirmTimer = 0.0f;
    confirmedOption = -1;
    AudioManager::instance().setDucked(false);
    AudioManager::instance().playMenuMusic();
}


void MainMenuState::handleInput(Game& game)
{
    // Mouse position
    Vector2 mouse = GetMousePosition();

    // Match the Y positions and sizes from draw
    const char* opts[] = { "START GAME", "NEW SCOREBOARD", "QUIT GAME" };
    for (int i = 0; i < 3; ++i)
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
                else if (selected == 1)
                {
                    Scoreboard::instance().reset();
                    confirmTimer = 0.6f;
                    confirmedOption = 1;
                }
                else
                {
                    CloseWindow();
                }
            }
        }
    }

    // Keyboard support
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) selected = (selected + 1) % 3;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) selected = (selected + 2) % 3;

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        if (selected == 0) 
        { 
            game.resetWorld(); game.changeState(GameStateID::PLAYING); 
        }
        else if (selected == 1) 
        { 
            Scoreboard::instance().reset();
            confirmTimer = 0.6f;
            confirmedOption = 1;
        }
        else CloseWindow();
    } 
}


void MainMenuState::update(Game&, float deltaTime) 
{
    anim += deltaTime;
    if (confirmTimer > 0.0f) confirmTimer -= deltaTime;
}


void MainMenuState::draw(Game&) 
{
    ClearBackground({ 8, 8, 18, 255 });

    // Animated grid dots for the funz
    for (int x = 0; x < SCREEN_WIDTH; x += 100)
        for (int y = 0; y < SCREEN_HEIGHT; y += 100) 
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
    const char* opts[] = { "START GAME", "NEW SCOREBOARD", "QUIT GAME" };
    for (int i = 0; i < 3; ++i)
    {
        bool select = (i == selected);
        bool confirmed = (confirmTimer > 0.0f && confirmedOption == i);
        float bounce = select && !confirmed ? sinf(anim * 4.0f) * 3.0f : 0.0f;
        int y = 270 + i * 70 + (int)bounce;

        Color c;
        if (confirmed) c = Color{ 120, 120, 120, 200 };
        else if (select) c = Color{ 255, 220, 60, 255 };
        else c = Color{ 160, 180, 200, 200 };
        
        int size = select ? 34 : 28;
        
        if (select && !confirmed) 
        {
            int w = MeasureText(opts[i], size) + 30;

            DrawRectangle(SCREEN_WIDTH / 2 - w / 2, y - 6, w, size + 10, { 255, 220, 60, 30 });
            DrawRectangleLines(SCREEN_WIDTH / 2 - w / 2, y - 6, w, size + 10, { 255, 220, 60, 120 });
        }

        DrawText(opts[i], SCREEN_WIDTH / 2 - MeasureText(opts[i], size) / 2, y, size, c);
    }


    // Menu instructions
    int intSpace = 500;
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


//---------------------------------------------------------------------------PLAYING STATE---------------------------------------------------------------------------//
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

    laserDamageTimer -= deltaTime;
    
    if (player.isLaserBeamActive() && laserDamageTimer <= 0.0f)
    {
        laserDamageTimer = LASER_DAMAGE_INTERVAL;
        
        Vector2 beamOrigin = player.getLaserBeamOrigin();
        Vector2 beamDir    = player.getLaserBeamDir();

        for (int i = 0; i < MAX_ENEMIES; ++i)
        {
            Enemy& e = waveManager.getEnemies()[i];
            if (!e.active) continue;

            float ex   = e.position.x - beamOrigin.x;
            float ey   = e.position.y - beamOrigin.y;
            float proj = ex * beamDir.x + ey * beamDir.y;
            
            if (proj < 0) continue;

            float closestX = beamOrigin.x + beamDir.x * proj;
            float closestY = beamOrigin.y + beamDir.y * proj;
            float distX = e.position.x - closestX;
            float distY = e.position.y - closestY;
            float dist2 = distX * distX + distY * distY;

            if (dist2 < e.radius * e.radius)
            {
                e.health -= LASER_DAMAGE;

                if (e.isDead())
                {
                    e.active = false;
                    game.addScore(e.type == EnemyType::ZOMBIE ? 10 : 30);

                    if (e.type == EnemyType::ZOMBIE)
                    TimeBoostManager::instance().trySpawn(e.position, waveManager.getWave());

                    if (e.type == EnemyType::TANK)
                    {
                        int activeTanks = 0;
                        for (int ti = 0; ti < MAX_ENEMIES; ++ti)
                        if (waveManager.getEnemies()[ti].active && waveManager.getEnemies()[ti].type == EnemyType::TANK)
                            activeTanks++;
                            HealthPackManager::instance().trySpawn(e.position, activeTanks);
                        }
                    }
                }
            }
        }
        
        if (waveManager.update(deltaTime)) game.changeState(GameStateID::GAME_OVER);
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

    bool isLaser = player.getCurrentWeaponType() == WeaponType::LASER;
    Color wpnColor = isLaser ? Color{ 255, 160, 30, 255 } : Color{ 200, 0, 200, 255 };

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

//---------------------------------------------------------------------------PAUSED STATE---------------------------------------------------------------------------//
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

//---------------------------------------------------------------------------GAME OVER STATE---------------------------------------------------------------------------//
void GameOverState::enter(Game& game)
{
    finalScore = game.getScore();
    finalWave = game.getWaveManager().getWave();
    currentSlot = 0;
    submitted = false;
    showScoreboard = false;

    for (int i = 0; i < 4; ++i) 
    {
        letterIndex[i] = 0;
        initials[0] = '\0';
        latestInitials[0] = '\0';
    }

    Scoreboard::instance().load();
    AudioManager::instance().setDucked(false);
    AudioManager::instance().playGameOverMusic();
}


void GameOverState::handleInput(Game& game)
{
    if (!submitted)
    {
        // Cycle letter up
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            letterIndex[currentSlot] = (letterIndex[currentSlot] + 1) % 26;

        // Cycle letter down
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            letterIndex[currentSlot] = (letterIndex[currentSlot] + 25) % 26;

        // Move right
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
            if (currentSlot < 3) currentSlot++;

        // Move left
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
            if (currentSlot > 0) currentSlot--;

        // Confirm initials
        if (IsKeyPressed(KEY_ENTER))
        {
            for (int i = 0; i < 4; ++i) 
            {
                initials[i] = 'A' + letterIndex[i];
                initials[4] = '\0';
                strncpy(latestInitials, initials, 5);
            }

            Scoreboard::instance().addEntry(initials, finalScore, finalWave);
            submitted = true;
        }
    }
    else
    {
        // Toggle scoreboard view
        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_Q))
            showScoreboard = !showScoreboard;

        if (IsKeyPressed(KEY_R))
        {
            game.resetWorld();
            game.changeState(GameStateID::PLAYING);
        }

        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M))
            game.changeState(GameStateID::MAIN_MENU);
    }
}


void GameOverState::draw(Game&)
{
    ClearBackground({ 8, 4, 10, 255 });
    const int yOffset = 150;

    // Red vignette
    for (int i = 0; i < 8; ++i)
    {
        unsigned char alpha = (unsigned char)(30 - i * 3);
        DrawRectangleLinesEx({ (float)i * 12, (float)i * 8, (float)(SCREEN_WIDTH - i * 24), 
                            (float)(SCREEN_HEIGHT - i * 16) }, 12, { 180, 30, 30, alpha });
    }

    // GAME OVER title
    const char* goText = "GAME OVER";
    int siz = 90;
    DrawText(goText, SCREEN_WIDTH / 2 - MeasureText(goText, siz) / 2 + 3, 53 + yOffset, siz, { 80, 0, 0, 200 });
    DrawText(goText, SCREEN_WIDTH / 2 - MeasureText(goText, siz) / 2, 50 + yOffset, siz, { 220, 50, 50, 255 });

    // Score and wave
    const char* statsText = TextFormat("Score: %d     Wave: %d", finalScore, finalWave);
    DrawText(statsText, SCREEN_WIDTH / 2 - MeasureText(statsText, 24) / 2, 155 + yOffset, 24, { 200, 220, 255, 200 });

    // Divider
    DrawRectangle(SCREEN_WIDTH / 2 - 300, 190 + yOffset, 600, 1, { 80, 40, 40, 180 });

    if (!submitted)
    {
        // Initials input section
        const char* enterText = "ENTER YOUR INITIALS";
        DrawText(enterText, SCREEN_WIDTH / 2 - MeasureText(enterText, 28) / 2, 210 + yOffset, 28, { 255, 220, 60, 255 });

        const char* sub = "W/S change letter   A/D move   ENTER confirm";
        DrawText(sub, SCREEN_WIDTH / 2 - MeasureText(sub, 16) / 2, 248 + yOffset, 16, { 140, 160, 200, 180 });

        // Letter slots
        int slotSize = 70;
        int slotGap = 20;
        int totalW = 4 * slotSize + 3 * slotGap;
        int startX = SCREEN_WIDTH / 2 - totalW / 2;
        int slotY = 320 + yOffset;

        for (int i = 0; i < 4; ++i)
        {
            int x = startX + i * (slotSize + slotGap);
            bool sel = (i == currentSlot);

            DrawRectangle(x, slotY, slotSize, slotSize, sel ? Color{ 255, 220, 60, 60 } : Color{ 30, 40, 60, 200 });
            DrawRectangleLines(x, slotY, slotSize, slotSize, sel ? Color{ 255, 220, 60, 255 } : Color{ 80, 100, 140, 200 });

            char letter[2] = { (char)('A' + letterIndex[i]), '\0' };
            int lw = MeasureText(letter, 48);
            DrawText(letter, x + slotSize / 2 - lw / 2, slotY + 11, 48, sel ? Color{ 255, 220, 60, 255 } : Color{ 200, 220, 255, 255 });

            if (sel)
            {
                DrawText("^", x + slotSize / 2 - 8, slotY - 24, 22, { 255, 220, 60, 200 });
                DrawText("v", x + slotSize / 2 - 8, slotY + slotSize + 6, 22, { 255, 220, 60, 200 });
            }
        }
    }
    else if (!showScoreboard)
    {
        // Confirmed initials display
        const char* savedText = TextFormat("Saved as: %s", latestInitials);
        DrawText(savedText, SCREEN_WIDTH / 2 - MeasureText(savedText, 32) / 2, 220 + yOffset, 32, { 255, 220, 60, 255 });

        const char* tabText = "[ TAB ]  View Scoreboard";
        DrawText(tabText, SCREEN_WIDTH / 2 - MeasureText(tabText, 20) / 2, 290 + yOffset, 20, { 140, 180, 220, 200 });
    }
    else
    {
        // Scoreboard view
        const char* lbTitle = "HIGH SCORES";
        DrawText(lbTitle, SCREEN_WIDTH / 2 - MeasureText(lbTitle, 28) / 2, 210 + yOffset, 28, { 255, 220, 60, 255 });

        const Scoreboard& sb = Scoreboard::instance();
        int rowY = 255 + yOffset;

        for (int i = 0; i < sb.getCount(); ++i)
        {
            const ScoreEntry& e = sb.getEntries()[i];
            bool isLatest = (strcmp(e.initials, latestInitials) == 0 && e.score == finalScore);
            Color c = isLatest ? Color{ 255, 220, 60, 255 } : Color{ 180, 200, 220, 220 };
            const char* row = TextFormat("%d.  %s     %d pts     Wave %d", i + 1, e.initials, e.score, e.wave);
            DrawText(row, SCREEN_WIDTH / 2 - MeasureText(row, 22) / 2, rowY, 22, c);
            rowY += 35;
        }
    }

    // Bottom divider
    DrawRectangle(SCREEN_WIDTH / 2 - 300, 430 + yOffset, 600, 1, { 80, 40, 40, 180 });

    // Action buttons — only shown after submitting
    if (submitted)
    {
        DrawText("[ R ]  Play Again",
                 SCREEN_WIDTH / 2 - MeasureText("[ R ]  Play Again", 24) / 2, 450 + yOffset, 24, { 80, 200, 255, 255 });
        DrawText("[ ESC / M ]  Main Menu",
                 SCREEN_WIDTH / 2 - MeasureText("[ ESC / M ]  Main Menu", 20) / 2, 485 + yOffset, 20, { 140, 160, 200, 200 });
    }
    else
    {
        DrawText("Enter your initials to continue",
                 SCREEN_WIDTH / 2 - MeasureText("Enter your initials to continue", 16) / 2, 450 + yOffset, 16, { 100, 120, 150, 160 });
    }
}

