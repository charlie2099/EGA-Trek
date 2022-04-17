//
// Created by huxy on 23/02/2020.
//

#ifndef GAMELIB_GAMECOMPONENT_HPP
#define GAMELIB_GAMECOMPONENT_HPP

class GameComponent
{
 public:
  enum class ID
  {
    INVALID_COMPONENT_ID = -1,
    NUM_OF_SUPPORTED_COMPONENTS
  };

 public:
  [[maybe_unused]] explicit GameComponent(ID id);
  virtual ~GameComponent() = default;

  GameComponent(const GameComponent&) = default;
  GameComponent(GameComponent&&)      = default;
  GameComponent& operator=(const GameComponent&) = default;
  GameComponent& operator=(GameComponent&&) = default;

  [[maybe_unused]] virtual void update(double dt) = 0;
  [[maybe_unused]] [[nodiscard("did you now want the ID?")]] ID typeID() const;

 private:
  ID enum_id = ID::INVALID_COMPONENT_ID;
};

#endif // GAMELIB_GAMECOMPONENT_HPP
