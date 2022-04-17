#ifndef ASGETREK_ENEMY_HPP
#define ASGETREK_ENEMY_HPP
#include "Starship.hpp"
#include <vector>

class Enemy : public Starship
{
 public:
  Enemy()  = default;
  ~Enemy() = default;

  bool init(size_t enemy_count, ASGE::Renderer* renderer);
  void render(ASGE::Renderer* renderer);
  void setColour(ASGE::Colour colour) const;
  float setPos(float x, float y);
  [[nodiscard]] const std::vector<std::unique_ptr<Starship>>& getEnemyVec() const;

 private:
  size_t SIZE = 0;
  float XPOS  = 0;
  float YPOS  = 0;
  std::vector<std::unique_ptr<Starship>> enemy_vec;
};

#endif // ASGETREK_ENEMY_HPP
