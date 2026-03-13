#pragma once

#include "IGameState.h"
#include "Player.h"
#include "WaveManager.h"
#include <memory>
#include <unordered_map>


class Game 
{
public:
    Game();
    ~Game();

    void run();

    // State machine
    void changeState(GameStateID id);

    // World access for states
    Player& getPlayer() { return m_player; }

    WaveManager& getWaveManager() { return m_waveManager; }

    int getScore() const { return m_score; }

    void addScore(int v) { m_score += v;  }

    void resetWorld();

private:
    void initStates();

    Player m_player;
    WaveManager m_waveManager;
    int m_score {0};

    std::unordered_map<GameStateID, std::unique_ptr<IGameState>> m_states;
    IGameState* m_currentState {nullptr};
};
