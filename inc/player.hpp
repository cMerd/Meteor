#pragma once

#include "bullet.hpp"

class player {
public:
  player(raylib::Vector2 playerPos, raylib::Color playerColor, int screenWidth,
         int screenHeight);
  void update(bool force, bool force2, raylib::Sound bulletSound, bool forceSound,
              bool forceShield);
  void noCheckUpdate();
  float getCharge();
  float getAmmo();
  bullet *getBullet();
  raylib::Vector2 getPos();
  int getGamepadId();

private:
  void checkPosition();
  void checkBullet(bool force, raylib::Sound bSound, bool forceSound);
  float checkSprint(float changeVal, bool force);

  int frameCount = 0, index = 0;
  float charge = 100.0f, ammoCharge = 0.0f;
  raylib::Vector2 player_pos;
  raylib::Color player_color;
  int screen_width, screen_height;
  bullet ammo;
  raylib::Image playerSprite;
  raylib::Texture2D playerTexture;
  int gamepad = 1;
};
