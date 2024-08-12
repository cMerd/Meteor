#include "../inc/meteor.hpp"

#include <chrono>
#include <cstdlib>

void DrawProgressBar(int posX, int posY, int width, int height, float progress,
                     raylib::Color barColor, raylib::Color backgroundColor) {
  DrawRectangle(posX, posY, width, height, backgroundColor);
  int barWidth = (int)(width * progress / 100);
  DrawRectangle(posX, posY, barWidth, height, barColor);
}

bool shouldSpawnEnemies(int frameCount, int currentScore) {
  return ((int64_t)(frameCount /
                    (int64_t)(240 - (double)((double)currentScore / 100))) %
          2) == 1;
}

unsigned int getNewEnemyCount(int currentScore) {
  return currentScore / 60 + 8;
}

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
