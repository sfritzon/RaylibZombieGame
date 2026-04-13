#pragma once

#include "IGameState.h"
#include "Player.h"
#include "WaveManager.h"
#include <memory>
#include <unordered_map>
#include <raylib.h>


class Game 
{
public:
    Game();
    ~Game();

    void run();

    // State machine
    void changeState(GameStateID id);

    // World access for states
    Player& getPlayer() { return player; }

    WaveManager& getWaveManager() { return waveManager; }

    int getScore() const { return score; }

    void addScore(int v) { score += v;  }

    void resetWorld();

    float getPixelSize() const { return pixelSize; }

    void exit();

private:
    void initStates();
    void initShader();

    bool shouldExit = false;

    Player player;
    WaveManager waveManager;
    int score {0};

    std::unordered_map<GameStateID, std::unique_ptr<IGameState>> states;
    IGameState* currentState {nullptr};

    RenderTexture2D renderTarget {};
    Shader pixelShader {};
    int pixelSizeLoc {-1};
    float pixelSize {4.0f};
};
