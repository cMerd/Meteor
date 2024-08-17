#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../inc/button.hpp"
#include "../inc/cameraShaker.hpp"
#include "../inc/enemy.hpp"
#include "../inc/meteor.hpp"
#include "../inc/particleDestruction.hpp"
#include "../inc/player.hpp"
#include "../inc/powerup.hpp"
#include "../inc/smartEnemy.hpp"
#include "../inc/superEnemy.hpp"

int getData(const std::string &filePath);
bool setData(const std::string &filePath, int highScore);

int main() {

  constexpr int screenWidth = 1000;
  constexpr int screenHeight = 800;
  constexpr int animFrameCount = 30;
  constexpr int powerupFrameCount = 300;
  constexpr int powerupExtraFrameCount = 600;
  const std::string storageDir =
      std::string(raylib::GetApplicationDirectory()) + "/../data/";
  const std::string highScoreFilePath = storageDir + "high_score";
  const std::string soundOptionFilePath = storageDir + "sound";
  const std::string coinNumberFilePath = storageDir + "coins";
  const std::string levelFilePath = storageDir + "upgrades";
  constexpr raylib::Color backgroundColor = {8, 8, 33, 0};

  if (!std::filesystem::exists(storageDir)) {
    std::filesystem::create_directory(storageDir);
  }

  MENU currentMenu = MAIN_MENU;
  unsigned int frameCount = 0, currentScore = 0, speedPowerUpFrameCounter = 0,
               ammoPowerUpFrameCounter = 0,
               highScore = getData(highScoreFilePath),
               coins = getData(coinNumberFilePath), coinsEarned = 0;
  bool shieldWasted = false;
  std::vector<enemy> enemies;
  std::vector<superEnemy> superEnemies;
  std::vector<smartEnemy> smartEnemies;
  std::vector<powerup> powerups;
  std::vector<particleDestruction> particles;
  bool forceSprint = false, forceAmmo = false, forceShield = false,
       forceSound = getData(soundOptionFilePath);
  raylib::SetTraceLogLevel(raylib::LOG_NONE);
  raylib::InitWindow(screenWidth, screenHeight, "Meteor");
  raylib::InitAudioDevice();

  player p({500.0f, 400.0f}, raylib::RED, screenWidth, screenHeight);
  raylib::Camera2D camera = {0};
  camera.target = (raylib::Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
  camera.offset = (raylib::Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
  cameraShaker cameraController(camera);

  powerUpgrades levels;
  int levelData = getData(levelFilePath);
  levels.shieldLevel = levelData % 10;
  levels.ammoLevel = (levelData / 10) % 10;
  levels.speedLevel = levelData / 100;

  raylib::SetTargetFPS(60);
  button startButton((raylib::Rectangle){300, 400, 400, 100}, raylib::BLACK,
                     "Start", raylib::RAYWHITE, 60, 400, 425, raylib::RED);
  button quitButton((raylib::Rectangle){300, 640, 400, 100}, raylib::BLACK,
                    "Quit", raylib::RAYWHITE, 60, 420, 665, raylib::RED);
  button shopButton((raylib::Rectangle){300, 520, 400, 100}, raylib::BLACK,
                    "Shop", raylib::RAYWHITE, 60, 420, 545, raylib::RED);
  button backButton((raylib::Rectangle){50, screenHeight - 100, 150, 50},
                    raylib::BLACK, "Back", raylib::RAYWHITE, 30, 70,
                    screenHeight - 90, raylib::RED);
  button upgradeBackButton(
      (raylib::Rectangle){800, screenHeight - 100, 150, 50}, raylib::BLACK,
      "Back", raylib::RAYWHITE, 30, 820, screenHeight - 90, raylib::RED);
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
  button retryButton((raylib::Rectangle){300, 500, 400, 100}, raylib::BLACK,
                     "Retry", raylib::RAYWHITE, 60, 390, 525, raylib::RED);
  button menuButton((raylib::Rectangle){300, 620, 400, 100}, raylib::BLACK,
                    "Go to menu", raylib::RAYWHITE, 60, 350, 645, raylib::RED);

  button speedPowerupButton((raylib::Rectangle){100, 150, 200, 200},
                            raylib::Color{0, 12, 102, 255}, "", raylib::BLANK,
                            0, 0, 0, raylib::DARKPURPLE);

  button ammoPowerupButton((raylib::Rectangle){400, 150, 200, 200},
                           raylib::Color{0, 12, 102, 255}, "", raylib::BLANK, 0,
                           0, 0, raylib::ORANGE);

  button shieldPowerupButton((raylib::Rectangle){700, 150, 200, 200},
                             raylib::Color{0, 12, 102, 255}, "", raylib::BLANK,
                             0, 0, 0, raylib::BLUE);
  button upgradeButton((raylib::Rectangle){50, 700, 200, 100}, raylib::GREEN,
                       "Upgrade", raylib::WHITE, 40, 65, 720, raylib::LIME);

  raylib::Texture2D coinIcon = raylib::LoadTexture(
      (std::string(raylib::GetApplicationDirectory()) + "/../assets/coin.png")
          .c_str());

  raylib::Texture2D speedIcon =
      raylib::LoadTexture((std::string(raylib::GetApplicationDirectory()) +
                           "../assets/big_speed_icon.png")
                              .c_str());

  raylib::Texture2D ammoIcon =
      raylib::LoadTexture((std::string(raylib::GetApplicationDirectory()) +
                           "../assets/big_ammo_icon.png")
                              .c_str());
  raylib::Texture2D shieldIcon =
      raylib::LoadTexture((std::string(raylib::GetApplicationDirectory()) +
                           "../assets/big_shield_icon.png")
                              .c_str());

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

#ifdef _METEOR_BUILD_WITH_CHEATS_
  levels.ammoLevel = 3;
  levels.speedLevel = 3;
  levels.shieldLevel = 3;
  forceAmmo = true;
  forceSprint = true;
  forceShield = true;
  coins = 999999999;
  highScore = 999999999;
#endif

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
    BeginMode2D(camera);

    raylib::ClearBackground(backgroundColor);

    if (currentMenu == MAIN_MENU) {

      const std::string highScoreText =
          "High Score: " + std::to_string(highScore);

      DrawText("Meteor", 350, 200, 80, raylib::WHITE);
      for (int i = 280; i <= 285; i++)
        DrawLine(300, i, 700, i, raylib::RED);
      DrawText(highScoreText.c_str(), 50, 50, 50, raylib::WHITE);

      startButton.draw();
      shopButton.draw();
      quitButton.draw();
      soundButton.draw();

      if (startButton.isClicked()) {
        if (forceSound) {
          raylib::PlaySound(buttonSound);
        }
        currentMenu = GAME;
      } else if (quitButton.isClicked()) {
        break;
      } else if (soundButton.isClicked()) {
        forceSound = !forceSound;
        soundButton.changeImage(
            (forceSound ? std::string(raylib::GetApplicationDirectory()) +
                              "/../assets/sound-on.png"
                        : std::string(raylib::GetApplicationDirectory()) +
                              "/../assets/sound-off.png"),
            (forceSound ? std::string(raylib::GetApplicationDirectory()) +
                              "/../assets/sound-on-hover.png"
                        : std::string(raylib::GetApplicationDirectory()) +
                              "/../assets/sound-off-hover.png"));
        if (forceSound) {
          raylib::PlayMusicStream(backgroundMusic);
        }
        setData(soundOptionFilePath, forceSound);
      } else if (shopButton.isClicked()) {
        if (forceSound) {
          raylib::PlaySound(buttonSound);
        }
        currentMenu = SHOP_MENU;
      }
    } else if (currentMenu == GAME) {

      frameCount++;

      if (shouldSpawnEnemies(frameCount, currentScore)) {

        for (unsigned int i = 0; i < getNewEnemyCount(currentScore); i++) {
          enemies.push_back(
              enemy(screenWidth, screenHeight, (float)getRandomValue()));
        }

        int key = raylib::GetRandomValue(1, 10);
        if (key == 7 or key == 1 or key == 9) {
          enemies.push_back(
              enemy(screenWidth, screenHeight, 3, planetSize, planetSize));
        }

        key = raylib::GetRandomValue(1, 5);
        if (key == 2) {
          superEnemies.push_back(
              superEnemy(screenWidth, screenHeight, (float)getRandomValue()));
        }

        key = raylib::GetRandomValue(1, 10);
        if (key == 5) {
          smartEnemies.push_back(smartEnemy(screenWidth, screenHeight,
                                            raylib::GetRandomValue(3, 4)));
        }

        key = raylib::GetRandomValue(1, 20);
        if (key == 5 or (levels.speedLevel >= 3 and key == 18)) {
          powerups.push_back(powerup(screenWidth, screenHeight, SPEED_POWERUP));
        } else if (key == 6 or (levels.ammoLevel >= 3 and key == 19)) {
          powerups.push_back(powerup(screenWidth, screenHeight, AMMO_POWERUP));
        } else if (key == 1 or (levels.shieldLevel >= 3 and key == 20)) {
          powerups.push_back(
              powerup(screenWidth, screenHeight, SHIELD_POWERUP));
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
            particleDestruction particle(Enemy.getPos().x, Enemy.getPos().y,
                                         false, false);
            particles.push_back(particle);
            cameraController.start();
            enemies.erase(enemies.begin() + i);
            coins++;
            coinsEarned++;
            if (forceSound) {
              PlaySound(killSounds[raylib::GetRandomValue(0, 3)]);
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
              PlaySound(killSounds[raylib::GetRandomValue(0, 3)]);
            }
            frameCount = 0;
            currentMenu = GAME_OVER;
            highScore = std::max(highScore, currentScore);
            setData(highScoreFilePath, highScore);
            setData(coinNumberFilePath, coins);
          } else {
            if (forceSound) {
              PlaySound(killSounds[raylib::GetRandomValue(0, 3)]);
            }
            if (levels.shieldLevel >= 1 and !shieldWasted) {
              shieldWasted = true;
            } else {
#ifndef _METEOR_BUILD_WITH_CHEATS_
              forceShield = false;
#endif
            }
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
            particleDestruction particle(sEnemy.getPos().x, sEnemy.getPos().y,
                                         true, false);
            particles.push_back(particle);
            cameraController.start();
            superEnemies.erase(superEnemies.begin() + i);
            coins += 5;
            coinsEarned += 5;
            if (forceSound) {
              PlaySound(killSounds[raylib::GetRandomValue(0, 3)]);
            }
            goto super_enemy_loop_continue;
          }
        }
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, enemyHitbox)) {
          if (!forceShield) {
            if (forceSound) {
              PlaySound(killSounds[raylib::GetRandomValue(0, 3)]);
            }
            frameCount = 0;
            currentMenu = GAME_OVER;
            highScore = std::max(highScore, currentScore);
            setData(highScoreFilePath, highScore);
            setData(coinNumberFilePath, coins);
          } else {
            if (forceSound) {
              PlaySound(killSounds[raylib::GetRandomValue(0, 3)]);
            }
#ifndef _METEOR_BUILD_WITH_CHEATS_
            forceShield = false;
#endif
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
            particleDestruction particle(smEnemy.getPos().x, smEnemy.getPos().y,
                                         false, true);
            particles.push_back(particle);
            cameraController.start();
            smartEnemies.erase(smartEnemies.begin() + i);
            coins += 10;
            coinsEarned += 10;
            if (forceSound) {
              PlaySound(killSounds[raylib::GetRandomValue(0, 3)]);
            }
            goto smart_enemy_loop_continue;
          }
        }
        if (CheckCollisionCircleRec(p.getPos(), 20.0f, enemyHitbox)) {
          if (!forceShield) {
            if (forceSound) {
              PlaySound(killSounds[raylib::GetRandomValue(0, 3)]);
            }
            frameCount = 0;
            currentMenu = GAME_OVER;
            highScore = std::max(highScore, currentScore);
            setData(highScoreFilePath, highScore);
            setData(coinNumberFilePath, coins);
          } else {
            if (forceSound) {
              PlaySound(killSounds[raylib::GetRandomValue(0, 3)]);
            }
            smartEnemies.erase(smartEnemies.begin() + i);
#ifndef _METEOR_BUILD_WITH_CHEATS_
            forceShield = false;
#endif
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
            speedPowerUpFrameCounter =
                (levels.speedLevel >= 1 ? powerupExtraFrameCount
                                        : powerupFrameCount);
            break;
          case AMMO_POWERUP:
            forceAmmo = true;
            ammoPowerUpFrameCounter =
                (levels.ammoLevel >= 1 ? powerupExtraFrameCount
                                       : powerupFrameCount);
            ;
            break;
          case SHIELD_POWERUP:
            forceShield = true;
            break;
          default:
            break;
          }
          powerups.erase(powerups.begin() + i);
          if (forceSound) {
            PlaySound(powerUpSounds[raylib::GetRandomValue(0, 1)]);
          }
          continue;
        }
        Power.update();
      }

#ifndef _METEOR_BUILD_WITH_CHEATS_
      if (speedPowerUpFrameCounter == 0) {
        forceSprint = false;
      } else {
        speedPowerUpFrameCounter--;
      }

      if (ammoPowerUpFrameCounter == 0) {
        forceAmmo = false;
      } else {
        ammoPowerUpFrameCounter--;
      }
#endif

      for (size_t i = 0; i < particles.size(); i++) {
        if (particles[i].IsFinished()) {
          particles.erase(particles.begin() + i);
          continue;
        }
        particles[i].Draw();
        particles[i].Update();
      }

      if (!cameraController.isFinished()) {
        cameraController.continueShake();
      }

      p.update(forceSprint, forceAmmo, bulletSound, forceSound, forceShield,
               (levels.speedLevel >= 2), (levels.ammoLevel >= 2),
               (levels.shieldLevel >= 2 and forceShield));

      raylib::EndMode2D();

      const std::string scoreText = "Score: " + std::to_string(currentScore);

      DrawProgressBar(30, 30, 300, 30, p.getCharge(),
                      raylib::Color{0, 121, 241, 200},
                      raylib::Color{255, 255, 255, 150});
      DrawProgressBar(30, 90, 300, 30, p.getAmmo(),
                      raylib::Color{253, 249, 0, 200},
                      raylib::Color{255, 255, 255, 150});
      DrawText(scoreText.c_str(), 30, 150, 50, raylib::WHITE);

      std::vector<std::pair<POWERUP, std::optional<int>>> types;
      if (forceShield) {
        types.push_back({SHIELD_POWERUP, {}});
      }
      if (forceSprint) {
        types.push_back({SPEED_POWERUP, {speedPowerUpFrameCounter}});
      }
      if (forceAmmo) {
        types.push_back({AMMO_POWERUP, {ammoPowerUpFrameCounter}});
      }
      DrawPowerUpSign(types, powerupFrameCount);

    } else if (currentMenu == GAME_OVER) {
      std::string scoreText = "Score: " + std::to_string(currentScore);
      std::string highScoreText = "High Score: " + std::to_string(highScore);
      std::string coinText = std::to_string(coinsEarned);

      DrawText("Game Over!", 300, 200, 80, raylib::WHITE);
      for (int i = 280; i <= 285; i++)
        DrawLine(280, i, 750, i, raylib::RED);
      DrawText(scoreText.c_str(), 30, 50, 50, raylib::WHITE);
      DrawText(highScoreText.c_str(), 30, 100, 50, raylib::WHITE);
      DrawText(coinText.c_str(), 30, 180, 50, raylib::WHITE);
      raylib::DrawTexture(coinIcon,
                          45 + raylib::MeasureText(coinText.c_str(), 50), 165,
                          raylib::WHITE);

      retryButton.draw();
      menuButton.draw();

      if (retryButton.isClicked()) {
        if (forceSound)
          PlaySound(buttonSound);
        p = player({500.0f, 400.0f}, raylib::RED, screenWidth, screenHeight);
        enemies.clear();
        superEnemies.clear();
        smartEnemies.clear();
        powerups.clear();
        particles.clear();
        frameCount = 0;
        currentMenu = GAME;
        currentScore = 0;
#ifndef _METEOR_BUILD_WITH_CHEATS_
        forceAmmo = false;
        forceSprint = false;
        speedPowerUpFrameCounter = 0;
        ammoPowerUpFrameCounter = 0;
#endif
        coinsEarned = 0;
        shieldWasted = false;
      } else if (menuButton.isClicked()) {
        if (forceSound)
          PlaySound(buttonSound);
        p = player({500.0f, 400.0f}, raylib::RED, screenWidth, screenHeight);
        enemies.clear();
        superEnemies.clear();
        smartEnemies.clear();
        powerups.clear();
        particles.clear();
        frameCount = 0;
        currentMenu = MAIN_MENU;
        currentScore = 0;
#ifndef _METEOR_BUILD_WITH_CHEATS_
        forceAmmo = false;
        forceSprint = false;
#endif
        speedPowerUpFrameCounter = 0;
        ammoPowerUpFrameCounter = 0;
        coinsEarned = 0;
        shieldWasted = false;
      }

      frameCount += 1;
      if (frameCount % animFrameCount * 2 >= animFrameCount) {
        p.noCheckUpdate();
      } else if (frameCount == animFrameCount) {
        frameCount = 0;
      }
    } else if (currentMenu == SHOP_MENU) {
      backButton.draw();
      if (backButton.isClicked()) {
        if (forceSound) {
          raylib::PlaySound(buttonSound);
        }
        currentMenu = MAIN_MENU;
      }
      raylib::DrawText(std::to_string(coins).c_str(), 50, 50, 50,
                       raylib::WHITE);
      raylib::DrawTexture(
          coinIcon, 75 + raylib::MeasureText(std::to_string(coins).c_str(), 50),
          30, raylib::WHITE);

      speedPowerupButton.draw();
      ammoPowerupButton.draw();
      shieldPowerupButton.draw();

      raylib::DrawTexture(speedIcon, 100, 150, raylib::WHITE);
      raylib::DrawTexture(ammoIcon, 400, 150, raylib::WHITE);
      raylib::DrawTexture(shieldIcon, 700, 150, raylib::WHITE);

      if (speedPowerupButton.isClicked()) {
        if (forceSound) {
          raylib::PlaySound(buttonSound);
        }
        currentMenu = SPEED_UPGRADE_MENU;
      } else if (ammoPowerupButton.isClicked()) {
        if (forceSound) {
          raylib::PlaySound(buttonSound);
        }
        currentMenu = AMMO_UPGRADE_MENU;
      } else if (shieldPowerupButton.isClicked()) {
        if (forceSound) {
          raylib::PlaySound(buttonSound);
        }
        currentMenu = SHIELD_UPGRADE_MENU;
      }
    } else if (currentMenu == SPEED_UPGRADE_MENU) {
      for (float i = 300; i <= 305; i++) {
        raylib::DrawLine(i, 0, i, screenHeight, raylib::WHITE);
        raylib::DrawLine(0, i, 300, i, raylib::WHITE);
      }

      raylib::DrawText("Speed Powerup", 350, 50, 50, raylib::WHITE);
      raylib::DrawText("Makes you go faster for a few seconds", 350, 100, 30,
                       raylib::Color{200, 200, 200, 200});

      raylib::DrawTexture(speedIcon, 50, 50, raylib::WHITE);

      DrawLevelBar(levels.speedLevel);

      if (levels.speedLevel < 3) {
        upgradeButton.draw();
      }

      upgradeBackButton.draw();
      if (upgradeBackButton.isClicked()) {
        if (forceSound) {
          raylib::PlaySound(buttonSound);
        }
        currentMenu = SHOP_MENU;
      }

      if (levels.speedLevel == 0) {
        raylib::DrawText("For 30 coins", 80, 770, 20, raylib::YELLOW);
        raylib::DrawText("Next upgrade will double", 350, 200, 30,
                         raylib::WHITE);
        raylib::DrawText("the time of the effect", 350, 230, 30, raylib::WHITE);
      } else if (levels.speedLevel == 1) {
        raylib::DrawText("For 50 coins", 80, 770, 20, raylib::YELLOW);
        raylib::DrawText("Next upgrade will increase", 350, 200, 30,
                         raylib::WHITE);
        raylib::DrawText("speed by 1.3x", 350, 230, 30, raylib::WHITE);
      } else if (levels.speedLevel == 2) {
        raylib::DrawText("For 100 coins", 80, 770, 20, raylib::YELLOW);
        raylib::DrawText("Next upgrade will double", 350, 200, 30,
                         raylib::WHITE);
        raylib::DrawText("the chance of spawn", 350, 230, 30, raylib::WHITE);
      }
      if (upgradeButton.isClicked() and levels.speedLevel < 3) {
        if (coins >= (levels.speedLevel == 0
                          ? 30
                          : (levels.speedLevel == 1 ? 50 : 100))) {
          coins -=
              (levels.speedLevel == 0 ? 30
                                      : (levels.speedLevel == 1 ? 50 : 100));
          levels.speedLevel++;
          setData(coinNumberFilePath, coins);
          setData(levelFilePath, levels.speedLevel * 100 +
                                     levels.ammoLevel * 10 +
                                     levels.shieldLevel);
        }
      }
      raylib::DrawText(
          (std::string("LVL ") + std::to_string(levels.speedLevel)).c_str(),
          150, 360, 30, raylib::WHITE);

    } else if (currentMenu == AMMO_UPGRADE_MENU) {

      for (float i = 300; i <= 305; i++) {
        raylib::DrawLine(i, 0, i, screenHeight, raylib::WHITE);
        raylib::DrawLine(0, i, 300, i, raylib::WHITE);
      }

      raylib::DrawText("Ammo Powerup", 350, 50, 50, raylib::WHITE);
      raylib::DrawText("Reloads ammo faster", 350, 100, 30,
                       raylib::Color{200, 200, 200, 200});

      raylib::DrawTexture(ammoIcon, 50, 50, raylib::WHITE);

      DrawLevelBar(levels.ammoLevel);

      if (levels.ammoLevel < 3) {
        upgradeButton.draw();
      }

      upgradeBackButton.draw();
      if (upgradeBackButton.isClicked()) {
        if (forceSound) {
          raylib::PlaySound(buttonSound);
        }
        currentMenu = SHOP_MENU;
      }

      if (levels.ammoLevel == 0) {
        raylib::DrawText("For 30 coins", 80, 770, 20, raylib::YELLOW);
        raylib::DrawText("Next upgrade will double", 350, 200, 30,
                         raylib::WHITE);
        raylib::DrawText("the time of the effect", 350, 230, 30, raylib::WHITE);
      } else if (levels.ammoLevel == 1) {
        raylib::DrawText("For 50 coins", 80, 770, 20, raylib::YELLOW);
        raylib::DrawText("Next upgrade will increase", 350, 200, 30,
                         raylib::WHITE);
        raylib::DrawText("reloading speed by 1.5x", 350, 230, 30,
                         raylib::WHITE);
      } else if (levels.ammoLevel == 2) {
        raylib::DrawText("For 100 coins", 80, 770, 20, raylib::YELLOW);
        raylib::DrawText("Next upgrade will double", 350, 200, 30,
                         raylib::WHITE);
        raylib::DrawText("the chance of spawn", 350, 230, 30, raylib::WHITE);
      }
      if (upgradeButton.isClicked() and levels.ammoLevel < 3) {
        if (coins >=
            (levels.ammoLevel == 0 ? 30 : (levels.ammoLevel == 1 ? 50 : 100))) {
          coins -=
              (levels.ammoLevel == 0 ? 30 : (levels.ammoLevel == 1 ? 50 : 100));
          levels.ammoLevel++;
          setData(coinNumberFilePath, coins);
          setData(levelFilePath, levels.speedLevel * 100 +
                                     levels.ammoLevel * 10 +
                                     levels.shieldLevel);
        }
      }
      raylib::DrawText(
          (std::string("LVL ") + std::to_string(levels.ammoLevel)).c_str(), 150,
          360, 30, raylib::WHITE);

    } else if (currentMenu == SHIELD_UPGRADE_MENU) {

      for (float i = 300; i <= 305; i++) {
        raylib::DrawLine(i, 0, i, screenHeight, raylib::WHITE);
        raylib::DrawLine(0, i, 300, i, raylib::WHITE);
      }

      raylib::DrawText("Shield Powerup", 350, 50, 50, raylib::WHITE);
      raylib::DrawText("Protects you from enemies", 350, 100, 30,
                       raylib::Color{200, 200, 200, 200});

      raylib::DrawTexture(shieldIcon, 50, 50, raylib::WHITE);

      DrawLevelBar(levels.shieldLevel);

      if (levels.shieldLevel < 3) {
        upgradeButton.draw();
      }

      upgradeBackButton.draw();
      if (upgradeBackButton.isClicked()) {
        if (forceSound) {
          raylib::PlaySound(buttonSound);
        }
        currentMenu = SHOP_MENU;
      }

      if (levels.shieldLevel == 0) {
        raylib::DrawText("For 30 coins", 80, 770, 20, raylib::YELLOW);
        raylib::DrawText("Next upgrade will protect", 350, 200, 30,
                         raylib::WHITE);
        raylib::DrawText("you from 2 hits", 350, 230, 30, raylib::WHITE);
      } else if (levels.shieldLevel == 1) {
        raylib::DrawText("For 50 coins", 80, 770, 20, raylib::YELLOW);
        raylib::DrawText("Next upgrade will make speed 1.2x", 350, 200, 30,
                         raylib::WHITE);
        raylib::DrawText("faster when shield is enabled", 350, 230, 30,
                         raylib::WHITE);
      } else if (levels.shieldLevel == 2) {
        raylib::DrawText("For 100 coins", 80, 770, 20, raylib::YELLOW);
        raylib::DrawText("Next upgrade will double", 350, 200, 30,
                         raylib::WHITE);
        raylib::DrawText("the chance of spawn", 350, 230, 30, raylib::WHITE);
      }
      if (upgradeButton.isClicked() and levels.shieldLevel < 3) {
        if (coins >= (levels.shieldLevel == 0
                          ? 30
                          : (levels.shieldLevel == 1 ? 50 : 100))) {
          coins -=
              (levels.shieldLevel == 0 ? 30
                                       : (levels.shieldLevel == 1 ? 50 : 100));
          levels.shieldLevel++;
          setData(coinNumberFilePath, coins);
          setData(levelFilePath, levels.speedLevel * 100 +
                                     levels.ammoLevel * 10 +
                                     levels.shieldLevel);
        }
      }
      raylib::DrawText(
          (std::string("LVL ") + std::to_string(levels.shieldLevel)).c_str(),
          150, 360, 30, raylib::WHITE);
    }

    raylib::EndDrawing();
  }

  p.destroy();
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
    std::ofstream highScoreFile(filePath);
    return 0;
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
