#pragma once

#include "bullet.hpp"
#include <array>
#include <vector>

class player {
public:
  player(raylib::Vector2 playerPos, raylib::Color playerColor, int screenWidth,
         int screenHeight);
  void update(bool force, bool force2, raylib::Sound bulletSound,
              bool forceSound, bool forceShield, bool increaseSpeed,
              bool increaseAmmo, bool increaseSpeedMore, bool reloadAmmoFaster);
  void noCheckUpdate();
  float getCharge();
  float getAmmo();
  std::vector<bullet> &getBullet();
  raylib::Vector2 getPos();
  void destroy();

private:
  void checkPosition();
  void checkBullet(bool force, raylib::Sound bSound, bool forceSound,
                   bool increase, bool reloadFaster);
  float checkSprint(float changeVal, bool force);

  int frameCount = 0, index = 0;
  float charge = 100.0f, ammoCharge = 0.0f;
  raylib::Vector2 player_pos;
  raylib::Color player_color;
  int screen_width, screen_height;
  std::vector<bullet> ammos;
  raylib::Texture2D bulletTexture;
  std::array<raylib::Texture2D, 4> playerTextures;
  std::array<raylib::Texture2D, 4> playerwShieldTextures;
};
