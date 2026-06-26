#pragma once


// Screen
static const int SCREEN_WIDTH  = 1600;
static const int SCREEN_HEIGHT = 900;
static const char* WINDOW_TITLE = "Zombie Arena";

// Player
static const float PLAYER_SPEED = 225.0f;
static const float PLAYER_MAX_HEALTH = 100.0f;
static const float PLAYER_RADIUS = 20.0f;

// Weapons
static const float PISTOL_DAMAGE = 20.0f;
static const float PISTOL_FIRE_RATE = 0.18f; // Timing between shots
static const float PISTOL_BULLET_SPEED = 650.0f;
static const float PISTOL_BULLET_RADIUS = 5.0f;

static const float LASER_DAMAGE = 45.0f;
static const float LASER_DAMAGE_INTERVAL = 0.1f;
static const float LASER_FIRE_RATE = 6.0f;
static const float LASER_DURATION = 3.0f;
static const float LASER_WIDTH = 4.0f;

// Bullet pool
static const int MAX_BULLETS = 256;
static const float BULLET_LIFETIME = 2.5f;

// Enemies
static const float ZOMBIE_SPEED = 80.0f;
static const float ZOMBIE_HEALTH = 40.0f;
static const float ZOMBIE_DAMAGE = 10.0f;
static const float ZOMBIE_RADIUS = 16.0f;

static const float TANK_SPEED = 50.0f;
static const float TANK_HEALTH = 180.0f;
static const float TANK_DAMAGE = 25.0f;
static const float TANK_RADIUS = 26.0f;

static const float ENEMY_ATTACK_INTERVAL = 0.8f; // Seconds between damage

// Waves
static const float WAVE_DURATION = 30.0f;
static const int MAX_ENEMIES = 100;

// Spawning
static const float SPAWN_MARGIN = 60.0f;