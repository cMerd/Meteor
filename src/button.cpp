#include "../inc/button.hpp"
#include <raylib.h>

button::button(Rectangle buttonRectangle, Color buttonColor,
               const std::string &text, Color textColor, int fontSize,
               int textxPos, int textyPos, Color hoverColor) {
  this->button_rectangle = buttonRectangle;
  if (!isHovered())
    DrawRectangleRec(this->button_rectangle, buttonColor);
  else
    DrawRectangleRec(this->button_rectangle, hoverColor);
  DrawText(text.c_str(), textxPos, textyPos, fontSize, textColor);
}

button:: button(const std::string &image, const std::string &image_hover,
         Rectangle buttonRectangle) {
  this->button_rectangle = buttonRectangle;
  Image btn;
  if (!isHovered()) {
    btn = LoadImage(image.c_str());
  } else {
    btn = LoadImage(image_hover.c_str());
  }
  Texture2D texture = LoadTextureFromImage(btn);
  DrawTexture(texture, button_rectangle.x, button_rectangle.y, WHITE);

}

bool button::isHovered() {
  return (CheckCollisionPointRec(GetMousePosition(), this->button_rectangle));
}

bool button::isClicked() {
  return (this->isHovered() and IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
}
