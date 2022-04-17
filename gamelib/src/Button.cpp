#include "gamelib/Button.hpp"

bool Button::init(const std::string& filename, ASGE::Renderer* renderer)
{
  return button.loadSprite(renderer, filename);
}

void Button::size(float width, float height)
{
  button.getSprite()->width(width);
  button.getSprite()->height(height);
}

void Button::position(float x, float y)
{
  button.getSprite()->xPos(x);
  button.getSprite()->yPos(y);
  button.getSprite()->setGlobalZOrder(1);
}

void Button::setColour(ASGE::Colour colour)
{
  button.getSprite()->colour(colour);
}

void Button::render(ASGE::Renderer* renderer)
{
  renderer->renderSprite(*button.getSprite());
}
