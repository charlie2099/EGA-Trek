#include "gamelib/game_component.hpp"
[[maybe_unused]] GameComponent::GameComponent(GameComponent::ID id) : enum_id(id)
{
  // constructor
}

[[maybe_unused]] GameComponent::ID GameComponent::typeID() const
{
  return enum_id;
}
