#include "../inc/player.hpp"
#include <algorithm>
#include <raylib.h>
#include <string>

player::player(Vector2 playerPos, Color playerColor, int screenWidth,
               int screenHeight) {
  this->player_pos = playerPos;
  this->player_color = playerColor;
  this->screen_width = screenWidth;
  this->screen_height = screenHeight;
  ammo = bullet((Rectangle){-1, -1, 20, 20}, YELLOW);
}

void player::update(bool force, bool force2, Sound bulletSound, bool forceSound, bool forceShield) {

  checkBullet(force2, bulletSound, forceSound);
  float changeValue = checkSprint(2.0f, force);

  if (IsKeyDown(KEY_RIGHT) or IsKeyDown(KEY_D))
    player_pos.x += changeValue;
  if (IsKeyDown(KEY_LEFT) or IsKeyDown(KEY_A))
    player_pos.x -= changeValue;
  if (IsKeyDown(KEY_UP) or IsKeyDown(KEY_W))
    player_pos.y -= changeValue;
  if (IsKeyDown(KEY_DOWN) or IsKeyDown(KEY_S))
    player_pos.y += changeValue;

  checkPosition();

  if (frameCount == 15) {
    frameCount = 0;
    index++;
  }
  if (index == 4) {
    index = 0;
  }

  std::string assetPath;
  if (!forceShield)
    assetPath = std::string(GetApplicationDirectory()) + "/../assets/player" + std::to_string(index) + ".png";
  else
    assetPath = std::string(GetApplicationDirectory()) + "/../assets/shield-on" + std::to_string(index) + ".png";
   

  playerSprite = LoadImage(assetPath.c_str());

  playerTexture = LoadTextureFromImage(playerSprite);
  DrawTexture(playerTexture, player_pos.x - 20, player_pos.y - 20, WHITE); 
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

void player::checkBullet(bool force, Sound bSound, bool forceSound) {
  if (force) {
    ammoCharge = 100.0f;
    if (IsKeyDown(KEY_F) or IsKeyDown(KEY_ENTER) or
        IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      if (ammoCharge == 100.0f) {
        if (forceSound)
          PlaySound(bSound);
        ammo = bullet((Rectangle){player_pos.x, player_pos.y, 20, 20}, YELLOW);
      }
    }
  } else {
    if (IsKeyDown(KEY_F) or IsKeyDown(KEY_ENTER) or
        IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      if (ammoCharge == 100.0f) {
        if (forceSound)
          PlaySound(bSound);
        ammo = bullet((Rectangle){player_pos.x, player_pos.y, 20, 20}, YELLOW);
        ammoCharge = 0.0f;
      }
    }
    if (ammoCharge < 100.0f) {
      ammoCharge += 1.0f;
    }
  }
  ammo.update();
}

float player::checkSprint(float changeVal, bool force) {
  if (force) {
    changeVal += 4.0f;
    charge = 100.0f;
  } else {
    if (IsKeyDown(KEY_LEFT_SHIFT) or IsKeyDown(KEY_RIGHT_SHIFT)) {
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

bullet *player::getBullet() { return &this->ammo; }

Vector2 player::getPos() { return this->player_pos; }

void player::noCheckUpdate() {
  if (frameCount == 15) {
    frameCount = 0;
    index++;
  }
  if (index == 4) {
    index = 0;
  }

  std::string assetPath = std::string(GetApplicationDirectory()) + "/../assets/player" + std::to_string(index) + ".png";

  playerSprite = LoadImage(assetPath.c_str());

  playerTexture = LoadTextureFromImage(playerSprite);
  DrawTexture(playerTexture, player_pos.x - 20, player_pos.y - 20, WHITE); 
  frameCount++; // will be called every frame (60 fps)
}
