#ifndef ASGETREK_LOSESCENE_HPP
#define ASGETREK_LOSESCENE_HPP
#include "SceneManager.hpp"

class LoseScene : public SceneManager
{
 public:
  bool init(ASGE::Renderer* renderer) override;
  void update();
  void render(ASGE::Renderer* renderer);

  void loadLoseFont(ASGE::Renderer* renderer);
  void enlargeDefeatText();

 private:
  ASGE::Text lose_title_text{};
};

#endif // ASGETREK_LOSESCENE_HPP
