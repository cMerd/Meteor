#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../inc/button.hpp"
#include "../inc/enemy.hpp"
#include "../inc/meteor.hpp"
#include "../inc/player.hpp"
#include "../inc/powerup.hpp"
#include "../inc/smartEnemy.hpp"
#include "../inc/superEnemy.hpp"

#include "raylib.h"

int getData(const std::string &filePath);
bool setData(const std::string &filePath, int highScore);

int main() {

  const int screenWidth = 1000;
  const int screenHeight = 800;
  const int animFrameCount = 30;
  const int powerupFrameCount = 300;
  const std::string storageDir =
      std::string(GetApplicationDirectory()) + "/../data/";
  const std::string highScoreFilePath = storageDir + "high_score";
  const std::string soundOptionFilePath = storageDir + "sound";
  const Color backgroundColor = {8, 8, 33, 0};

  if (!std::filesystem::exists(storageDir)) {
    std::filesystem::create_directory(storageDir);
  }

  MENU currentMenu = MAIN_MENU;
  player p({500.0f, 400.0f}, RED, screenWidth, screenHeight);
  unsigned int frameCount = 0, currentScore = 0, powerUpFrameCounter = 0;
  std::vector<enemy> enemies;
  std::vector<superEnemy> superEnemies;
  std::vector<smartEnemy> smartEnemies;
  std::vector<powerup> speedPowerups;
  std::vector<powerup> ammoPowerups;
  std::vector<powerup> shieldPowerups;
  bool forceSprint = false, forceAmmo = false, forceShield = false,
       forceSound = getData(soundOptionFilePath);

  SetTraceLogLevel(LOG_NONE);
  InitWindow(screenWidth, screenHeight, "Meteor");
  InitAudioDevice();
  SetTargetFPS(60);
  Sound buttonSound = LoadSound(
      (std::string(GetApplicationDirectory()) + "/../assets/button.wav")
          .c_str());

  Sound bulletSound = LoadSound(
      (std::string(GetApplicationDirectory()) + "/../assets/bullet.wav")
          .c_str());

  Sound killSounds[4] = {LoadSound((std::string(GetApplicationDirectory()) +
                                    "/../assets/kill1.wav")
                                       .c_str()),
                         LoadSound((std::string(GetApplicationDirectory()) +
                                    "/../assets/kill2.wav")
                                       .c_str()),
                         LoadSound((std::string(GetApplicationDirectory()) +
                                    "/../assets/kill3.wav")
                                       .c_str()),
                         LoadSound((std::string(GetApplicationDirectory()) +
                                    "/../assets/kill4.wav")
                                       .c_str())};
  Sound powerUpSounds[2] = {LoadSound((std::string(GetApplicationDirectory()) +
                                       "/../assets/powerUp1.wav")
                                          .c_str()),
                            LoadSound((std::string(GetApplicationDirectory()) +
                                       "/../assets/powerUp2.wav")
                                          .c_str())};
  Music backgroundMusic =
      LoadMusicStream((std::string(GetApplicationDirectory()) +
                       "/../assets/backgroundMusic.wav")
                          .c_str());
  PlayMusicStream(backgroundMusic);

  while (!WindowShouldClose()) {

    UpdateMusicStream(backgroundMusic);

    if (GetMusicTimePlayed(backgroundMusic) >=
            GetMusicTimeLength(backgroundMusic) &&
        forceSound) {
      StopMusicStream(backgroundMusic);
      PlayMusicStream(backgroundMusic);
    }

    if (!forceSound) {
      StopMusicStream(backgroundMusic);
    }

    BeginDrawing();

    ClearBackground(backgroundColor);

    if (currentMenu == MAIN_MENU) {

      std::string highScoreText =
          "High Score: " + std::to_string(getData(highScoreFilePath));

      DrawText("Meteor", 350, 200, 80, WHITE);
      for (int i = 280; i <= 285; i++)
        DrawLine(300, i, 700, i, RED);
      DrawText(highScoreText.c_str(), 50, 50, 50, WHITE);

      button startButton((Rectangle){300, 500, 400, 100}, BLACK, "Start",
                         RAYWHITE, 60, 400, 525, RED);
      button quitButton((Rectangle){300, 620, 400, 100}, BLACK, "Quit",
                        RAYWHITE, 60, 420, 645, RED);
      button soundButton((forceSound ? std::string(GetApplicationDirectory()) +
                                           "/../assets/sound-on.png"
                                     : std::string(GetApplicationDirectory()) +
                                           "/../assets/sound-off.png"),
                         (forceSound ? std::string(GetApplicationDirectory()) +
                                           "/../assets/sound-on-hover.png"
                                     : std::string(GetApplicationDirectory()) +
                                           "/../assets/sound-off-hover.png"),
                         (Rectangle){50, 720, 50, 50});

      if (startButton.isClicked()) {
        if (forceSound) {
          PlaySound(buttonSound);
        }
        currentMenu = GAME;
      } else if (quitButton.isClicked()) {
        break;
      } else if (soundButton.isClicked()) {
        forceSound = !forceSound;
        if (forceSound) {
          PlayMusicStream(backgroundMusic);
        }
        setData(soundOptionFilePath, forceSound);
      }
    } else if (currentMenu == GAME) {

      frameCount += 1;

      if (((frameCount / 240) % 2) == 1) {

        for (unsigned int i = 0; i < 8 + currentScore / 60; i++) {
          enemy Enemy(screenWidth, screenHeight, (float)getRandomValue());
          enemies.push_back(Enemy);
        }

        int key = GetRandomValue(1, 10);
        if (key == 7 or key == 1 or key == 9) {
          enemy Planet(screenWidth, screenHeight, 3, planetSize, planetSize);
          enemies.push_back(Planet);
        }

        key = GetRandomValue(1, 5);
        if (key == 2) {
          superEnemy sEnemy(screenWidth, screenHeight, (float)getRandomValue());
          superEnemies.push_back(sEnemy);
        }

        key = GetRandomValue(1, 10);
        if (key == 5) {
          smartEnemy smEnemy(screenWidth, screenHeight, GetRandomValue(3, 4));
          smartEnemies.push_back(smEnemy);
        }

        key = GetRandomValue(1, 20);
        if (key == 5) {
          powerup Power(screenWidth, screenHeight, SPEED_POWERUP);
          speedPowerups.push_back(Power);
        } else if (key == 6) {
          powerup Power(screenWidth, screenHeight, AMMO_POWERUP);
          ammoPowerups.push_back(Power);
        } else if (key == 1) {
          powerup Power(screenWidth, screenHeight, SHIELD_POWERUP);
          shieldPowerups.push_back(Power);
        }

        frameCount = 0;
      }

      for (size_t i = 0; i < enemies.size(); i++) {
        enemy &Enemy = enemies[i];
        if (Enemy.outOfScreen()) {
          enemies.erase(enemies.begin() + i);
          continue;
        }
        Rectangle enemyHitbox = Enemy.getHitbox();
        Rectangle bulletHitBox = p.getBullet()->getHitbox();
        if (CheckCollisionRecs(enemyHitbox, bulletHitBox) and
            enemyHitbox.width == enemySize) {
          currentScore += Enemy.getSpeed();
          enemies.erase(enemies.begin() + i);
          auto b = p.getBullet();
          *b = bullet({-100, -100, 1, 1}, WHITE);
          if (forceSound) {
            int i = GetRandomValue(0, 3);
            PlaySound(killSounds[i]);
          }
          continue;
        } else if (CheckCollisionRecs(enemyHitbox, bulletHitBox) and
                   enemyHitbox.width == planetSize) {
          auto b = p.getBullet();
          *b = bullet({-100, -100, 1, 1}, WHITE);
          continue;
        }
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, enemyHitbox)) {
          if (!forceShield) {
            if (forceSound) {
              int i = GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            frameCount = 0;
            currentMenu = GAME_OVER;
          } else {
            if (forceSound) {
              int i = GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            forceShield = false;
            enemies.erase(enemies.begin() + i);
          }
        }
        Enemy.update();
      }

      for (size_t i = 0; i < superEnemies.size(); i++) {
        superEnemy &sEnemy = superEnemies[i];
        if (sEnemy.outOfScreen()) {
          superEnemies.erase(superEnemies.begin() + i);
          continue;
        }
        Rectangle enemyHitbox = sEnemy.getHitbox();
        Rectangle bulletHitBox = p.getBullet()->getHitbox();
        if (CheckCollisionRecs(enemyHitbox, bulletHitBox)) {
          auto b = p.getBullet();
          *b = bullet({-100, -100, 1, 1}, WHITE);
          sEnemy.hit();
          if (sEnemy.isDead()) {
            currentScore += sEnemy.getSpeed() * 3;
            superEnemies.erase(superEnemies.begin() + i);
            if (forceSound) {
              int i = GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            continue;
          }
          int i = GetRandomValue(0, 3);
          PlaySound(killSounds[i]);
        }
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, enemyHitbox)) {
          if (!forceShield) {
            if (forceSound) {
              int i = GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            frameCount = 0;
            currentMenu = GAME_OVER;
          } else {
            if (forceSound) {
              int i = GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            forceShield = false;
            superEnemies.erase(superEnemies.begin() + i);
          }
        }
        sEnemy.update(p.getPos());
      }

      for (size_t i = 0; i < smartEnemies.size(); i++) {
        smartEnemy &smEnemy = smartEnemies[i];
        if (smEnemy.outOfScreen()) {
          smartEnemies.erase(smartEnemies.begin() + i);
          continue;
        }
        Rectangle enemyHitbox = smEnemy.getHitbox();
        Rectangle bulletHitBox = p.getBullet()->getHitbox();
        if (CheckCollisionRecs(enemyHitbox, bulletHitBox)) {
          auto b = p.getBullet();
          *b = bullet({-100, -100, 1, 1}, WHITE);
          currentScore += smEnemy.getSpeed() * 5;
          smartEnemies.erase(smartEnemies.begin() + i);
          if (forceSound) {
            int i = GetRandomValue(0, 3);
            PlaySound(killSounds[i]);
          }
          continue;
        }
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, enemyHitbox)) {
          if (!forceShield) {
            if (forceSound) {
              int i = GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            frameCount = 0;
            currentMenu = GAME_OVER;
          } else {
            if (forceSound) {
              int i = GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            smartEnemies.erase(smartEnemies.begin() + i);
            forceShield = false;
          }
        }
        smEnemy.update(p.getPos());
      }

      for (size_t i = 0; i < speedPowerups.size(); i++) {
        powerup &Power = speedPowerups[i];
        Rectangle powerHitbox = Power.getHitBox();
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, powerHitbox)) {
          forceSprint = true;
          powerUpFrameCounter = powerupFrameCount;
          speedPowerups.erase(speedPowerups.begin() + i);
          if (forceSound) {
            int i = GetRandomValue(0, 1);
            PlaySound(powerUpSounds[i]);
          }
          continue;
        }
        Power.update();
      }

      for (size_t i = 0; i < ammoPowerups.size(); i++) {
        powerup &Power = ammoPowerups[i];
        Rectangle powerHitbox = Power.getHitBox();
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, powerHitbox)) {
          forceAmmo = true;
          powerUpFrameCounter = powerupFrameCount;
          ammoPowerups.erase(ammoPowerups.begin() + i);
          if (forceSound) {
            int i = GetRandomValue(0, 1);
            PlaySound(powerUpSounds[i]);
          }
          continue;
        }
        Power.update();
      }

      for (size_t i = 0; i < shieldPowerups.size(); i++) {
        powerup &Power = shieldPowerups[i];
        Rectangle powerHitbox = Power.getHitBox();
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, powerHitbox)) {
          forceShield = true;
          shieldPowerups.erase(shieldPowerups.begin() + i);
          if (forceSound) {
            int i = GetRandomValue(0, 1);
            PlaySound(powerUpSounds[i]);
          }
          continue;
        }
        Power.update();
      }

      if (powerUpFrameCounter == 0) {
        forceSprint = false;
        forceAmmo = false;
      } else {
        powerUpFrameCounter--;
      }

      p.update(forceSprint, forceAmmo, bulletSound, forceSound, forceShield);
      std::string scoreText = "Score: " + std::to_string(currentScore);

      DrawProgressBar(30, 30, 300, 30, p.getCharge(), BLUE, WHITE);
      DrawProgressBar(30, 90, 300, 30, p.getAmmo(), YELLOW, WHITE);
      DrawText(scoreText.c_str(), 30, 150, 50, WHITE);
    } else if (currentMenu == GAME_OVER) {
      std::string scoreText = "Score: " + std::to_string(currentScore);

      unsigned int highScore = getData(highScoreFilePath);
      highScore = std::max(highScore, currentScore);
      std::string highScoreText = "High Score: " + std::to_string(highScore);

      setData(highScoreFilePath, highScore);

      DrawText("Game Over!", 300, 200, 80, WHITE);
      for (int i = 280; i <= 285; i++)
        DrawLine(280, i, 750, i, RED);
      DrawText(scoreText.c_str(), 30, 50, 50, WHITE);
      DrawText(highScoreText.c_str(), 30, 100, 50, WHITE);

      button retryButton((Rectangle){300, 500, 400, 100}, BLACK, "Retry",
                         RAYWHITE, 60, 390, 525, RED);
      button menuButton((Rectangle){300, 620, 400, 100}, BLACK, "Go to menu",
                        RAYWHITE, 60, 350, 645, RED);

      if (retryButton.isClicked()) {
        if (forceSound)
          PlaySound(buttonSound);
        p = player({500.0f, 400.0f}, RED, screenWidth, screenHeight);
        enemies.clear();
        superEnemies.clear();
        smartEnemies.clear();
        speedPowerups.clear();
        ammoPowerups.clear();
        shieldPowerups.clear();
        frameCount = 0;
        currentMenu = GAME;
        currentScore = 0;
        forceAmmo = false;
        forceSprint = false;
        powerUpFrameCounter = 0;
      } else if (menuButton.isClicked()) {
        if (forceSound)
          PlaySound(buttonSound);
        p = player({500.0f, 400.0f}, RED, screenWidth, screenHeight);
        enemies.clear();
        superEnemies.clear();
        smartEnemies.clear();
        speedPowerups.clear();
        ammoPowerups.clear();
        shieldPowerups.clear();
        frameCount = 0;
        currentMenu = MAIN_MENU;
        currentScore = 0;
        forceAmmo = false;
        forceSprint = false;
        powerUpFrameCounter = 0;
      }

      frameCount += 1;
      if (frameCount % animFrameCount * 2 >= animFrameCount) {
        p.noCheckUpdate();
      } else if (frameCount == animFrameCount) {
        frameCount = 0;
      }
    }

    EndDrawing();
  }

  UnloadSound(buttonSound);
  UnloadSound(bulletSound);
  for (int i = 0; i < 4; i++) {
    UnloadSound(killSounds[i]);
  }
  for (int i = 0; i < 2; i++) {
    UnloadSound(powerUpSounds[i]);
  }
  UnloadMusicStream(backgroundMusic);
  CloseAudioDevice();
  CloseWindow();

  return 0;
}

int getData(const std::string &filePath) {
  std::string highScoreBuff;
  std::ifstream highScoreFile(filePath, std::ios::in | std::ios::out);
  if (!highScoreFile.is_open()) {
    std::ofstream highScoreFile(filePath, std::ios::in | std::ios::out);
    return 1;
  }
  std::getline(highScoreFile, highScoreBuff);

  unsigned int highScore = 0;
  if (!highScoreBuff.empty())
    highScore = std::stoi(highScoreBuff);

  return highScore;
}

bool setData(const std::string &filePath, int highScore) {
  std::ofstream highScoreFile(filePath, std::ios::out | std::ios::trunc);
  if (!highScoreFile.is_open()) {
    return false;
  }
  highScoreFile << highScore;
  highScoreFile.close();
  return true;
}
