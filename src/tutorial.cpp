#include "../inc/tutorial.hpp"

namespace raylib {
#include "../inc/raylib.h"
}

#include <sstream>

tutorialUI::tutorialUI() {
  backBtn = button((raylib::Rectangle){600, 250, 100, 40}, raylib::BLACK,
                   "Back", raylib::WHITE, 25, 620, 255, raylib::RED);
  nextBtn = button((raylib::Rectangle){800, 250, 100, 40}, raylib::BLACK,
                   "Next", raylib::WHITE, 25, 820, 255, raylib::RED);
}

void tutorialUI::setPage(size_t page_) {
  if (page_ >= this->texts.size()) {
    this->page = this->texts.size() - 1;
    return;
  }
  this->page = page_;
}

size_t tutorialUI::getPage() const { return this->page; }

void tutorialUI::draw() {
  DrawRectangle(500, 0, raylib::GetScreenWidth() - 500, 300,
                raylib::Color{80, 80, 80, 150});

  const std::string &text = texts[page];

  const int rectX = 500;
  const int rectY = 0;
  const int rectWidth = raylib::GetScreenWidth() - 500;
  const int rectHeight = 300;
  int fontSize = 40;

  raylib::Font font = raylib::GetFontDefault();
  std::vector<std::string> lines;
  std::string currentLine;
  int spaceWidth = raylib::MeasureTextEx(font, " ", fontSize, 1).x;

  while (fontSize > 10) {
    lines.clear();
    currentLine.clear();
    int currentWidth = 0;
    int currentHeight = 0;
    bool fits = true;

    std::istringstream stream(text);
    std::string word;

    while (stream >> word) {
      int wordWidth = raylib::MeasureTextEx(font, word.c_str(), fontSize, 1).x;
      if (currentWidth + wordWidth + spaceWidth > rectWidth) {
        lines.push_back(currentLine);
        currentLine = word;
        currentWidth = wordWidth;
        currentHeight += fontSize;
        if (currentHeight + fontSize > rectHeight) {
          fits = false;
          break;
        }
      } else {
        if (!currentLine.empty()) {
          currentWidth += spaceWidth;
        }
        currentLine += " " + word;
        currentWidth += wordWidth;
      }
    }

    if (fits) {
      lines.push_back(currentLine);
      break;
    }

    fontSize--;
    if (fontSize < 10) {
      break;
    }
  }

  int yOffset = rectY + (rectHeight - (lines.size() * fontSize)) / 2;
  for (const std::string &line : lines) {
    int textWidth = raylib::MeasureTextEx(font, line.c_str(), fontSize, 1).x;
    raylib::DrawTextEx(
        font, line.c_str(),
        {(float)(rectX + (float)(rectWidth - textWidth) / 2), (float)yOffset},
        fontSize, 1, raylib::WHITE);
    yOffset += fontSize;
  }

  if (page > 0) {
    backBtn.draw();
    if (backBtn.isClicked()) {
      page--;
    }
  }
  if (page < texts.size() - 1) {
    nextBtn.draw();
    if (nextBtn.isClicked()) {
      page++;
    }
  }
}
