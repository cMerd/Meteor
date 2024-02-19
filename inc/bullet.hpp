#pragma once

#include "raylib.h"

class bullet {
public:
  bullet(Rectangle bulletRect, Color bulletColor);
  bullet();
  void update();
  Rectangle getHitbox();

private:
  Rectangle bullet_rect;
  Color bullet_color;
};
