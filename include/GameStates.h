#pragma once

#include "IGameState.h"


class MainMenuState : public IGameState 
{
public:
    void enter(Game& game) override;
    void handleInput(Game& game) override;
    void update(Game& game, float dt) override;
    void draw(Game& game) override;
    GameStateID getID() const override { return GameStateID::MAIN_MENU; }

private:
    int m_selected {0}; // 0 = Start, 1 = Quit
    float m_anim {0};
};


class PlayingState : public IGameState 
{
public:
    void enter(Game& game) override;
    void handleInput(Game& game) override;
    void update(Game& game, float dt) override;
    void draw(Game& game) override;
    GameStateID getID() const override { return GameStateID::PLAYING; }

private:
    void updateBulletEnemyCollisions(Game& game);
    void drawHUD(Game& game) const;
};


class PausedState : public IGameState 
{
public:
    void enter(Game& game) override;
    void exit(Game& game) override;
    void handleInput(Game& game) override;
    void update(Game& game, float dt) override {}
    void draw(Game& game) override;
    GameStateID getID() const override { return GameStateID::PAUSED; }

private:
    int m_selected {0}; // 0 = Continue, 1 = Quit
};


class GameOverState : public IGameState 
{
public:
    void enter(Game& game) override;
    void handleInput(Game& game) override;
    void update(Game& game, float dt) override {}
    void draw(Game& game) override;
    GameStateID getID() const override { return GameStateID::GAME_OVER; }
    
private:
    int m_finalScore {0};
    int m_finalWave  {0};
};
