#ifndef ASGETREK_PLAYER_HPP
#define ASGETREK_PLAYER_HPP
#include "Starship.hpp"
#include "gamelib/Ships/Enemy.hpp"
#include <cmath>
#include <vector>

class Player : public Starship
{
 public:
  Player()  = default;
  ~Player() = default;

  bool init(size_t player_count, ASGE::Renderer* renderer);
  void render(ASGE::Renderer* renderer);
  void shootAt(const Enemy& enemy, int damage);
  [[nodiscard]] const std::vector<std::unique_ptr<Starship>>& getPlayerVec() const;
  [[nodiscard]] bool isInVicinity(size_t obj_pos, size_t grid_size) const;

 private:
  size_t SIZE = 0;
  float XPOS  = 0;
  float YPOS  = 0;
  std::vector<std::unique_ptr<Starship>> player_vec;
};

#endif // ASGETREK_PLAYER_HPP
