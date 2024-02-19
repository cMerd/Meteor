#include "../inc/smartEnemy.hpp"
#include <string>

smartEnemy::smartEnemy(int width, int height, float Speed)
    : enemy(width, height, Speed) {
  Image img = LoadImage((std::string(GetApplicationDirectory()) + "/../assets/smartMeteor.png").c_str());
  this->texture = LoadTextureFromImage(img);
}

void smartEnemy::update(Vector2 playerPos) {

  if (playerPos.y < position.y) {
    position.y -= speed;
  } else if (playerPos.y > position.y) {
    position.y += speed;
  }

  if (playerPos.x < position.x) {
    position.x -= speed;
  } else if (playerPos.y > position.y) {
    position.x += speed;
  }

  DrawTexture(texture, position.x, position.y, WHITE);
}

