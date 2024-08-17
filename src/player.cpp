#include "../inc/player.hpp"
#include <algorithm>
#include <string>

player::player(raylib::Vector2 playerPos, raylib::Color playerColor,
               int screenWidth, int screenHeight) {
  this->player_pos = playerPos;
  this->player_color = playerColor;
  this->screen_width = screenWidth;
  this->screen_height = screenHeight;
  this->ammos = {};
  for (size_t i = 0; i < playerTextures.size(); i++) {
    playerTextures[i] =
        raylib::LoadTexture((std::string(raylib::GetApplicationDirectory()) +
                             "../assets/player" + std::to_string(i) + ".png")
                                .c_str());
  }
  for (size_t i = 0; i < playerwShieldTextures.size(); i++) {
    playerwShieldTextures[i] =
        raylib::LoadTexture((std::string(raylib::GetApplicationDirectory()) +
                             "../assets/shield-on" + std::to_string(i) + ".png")
                                .c_str());
  }
  bulletTexture = raylib::LoadTexture(
      (std::string(raylib::GetApplicationDirectory()) + "/../assets/bullet.png")
          .c_str());
}

void player::destroy() {
  for (size_t i = 0; i < playerTextures.size(); i++) {
    raylib::UnloadTexture(playerTextures[i]);
  }
  for (size_t i = 0; i < playerwShieldTextures.size(); i++) {
    raylib::UnloadTexture(playerwShieldTextures[i]);
  }
  raylib::UnloadTexture(bulletTexture);
}

void player::update(bool force, bool force2, raylib::Sound bulletSound,
                    bool forceSound, bool forceShield, bool increaseSpeed,
                    bool increaseAmmo, bool increaseSpeedMore) {

  checkBullet(force2, bulletSound, forceSound, increaseAmmo);
  float changeValue = checkSprint(2.0f, force);
  if (increaseSpeed) {
    changeValue *= 1.3f;
  }
  if (increaseSpeedMore) {
    changeValue *= 1.2f;
  }

  if (IsKeyDown(raylib::KEY_RIGHT) or IsKeyDown(raylib::KEY_D))
    player_pos.x += changeValue;
  if (IsKeyDown(raylib::KEY_LEFT) or IsKeyDown(raylib::KEY_A))
    player_pos.x -= changeValue;
  if (IsKeyDown(raylib::KEY_UP) or IsKeyDown(raylib::KEY_W))
    player_pos.y -= changeValue;
  if (IsKeyDown(raylib::KEY_DOWN) or IsKeyDown(raylib::KEY_S))
    player_pos.y += changeValue;

  checkPosition();

  if (frameCount == 15) {
    frameCount = 0;
    index++;
  }
  if (index == 4) {
    index = 0;
  }

  DrawTexture(
      (forceShield ? playerwShieldTextures[index] : playerTextures[index]),
      player_pos.x - 23, player_pos.y - 23, raylib::WHITE);
  frameCount++; // will be called every frame (60 fps)
}

float player::getCharge() { return this->charge; }
float player::getAmmo() { return this->ammoCharge; }

void player::checkPosition() {
  if (player_pos.y > screen_height) {
    player_pos.y -= screen_height;
  } else if (player_pos.y < 0) {
    player_pos.y += screen_height;
  }

  if (player_pos.x > screen_width) {
    player_pos.x -= screen_width;
  } else if (player_pos.x < 0) {
    player_pos.x += screen_width;
  }
}

void player::checkBullet(bool force, raylib::Sound bSound, bool forceSound,
                         bool increase) {
  if (force) {
    if (ammoCharge < 100.0f) {
      ammoCharge += 25.0f * (increase ? 1.3f : 1.0f);
      ammoCharge = std::min(100.0f, ammoCharge);
    }
    if (raylib::IsKeyDown(raylib::KEY_F) or
        raylib::IsKeyDown(raylib::KEY_ENTER) or
        raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON)) {
      if (ammoCharge >= 100.0f) {
        if (forceSound)
          PlaySound(bSound);
        ammos.push_back(
            bullet((raylib::Rectangle){player_pos.x, player_pos.y, 20, 20},
                   bulletTexture));
        ammoCharge = 0.0f;
      }
    }
  } else {
    if (raylib::IsKeyDown(raylib::KEY_F) or
        raylib::IsKeyDown(raylib::KEY_ENTER) or
        raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON)) {
      if (ammoCharge >= 100.0f) {
        if (forceSound)
          PlaySound(bSound);
        ammos.push_back(
            bullet((raylib::Rectangle){player_pos.x, player_pos.y, 20, 20},
                   bulletTexture));
        ammoCharge = 0.0f;
      }
    }
    if (ammoCharge < 100.0f) {
      ammoCharge += 1.0f;
      ammoCharge = std::min(100.0f, ammoCharge);
    }
  }
  for (bullet &ammo : ammos) {
    ammo.update();
  }
}

float player::checkSprint(float changeVal, bool force) {
  if (force) {
    changeVal += 4.0f;
    charge = 100.0f;
  } else {
    if (raylib::IsKeyDown(raylib::KEY_LEFT_SHIFT) or
        raylib::IsKeyDown(raylib::KEY_RIGHT_SHIFT)) {
      if (charge > 0) {
        changeVal += 2.0f;
        charge -= 2.0f;
      }
      charge = std::max(charge, 0.0f);
    } else {
      if (charge != 100.0f)
        charge += 1.0f;
      charge = std::max(charge, 0.0f);
    }
  }
  return changeVal;
}

std::vector<bullet> &player::getBullet() { return this->ammos; }

raylib::Vector2 player::getPos() { return this->player_pos; }

void player::noCheckUpdate() {
  if (frameCount == 15) {
    frameCount = 0;
    index++;
  }
  if (index == 4) {
    index = 0;
  }

  raylib::DrawTexture(playerTextures[index], player_pos.x - 20,
                      player_pos.y - 20, raylib::WHITE);
  frameCount++; // will be called every frame (60 fps)
}
