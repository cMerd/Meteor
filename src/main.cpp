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

int getData(const std::string &filePath);
bool setData(const std::string &filePath, int highScore);

int main() {

  const int screenWidth = 1000;
  const int screenHeight = 800;
  const int animFrameCount = 30;
  const int powerupFrameCount = 300;
  const std::string storageDir =
      std::string(raylib::GetApplicationDirectory()) + "/../data/";
  const std::string highScoreFilePath = storageDir + "high_score";
  const std::string soundOptionFilePath = storageDir + "sound";
  const raylib::Color backgroundColor = {8, 8, 33, 0};

  if (!std::filesystem::exists(storageDir)) {
    std::filesystem::create_directory(storageDir);
  }

  MENU currentMenu = MAIN_MENU;
  player p({500.0f, 400.0f}, raylib::RED, screenWidth, screenHeight);
  unsigned int frameCount = 0, currentScore = 0, powerUpFrameCounter = 0;
  std::vector<enemy> enemies;
  std::vector<superEnemy> superEnemies;
  std::vector<smartEnemy> smartEnemies;
  std::vector<powerup> speedPowerups;
  std::vector<powerup> ammoPowerups;
  std::vector<powerup> shieldPowerups;
  std::vector<powerup> powerups;
  bool forceSprint = false, forceAmmo = false, forceShield = false,
       forceSound = getData(soundOptionFilePath);

  raylib::SetTraceLogLevel(raylib::LOG_NONE);
  raylib::InitWindow(screenWidth, screenHeight, "Meteor");
  raylib::InitAudioDevice();
  raylib::SetTargetFPS(60);
  raylib::Sound buttonSound = raylib::LoadSound(
      (std::string(raylib::GetApplicationDirectory()) + "/../assets/button.wav")
          .c_str());

  raylib::Sound bulletSound = raylib::LoadSound(
      (std::string(raylib::GetApplicationDirectory()) + "/../assets/bullet.wav")
          .c_str());

  raylib::Sound killSounds[4] = {
      raylib::LoadSound((std::string(raylib::GetApplicationDirectory()) +
                         "/../assets/kill1.wav")
                            .c_str()),
      raylib::LoadSound((std::string(raylib::GetApplicationDirectory()) +
                         "/../assets/kill2.wav")
                            .c_str()),
      raylib::LoadSound((std::string(raylib::GetApplicationDirectory()) +
                         "/../assets/kill3.wav")
                            .c_str()),
      raylib::LoadSound((std::string(raylib::GetApplicationDirectory()) +
                         "/../assets/kill4.wav")
                            .c_str())};
  raylib::Sound powerUpSounds[2] = {
      raylib::LoadSound((std::string(raylib::GetApplicationDirectory()) +
                         "/../assets/powerUp1.wav")
                            .c_str()),
      raylib::LoadSound((std::string(raylib::GetApplicationDirectory()) +
                         "/../assets/powerUp2.wav")
                            .c_str())};
  raylib::Music backgroundMusic =
      raylib::LoadMusicStream((std::string(raylib::GetApplicationDirectory()) +
                               "/../assets/backgroundMusic.wav")
                                  .c_str());
  raylib::PlayMusicStream(backgroundMusic);

  while (!raylib::WindowShouldClose()) {

    raylib::UpdateMusicStream(backgroundMusic);

    if (raylib::GetMusicTimePlayed(backgroundMusic) >=
            raylib::GetMusicTimeLength(backgroundMusic) &&
        forceSound) {
      raylib::StopMusicStream(backgroundMusic);
      raylib::PlayMusicStream(backgroundMusic);
    }

    if (!forceSound) {
      raylib::StopMusicStream(backgroundMusic);
    }

    raylib::BeginDrawing();

    raylib::ClearBackground(backgroundColor);

    if (currentMenu == MAIN_MENU) {

      std::string highScoreText =
          "High Score: " + std::to_string(getData(highScoreFilePath));

      DrawText("Meteor", 350, 200, 80, raylib::WHITE);
      for (int i = 280; i <= 285; i++)
        DrawLine(300, i, 700, i, raylib::RED);
      DrawText(highScoreText.c_str(), 50, 50, 50, raylib::WHITE);

      button startButton((raylib::Rectangle){300, 500, 400, 100}, raylib::BLACK,
                         "Start", raylib::RAYWHITE, 60, 400, 525, raylib::RED);
      button quitButton((raylib::Rectangle){300, 620, 400, 100}, raylib::BLACK,
                        "Quit", raylib::RAYWHITE, 60, 420, 645, raylib::RED);
      button soundButton(
          (forceSound ? std::string(raylib::GetApplicationDirectory()) +
                            "/../assets/sound-on.png"
                      : std::string(raylib::GetApplicationDirectory()) +
                            "/../assets/sound-off.png"),
          (forceSound ? std::string(raylib::GetApplicationDirectory()) +
                            "/../assets/sound-on-hover.png"
                      : std::string(raylib::GetApplicationDirectory()) +
                            "/../assets/sound-off-hover.png"),
          (raylib::Rectangle){50, 720, 50, 50});

      if (startButton.isClicked()) {
        if (forceSound) {
          raylib::PlaySound(buttonSound);
        }
        currentMenu = GAME;
      } else if (quitButton.isClicked()) {
        break;
      } else if (soundButton.isClicked()) {
        forceSound = !forceSound;
        if (forceSound) {
          raylib::PlayMusicStream(backgroundMusic);
        }
        setData(soundOptionFilePath, forceSound);
      }
    } else if (currentMenu == GAME) {

      frameCount += 1;

      if (shouldSpawnEnemies(frameCount, currentScore)) {

        for (unsigned int i = 0; i < getNewEnemyCount(currentScore); i++) {
          enemy Enemy(screenWidth, screenHeight, (float)getRandomValue());
          enemies.push_back(Enemy);
        }

        int key = raylib::GetRandomValue(1, 10);
        if (key == 7 or key == 1 or key == 9) {
          enemy Planet(screenWidth, screenHeight, 3, planetSize, planetSize);
          enemies.push_back(Planet);
        }

        key = raylib::GetRandomValue(1, 5);
        if (key == 2) {
          superEnemy sEnemy(screenWidth, screenHeight, (float)getRandomValue());
          superEnemies.push_back(sEnemy);
        }

        key = raylib::GetRandomValue(1, 10);
        if (key == 5) {
          smartEnemy smEnemy(screenWidth, screenHeight,
                             raylib::GetRandomValue(3, 4));
          smartEnemies.push_back(smEnemy);
        }

        key = raylib::GetRandomValue(1, 20);
        if (key == 5) {
          powerup Power(screenWidth, screenHeight, SPEED_POWERUP);
          // speedPowerups.push_back(Power);
          powerups.push_back(Power);
        } else if (key == 6) {
          powerup Power(screenWidth, screenHeight, AMMO_POWERUP);
          powerups.push_back(Power);
          // ammoPowerups.push_back(Power);
        } else if (key == 1) {
          powerup Power(screenWidth, screenHeight, SHIELD_POWERUP);
          powerups.push_back(Power);
          // shieldPowerups.push_back(Power);
        }

        frameCount = 0;
      }

      for (size_t i = 0; i < enemies.size(); i++) {
      enemy_loop_continue:
        enemy &Enemy = enemies[i];
        if (Enemy.outOfScreen()) {
          enemies.erase(enemies.begin() + i);
          continue;
        }
        raylib::Rectangle enemyHitbox = Enemy.getHitbox();
        // for (bullet &ammo : p.getBullet()) {
        for (size_t j = 0; j < p.getBullet().size(); j++) {
          bullet &ammo = p.getBullet()[j];
          raylib::Rectangle bulletHitBox = ammo.getHitbox();
          if (CheckCollisionRecs(enemyHitbox, bulletHitBox) and
              enemyHitbox.width == enemySize) {
            //*b = bullet({-100, -100, 1, 1}, raylib::WHITE);
            p.getBullet().erase(p.getBullet().begin() + j);
            currentScore += Enemy.getSpeed() * 5;
            enemies.erase(enemies.begin() + i);
            if (forceSound) {
              int audio = raylib::GetRandomValue(0, 3);
              PlaySound(killSounds[audio]);
            }
            i++;
            goto enemy_loop_continue;
          } else if (CheckCollisionRecs(enemyHitbox, bulletHitBox) and
                     enemyHitbox.width == planetSize) {
            p.getBullet().erase(p.getBullet().begin() + j);
            goto enemy_loop_continue;
          }
        }
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, enemyHitbox)) {
          if (!forceShield) {
            if (forceSound) {
              int i = raylib::GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            frameCount = 0;
            currentMenu = GAME_OVER;
          } else {
            if (forceSound) {
              int i = raylib::GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            forceShield = false;
            enemies.erase(enemies.begin() + i);
          }
        }
        Enemy.update();
      }

      for (size_t i = 0; i < superEnemies.size(); i++) {
      super_enemy_loop_continue:
        superEnemy &sEnemy = superEnemies[i];
        if (sEnemy.outOfScreen()) {
          superEnemies.erase(superEnemies.begin() + i);
          continue;
        }
        raylib::Rectangle enemyHitbox = sEnemy.getHitbox();
        // for (bullet &ammo : p.getBullet()) {
        for (size_t j = 0; j < p.getBullet().size(); j++) {
          bullet &ammo = p.getBullet()[j];
          raylib::Rectangle bulletHitBox = ammo.getHitbox();
          if (CheckCollisionRecs(enemyHitbox, bulletHitBox)) {
            //*b = bullet({-100, -100, 1, 1}, raylib::WHITE);
            p.getBullet().erase(p.getBullet().begin() + j);
            currentScore += sEnemy.getSpeed() * 5;
            superEnemies.erase(superEnemies.begin() + i);
            if (forceSound) {
              int audio = raylib::GetRandomValue(0, 3);
              PlaySound(killSounds[audio]);
            }
            goto super_enemy_loop_continue;
          }
        }
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, enemyHitbox)) {
          if (!forceShield) {
            if (forceSound) {
              int i = raylib::GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            frameCount = 0;
            currentMenu = GAME_OVER;
          } else {
            if (forceSound) {
              int i = raylib::GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            forceShield = false;
            superEnemies.erase(superEnemies.begin() + i);
          }
        }
        sEnemy.update(p.getPos());
      }

      for (size_t i = 0; i < smartEnemies.size(); i++) {
      smart_enemy_loop_continue:
        smartEnemy &smEnemy = smartEnemies[i];
        if (smEnemy.outOfScreen()) {
          smartEnemies.erase(smartEnemies.begin() + i);
          continue;
        }
        raylib::Rectangle enemyHitbox = smEnemy.getHitbox();
        // for (bullet &ammo : p.getBullet()) {
        for (size_t j = 0; j < p.getBullet().size(); j++) {
          bullet &ammo = p.getBullet()[j];
          raylib::Rectangle bulletHitBox = ammo.getHitbox();
          if (CheckCollisionRecs(enemyHitbox, bulletHitBox)) {
            auto b = p.getBullet();
            //*b = bullet({-100, -100, 1, 1}, raylib::WHITE);
            p.getBullet().erase(p.getBullet().begin() + j);
            currentScore += smEnemy.getSpeed() * 5;
            smartEnemies.erase(smartEnemies.begin() + i);
            if (forceSound) {
              int audio = raylib::GetRandomValue(0, 3);
              PlaySound(killSounds[audio]);
            }
            goto smart_enemy_loop_continue;
          }
        }
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, enemyHitbox)) {
          if (!forceShield) {
            if (forceSound) {
              int i = raylib::GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            frameCount = 0;
            currentMenu = GAME_OVER;
          } else {
            if (forceSound) {
              int i = raylib::GetRandomValue(0, 3);
              PlaySound(killSounds[i]);
            }
            smartEnemies.erase(smartEnemies.begin() + i);
            forceShield = false;
          }
        }
        smEnemy.update(p.getPos());
      }

      for (size_t i = 0; i < powerups.size(); i++) {
        powerup &Power = powerups[i];
        raylib::Rectangle powerHitbox = Power.getHitBox();
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, powerHitbox)) {
          switch (Power.getType()) {
          case SPEED_POWERUP:
            forceSprint = true;
            powerUpFrameCounter = powerupFrameCount;
            break;
          case AMMO_POWERUP:
            forceAmmo = true;
            powerUpFrameCounter = powerupFrameCount;
            break;
          case SHIELD_POWERUP:
            forceShield = true;
            break;
          default:
            break;
          }
          powerups.erase(powerups.begin() + i);
          if (forceSound) {
            int soundEffectId = raylib::GetRandomValue(0, 1);
            PlaySound(powerUpSounds[soundEffectId]);
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

      DrawProgressBar(30, 30, 300, 30, p.getCharge(), raylib::BLUE,
                      raylib::WHITE);
      DrawProgressBar(30, 90, 300, 30, p.getAmmo(), raylib::YELLOW,
                      raylib::WHITE);
      DrawText(scoreText.c_str(), 30, 150, 50, raylib::WHITE);
    } else if (currentMenu == GAME_OVER) {
      std::string scoreText = "Score: " + std::to_string(currentScore);

      unsigned int highScore = getData(highScoreFilePath);
      highScore = std::max(highScore, currentScore);
      std::string highScoreText = "High Score: " + std::to_string(highScore);

      setData(highScoreFilePath, highScore);

      DrawText("Game Over!", 300, 200, 80, raylib::WHITE);
      for (int i = 280; i <= 285; i++)
        DrawLine(280, i, 750, i, raylib::RED);
      DrawText(scoreText.c_str(), 30, 50, 50, raylib::WHITE);
      DrawText(highScoreText.c_str(), 30, 100, 50, raylib::WHITE);

      button retryButton((raylib::Rectangle){300, 500, 400, 100}, raylib::BLACK,
                         "Retry", raylib::RAYWHITE, 60, 390, 525, raylib::RED);
      button menuButton((raylib::Rectangle){300, 620, 400, 100}, raylib::BLACK,
                        "Go to menu", raylib::RAYWHITE, 60, 350, 645,
                        raylib::RED);

      if (retryButton.isClicked()) {
        if (forceSound)
          PlaySound(buttonSound);
        p = player({500.0f, 400.0f}, raylib::RED, screenWidth, screenHeight);
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
        p = player({500.0f, 400.0f}, raylib::RED, screenWidth, screenHeight);
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

    raylib::EndDrawing();
  }

  raylib::UnloadSound(buttonSound);
  raylib::UnloadSound(bulletSound);
  for (int i = 0; i < 4; i++) {
    raylib::UnloadSound(killSounds[i]);
  }
  for (int i = 0; i < 2; i++) {
    raylib::UnloadSound(powerUpSounds[i]);
  }
  raylib::UnloadMusicStream(backgroundMusic);
  raylib::CloseAudioDevice();
  raylib::CloseWindow();

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
