#ifndef ASGETREK_WINSCENE_HPP
#define ASGETREK_WINSCENE_HPP
#include "SceneManager.hpp"

class WinScene : public SceneManager
{
 public:
  bool init(ASGE::Renderer* renderer) override;
  void update();
  void render(ASGE::Renderer* renderer);

  void loadWinFont(ASGE::Renderer* renderer);
  void enlargeVictoryText();

 private:
  ASGE::Text win_title_text{};
};

#endif // ASGETREK_WINSCENE_HPP
