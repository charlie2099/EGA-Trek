#ifndef ASGETREK_GAMEOBJECT_HPP
#define ASGETREK_GAMEOBJECT_HPP

#include <Engine/Renderer.h>
#include <Engine/Sprite.h>
#include <vector>

class GameObject
{
 public:
  GameObject()  = default;
  ~GameObject() = default;

  bool loadSprite(ASGE::Renderer* renderer, const std::string& texture_file_name);
  std::unique_ptr<ASGE::Sprite>& getSprite();

  [[nodiscard]] bool isVisible() const;
  void setIsVisible(bool isVisible);
  [[nodiscard]] bool isOccupied() const;
  void setOccupied(bool occupation);
  [[nodiscard]] bool isKilled() const;
  void setKilled(bool eliminated);

  bool initObject(
    size_t object_count, const std::string& filename, ASGE::Renderer* renderer);
  void renderVec(ASGE::Renderer* renderer);
  [[nodiscard]] const std::vector<std::unique_ptr<GameObject>>& getVec() const;

  void setMongolPos(size_t pos);
  [[nodiscard]] size_t getMongolPos() const;

  void setPlayerPos(size_t pos);
  [[nodiscard]] size_t getPlayerPos() const;

  void setStarBasePos(size_t pos);
  [[nodiscard]] size_t getStarBasePos() const;

  void setStarPos(size_t pos);
  [[nodiscard]] size_t getStarPos() const;

 private:
  size_t MONGOL_POS   = 0;
  size_t PLAYER_POS   = 0;
  size_t STARBASE_POS = 0;
  size_t STAR_POS     = 0;
  size_t SIZE         = 0;
  bool killed         = false;
  bool occupied       = false;
  bool is_visible     = true;
  std::vector<std::unique_ptr<GameObject>> object_vec;
  std::unique_ptr<ASGE::Sprite> sprite_ptr = nullptr;
};

#endif // ASGETREK_GAMEOBJECT_HPP
