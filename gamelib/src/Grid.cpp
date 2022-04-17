#include "gamelib/Grid.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <random>

bool Grid::initSector(int size, float spacing, ASGE::Renderer* renderer)
{
  auto x_pos = 0.F;
  auto y_pos = 0.F;
  SPACING    = spacing;
  XPOS       = 0;
  YPOS       = 0;
  SIZE       = static_cast<float>(size * size);

  for (size_t i = 0; static_cast<float>(i) < SIZE; i++)
  {
    sector_grid.emplace_back(std::make_unique<GameObject>());
    if (!sector_grid.back()->loadSprite(renderer, "/data/images/cube.png"))
    {
      return false;
    }
    sector_grid[i]->getSprite()->width(6);
    sector_grid[i]->getSprite()->height(6);
    sector_grid[i]->getSprite()->xPos(x_pos + XPOS);
    sector_grid[i]->getSprite()->yPos(y_pos + YPOS);
    sector_grid[i]->getSprite()->colour(ASGE::COLOURS::RED);

    x_pos += sector_grid[i]->getSprite()->width() * SPACING;
    if (
      x_pos >=
      sector_grid[i]->getSprite()->width() * SPACING * static_cast<float>(size))
    {
      x_pos = 0;
      y_pos += sector_grid[i]->getSprite()->height() * SPACING;
    }
  }
  return true;
}

bool Grid::initQuadrant(int size, float spacing, ASGE::Renderer* renderer)
{
  auto x_pos = 0.F;
  auto y_pos = 0.F;
  SPACING    = spacing;
  XPOS       = 0;
  YPOS       = 0;
  SIZE       = static_cast<float>(size * size);

  for (size_t i = 0; static_cast<float>(i) < SIZE; i++)
  {
    quadrant_grid.emplace_back(std::make_unique<GameObject>());
    if (!quadrant_grid.back()->loadSprite(renderer, "/data/images/cube.png"))
    {
      return false;
    }
    quadrant_grid[i]->getSprite()->width(6);
    quadrant_grid[i]->getSprite()->height(6);
    quadrant_grid[i]->getSprite()->xPos(x_pos + XPOS);
    quadrant_grid[i]->getSprite()->yPos(y_pos + YPOS);
    quadrant_grid[i]->getSprite()->colour(ASGE::COLOURS::RED);

    x_pos += quadrant_grid[i]->getSprite()->width() * SPACING;
    if (
      x_pos >=
      quadrant_grid[i]->getSprite()->width() * SPACING * static_cast<float>(size))
    {
      x_pos = 0;
      y_pos += quadrant_grid[i]->getSprite()->height() * SPACING;
    }
  }
  return true;
}

void Grid::renderSector(ASGE::Renderer* renderer)
{
  size_t vec_size = sector_grid.size();
  for (size_t i = 0; i < vec_size; i++)
  {
    if (sector_grid[i]->isVisible())
    {
      renderer->renderSprite(*sector_grid[i]->getSprite());
    }
  }

  /*for (size_t i = 0; static_cast<double>(i) < sqrt(vec_size); i++)
  {
      renderer->renderText(*quadrant_grid[i]);
  }*/
}

void Grid::renderQuadrant(ASGE::Renderer* renderer)
{
  size_t vec_size = quadrant_grid.size();
  for (size_t i = 0; i < vec_size; i++)
  {
    if (quadrant_grid[i]->isVisible())
    {
      renderer->renderSprite(*quadrant_grid[i]->getSprite());
    }
  }
}

void Grid::setSectorPos(float x, float y)
{
  auto x_pos = 0.F;
  auto y_pos = 0.F;
  XPOS       = 0;
  YPOS       = 0;

  for (size_t i = 0; static_cast<float>(i) < SIZE; i++)
  {
    sector_grid[i]->getSprite()->xPos((x_pos + XPOS) + x);
    sector_grid[i]->getSprite()->yPos((y_pos + YPOS) + y);

    x_pos += sector_grid[i]->getSprite()->width() * SPACING;
    if (x_pos >= sector_grid[i]->getSprite()->width() * SPACING * 8 /*sqrt(SIZE)*/)
    {
      x_pos = 0;
      y_pos += sector_grid[i]->getSprite()->height() * SPACING;
    }
  }
}

void Grid::setQuadrantPos(float x, float y)
{
  auto x_pos = 0.F;
  auto y_pos = 0.F;
  XPOS       = 0;
  YPOS       = 0;

  for (size_t i = 0; static_cast<float>(i) < SIZE; i++)
  {
    quadrant_grid[i]->getSprite()->xPos((x_pos + XPOS) + x);
    quadrant_grid[i]->getSprite()->yPos((y_pos + YPOS) + y);

    x_pos += quadrant_grid[i]->getSprite()->width() * 10;
    if (x_pos >= quadrant_grid[i]->getSprite()->width() * 10 * 8 /*sqrt(SIZE)*/)
    {
      x_pos = 0;
      y_pos += quadrant_grid[i]->getSprite()->height() * 4.5F;
    }
  }
}

const std::vector<std::unique_ptr<GameObject>>& Grid::getSectorVec() const
{
  return sector_grid;
}

const std::vector<std::unique_ptr<GameObject>>& Grid::getQuadranttVec() const
{
  return quadrant_grid;
}

void Grid::randomiseStarGridPos(const std::unique_ptr<GameObject>& obj)
{
  /// Change 'Grid::' to 'this'?
  for (const auto& i : obj->getVec())
  {
    size_t random = grid_distrib(generator);
    while (Grid::getSectorVec()[random]->isOccupied())
    {
      random = grid_distrib(generator);
    }
    Grid::getSectorVec()[random]->setOccupied(true);
    auto grid_x      = Grid::getSectorVec()[random]->getSprite()->xPos();
    auto grid_y      = Grid::getSectorVec()[random]->getSprite()->yPos();
    auto grid_width  = Grid::getSectorVec()[random]->getSprite()->width() / 2;
    auto grid_height = Grid::getSectorVec()[random]->getSprite()->height() / 2;
    i->getSprite()->xPos((grid_x - grid_width) - i->getSprite()->width() / 2 + 5);
    i->getSprite()->yPos((grid_y - grid_height) - i->getSprite()->height() / 2 + 5);
    i->setStarPos(random);
  }
}

void Grid::randomiseStarbaseGridPos(const std::unique_ptr<GameObject>& obj)
{
  for (const auto& i : obj->getVec())
  {
    size_t random = grid_distrib(generator);
    while (Grid::getSectorVec()[random]->isOccupied())
    {
      random = grid_distrib(generator);
    }
    Grid::getSectorVec()[random]->setOccupied(true);
    auto grid_x      = Grid::getSectorVec()[random]->getSprite()->xPos();
    auto grid_y      = Grid::getSectorVec()[random]->getSprite()->yPos();
    auto grid_width  = Grid::getSectorVec()[random]->getSprite()->width() / 2;
    auto grid_height = Grid::getSectorVec()[random]->getSprite()->height() / 2;
    i->getSprite()->xPos((grid_x - grid_width) - i->getSprite()->width() / 2 + 5);
    i->getSprite()->yPos((grid_y - grid_height) - i->getSprite()->height() / 2 + 5);
    i->setStarBasePos(random);
  }
}

void Grid::randomisePlayerGridPos(const Player& obj)
{
  for (const auto& i : obj.getPlayerVec())
  {
    size_t random = grid_distrib(generator);
    while (Grid::getSectorVec()[random]->isOccupied())
    {
      random = grid_distrib(generator);
    }
    Grid::getSectorVec()[random]->setOccupied(true);
    auto grid_x      = Grid::getSectorVec()[random]->getSprite()->xPos();
    auto grid_y      = Grid::getSectorVec()[random]->getSprite()->yPos();
    auto grid_width  = Grid::getSectorVec()[random]->getSprite()->width() / 2;
    auto grid_height = Grid::getSectorVec()[random]->getSprite()->height() / 2;
    i->getSprite()->xPos((grid_x - grid_width) - i->getSprite()->width() / 2 + 5);
    i->getSprite()->yPos((grid_y - grid_height) - i->getSprite()->height() / 2 + 5);
    i->setPlayerPos(random);
  }
}

void Grid::randomiseMongolGridPos(const std::unique_ptr<Enemy>& obj)
{
  for (const auto& i : obj->getEnemyVec())
  {
    size_t random = grid_distrib(generator);
    while (Grid::getSectorVec()[random]->isOccupied())
    {
      /// Re-roll
      random = grid_distrib(generator);
    }
    Grid::getSectorVec()[random]->setOccupied(true);
    auto grid_x      = Grid::getSectorVec()[random]->getSprite()->xPos();
    auto grid_y      = Grid::getSectorVec()[random]->getSprite()->yPos();
    auto grid_width  = Grid::getSectorVec()[random]->getSprite()->width() / 2;
    auto grid_height = Grid::getSectorVec()[random]->getSprite()->height() / 2;
    i->getSprite()->xPos((grid_x - grid_width) - i->getSprite()->width() / 2 + 5);
    i->getSprite()->yPos((grid_y - grid_height) - i->getSprite()->height() / 2 + 5);
    i->setMongolPos(random);
    std::cout << "Mongol presence at: " << random + 1 << std::endl;
  }
}

void Grid::depopulateMongolsInSector(const Enemy& mongol) const
{
  for (size_t i = 0; i < Grid::getSectorVec().size(); ++i)
  {
    for (const auto& j : mongol.getEnemyVec())
    {
      /// if grid position equals to mongol positions
      if (i == j->getMongolPos() && j->isVisible())
      {
        Grid::getSectorVec()[j->getMongolPos()]->setOccupied(false);
        Grid::getSectorVec()[j->getMongolPos()]->setIsVisible(true);
      }
    }
  }
}

void Grid::setSectorColour(ASGE::Colour colour, float opacity) const
{
  for (const auto& grid_index : Grid::getSectorVec())
  {
    grid_index->getSprite()->colour(colour);
    grid_index->getSprite()->opacity(opacity);
  }
}

void Grid::setQuadrantColour(ASGE::Colour colour, float opacity) const
{
  for (const auto& grid_index : Grid::getQuadranttVec())
  {
    grid_index->getSprite()->colour(colour);
    grid_index->getSprite()->opacity(opacity);
  }
}
