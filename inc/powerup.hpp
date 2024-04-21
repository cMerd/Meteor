#pragma once

#include "raylib.h"

enum POWERUP { SPEED_POWERUP = 0, AMMO_POWERUP = 1, SHIELD_POWERUP = 2 };

class powerup {
public:
  powerup(int width, int height, POWERUP type);
  void update();
  POWERUP getType();
  Rectangle getHitBox();

protected:
  POWERUP power_up;
  Vector2 position;
};
