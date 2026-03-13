# Zombie Arena
### Documentation
*Raylib School Project by Sara Fritzon*

>A top-down 2D wave survival shooter built with raylib and C++17. Fight off waves of Zombies and Tanks with two weapons, survive as long as you can!

---
## Table of Contents

1. [Project Overview and Requirements](#1-project-overview-and-requirements)
2. [Gameplay](#2-gameplay)
3. [Controls](#3-controls)
4. [Vector Functions](#4-vector-functions--deep-analysis)
5. [Game Systems](#5-game-systems--implementation-analysis)
6. [Bugs Encountered](#6-bugs-encountered--root-cause-analysis)
7. [Why Vectors and Matrices Matter in Game Development](#7-why-vectors-and-matrices-matter-in-game-development)
8. [Course Reflection](#8-technical-reflection)

---
## 1. Project Overview and Requirements

This document provides a reflection on the development of my Vampire-Survivor-Inspired-Raylib-Zombie-game, built only using Raylib and C++ in Visual Studio Code. The requirements for the game were:
>>**For G:**
> 1. Make use of at least 1 design pattern of your choice
> 2. Have 2 weapons with different firing styles (swap by pressing a button)
> 3. At least one type of enemy
> 4. Endless spawning of enemies
> 5. Enemies walking towards a target
> 6. In-game timer
> 7. Player health
> 8. WASD movement
> 9. Auto shooting in either moving direction or towards mouse location depending on firing style of said weapon

>>**For VG:**
> 1. All of G's requirements
> 2. A minimum of 2 design patterns used
> 3. At least 2 bullet types with different damage values
> 4. At least 2 different enemy types with different health values
> 5. A bit more sophisticated spawning system with intervals and multi-spawning
   
## 2. Gameplay

Enemies spawn in waves that last for 30 seconds. Each wave brings more enemies. Survive all enemies in a wave before the next one begins. The game ends when your health reaches zero or if you don't complete a wave and the timer runs out.

**Zombies** are fast and low health. **Tanks** are slow and heavily armored. Tanks begin spawning from wave 2 and onwards.

---

## 3. Controls

| Key | Action |
|-----|--------|
| `WASD` | Move |
| `Left Mouse Button` | Shoot toward cursor |
| `Q` | Swap weapon |
| `ESC` | Pause / unpause |
| `R` | Reset game |
| `Enter` | Confirm menu selection |
| `W / S` | Navigate menus |

---

## Weapons

| Weapon | Damage | Fire Rate | Bullet Color |
|--------|--------|-----------|--------------|
| Pistol | 20 | Fast (0.18s) | Cyan |
| Sniper | 90 | Slow (0.85s) | Orange |

Swap between them at any time with `Q`. The fire rate bar in the HUD shows when you can shoot again.

---

## HUD

The bottom-left corner shows three stacked elements:

- **Weapon name** and swap hint, colored to match the active weapon's bullet color
- **Fire rate bar** — fills up as the weapon cooldown resets
- **HP bar** — green when healthy, orange when low, red when critical

Score is displayed in the top-right. The wave number and wave timer are centered at the top.

---

## Building

### Requirements

- Linux (tested on Ubuntu 24)
- g++ with C++17 support
- raylib (downloaded automatically by premake on first build)
- premake5 (included in `build/`)

### Build Steps

```bash
cd build
./premake5 gmake
cd ..
make
```

The compiled binary will be at `bin/Debug/ZombieGame`.

### Clean Build

```bash
make clean
cd build && ./premake5 gmake && cd .. && make
```

---

## Project Structure

```
project/
├── build/
│   └── premake5.lua        — build configuration
├── include/
│   ├── Constants.h         — all tunable values (screen size, speeds, damage, etc.)
│   ├── IGameState.h        — state pattern interface
│   ├── Game.h              — central game class
│   ├── GameStates.h        — four concrete states
│   ├── Player.h
│   ├── Weapon.h
│   ├── BulletPool.h        — singleton bullet pool
│   ├── Enemy.h
│   └── WaveManager.h
├── src/
│   ├── main.cpp            — entry point
│   ├── Game.cpp
│   ├── GameStates.cpp
│   ├── Player.cpp
│   ├── Weapon.cpp
│   ├── BulletPool.cpp
│   ├── Enemy.cpp
│   └── WaveManager.cpp
└── README.md
```

---

## Architecture

### State Pattern

The game uses a state machine to keep `main.cpp` lean and separate concerns cleanly. `IGameState` is a pure interface that all states implement. `Game` holds a map of all four states and a pointer to the current one. Each frame it calls `handleInput`, `update`, and `draw` on the active state. Transitions call `exit()` on the old state and `enter()` on the new one.

| State | Entered when |
|-------|-------------|
| `MainMenuState` | Game starts, or returning from game over |
| `PlayingState` | Player starts or resets the game |
| `PausedState` | ESC is pressed during play |
| `GameOverState` | Player health reaches zero |

### Singleton Pattern

`BulletPool` is a singleton accessed via `BulletPool::instance()`. It owns a fixed array of 256 `Bullet` structs. Bullets are never heap allocated — spawning fills the first inactive slot, and deactivating just flips a boolean. This makes it accessible from `Weapon::fire()` without needing to pass references down through `Player → Weapon`.

### Ownership Hierarchy

```
main.cpp
└── Game
    ├── States (MainMenu / Playing / Paused / GameOver)
    ├── Player
    │   ├── Weapon (Pistol)
    │   └── Weapon (Sniper)
    ├── WaveManager
    │   └── Enemy[80]
    └── ──────────────────▶ BulletPool (singleton)
                            └── Bullet[256]
```

States do not own any game data — they borrow references to `Game` and its contents each frame to read and modify them.

---

## Tuning

All gameplay values are in `include/Constants.h`. Change any of these and recompile:

```cpp
// Screen
static const int SCREEN_WIDTH  = 1600;
static const int SCREEN_HEIGHT = 900;

// Player
static const float PLAYER_SPEED      = 220.0f;
static const float PLAYER_MAX_HEALTH = 100.0f;

// Pistol
static const float PISTOL_DAMAGE     = 20.0f;
static const float PISTOL_FIRE_RATE  = 0.18f;

// Sniper
static const float SNIPER_DAMAGE     = 90.0f;
static const float SNIPER_FIRE_RATE  = 0.85f;

// Zombie
static const float ZOMBIE_SPEED      = 80.0f;
static const float ZOMBIE_HEALTH     = 40.0f;

// Tank
static const float TANK_SPEED        = 45.0f;
static const float TANK_HEALTH       = 180.0f;

// Waves
static const float WAVE_DURATION     = 30.0f;
```

---

## Scoring

| Kill | Points |
|------|--------|
| Zombie | 10 |
| Tank | 30 |

Score is tracked in `Game` and displayed in the top-right corner during play. Your final score and wave reached are shown on the game over screen.
