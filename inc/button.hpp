#pragma once

#include <string>
namespace raylib {
#include "./raylib.h"
};

class button {
public:
  button(raylib::Rectangle buttonRectangle, raylib::Color buttonColor,
         const std::string &text, raylib::Color textColor, int fontSize,
         int textxPos, int textyPos, raylib::Color hoverColor);
  button(const std::string &image, const std::string &image_hover,
         raylib::Rectangle button_rectangle);
  bool isHovered();
  bool isClicked();

private:
  raylib::Rectangle button_rectangle;
};
