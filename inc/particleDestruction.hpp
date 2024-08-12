#include <vector>

namespace raylib {
#include "./raylib.h"
}

class particleDestruction {
public:
  particleDestruction(int centerX, int centerY, bool includeRed,
                      bool includeBlue);
  void Update();
  void Draw() const;
  bool IsFinished() const;

private:
  struct Particle {
    float x, y;
    float size;
    float vx, vy;
    float life;
    raylib::Color color;
  };

  std::vector<Particle> particles;
  bool finished;
  int frameCount;
};
