#include "gamelib/Ships/Enemy.hpp"
#include <iostream>
#include <random>

bool Enemy::init(size_t enemy_count, ASGE::Renderer* renderer)
{
  auto x_pos = 0.F;
  XPOS       = 50;
  YPOS       = 450;
  SIZE       = enemy_count;
  for (size_t i = 0; i < SIZE; i++)
  {
    enemy_vec.emplace_back(std::make_unique<Starship>());
    enemy_vec.back()->loadSprite(renderer, "/data/images/mongol.png");
    enemy_vec[i]->getSprite()->xPos(x_pos + XPOS);
    enemy_vec[i]->getSprite()->yPos(YPOS);
    x_pos += enemy_vec[i]->getSprite()->width();
  }
  // std::cout << "Enemies Spawned: " << enemy_vec.size() << std::endl;
  return false;
}

float Enemy::setPos(float x, float y)
{
  for (size_t i = 0; i < SIZE; i++)
  {
    enemy_vec[i]->getSprite()->xPos(x);
    enemy_vec[i]->getSprite()->xPos(y);
  }

  return 0;
}

void Enemy::render(ASGE::Renderer* renderer)
{
  for (size_t i = 0; i < SIZE; i++)
  {
    if (enemy_vec[i]->isVisible())
    {
      renderer->renderSprite(*enemy_vec[i]->getSprite());
    }
  }
}

const std::vector<std::unique_ptr<Starship>>& Enemy::getEnemyVec() const
{
  return enemy_vec;
}

void Enemy::setColour(ASGE::Colour colour) const
{
  for (const auto& i : Enemy::getEnemyVec())
  {
    i->getSprite()->colour(colour);
  }
}
