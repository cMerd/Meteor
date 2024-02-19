#include "../inc/powerup.hpp"
#include <raylib.h>
#include <string>

powerup::powerup(int width, int height, POWERUP type) {
  this->position.x = GetRandomValue(5, width - 5);
  this->position.y = GetRandomValue(5, height - 5);
  this->power_up = type;
}

void powerup::update() {
  Color powerup_color;
  switch (this->power_up) {
  case AMMO_POWERUP: {
    powerup_color = ORANGE;
    const std::string imagePath =
        std::string(GetApplicationDirectory()) + "/../assets/ammo_powerup.png";
    Image asset = LoadImage(imagePath.c_str());
    Texture2D texture = LoadTextureFromImage(asset);
    DrawTexture(texture, position.x, position.y, WHITE);
    break;
  }
  case SPEED_POWERUP: {
    powerup_color = LIME;
    const std::string imagePath =
        std::string(GetApplicationDirectory()) + "/../assets/speed_powerup.png";
    Image asset = LoadImage(imagePath.c_str());
    Texture2D texture = LoadTextureFromImage(asset);
    DrawTexture(texture, position.x, position.y, WHITE);
    break;
  }
  case SHIELD_POWERUP: {
    powerup_color = BLUE;
    const std::string imagePath =
        std::string(GetApplicationDirectory()) + "/../assets/shield_powerup.png";
    Image asset = LoadImage(imagePath.c_str());
    Texture2D texture = LoadTextureFromImage(asset);
    DrawTexture(texture, position.x, position.y, WHITE);
    break;
  }
  default:
    powerup_color = RED;
    DrawRectangle(position.x, position.y, 20, 20, powerup_color);
    break;
  }
}

Rectangle powerup::getHitBox() {
  return (Rectangle){position.x, position.y, 20, 20};
}
