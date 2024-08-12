#include "../inc/enemy.hpp"
#include <string>

enemy::enemy(int width, int height, float Speed) {
  float xPos = raylib::GetRandomValue(0, width) * 1.0f;
  float yPos = raylib::GetRandomValue(-30, 0) * 1.0f;
  this->position = {xPos, yPos};
  this->yLimit = height + 3.0f;
  this->speed = Speed;
  int index = raylib::GetRandomValue(0, 3);
  const std::string filePath = std::string(raylib::GetApplicationDirectory()) +
                               "/../assets/meteor" + std::to_string(index) +
                               ".png";
  raylib::Image img = raylib::LoadImage(filePath.c_str());
  raylib::Texture2D texture_ = LoadTextureFromImage(img);
  this->texture = texture_;

  const std::string filePath2 = std::string(raylib::GetApplicationDirectory()) +
                                "/../assets/bigMeteor.png";
  raylib::Image img2 = raylib::LoadImage(filePath2.c_str());
  raylib::Texture2D texture_2 = LoadTextureFromImage(img2);
  this->bigTexture = texture_2;
}

bool enemy::outOfScreen() { return (position.y >= yLimit); }

void enemy::update() {
  if (this->outOfScreen()) {
    return;
  }

  position.y += speed;
  if (enemy_width != enemySize)
    DrawTexture(bigTexture, position.x, position.y, raylib::WHITE);
  else {
    DrawTexture(texture, position.x, position.y, raylib::WHITE);
  }
}

raylib::Rectangle enemy::getHitbox() {
  return (raylib::Rectangle){position.x, position.y, 1.0f * enemy_width,
                             1.0f * enemy_height};
}

float enemy::getSpeed() { return this->speed; }

enemy::enemy(int width, int height, float Speed, int enemyWidth,
             int enemyHeight) {
  float xPos = raylib::GetRandomValue(0, width) * 1.0f;
  this->position = {xPos, 0.0f};
  this->yLimit = height + 3.0f;
  this->speed = Speed;
  this->enemy_height = enemyHeight;
  this->enemy_width = enemyWidth;
}

raylib::Vector2 enemy::getPos() { return this->position; }
