#include "../inc/meteor.hpp"

#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <string>

void DrawProgressBar(int posX, int posY, int width, int height, float progress,
                     raylib::Color barColor, raylib::Color backgroundColor) {
  DrawRectangle(posX, posY, width, height, backgroundColor);
  DrawRectangle(posX, posY, (int)(width * progress / 100), height, barColor);
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

// types - a vector of pair storing type of the powerup and time left for that
// powerup (optional)
void DrawPowerUpSign(
    const std::vector<std::pair<POWERUP, std::optional<int>>> &types,
    int maxTime) {
  static raylib::Texture2D shieldIcon = raylib::LoadTexture(
      std::string((std::string(raylib::GetApplicationDirectory()) +
                   "/../assets/shield_icon.png"))
          .c_str());
  static raylib::Texture2D speedIcon = raylib::LoadTexture(
      std::string((std::string(raylib::GetApplicationDirectory()) +
                   "/../assets/speed_icon.png"))
          .c_str());
  static raylib::Texture2D ammoIcon = raylib::LoadTexture(
      std::string((std::string(raylib::GetApplicationDirectory()) +
                   "/../assets/ammo_icon.png"))
          .c_str());

  float xPos = 50.0f;
  for (const auto &[type, time] : types) {
    switch (type) {
    case SHIELD_POWERUP:
      raylib::DrawTexture(shieldIcon, xPos, raylib::GetScreenHeight() - 110.0f,
                          raylib::WHITE);
      break;
    case SPEED_POWERUP: {
      float percentageDone = 100 - ((float)time.value() * 100 / maxTime);
      if (percentageDone < 50 or (int) percentageDone % 8 == 0) {
        raylib::DrawTexture(speedIcon, xPos, raylib::GetScreenHeight() - 110.0f,
                            raylib::WHITE);
      }
      break;
    }
    case AMMO_POWERUP: {
      float percentageDone = 100 - ((float)time.value() * 100 / maxTime);
      if (percentageDone < 50 or (int) percentageDone % 8 == 0) {
        raylib::DrawTexture(ammoIcon, xPos, raylib::GetScreenHeight() - 110.0f,
                            raylib::WHITE);
      }
      break;
    }
    default:
      throw std::logic_error("Invalid powerup type.");
      break;
    }
    xPos += 80.0f;
  }
}

void DrawLevelBar(int level) {
  raylib::DrawRectangleRoundedLines({50, 350, 50, 201}, 0.3f, 1, 2.0f,
                                    raylib::WHITE);

  raylib::DrawRectangle(
      50, 484, 50, 67,
      (level >= 1 ? raylib::GREEN : raylib::Color{0, 144, 54, 158}));

  raylib::DrawRectangle(
      50, 417, 50, 67,
      (level >= 2 ? raylib::GREEN : raylib::Color{0, 144, 54, 158}));

  raylib::DrawRectangle(
      50, 350, 50, 67,
      (level >= 3 ? raylib::GREEN : raylib::Color{0, 144, 54, 158}));

  for (float i = 1; i <= 2; i++) {
    for (float j = 0; j <= 5; j++) {
      raylib::DrawLine(50, 350 + i * 67 + j, 100, 350 + i * 67 + j,
                       raylib::WHITE);
    }
  }
}
