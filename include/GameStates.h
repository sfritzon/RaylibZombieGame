#pragma once

#include "IGameState.h"


class MainMenuState : public IGameState 
{
public:
    void enter(Game& game) override;
    void handleInput(Game& game) override;
    void update(Game& game, float deltaTime) override;
    void draw(Game& game) override;
    GameStateID getID() const override { return GameStateID::MAIN_MENU; }

private:
    int selected {0}; // 0 = Start, 1 = Quit
    float anim {0};
    float confirmTimer {0};
    int confirmedOption {-1};
};


class PlayingState : public IGameState 
{
public:
    void enter(Game& game) override;
    void handleInput(Game& game) override;
    void update(Game& game, float deltaTime) override;
    void draw(Game& game) override;
    GameStateID getID() const override { return GameStateID::PLAYING; }


private:
    void updateBulletEnemyCollisions(Game& game);
    void drawHUD(Game& game) const;
    float laserDamageTimer {0.0f};
};


class PausedState : public IGameState 
{
public:
    void enter(Game& game) override;
    void exit(Game& game) override;
    void handleInput(Game& game) override;
    void update(Game& game, float deltaTime) override {}
    void draw(Game& game) override;
    GameStateID getID() const override { return GameStateID::PAUSED; }

private:
    int selected {0}; // 0 = Continue, 1 = Quit
};


class GameOverState : public IGameState 
{
public:
    void enter(Game& game) override;
    void handleInput(Game& game) override;
    void update(Game& game, float deltaTime) override {}
    void draw(Game& game) override;
    GameStateID getID() const override { return GameStateID::GAME_OVER; }
    
private:
    int finalScore {0};
    int finalWave  {0};
    char initials[5] {0};
    int currentSlot {0};
    int letterIndex[4] {0};
    bool submitted {false};
    bool showScoreboard {false};
    char latestInitials[5] {0};
};