#include "gamelib/EnergyBar.hpp"

bool EnergyBar::init(ASGE::Colour colour, ASGE::Renderer* renderer)
{
  bar = std::make_unique<GameObject>();
  bar->loadSprite(renderer, "data/images/cube.png");
  bar->getSprite()->xPos(20);
  bar->getSprite()->yPos(550);
  bar->getSprite()->width(245);
  bar->getSprite()->height(15);
  bar->getSprite()->colour(colour);
  return false;
}

void EnergyBar::render(ASGE::Renderer* renderer)
{
  renderer->renderSprite(*bar->getSprite());
}

void EnergyBar::gain(float health)
{
  bar->getSprite()->width(bar->getSprite()->width() + health);
}

void EnergyBar::deplete(float health)
{
  bar->getSprite()->width(bar->getSprite()->width() - health);
}

void EnergyBar::setPos(float x, float y)
{
  bar->getSprite()->xPos(x);
  bar->getSprite()->yPos(y);
}

void EnergyBar::setSize(float width, float height)
{
  bar->getSprite()->width(width);
  bar->getSprite()->height(height);
}

float EnergyBar::getHeight()
{
  return bar->getSprite()->height();
}
