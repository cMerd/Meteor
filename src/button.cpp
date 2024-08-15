#include "../inc/button.hpp"

button::button(raylib::Rectangle buttonRectangle, raylib::Color buttonColor,
               const std::string &text, raylib::Color textColor, int fontSize,
               int textxPos, int textyPos, raylib::Color hoverColor) {
  this->button_rectangle = buttonRectangle;
  this->button_color = buttonColor;
  this->btn_text = text;
  this->text_color = textColor;
  this->font_size = fontSize;
  this->text_x = textxPos;
  this->text_y = textyPos;
  this->hover_color = hoverColor;
  this->drawTexture = false;
}

button::button(const std::string &image, const std::string &image_hover,
               raylib::Rectangle buttonRectangle) {
  this->button_rectangle = buttonRectangle;
  this->texture = raylib::LoadTexture(image.c_str());
  this->hover_texture = raylib::LoadTexture(image_hover.c_str());
  this->drawTexture = true;
}

void button::changeImage(const std::string &image,
                         const std::string &image_hover) {
  this->texture = raylib::LoadTexture(image.c_str());
  this->hover_texture = raylib::LoadTexture(image_hover.c_str());
  this->drawTexture = true;
}

bool button::isHovered() {
  return (CheckCollisionPointRec(raylib::GetMousePosition(),
                                 this->button_rectangle));
}

bool button::isClicked() {
  return (this->isHovered() and
          IsMouseButtonPressed(raylib::MOUSE_BUTTON_LEFT));
}

void button::drawImage() {
  if (isHovered()) {
    DrawTexture(hover_texture, button_rectangle.x, button_rectangle.y,
                raylib::WHITE);
    return;
  }
  DrawTexture(texture, button_rectangle.x, button_rectangle.y, raylib::WHITE);
}

void button::drawRect() {
  if (!isHovered()) {
    DrawRectangleRec(this->button_rectangle, button_color);
  } else {
    DrawRectangleRec(this->button_rectangle, hover_color);
  }
  DrawText(btn_text.c_str(), text_x, text_y, font_size, text_color);
}

void button::draw() {
  if (!drawTexture) {
    drawRect();
    return;
  }
  drawImage();
}

void button::changeText(const std::string &text) { this->btn_text = text; }
