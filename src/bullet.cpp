#include "../inc/bullet.hpp"
#include <raylib.h>
#include <string>

bullet::bullet(Rectangle bulletRect, Color bulletColor) {
  this->bullet_rect = bulletRect;
  this->bullet_color = bulletColor;
}

bullet::bullet() {}

void bullet::update() {
  bullet_rect.y -= 10.0f;
  if (bullet_rect.y > 0) {
    const std::string imagePath = std::string(GetApplicationDirectory()) + "/../assets/bullet.png";
    Image img = LoadImage(imagePath.c_str());
    Texture2D texture = LoadTextureFromImage(img);
    DrawTexture(texture, bullet_rect.x, bullet_rect.y, WHITE);
  }
}

Rectangle bullet::getHitbox() { return this->bullet_rect; }

