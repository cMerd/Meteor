#pragma once

#include "bullet.hpp"
#include <array>
#include <optional>
#include <vector>

class player {
public:
  player(raylib::Vector2 playerPos, raylib::Color playerColor, int screenWidth,
         int screenHeight);
  void update(bool force, bool force2, raylib::Sound bulletSound,
              bool forceSound, bool forceShield, bool increaseSpeed,
              bool increaseAmmo, bool increaseSpeedMore, bool reloadAmmoFaster,
              bool slowDown);
  void noCheckUpdate();
  float getCharge();
  float getAmmo();
  std::vector<bullet> &getBullet();
  raylib::Vector2 getPos();
  void destroy();
  void checkForGamepad();
  std::optional<int> getGamepad();

private:
  void checkPosition();
  void checkBullet(bool force, raylib::Sound bSound, bool forceSound,
                   bool increase, bool reloadFaster);
  float checkSprint(float changeVal, bool force);

  bool gamepadAvailable = false;
  int frameCount = 0, index = 0, gamepadID = 0;
  float charge = 100.0f, ammoCharge = 0.0f;
  raylib::Vector2 player_pos;
  raylib::Color player_color;
  int screen_width, screen_height;
  std::vector<bullet> ammos;
  raylib::Texture2D bulletTexture;
  std::array<raylib::Texture2D, 4> playerTextures;
  std::array<raylib::Texture2D, 4> playerwShieldTextures;

  static constexpr const char *PS4_WIRELESS_CONTROLLER_NAME =
      "Wireless Controller";
  static constexpr const char *PS4_WIRED_CONTROLLER_NAME =
      "Sony Interactive Entertainment Wireless Controller";
  static constexpr const char *PS4V2_WIRED_CONTROLLER_NAME =
      "Sony Computer Entertainment Wireless Controller";
};
