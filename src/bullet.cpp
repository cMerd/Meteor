#include "../inc/bullet.hpp"

bullet::bullet(raylib::Rectangle bulletRect, raylib::Texture2D &bulletTexture) {
  this->bullet_rect = bulletRect;
  this->bullet_texture = bulletTexture;
}

bullet::bullet() {}

void bullet::update(bool slowDown) {
  bullet_rect.y -= (slowDown ? 6.67f : 10.0f);
  if (bullet_rect.y > 0) {
    DrawTexture(bullet_texture, bullet_rect.x, bullet_rect.y, raylib::WHITE);
  }
}

raylib::Rectangle bullet::getHitbox() { return this->bullet_rect; }
