#pragma once

#include <string>

#include "raylib.h"

class button {
public:
  button(Rectangle buttonRectangle, Color buttonColor, const std::string &text,
         Color textColor, int fontSize, int textxPos, int textyPos,
         Color hoverColor);
  button(const std::string &image, const std::string &image_hover,
         Rectangle button_rectangle);
  bool isHovered();
  bool isClicked();

private:
  Rectangle button_rectangle;
};
