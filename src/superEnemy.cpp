#include "../inc/superEnemy.hpp"
#include <stdexcept>

superEnemy::superEnemy(int width, int height, float Speed)
    : enemy(width, height, Speed) {
  raylib::Image img =
      raylib::LoadImage((std::string(raylib::GetApplicationDirectory()) +
                         "/../assets/superMeteor.png")
                            .c_str());
  this->texture = LoadTextureFromImage(img);
}

void superEnemy::update(raylib::Vector2 playerPos, bool slowDown) {
  if (this->outOfScreen()) {
    return;
  }

  if (playerPos.y < position.y) {
    position.y -= (slowDown ? speed / 3.0f : speed);
  } else if (playerPos.y > position.y) {
    position.y += (slowDown ? speed / 3.0f : speed);
  }

  DrawTexture(texture, position.x, position.y, raylib::WHITE);
}

void superEnemy::hit() {
  if (this->hp <= superEnemyMinHP) {
    throw std::logic_error(
        "ERRROR: superEnemy.hit(): Super Enemy is already dead.");
    return;
  }
  this->hp--;
}

bool superEnemy::isDead() { return (this->hp <= superEnemyMinHP); }
