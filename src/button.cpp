#include "../inc/button.hpp"

button::button(raylib::Rectangle buttonRectangle, raylib::Color buttonColor,
               const std::string &text, raylib::Color textColor, int fontSize,
               int textxPos, int textyPos, raylib::Color hoverColor) {
  this->button_rectangle = buttonRectangle;
  if (!isHovered())
    DrawRectangleRec(this->button_rectangle, buttonColor);
  else
    DrawRectangleRec(this->button_rectangle, hoverColor);
  DrawText(text.c_str(), textxPos, textyPos, fontSize, textColor);
}

button:: button(const std::string &image, const std::string &image_hover,
         raylib::Rectangle buttonRectangle) {
  this->button_rectangle = buttonRectangle;
  raylib::Image btn;
  if (!isHovered()) {
    btn = raylib::LoadImage(image.c_str());
  } else {
    btn = raylib::LoadImage(image_hover.c_str());
  }
  raylib::Texture2D texture = LoadTextureFromImage(btn);
  DrawTexture(texture, button_rectangle.x, button_rectangle.y, raylib::WHITE);

}

bool button::isHovered() {
  return (CheckCollisionPointRec(raylib::GetMousePosition(), this->button_rectangle));
}

bool button::isClicked() {
  return (this->isHovered() and IsMouseButtonPressed(raylib::MOUSE_BUTTON_LEFT));
}
