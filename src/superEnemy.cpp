#include "../inc/superEnemy.hpp"
#include <raylib.h>
#include <stdexcept>

superEnemy::superEnemy(int width, int height, float Speed)
    : enemy(width, height, Speed) {
  Image img = LoadImage((std::string(GetApplicationDirectory()) + "/../assets/superMeteor.png").c_str());
  this->texture = LoadTextureFromImage(img);
}

void superEnemy::update(Vector2 playerPos) {
  if (this->outOfScreen()) {
    return;
  }

  if (playerPos.y < position.y) {
    position.y -= speed;
  } else if (playerPos.y > position.y) {
    position.y += speed;
  }

  DrawTexture(texture, position.x, position.y, WHITE);
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
