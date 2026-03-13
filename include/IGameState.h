#pragma once


enum class GameStateID 
{
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};


class Game;


class IGameState 
{
public:
    virtual ~IGameState() = default;
    virtual void enter(Game& game) {}
    virtual void exit(Game& game) {}
    virtual void handleInput(Game& game) = 0;
    virtual void update(Game& game, float dt) = 0;
    virtual void draw(Game& game) = 0;
    virtual GameStateID getID() const = 0;
};
