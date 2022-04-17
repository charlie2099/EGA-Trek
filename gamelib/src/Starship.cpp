#include "gamelib/Ships/Starship.hpp"
#include <iostream>

void Starship::takeDamage(int damage)
{
  if (SHIELDS <= 0)
  {
    HEALTH -= damage;
  }
  else
  {
    SHIELDS -= (damage * 5);
  }
}

void Starship::increaseWeaponTemp(int temp)
{
  if (TEMPERATURE < 100)
  {
    TEMPERATURE += temp;
  }
}

void Starship::decreaseWeaponTemp(int temp)
{
  if (TEMPERATURE > 0)
  {
    TEMPERATURE -= temp;
  }
}

void Starship::increaseEfficiency(int eff)
{
  if (EFFICIENCY < 100)
  {
    EFFICIENCY += eff;
  }
}

void Starship::decreaseEfficiency(int eff)
{
  if (EFFICIENCY > 0)
  {
    EFFICIENCY -= eff;
  }
}

void Starship::restoreHealth(int health)
{
  HEALTH += health;
}

int Starship::getHealth() const
{
  return HEALTH;
}

int Starship::getShields() const
{
  return SHIELDS;
}

int Starship::getTemperature() const
{
  return TEMPERATURE;
}

int Starship::getEfficiency() const
{
  return EFFICIENCY;
}
