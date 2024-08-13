#pragma once

namespace raylib {
  #include "./raylib.h"
};

class bullet {
public:
  bullet(raylib::Rectangle bulletRect, raylib::Texture &bulletTexture);
  bullet();
  void update();
  raylib::Rectangle getHitbox();

private:
  raylib::Rectangle bullet_rect;
  raylib::Texture2D bullet_texture;
};
