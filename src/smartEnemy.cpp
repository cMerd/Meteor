#include "../inc/smartEnemy.hpp"
#include <string>

smartEnemy::smartEnemy(int width, int height, float Speed)
    : enemy(width, height, Speed) {
  raylib::Image img =
      raylib::LoadImage((std::string(raylib::GetApplicationDirectory()) +
                         "/../assets/smartMeteor.png")
                            .c_str());
  this->texture = LoadTextureFromImage(img);
}

void smartEnemy::update(raylib::Vector2 playerPos, bool slowDown) {

  if (playerPos.y < position.y) {
    position.y -= (slowDown ? speed / 3.0f : speed);
  } else if (playerPos.y > position.y) {
    position.y += (slowDown ? speed / 3.0f : speed);
  }

  if (playerPos.x < position.x) {
    position.x -= (slowDown ? speed / 3.0f : speed);
  } else if (playerPos.y > position.y) {
    position.x += (slowDown ? speed / 3.0f : speed);
  }

  DrawTexture(texture, position.x, position.y, raylib::WHITE);
}
