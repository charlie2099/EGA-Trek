#include "gamelib/Ships/Player.hpp"
#include <algorithm>
#include <iostream>

bool Player::init(size_t player_count, ASGE::Renderer* renderer)
{
  auto x_pos = 0.F;
  XPOS       = 525;
  YPOS       = 550;
  SIZE       = player_count;
  for (size_t i = 0; i < SIZE; i++)
  {
    player_vec.emplace_back(std::make_unique<Starship>());
    player_vec.back()->loadSprite(renderer, "/data/images/lexington_white.png");
    player_vec[i]->getSprite()->xPos(x_pos + XPOS);
    player_vec[i]->getSprite()->yPos(YPOS);
    player_vec[i]->getSprite()->colour(ASGE::COLOURS::WHITE);
    x_pos += player_vec[i]->getSprite()->width();
  }

  // std::cout << "Players Spawned: " << player_vec.size() << std::endl;

  return false;
}

void Player::render(ASGE::Renderer* renderer)
{
  for (size_t i = 0; i < SIZE; i++)
  {
    if (player_vec[i]->isVisible())
    {
      renderer->renderSprite(*player_vec[i]->getSprite());
    }
  }
}

const std::vector<std::unique_ptr<Starship>>& Player::getPlayerVec() const
{
  return player_vec;
}

void Player::shootAt(const Enemy& enemy, int damage)
{
  for (const auto& i : enemy.getEnemyVec())
  {
    i->takeDamage(damage);
  }
}

bool Player::isInVicinity(size_t obj_pos, size_t grid_size) const
{
  auto sqrt_of_grid_size = static_cast<size_t>(std::sqrt(grid_size));
  for (int i = 0; i < static_cast<int>(player_vec.size()); ++i)
  {
    if (
      player_vec[static_cast<size_t>(i)]->getPlayerPos() ==
        (obj_pos + 1) || /// RIGHT
      player_vec[static_cast<size_t>(i)]->getPlayerPos() == (obj_pos - 1) || /// LEFT
      player_vec[static_cast<size_t>(i)]->getPlayerPos() ==
        (obj_pos + sqrt_of_grid_size) || /// ABOVE
      player_vec[static_cast<size_t>(i)]->getPlayerPos() ==
        (obj_pos - sqrt_of_grid_size) || /// BELOW
      player_vec[static_cast<size_t>(i)]->getPlayerPos() ==
        (obj_pos - (sqrt_of_grid_size + 1)) || /// LEFT TOP
      player_vec[static_cast<size_t>(i)]->getPlayerPos() ==
        (obj_pos - (sqrt_of_grid_size - 1)) || /// RIGHT TOP
      player_vec[static_cast<size_t>(i)]->getPlayerPos() ==
        (obj_pos + (sqrt_of_grid_size - 1)) || /// LEFT BOTTOM
      player_vec[static_cast<size_t>(i)]->getPlayerPos() ==
        (obj_pos + (sqrt_of_grid_size + 1))) /// RIGHT BOTTOM
    {
      return true;
    }
  }
  return false;
}
