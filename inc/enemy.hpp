#pragma once

namespace raylib {
#include "./raylib.h"
}

const int enemySize = 30;
const int planetSize = 80;

class enemy {
public:
  enemy(int width, int height, float Speed);
  enemy(int width, int height, float Speed, int enemyWidth, int enemyHeight);
  void update();
  raylib::Vector2 getPos();
  raylib::Rectangle getHitbox();
  bool outOfScreen();
  float getSpeed();

protected:
  raylib::Vector2 position;
  raylib::Texture2D texture, bigTexture;
  float yLimit, speed;
  int enemy_width = enemySize, enemy_height = enemySize;
};
