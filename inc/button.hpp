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
  void changeImage(const std::string &image, const std::string &image_hover);
  void changeText(const std::string &text);
  void draw();
  bool isHovered();
  bool isClicked();

private:
  void drawImage();
  void drawRect();

  bool drawTexture;

  raylib::Rectangle button_rectangle;
  raylib::Color button_color;
  std::string btn_text;
  raylib::Color text_color;
  int font_size;
  int text_x;
  int text_y;
  raylib::Color hover_color;

  raylib::Texture2D texture;
  raylib::Texture2D hover_texture;
};
