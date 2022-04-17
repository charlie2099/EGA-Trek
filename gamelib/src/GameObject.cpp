#include <cmath>
#include <gamelib/GameObject.hpp>
#include <iostream>

bool GameObject::loadSprite(
  ASGE::Renderer* renderer, const std::string& texture_file_name)
{
  sprite_ptr = renderer->createUniqueSprite();
  return sprite_ptr->loadTexture(texture_file_name);
}
std::unique_ptr<ASGE::Sprite>& GameObject::getSprite()
{
  return sprite_ptr;
}

bool GameObject::initObject(
  size_t object_count, const std::string& filename, ASGE::Renderer* renderer)
{
  auto x_pos = 0.F;
  SIZE       = object_count;
  for (size_t i = 0; i < SIZE; i++)
  {
    object_vec.emplace_back(std::make_unique<GameObject>());
    object_vec.back()->loadSprite(renderer, filename);
    object_vec[i]->getSprite()->xPos(x_pos + 50);
    object_vec[i]->getSprite()->yPos(400);
    x_pos += object_vec[i]->getSprite()->width();
  }

  return false;
}

void GameObject::renderVec(ASGE::Renderer* renderer)
{
  for (size_t i = 0; i < SIZE; i++)
  {
    renderer->renderSprite(*object_vec[i]->getSprite());
  }
}

const std::vector<std::unique_ptr<GameObject>>& GameObject::getVec() const
{
  return object_vec;
}

bool GameObject::isVisible() const
{
  return is_visible;
}
void GameObject::setIsVisible(bool isVisible)
{
  is_visible = isVisible;
}

bool GameObject::isOccupied() const
{
  return occupied;
}
void GameObject::setOccupied(bool occupation)
{
  occupied = occupation;
}

void GameObject::setMongolPos(size_t pos)
{
  MONGOL_POS = pos;
}
size_t GameObject::getMongolPos() const
{
  return MONGOL_POS;
}

void GameObject::setPlayerPos(size_t pos)
{
  PLAYER_POS = pos;
}
size_t GameObject::getPlayerPos() const
{
  return PLAYER_POS;
}

void GameObject::setStarBasePos(size_t pos)
{
  STARBASE_POS = pos;
}
size_t GameObject::getStarBasePos() const
{
  return STARBASE_POS;
}

void GameObject::setStarPos(size_t pos)
{
  STAR_POS = pos;
}
size_t GameObject::getStarPos() const
{
  return STAR_POS;
}

bool GameObject::isKilled() const
{
  return killed;
}
void GameObject::setKilled(bool eliminated)
{
  killed = eliminated;
}
