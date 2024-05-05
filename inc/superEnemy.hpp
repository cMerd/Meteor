#pragma once

#include "enemy.hpp"

const int superEnemyMaxHP = 3;
const int superEnemyMinHP = 0;

class superEnemy : public enemy {
public:
  superEnemy(int width, int height, float Speed);
  void update(raylib::Vector2 playerPos);
  bool isDead();
  void hit();

private:
  int hp = superEnemyMaxHP;
};
