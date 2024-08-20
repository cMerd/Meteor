#pragma once

#include "../inc/button.hpp"
#include <string>
#include <vector>

class tutorialUI {
public:
  tutorialUI();

  void setPage(size_t page_);
  size_t getPage() const;

  void draw();

private:
  size_t page = 0;

  button backBtn;
  button nextBtn;

  const std::vector<std::string> texts = {
      "Press WASD to move",
      "Press LEFT MOUSE BTN or ENTER to shoot",
      "The yellow bar represents your ammo, you can only shoot when it's full",
      "Press SHIFT to sprint",
      "The blue bar represents your stamina, you can't sprint when it's empty",
      "There are 4 types of enemies, you'll lose if any of them touches you",
      "But they can't kill you in tutorial",
      "Normal enemies (gray) only go down",
      "Super enemies (gray and red) try to block your way on right or left",
      "Smart enemies (white and blue) follow you as fast as possible",
      "Planets (big) are like nomral enemies, but they can't be destroyed",
      "There are 3 types of powerups, go over them to collect",
      "Speed powerups will increase your speed and stamina for a few seconds",
      "Ammo powerups will increase your reload speed by 25x for a few seconds",
      "Shield powerup will protect you from next hit",
      "These powerups can be upgraded from shop",
      "Use buttons on the left to spawn enemies and powerups",
      "Press ESC to pause and exit the game",
  };
};
