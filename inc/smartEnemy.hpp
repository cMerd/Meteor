#pragma once

#include "enemy.hpp"

class smartEnemy : public enemy {
public:
  smartEnemy(int width, int height, float Speed);
  void update(Vector2 playerPos);
};
