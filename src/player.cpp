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
  checkForGamepad();
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

std::optional<int> player::getGamepad() {
  if (gamepadAvailable) {
    return gamepadID;
  }
  return {};
}

void player::update(bool force, bool force2, raylib::Sound bulletSound,
                    bool forceSound, bool forceShield, bool increaseSpeed,
                    bool increaseAmmo, bool increaseSpeedMore,
                    bool reloadAmmoFaster, bool slowDown) {

  checkBullet(force2, bulletSound, forceSound, increaseAmmo, reloadAmmoFaster);
  for (bullet &ammo : ammos) {
    ammo.update(slowDown);
  }
  float changeValue = checkSprint(2.0f, force);
  if (increaseSpeed) {
    changeValue *= 1.3f;
  }
  if (increaseSpeedMore) {
    changeValue *= 1.2f;
  }
  if (slowDown) {
    changeValue /= 1.5f;
  }

  if (IsKeyDown(raylib::KEY_RIGHT) or IsKeyDown(raylib::KEY_D) or
      raylib::IsGamepadButtonDown(gamepadID,
                                  raylib::GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
    player_pos.x += changeValue;
  if (IsKeyDown(raylib::KEY_LEFT) or IsKeyDown(raylib::KEY_A) or
      raylib::IsGamepadButtonDown(gamepadID,
                                  raylib::GAMEPAD_BUTTON_LEFT_FACE_LEFT))
    player_pos.x -= changeValue;
  if (IsKeyDown(raylib::KEY_UP) or IsKeyDown(raylib::KEY_W) or
      raylib::IsGamepadButtonDown(gamepadID,
                                  raylib::GAMEPAD_BUTTON_LEFT_FACE_UP))
    player_pos.y -= changeValue;
  if (IsKeyDown(raylib::KEY_DOWN) or IsKeyDown(raylib::KEY_S) or
      raylib::IsGamepadButtonDown(gamepadID,
                                  raylib::GAMEPAD_BUTTON_LEFT_FACE_DOWN))
    player_pos.y += changeValue;

  if (gamepadAvailable) {
    player_pos.y += changeValue * GetGamepadAxisMovement(
                                      gamepadID, raylib::GAMEPAD_AXIS_LEFT_Y);
    player_pos.x += changeValue * GetGamepadAxisMovement(
                                      gamepadID, raylib::GAMEPAD_AXIS_LEFT_X);
  }

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
                         bool increase, bool reloadFaster) {
  if (force) {
    if (ammoCharge < 100.0f) {
      ammoCharge +=
          25.0f * (increase ? 1.3f : 1.0f) * (reloadFaster ? 1.5f : 1.0f);
      ammoCharge = std::min(100.0f, ammoCharge);
    }
    if (raylib::IsKeyDown(raylib::KEY_ENTER) or
        raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON) or
        raylib::IsGamepadButtonDown(gamepadID,
                                    raylib::GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
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
    if (raylib::IsKeyDown(raylib::KEY_ENTER) or
        raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON) or
        raylib::IsGamepadButtonDown(gamepadID,
                                    raylib::GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
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
      ammoCharge += 1.0f * (reloadFaster ? 1.5f : 1.0f);
      ammoCharge = std::min(100.0f, ammoCharge);
    }
  }
}

float player::checkSprint(float changeVal, bool force) {
  if (force) {
    changeVal += 4.0f;
    charge = 100.0f;
  } else {
    float axisChange = -1.0f;
    if (gamepadAvailable) {
      axisChange = (GetGamepadAxisMovement(gamepadID,
                                           raylib::GAMEPAD_AXIS_RIGHT_TRIGGER) +
                    1) /
                   2.0f;
    }
    if (raylib::IsKeyDown(raylib::KEY_LEFT_SHIFT) or
        raylib::IsKeyDown(raylib::KEY_RIGHT_SHIFT) or axisChange > 0.0f) {
      if (charge > 0) {
        float change = 2.0f;
        if (axisChange > 0.0f) {
          change *= axisChange;
        }
        changeVal += change;
        charge -= change;
      }
      charge = std::max(charge, 0.0f);
      charge = std::min(charge, 100.0f);
    } else {
      if (charge != 100.0f)
        charge += 1.0f;
      charge = std::max(charge, 0.0f);
      charge = std::min(charge, 100.0f);
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

void player::checkForGamepad() {
  gamepadAvailable = false;
  for (gamepadID = 0; raylib::IsGamepadAvailable(gamepadID); gamepadID++) {
    if (raylib::TextIsEqual(raylib::GetGamepadName(gamepadID),
                            PS4_WIRED_CONTROLLER_NAME) or
        raylib::TextIsEqual(raylib::GetGamepadName(gamepadID),
                            PS4V2_WIRED_CONTROLLER_NAME) or
        raylib::TextIsEqual(raylib::GetGamepadName(gamepadID),
                            PS4_WIRELESS_CONTROLLER_NAME)) {
      gamepadAvailable = true;
      break;
    }
  }
}
