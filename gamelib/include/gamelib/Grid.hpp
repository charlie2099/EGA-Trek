#ifndef ASGETREK_GRID_HPP
#define ASGETREK_GRID_HPP
#include "gamelib/GameObject.hpp"
#include <Engine/Renderer.h>
#include <gamelib/Ships/Player.hpp>
#include <random>
#include <vector>

class Grid : public GameObject
{
 public:
  Grid()  = default;
  ~Grid() = default;

  bool initSector(int size, float spacing, ASGE::Renderer* renderer);
  bool initQuadrant(int size, float spacing, ASGE::Renderer* renderer);
  void renderSector(ASGE::Renderer* renderer);
  void renderQuadrant(ASGE::Renderer* renderer);

  void setSectorColour(ASGE::Colour colour, float opacity) const;
  void setQuadrantColour(ASGE::Colour colour, float opacity) const;

  void randomiseStarGridPos(const std::unique_ptr<GameObject>& obj);
  void randomiseStarbaseGridPos(const std::unique_ptr<GameObject>& obj);
  void randomisePlayerGridPos(const Player& obj);
  void randomiseMongolGridPos(const std::unique_ptr<Enemy>& obj);
  void depopulateMongolsInSector(const Enemy& mongol) const;

  void setSectorPos(float x, float y);
  void setQuadrantPos(float x, float y);
  [[nodiscard]] const std::vector<std::unique_ptr<GameObject>>& getSectorVec() const;
  [[nodiscard]] const std::vector<std::unique_ptr<GameObject>>&
  getQuadranttVec() const;

 private:
  std::random_device generator;
  std::uniform_int_distribution<size_t> grid_distrib{ 0, 63 };
  float SIZE    = 0;
  float SPACING = 0.F;
  float XPOS    = 0.F;
  float YPOS    = 0.F;
  std::vector<std::unique_ptr<GameObject>> sector_grid, quadrant_grid;
};

#endif // ASGETREK_GRID_HPP
