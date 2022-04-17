#ifndef ASGETREK_ENERGYBAR_HPP
#define ASGETREK_ENERGYBAR_HPP
#include "GameObject.hpp"
#include <iostream>

class EnergyBar
{
 public:
  EnergyBar()  = default;
  ~EnergyBar() = default;

  bool init(ASGE::Colour colour, ASGE::Renderer* renderer);
  void render(ASGE::Renderer* renderer);

  void gain(float health);
  void deplete(float health);

  void setPos(float x, float y);
  void setSize(float width, float height);
  float getHeight();

 private:
  std::unique_ptr<GameObject> bar;
};

#endif // ASGETREK_ENERGYBAR_HPP
