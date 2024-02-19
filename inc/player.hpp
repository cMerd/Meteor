#pragma once

#include "bullet.hpp"
#include "raylib.h"


class player {
public:
  player(Vector2 playerPos, Color playerColor, int screenWidth,
         int screenHeight);
  void update(bool force, bool force2, Sound bulletSound, bool forceSound, bool forceShield);
  void noCheckUpdate();
  float getCharge();
  float getAmmo();
  bullet *getBullet();
  Vector2 getPos();

private:
  void checkPosition();
  void checkBullet(bool force, Sound bSound, bool forceSound);
  float checkSprint(float changeVal, bool force);

  int frameCount = 0, index = 0;
  float charge = 100.0f, ammoCharge = 0.0f;
  Vector2 player_pos;
  Color player_color;
  int screen_width, screen_height;
  bullet ammo;
  Image playerSprite;
  Texture2D playerTexture;
};
