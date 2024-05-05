#include "../inc/bullet.hpp"
#include <string>

bullet::bullet(raylib::Rectangle bulletRect, raylib::Color bulletColor) {
  this->bullet_rect = bulletRect;
  this->bullet_color = bulletColor;
}

bullet::bullet() {}

void bullet::update() {
  bullet_rect.y -= 10.0f;
  if (bullet_rect.y > 0) {
    const std::string imagePath =
        std::string(raylib::GetApplicationDirectory()) +
        "/../assets/bullet.png";
    raylib::Image img = raylib::LoadImage(imagePath.c_str());
    raylib::Texture2D texture = LoadTextureFromImage(img);
    DrawTexture(texture, bullet_rect.x, bullet_rect.y, raylib::WHITE);
  }
}

raylib::Rectangle bullet::getHitbox() { return this->bullet_rect; }
