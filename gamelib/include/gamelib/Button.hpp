#ifndef ASGETREK_BUTTON_HPP
#define ASGETREK_BUTTON_HPP
#include "gamelib/GameObject.hpp"
#include <Engine/InputEvents.h>
#include <iostream>

class Button : public GameObject
{
 public:
  Button()  = default;
  ~Button() = default;

  bool init(const std::string& filename, ASGE::Renderer* renderer);
  void size(float width, float height);
  void position(float x, float y);
  void setColour(ASGE::Colour colour);
  void render(ASGE::Renderer* renderer);

 private:
  GameObject button;
};

#endif // ASGETREK_BUTTON_HPP
