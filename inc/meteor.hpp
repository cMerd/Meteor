#pragma once

#include "../inc/powerup.hpp"
#include <optional>
#include <utility>
#include <vector>

namespace raylib {
#include "./raylib.h"
}

const int MIN_VALUE = 2;
const int MAX_VALUE = 6;
const double PROBABILITY_FACTOR = 0.6;

void DrawProgressBar(int posX, int posY, int width, int height, float progress,
                     raylib::Color barColor, raylib::Color backgroundColor);

void DrawPowerUpSign(
    const std::vector<std::pair<POWERUP, std::optional<int>>> &types,
    int maxTime);

int getRandomValue();
unsigned int getNewEnemyCount(int currentScore);
bool shouldSpawnEnemies(int frameCount, int currentScore);

enum MENU { MAIN_MENU = 0, GAME = 1, GAME_OVER = 2 };
