#pragma once

namespace raylib {
  #include "./raylib.h"
}

enum POWERUP { SPEED_POWERUP = 0, AMMO_POWERUP = 1, SHIELD_POWERUP = 2 };

class powerup {
public:
  powerup(int width, int height, POWERUP type);
  void update();
  POWERUP getType();
  raylib::Rectangle getHitBox();

protected:
  POWERUP power_up;
  raylib::Vector2 position;
};
