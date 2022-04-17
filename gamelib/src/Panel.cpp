#include "gamelib/Panel.hpp"

void Panel::setPos(float x, float y)
{
  Panel::getSprite()->xPos(x);
  Panel::getSprite()->yPos(y);
}

float Panel::getXPos()
{
  return Panel::getSprite()->xPos();
}

float Panel::getYPos()
{
  return Panel::getSprite()->yPos();
}

void Panel::createPanelWithText(GameObject& gameObject, ASGE::Text& text)
{
  /// Wherever Text is positioned the Panel and Gameobject will follow, sizing
  /// accordingly
  Panel::getSprite()->width(
    (text.getWidth() + PADDING) + gameObject.getSprite()->width() / 1.3F);
  Panel::getSprite()->height(
    (text.getHeight() + PADDING * 12) + gameObject.getSprite()->height());
  Panel::getSprite()->xPos(
    (text.getPosition().x + text.getWidth() / 2) - Panel::getSprite()->width() / 2);
  Panel::getSprite()->yPos(text.getPosition().y - PADDING * 5);

  gameObject.getSprite()->xPos(
    (Panel::getXPos() + Panel::getSprite()->width() / 2) -
    gameObject.getSprite()->width() / 2);
  gameObject.getSprite()->yPos(
    ((Panel::getYPos() + Panel::getSprite()->height())) -
    gameObject.getSprite()->height() - PADDING * 4);

  Panel::getSprite()->colour(PANEL_COLOUR);
  gameObject.getSprite()->colour(ASGE::COLOURS::LIGHTBLUE);
  text.setColour(ASGE::COLOURS::BLUE);
}

void Panel::createPanelWithoutText(
  GameObject& obj, ASGE::Colour primary, ASGE::Colour secondary)
{
  Panel::getSprite()->width(obj.getSprite()->width() + PADDING);
  Panel::getSprite()->height(obj.getSprite()->height() + PADDING);
  Panel::getSprite()->xPos(
    (obj.getSprite()->xPos() + obj.getSprite()->width() / 2) -
    Panel::getSprite()->width() / 2);
  Panel::getSprite()->yPos(
    (obj.getSprite()->yPos() + obj.getSprite()->height() / 2) -
    Panel::getSprite()->height() / 2);

  obj.getSprite()->colour(primary);
  Panel::getSprite()->colour(secondary);
}

void Panel::setPanelSize(GameObject& gameObject, float size)
{
  gameObject.getSprite()->width(gameObject.getSprite()->width() * size);
  gameObject.getSprite()->height(gameObject.getSprite()->height() * size);
  Panel::getSprite()->width(gameObject.getSprite()->width() + PADDING);
  Panel::getSprite()->height(gameObject.getSprite()->height() + PADDING);
}

void Panel::renderPanel(ASGE::Renderer* renderer)
{
  renderer->renderSprite(*Panel::getSprite());
}

void Panel::centralisePanelText(ASGE::Text& text)
{
  /*text.setPositionX((this->getSprite()->xPos() + this->getSprite()->width() / 2) -
  text.getWidth() / 2); text.setPositionY((this->getSprite()->yPos() +
  this->getSprite()->height() / 7) + 65);*/
  text.setPositionX(
    (Panel::getSprite()->xPos() + Panel::getSprite()->width() / 2) -
    text.getWidth() / 2);
  text.setPositionY(
    (Panel::getSprite()->yPos() + Panel::getSprite()->height() / 7) + 65);
}

void Panel::centralisePanelText(ASGE::Text& text, float offset)
{
  /*text.setPositionX((this->getSprite()->xPos() + this->getSprite()->width() / 2) -
  text.getWidth() / 2); text.setPositionY((this->getSprite()->yPos() +
  this->getSprite()->height() / 7) + 65);*/
  text.setPositionX(
    (Panel::getSprite()->xPos() + Panel::getSprite()->width() / 2) -
    text.getWidth() / 2);
  text.setPositionY(
    (Panel::getSprite()->yPos() + Panel::getSprite()->height() / 7) + offset);
}
