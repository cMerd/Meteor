#pragma once

#include "../inc/powerup.hpp"
#include <optional>
#include <utility>
#include <vector>
#include <string>

namespace raylib {
#include "./raylib.h"
}

const int MIN_VALUE = 2;
const int MAX_VALUE = 6;
const double PROBABILITY_FACTOR = 0.6;

void DrawProgressBar(int posX, int posY, int width, int height, float progress,
                     raylib::Color barColor, raylib::Color backgroundColor);

void DrawLevelBar(int level);

void DrawPowerUpSign(
    const std::vector<std::pair<POWERUP, std::optional<int>>> &types,
    int maxTime);

void DrawSlowMoSign(unsigned int timeLeft, unsigned int maxTime);

int getRandomValue();
unsigned int getNewEnemyCount(int currentScore);
bool shouldSpawnEnemies(int frameCount, int currentScore);

std::string formatTime(double seconds);

bool isSlowMoStarted();

enum MENU {
  MAIN_MENU = 0,
  GAME = 1,
  GAME_OVER = 2,
  SHOP_MENU = 3,
  SPEED_UPGRADE_MENU = 4,
  AMMO_UPGRADE_MENU = 5,
  SHIELD_UPGRADE_MENU = 6,
  SLOWMO_UPGRADE_MENU = 7,
  GAME_SELECTION_MENU = 8,
  TUTORIAL = 9,
  TIME_RACE = 10,
  RACE_OVER = 11
};

struct powerUpgrades {
  int speedLevel;
  int ammoLevel;
  int shieldLevel;
  int slowMoLevel;
};
