#include "Game.h"
#include "GameStates.h"
#include "BulletPool.h"
#include "AudioManager.h"
#include "Constants.h"
#include "raylib.h"


Game::Game() 
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    AudioManager::instance().init();
    initStates();
    changeState(GameStateID::MAIN_MENU);
}


Game::~Game() 
{
    AudioManager::instance().shutdown();
    CloseWindow();
}


void Game::initStates() 
{
    m_states[GameStateID::MAIN_MENU] = std::make_unique<MainMenuState>();
    m_states[GameStateID::PLAYING] = std::make_unique<PlayingState>();
    m_states[GameStateID::PAUSED] = std::make_unique<PausedState>();
    m_states[GameStateID::GAME_OVER] = std::make_unique<GameOverState>();
}


void Game::changeState(GameStateID id) 
{
    if (m_currentState) m_currentState->exit(*this);
    m_currentState = m_states[id].get();
    m_currentState->enter(*this);
}


void Game::resetWorld() 
{
    m_score = 0;
    m_player.reset();
    BulletPool::instance().reset();
    m_waveManager.reset();
}


void Game::run() 
{
    while (!WindowShouldClose()) 
    {
        float deltaTime = GetFrameTime();
        deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;  // Cap deltaTime to prevent spiral of death

        AudioManager::instance().update();

        if (m_currentState) 
        {
            m_currentState->handleInput(*this);
            m_currentState->update(*this, deltaTime);
        }

        BeginDrawing();
        if (m_currentState) m_currentState->draw(*this);
        EndDrawing();
    }
}
