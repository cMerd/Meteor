#include "../inc/powerup.hpp"
#include <string>

powerup::powerup(int width, int height, POWERUP type) {
  this->position.x = raylib::GetRandomValue(5, width - 5);
  this->position.y = raylib::GetRandomValue(5, height - 5);
  this->power_up = type;
}

void powerup::update() {
  raylib::Color powerup_color;
  switch (this->power_up) {
  case AMMO_POWERUP: {
    powerup_color = raylib::ORANGE;
    const std::string imagePath =
        std::string(raylib::GetApplicationDirectory()) +
        "/../assets/ammo_powerup.png";
    raylib::Image asset = raylib::LoadImage(imagePath.c_str());
    raylib::Texture2D texture = LoadTextureFromImage(asset);
    DrawTexture(texture, position.x, position.y, raylib::WHITE);
    break;
  }
  case SPEED_POWERUP: {
    powerup_color = raylib::LIME;
    const std::string imagePath =
        std::string(raylib::GetApplicationDirectory()) +
        "/../assets/speed_powerup.png";
    raylib::Image asset = raylib::LoadImage(imagePath.c_str());
    raylib::Texture2D texture = LoadTextureFromImage(asset);
    DrawTexture(texture, position.x, position.y, raylib::WHITE);
    break;
  }
  case SHIELD_POWERUP: {
    powerup_color = raylib::BLUE;
    const std::string imagePath =
        std::string(raylib::GetApplicationDirectory()) +
        "/../assets/shield_powerup.png";
    raylib::Image asset = raylib::LoadImage(imagePath.c_str());
    raylib::Texture2D texture = LoadTextureFromImage(asset);
    DrawTexture(texture, position.x, position.y, raylib::WHITE);
    break;
  }
  default:
    powerup_color = raylib::RED;
    DrawRectangle(position.x, position.y, 20, 20, powerup_color);
    break;
  }
}

raylib::Rectangle powerup::getHitBox() {
  return (raylib::Rectangle){position.x, position.y, 20, 20};
}

POWERUP powerup::getType() { return this->power_up; }
