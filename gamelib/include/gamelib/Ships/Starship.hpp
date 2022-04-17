#ifndef ASGETREK_STARSHIP_HPP
#define ASGETREK_STARSHIP_HPP
#include "gamelib/GameObject.hpp"

class Starship : public GameObject
{
 public:
  Starship()  = default;
  ~Starship() = default;

  void takeDamage(int damage);
  void increaseWeaponTemp(int temp);
  void decreaseWeaponTemp(int temp);
  void increaseEfficiency(int eff);
  void decreaseEfficiency(int eff);
  void restoreHealth(int health);
  [[nodiscard]] int getHealth() const;
  [[nodiscard]] int getShields() const;
  [[nodiscard]] int getTemperature() const;
  [[nodiscard]] int getEfficiency() const;

 private:
  int HEALTH      = 100;
  int SHIELDS     = 500;
  int TEMPERATURE = 0;
  int EFFICIENCY  = 100;
};

#endif // ASGETREK_STARSHIP_HPP
