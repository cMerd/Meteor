#pragma once

namespace raylib {
  #include "./raylib.h"
}

const int MIN_VALUE = 2;
const int MAX_VALUE = 6;
const double PROBABILITY_FACTOR = 0.6;

void DrawProgressBar(int posX, int posY, int width, int height, float progress,
                     raylib::Color barColor, raylib::Color backgroundColor);

int getRandomValue();
unsigned int getNewEnemyCount(int currentScore);
bool shouldSpawnEnemies(int frameCount);

enum MENU { MAIN_MENU = 0, GAME = 1, GAME_OVER = 2 };
