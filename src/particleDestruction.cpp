#include "../inc/particleDestruction.hpp"
#include <algorithm>
#include <cstdlib>

particleDestruction::particleDestruction(int centerX, int centerY,
                                         bool includeRed, bool includeBlue)
    : finished(false), frameCount(0) {
  constexpr int numParticles = 50;

  for (int i = 0; i < numParticles; ++i) {
    Particle p;
    p.x = centerX;
    p.y = centerY;
    p.size = (float)(rand() % 10 + 5);
    unsigned char colorVal = (includeRed or includeBlue) ? (rand() % 100 + 100)
                                                         : (rand() % 55 + 200);
    p.color =
        raylib::Color{(includeRed ? (unsigned char)(255) : colorVal), colorVal,
                      includeBlue ? (unsigned char)(255) : colorVal, 255};
    p.vx = (float)(rand() % 200 - 100) / 100.0f;
    p.vy = (float)(rand() % 200 - 100) / 100.0f;
    p.life = (float)(rand() % 30 + 30);
    particles.push_back(p);
  }
}

void particleDestruction::Update() {
  if (finished)
    return;

  for (auto &p : particles) {
    p.x += p.vx;
    p.y += p.vy;
    p.life -= 1.0f;
  }

  particles.erase(std::remove_if(particles.begin(), particles.end(),
                                 [](const Particle &p) { return p.life <= 0; }),
                  particles.end());
  frameCount++;
  if (frameCount > 60) {
    finished = true;
  }
}

void particleDestruction::Draw() const {
  for (const auto &p : particles) {
    DrawRectangle(p.x, p.y, p.size, p.size, p.color);
  }
}

bool particleDestruction::IsFinished() const {
  return finished && particles.empty();
}
