#include "../inc/cameraShaker.hpp"
#include <cstdlib>
#include <ctime>

cameraShaker::cameraShaker(raylib::Camera2D &camera_p) : camera(camera_p) {
  std::srand(std::time(nullptr));
}

bool cameraShaker::isFinished() const { return frameShaked >= 60; }

void cameraShaker::continueShake() {
  frameShaked++;
  if (frameShaked < 30) {
    float shakeX = (std::rand() % 10 - 5); // Random offset between -5 and 5
    float shakeY = (std::rand() % 10 - 5) * 0.5f;
    camera.target.x += shakeX;
    camera.target.y += shakeY;
    if (frameShaked == 29) {
      calculateSpeedToReturn();
    }
  } else {
    camera.target.x += returnSpeed.x;
    camera.target.y += returnSpeed.y;
  }
}

void cameraShaker::start() { frameShaked = 0; }

void cameraShaker::calculateSpeedToReturn() {
  returnSpeed = {(originalTarget.x - camera.target.x) / 30,
                 (originalTarget.y - camera.target.y) / 30};
}
