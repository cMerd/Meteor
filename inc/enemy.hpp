#pragma once

#include "raylib.h"

const int enemySize = 30;
const int planetSize = 80;

class enemy {
public:
  enemy(int width, int height, float Speed);
  enemy(int width, int height, float Speed, int enemyWidth, int enemyHeight);
  void update();
  Vector2 getPos();
  Rectangle getHitbox();
  bool outOfScreen();
  float getSpeed();

protected:
  Vector2 position;
  Texture2D texture, bigTexture;
  float yLimit, speed;
  int enemy_width = enemySize, enemy_height = enemySize;
};


