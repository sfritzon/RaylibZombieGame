#include "Game.h"
#include "GameStates.h"
#include "BulletPool.h"
#include "AudioManager.h"
#include "Constants.h"
#include "HealthPackManager.h"
#include "TimeBoostManager.h"
#include <raylib.h>


Game::Game() 
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    AudioManager::instance().init();
    initShader();
    initStates();
    changeState(GameStateID::MAIN_MENU);
}


Game::~Game() 
{
    UnloadShader(pixelShader);
    UnloadRenderTexture(renderTarget);
    AudioManager::instance().shutdown();
    CloseWindow();
}


void Game::initStates() 
{
    states[GameStateID::MAIN_MENU] = std::make_unique<MainMenuState>();
    states[GameStateID::PLAYING] = std::make_unique<PlayingState>();
    states[GameStateID::PAUSED] = std::make_unique<PausedState>();
    states[GameStateID::GAME_OVER] = std::make_unique<GameOverState>();
}


void Game::changeState(GameStateID id) 
{
    if (currentState) currentState->exit(*this);
    currentState = states[id].get();
    currentState->enter(*this);
}


void Game::resetWorld() 
{
    score = 0;
    player.reset();
    BulletPool::instance().reset();
    HealthPackManager::instance().reset();
    TimeBoostManager::instance().reset();
    waveManager.reset();
}


void Game::initShader()
{
    renderTarget = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    pixelShader = LoadShader(0, "resources/Shaders/pixelation.fs");
    pixelSizeLoc = GetShaderLocation(pixelShader, "pixelSize");
    SetShaderValue(pixelShader, pixelSizeLoc, &pixelSize, SHADER_UNIFORM_FLOAT);
}

void Game::exit()
{
    shouldExit = true;
}

void Game::run() 
{
    while (!(WindowShouldClose() || shouldExit))
    {
        float deltaTime = GetFrameTime();
        deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;  // Cap deltaTime to prevent spiral of death

        AudioManager::instance().update();

        // Game increases in pixelSize when health goes down

        if (currentState && currentState->getID() == GameStateID::PLAYING)
        {
            float healthRatio = player.getHealth() / player.getMaxHealth();
            pixelSize = 1.0f + (1.0f - healthRatio) * 10.0f; // Scales pixeSize from 1 at full HP to 20 at 0 HP
        }
        else
        {
            pixelSize = 1.0f;
        }

        SetShaderValue(pixelShader, pixelSizeLoc, &pixelSize, SHADER_UNIFORM_FLOAT);

        if (currentState) 
        {
            currentState->handleInput(*this);
            currentState->update(*this, deltaTime);
        }


        BeginTextureMode(renderTarget);

        if (currentState) currentState->draw(*this);

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        BeginShaderMode(pixelShader);

        DrawTextureRec(
            renderTarget.texture,
            { 0, 0, (float) renderTarget.texture.width, 
            -(float)renderTarget.texture.height },
            { 0, 0 },
            WHITE
        );

        EndShaderMode();
        EndDrawing();
    }
}
