#include "../inc/meteor.hpp"

#include <chrono>
#include <cstdlib>

#include "raylib.h"

#ifdef _WIN32
#include <windows.h>
#elif __linux__ || __APPLE__
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

void DrawProgressBar(int posX, int posY, int width, int height, float progress,
                     Color barColor, Color backgroundColor) {
  DrawRectangle(posX, posY, width, height, backgroundColor);
  int barWidth = (int)(width * progress / 100);
  DrawRectangle(posX, posY, barWidth, height, barColor);
}

bool shouldSpawnEnemies(int frameCount) {
  return ((frameCount / 240) % 2) == 1;
}

int getNewEnemyCount(int currentScore) { return currentScore / 60 + 8; }



int getRandomValue() {
  auto now = std::chrono::high_resolution_clock::now();
  auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(
                   now.time_since_epoch())
                   .count();
  std::srand(nanos);
  double probability = (double)std::rand() / RAND_MAX;

  if (probability < PROBABILITY_FACTOR) {
    return std::rand() % (2 - MIN_VALUE + 1) + MIN_VALUE;
  } else {
    return std::rand() % (MAX_VALUE - 3 + 1) + 3;
  }
}
