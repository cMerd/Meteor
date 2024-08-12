namespace raylib {
#include "./raylib.h"
}

class cameraShaker {
public:
  cameraShaker(raylib::Camera2D &camera_p);

  bool isFinished() const;
  void continueShake();
  void start();

private:
  void calculateSpeedToReturn();

  int frameShaked = 61;
  raylib::Camera2D &camera;
  raylib::Vector2 originalTarget = camera.target;
  raylib::Vector2 returnSpeed = {0, 0};
};
