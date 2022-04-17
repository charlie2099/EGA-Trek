#pragma once
#include "gamelib/Button.hpp"
#include "gamelib/GameObject.hpp"
#include "gamelib/Grid.hpp"
#include "gamelib/Panel.hpp"
#include "gamelib/Scenes/MenuScene.hpp"
#include "gamelib/Ships/Enemy.hpp"
#include "gamelib/Ships/Player.hpp"
#include "gamelib/game_component.hpp"
#include <Engine/FileIO.h>
#include <Engine/OGLGame.h>
#include <gamelib/EnergyBar.hpp>
#include <gamelib/Scenes/LoseScene.hpp>
#include <gamelib/Scenes/WinScene.hpp>
#include <gamelib/Ships/Starship.hpp>
#include <memory>
#include <random>
#include <vector>

class ASGETrek : public ASGE::OGLGame
{
 public:
  explicit ASGETrek(const ASGE::GameSettings& settings);
  ~ASGETrek() override;
  ASGETrek(const ASGETrek&) = delete;
  ASGETrek& operator=(const ASGETrek&) = delete;

  bool init();
  void keyHandler(ASGE::SharedEventData data);
  void update(const ASGE::GameTime& gameTime) override;
  void render() override;
  void renderGame();
  void fixedUpdate(const ASGE::GameTime& gameTime) override;
  void initText();
  void initFont();
  void textAlignToQuestion(ASGE::Text& question);
  void activeText();
  void assigningInputBoxToState();
  void initshortRangeScanner();
  void initLongRangeScanner();
  void initPlayer();
  void initMongol();
  void initMongolIcon();
  void initStar();
  void initStarbase();
  void initCommandPanel();
  void initDamageReportPanel();
  void initWeaponsControlPanel();
  void initCommandBoxPanel();
  void initStatusPanel();
  void initNavigationPanel();
  void initSystemsPanel();
  void initControlsPanel();
  void initEnergyBars();
  void setupCommandBoxText();
  void setupStatusPanelText();
  void moveCommandInputOptions();
  void commandInputOptions(const ASGE::KeyEvent* key);
  void openMoveBox();
  void openLaserBox();
  void openTorpedoBox();
  void openSelfDestructBox();
  void closeMoveBox();
  void closeLaserBox();
  void closeTorpedoBox();
  void closeSelfDestructBox();
  void updateGridElementVisibility() const;
  void mongolDeathReportUpdate();
  void mongolCounterUpdate();
  void updateStatusBars();
  void playerInStarVicinity();
  void playerInStarbaseVicinity();
  void movePlayer(auto x, auto y, size_t sect_pos, const auto& i);
  void objectProximityChecks();
  void moveCommand(auto x, auto y, size_t sect_pos);
  void killMongol(const auto& i);
  void damageMongol(int dmg, int x, int y, const auto& i);
  void playerNearMongolsInSector(
    int laser_dmg, int torpedo_dmg, int x, int y, const auto& i);
  void shootCommand(int laser_dmg, int torpedo_dmg, int x, int y, size_t sect_pos);
  void winGame();
  void loseGame();
  void initScenes();
  void initScenePanels();
  void showMongolsInQuads();
  void quadrantOccupationSetup();
  void other();
  void incrementScoreAndStardate();
  void quadMoveRight();
  void quadMoveLeft();
  void quadColourCheck();
  void generalKeyCommands(const ASGE::KeyEvent* key);
  void activeCommandHandler(const ASGE::KeyEvent* key);
  [[nodiscard]] int getMongolsInQuad() const;
  void grabMongolsInQuadFrom(int front);
  [[nodiscard]] int getMongolTotal() const;
  void setMongolTotal(int mongolTotal);

 private:
  float window_width  = static_cast<float>(ASGE::SETTINGS.window_width);
  float window_height = static_cast<float>(ASGE::SETTINGS.window_height);

  using File = ASGE::FILEIO::File;
  File file  = File();
  File file3 = File();

  ASGE::Colour PRIMARY_TEXT_COLOUR   = ASGE::COLOURS::AQUA;
  ASGE::Colour SECONDARY_TEXT_COLOUR = ASGE::COLOURS::LIGHTGREY;

  std::random_device generator;
  std::uniform_int_distribution<int> laser_dmg_distrib{ 50, 100 };
  std::uniform_int_distribution<int> torpedo_dmg_distrib{ 100, 200 };
  std::uniform_int_distribution<size_t> quad_distrib{ 0, 63 };
  std::uniform_int_distribution<int> quad_mongols{ 1, 3 };

  std::vector<std::unique_ptr<GameComponent>> game_components;

  enum class ActiveText
  {
    COMMAND,
    MOVE,
    LASER,
    TORPEDO,
    SELFDESTRUCT
  };
  ActiveText active_text = ActiveText::COMMAND;

  std::string input_text;
  int key_callback_id       = -1;
  int star_date             = 1000;
  int text_selected_counter = 5;
  int score_counter         = 0;
  float PANEL_WIDTH         = 525.F;
  int MONGOL_TOTAL          = 0;
  int selected_mongol       = 0;
  int FRONT                 = 0;
  size_t mongol_counter     = 0;
  size_t icon_counter       = 0;
  std::array<size_t, 64> mongol_positions{};
  std::array<size_t, 3> mongol_num_in_quad{};

  /// Scenes
  SceneManager scene;
  MenuScene menu_scene;
  WinScene win_scene;
  LoseScene lose_scene;

  /// Main Sprites
  Player player;
  std::unique_ptr<Enemy> mongol, mongol_icon;
  std::unique_ptr<GameObject> star, starbase;
  std::unique_ptr<EnergyBar> health_bar, shield_bar, efficiency_bar, temperature_bar;
  std::unique_ptr<Grid> sector_grid, quadrant_grid;
  PanelSetup command, action, weapons, navigation, damage, status, system, controls;

  static const int LENGTH = 8;
  std::array<ASGE::Text, LENGTH> row_axis_text;
  std::array<ASGE::Text, LENGTH> column_axis_text;

  /// Panel - Text
  ASGE::Text shields_text{}, health_text{}, temperature_text{}, efficiency_text{},
    mongol_count_text{};
  std::array<ASGE::Text, 2> system_panel_text{}, system_panel_status_text{};
  ASGE::Text userInput_text{}, stardate_text{}, score_text{};
  ASGE::Text sector_pos_text{};
  ASGE::Text name_text{};
  std::vector<std::unique_ptr<ASGE::Text>> quadrant_text_grid{};
  void highlightPlayerQuad();
};
