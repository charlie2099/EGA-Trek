#ifndef ASGETREK_SCENEMANAGER_HPP
#define ASGETREK_SCENEMANAGER_HPP
#include "gamelib/Panel.hpp"
#include <Engine/FileIO.h>
#include <Engine/Game.h>
#include <Engine/Renderer.h>
#include <gamelib/Button.hpp>
#include <iostream>

class SceneManager
{
 public:
  virtual ~SceneManager() = default;
  virtual bool init(ASGE::Renderer* renderer);
  void renderScene(ASGE::Renderer* renderer);
  bool initPanel(Panel& pan, ASGE::Renderer* rend);
  bool initGameObject(GameObject& obj, ASGE::Renderer* rend);

  enum class GameState
  {
    MENU,
    GAME,
    WIN,
    LOSE
  };
  GameState& gameState()
  {
    static GameState game_state;
    return game_state; // Returns a reference
  }

 private:
  void initBackgroundPanel();

 protected:
  void loadGoldmanBoldFont(ASGE::Renderer* renderer);
  void loadGoldmanRegularFont(ASGE::Renderer* renderer);
  void loadGoldmanRegularFontFix(ASGE::Renderer* renderer);

  enum class ActiveText
  {
    BRIEF,
    USERNAME,
    DIFFICULTY,
    PASSWORD
  };
  ActiveText active_text = ActiveText::BRIEF;

  float window_width  = static_cast<float>(ASGE::SETTINGS.window_width);
  float window_height = static_cast<float>(ASGE::SETTINGS.window_height);

  using File = ASGE::FILEIO::File;
  File file  = File();
  File file2 = File();
  File file3 = File();

  int text_selected_counter = 0;
  std::string input_text;
  ASGE::Text userInput_text{};
  std::unique_ptr<PanelSetup> background, brief;
  std::unique_ptr<Button> right_arrow_button;
};

#endif // ASGETREK_SCENEMANAGER_HPP
