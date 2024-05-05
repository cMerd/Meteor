#pragma once

namespace raylib {
  #include "./raylib.h"
};

class bullet {
public:
  bullet(raylib::Rectangle bulletRect, raylib::Color bulletColor);
  bullet();
  void update();
  raylib::Rectangle getHitbox();

private:
  raylib::Rectangle bullet_rect;
  raylib::Color bullet_color;
};
