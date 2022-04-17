#include "ASGEGame.hpp"
#include <iostream>
#include <memory>
#include <random>

ASGETrek::ASGETrek(const ASGE::GameSettings& settings) : OGLGame(settings)
{
  renderer->setClearColour(ASGE::COLOURS::BLACK);
  key_callback_id = inputs->addCallbackFnc(ASGE::E_KEY, &ASGETrek::keyHandler, this);
  inputs->use_threads = true;
  toggleFPS();
}

ASGETrek::~ASGETrek()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));
}

bool ASGETrek::init()
{
  initScenes();
  initFont();
  initScenePanels();
  initText();
  initshortRangeScanner();
  initLongRangeScanner();
  initPlayer();
  initStar();
  initStarbase();
  initCommandPanel();
  initDamageReportPanel();
  initWeaponsControlPanel();
  initCommandBoxPanel();
  initNavigationPanel();
  setupCommandBoxText();
  initStatusPanel();
  setupStatusPanelText();
  initSystemsPanel();
  initControlsPanel();
  quadrantOccupationSetup();
  initMongol();
  initMongolIcon();
  initEnergyBars();
  other();

  return true;
}

void ASGETrek::keyHandler(ASGE::SharedEventData data)
{
  const auto* key = dynamic_cast<const ASGE::KeyEvent*>(data.get());
  generalKeyCommands(key);

  if (scene.gameState() == SceneManager::GameState::MENU)
  {
    menu_scene.keyInputs(data);
  }

  if (scene.gameState() == SceneManager::GameState::GAME)
  {
    if (
      (key->key == ASGE::KEYS::KEY_LEFT || key->key == ASGE::KEYS::KEY_RIGHT) &&
      key->action == 1)
    {
      quadColourCheck();
    }

    if (key->key == ASGE::KEYS::KEY_LEFT && key->action == 1)
    {
      quadMoveLeft();
    }
    else if (key->key == ASGE::KEYS::KEY_RIGHT && key->action == 1)
    {
      quadMoveRight();
    }

    if (
      (key->key == ASGE::KEYS::KEY_LEFT || key->key == ASGE::KEYS::KEY_RIGHT) &&
      key->action == 1)
    {
      showMongolsInQuads();
    }
    else if (
      (key->key == ASGE::KEYS::KEY_LEFT || key->key == ASGE::KEYS::KEY_RIGHT) &&
      key->action == 0)
    {
      input_text.clear();
    }

    if (key->key == ASGE::KEYS::KEY_ENTER && key->action == ASGE::KEYS::KEY_PRESSED)
    {
      activeCommandHandler(key);
    }
  }

  if (key->action == ASGE::KEYS::KEY_PRESSED)
  {
    userInput_text.setString((input_text += static_cast<char>(key->key)) + "|");
  }
}

void ASGETrek::update(const ASGE::GameTime& gameTime)
{
  for (auto& gc : game_components)
  {
    gc->update(gameTime.deltaInSecs());
  }

  if (scene.gameState() == SceneManager::GameState::MENU)
  {
    menu_scene.update();
  }

  else if (scene.gameState() == SceneManager::GameState::GAME)
  {
    activeText();
    assigningInputBoxToState();
    updateGridElementVisibility();

    for (auto& i : system_panel_status_text)
    {
      if (player.getPlayerVec()[0]->getTemperature() == 100)
      {
        i.setString("OFFLINE");
        i.setColour(ASGE::COLOURS::RED);
      }
      else
      {
        i.setString("ONLINE");
        i.setColour(ASGE::COLOURS::LIME);
      }
    }

    if (mongol_counter == 0)
    {
      winGame();
    }

    if (player.getPlayerVec()[0]->getHealth() <= 0)
    {
      loseGame();
    }

    highlightPlayerQuad();
  }

  else if (scene.gameState() == SceneManager::GameState::WIN)
  {
    win_scene.update();
  }

  else if (scene.gameState() == SceneManager::GameState::LOSE)
  {
    lose_scene.update();
  }
}
void ASGETrek::highlightPlayerQuad()
{ /// Colour of the quad that the player is in
  auto player_pos = mongol_positions[static_cast<size_t>(selected_mongol)];
  quadrant_text_grid[player_pos]->setColour(ASGE::COLOURS::WHITE);
}

void ASGETrek::fixedUpdate(const ASGE::GameTime& gameTime)
{
  Game::fixedUpdate(gameTime);
}

void ASGETrek::render()
{
  scene.renderScene(renderer.get());

  if (scene.gameState() == SceneManager::GameState::MENU)
  {
    menu_scene.render(renderer.get());
  }
  else if (scene.gameState() == SceneManager::GameState::GAME)
  {
    renderGame();
  }
  else if (scene.gameState() == SceneManager::GameState::WIN)
  {
    win_scene.render(renderer.get());
    renderer->renderText(name_text);
  }
  else if (scene.gameState() == SceneManager::GameState::LOSE)
  {
    lose_scene.render(renderer.get());
    renderer->renderText(name_text);
  }
}

void ASGETrek::renderGame()
{
  for (size_t i = 0; i < LENGTH; i++)
  {
    renderer->renderText(row_axis_text[i]);
    renderer->renderText(column_axis_text[i]);
  }

  command.panel.renderPanel(renderer.get());
  renderer->renderSprite(*command.container.getSprite());

  renderer->renderText(userInput_text);
  renderer->renderText(command.text);
  renderer->renderText(sector_pos_text);
  player.render(renderer.get());
  star->renderVec(renderer.get());
  starbase->renderVec(renderer.get());
  renderer->renderText(stardate_text);
  renderer->renderText(score_text);

  mongol->render(renderer.get());
  mongol_icon->render(renderer.get());

  sector_grid->renderSector(renderer.get());
  quadrant_grid->renderQuadrant(renderer.get());

  damage.panel.renderPanel(renderer.get());
  renderer->renderText(damage.title);
  renderer->renderText(damage.text);
  renderer->renderSprite(*damage.container.getSprite());

  weapons.panel.renderPanel(renderer.get());
  renderer->renderText(weapons.title);
  renderer->renderText(weapons.text);
  renderer->renderSprite(*weapons.container.getSprite());

  navigation.panel.renderPanel(renderer.get());
  renderer->renderText(navigation.title);
  renderer->renderText(navigation.text);
  renderer->renderSprite(*navigation.container.getSprite());

  status.panel.renderPanel(renderer.get());
  renderer->renderText(status.title);
  renderer->renderText(health_text);
  renderer->renderText(shields_text);
  renderer->renderText(temperature_text);
  renderer->renderText(efficiency_text);
  renderer->renderText(mongol_count_text);
  renderer->renderSprite(*status.container.getSprite());

  if (action.panel.isVisible())
  {
    action.panel.renderPanel(renderer.get());
    renderer->renderText(action.title);
    renderer->renderSprite(*action.container.getSprite());
    renderer->renderText(action.text);
  }

  system.panel.renderPanel(renderer.get());
  renderer->renderText(system.title);
  for (size_t i = 0; i < 2; ++i)
  {
    renderer->renderText(system_panel_text[i]);
    renderer->renderText(system_panel_status_text[i]);
  }
  renderer->renderSprite(*system.container.getSprite());

  controls.panel.renderPanel(renderer.get());
  renderer->renderSprite(*controls.container.getSprite());
  renderer->renderText(controls.title);
  renderer->renderText(controls.text);

  health_bar->render(renderer.get());
  shield_bar->render(renderer.get());
  efficiency_bar->render(renderer.get());
  temperature_bar->render(renderer.get());

  for (auto& i : quadrant_text_grid)
  {
    renderer->renderText(*i);
  }
}

void ASGETrek::other()
{
  action.panel.setIsVisible(false);
  icon_counter = mongol_icon->getEnemyVec().size();
}
void ASGETrek::quadrantOccupationSetup()
{
  int total = 0;
  for (size_t i = 0; i < 64; ++i)
  {
    size_t random = quad_distrib(generator);
    while (quadrant_grid->getQuadranttVec()[random]->isOccupied())
    {
      random = quad_distrib(generator);
    }
    quadrant_grid->getQuadranttVec()[random]->setOccupied(true);

    quadrant_text_grid[i]->setPositionX(
      (quadrant_grid->getQuadranttVec()[i]->getSprite()->xPos() +
       quadrant_grid->getQuadranttVec()[i]->getSprite()->width() / 2) -
      quadrant_text_grid[i]->getWidth() / 2);
    quadrant_text_grid[i]->setPositionY(
      (quadrant_grid->getQuadranttVec()[i]->getSprite()->yPos() +
       quadrant_grid->getQuadranttVec()[i]->getSprite()->height() / 2) -
      quadrant_text_grid[i]->getHeight() / 2 + 5);
  }

  for (size_t i = 0; i < 3; ++i)
  {
    size_t random = quad_distrib(generator);
    int mongols   = quad_mongols(generator);
    quadrant_text_grid[random]->setString(std::to_string(mongols) + "|0|0");
    quadrant_text_grid[random]->setColour(ASGE::COLOURS::RED);
    total += mongols;
    mongol_num_in_quad[i] = static_cast<size_t>(mongols);
    std::cout << "value in (" << i << "): " << mongol_num_in_quad[i] << std::endl;
  }
  setMongolTotal(total);
  std::cout << "MONGOLS SPAWNED: " << getMongolTotal() << std::endl;
}
void ASGETrek::initScenePanels()
{
  scene.initPanel(command.panel, renderer.get());
  scene.initGameObject(command.container, renderer.get());

  scene.initPanel(damage.panel, renderer.get());
  scene.initGameObject(damage.container, renderer.get());

  scene.initPanel(weapons.panel, renderer.get());
  scene.initGameObject(weapons.container, renderer.get());

  scene.initPanel(navigation.panel, renderer.get());
  scene.initGameObject(navigation.container, renderer.get());

  scene.initPanel(status.panel, renderer.get());
  scene.initGameObject(status.container, renderer.get());

  scene.initPanel(action.panel, renderer.get());
  scene.initGameObject(action.container, renderer.get());

  scene.initPanel(system.panel, renderer.get());
  scene.initGameObject(system.container, renderer.get());

  scene.initPanel(controls.panel, renderer.get());
  scene.initGameObject(controls.container, renderer.get());
}
void ASGETrek::initScenes()
{
  scene.init(renderer.get());
  menu_scene.init(renderer.get());
  win_scene.init(renderer.get());
  lose_scene.init(renderer.get());
}
void ASGETrek::initEnergyBars()
{
  health_bar      = std::make_unique<EnergyBar>();
  shield_bar      = std::make_unique<EnergyBar>();
  efficiency_bar  = std::make_unique<EnergyBar>();
  temperature_bar = std::make_unique<EnergyBar>();

  health_bar->init(ASGE::COLOURS::RED, renderer.get());
  health_bar->setPos(
    efficiency_text.getPosition().x + efficiency_text.getWidth() + 20,
    health_text.getPosition().y - 15);

  shield_bar->init(ASGE::COLOURS::AQUA, renderer.get());
  shield_bar->setPos(
    efficiency_text.getPosition().x + efficiency_text.getWidth() + 20,
    shields_text.getPosition().y - 15);

  efficiency_bar->init(ASGE::COLOURS::LIMEGREEN, renderer.get());
  efficiency_bar->setPos(
    efficiency_text.getPosition().x + efficiency_text.getWidth() + 20,
    efficiency_text.getPosition().y - 15);

  temperature_bar->init(ASGE::COLOURS::ORANGE, renderer.get());
  temperature_bar->setPos(
    efficiency_text.getPosition().x + efficiency_text.getWidth() + 20,
    temperature_text.getPosition().y - 15);
  temperature_bar->setSize(0.0F, temperature_bar->getHeight());
}

void ASGETrek::initControlsPanel()
{
  controls.container.getSprite()->xPos(15);
  controls.container.getSprite()->yPos(347);
  controls.container.getSprite()->width(360);
  controls.container.getSprite()->height(163);
  controls.panel.createPanelWithoutText(
    controls.container, ASGE::COLOURS::GRAYBLACK, SECONDARY_TEXT_COLOUR);
  controls.title.setPositionX(
    (controls.panel.getXPos() + controls.panel.getSprite()->width() / 2) -
    controls.title.getWidth() / 2);
  controls.title.setPositionY(
    (controls.panel.getYPos() + controls.panel.getSprite()->height() / 4) -
    controls.title.getHeight() / 2 - 2);
  controls.text.setPositionX(controls.panel.getXPos() + 35);
  controls.text.setPositionY(controls.panel.getYPos() + 75);
}

void ASGETrek::initSystemsPanel()
{
  system.container.getSprite()->xPos(390);
  system.container.getSprite()->yPos(
    status.container.getSprite()->yPos() + status.container.getSprite()->height() +
    15);
  system.container.getSprite()->width(335);
  system.container.getSprite()->height(278);
  system.panel.createPanelWithoutText(
    system.container, ASGE::COLOURS::GREYBLACK, SECONDARY_TEXT_COLOUR);
  system.title.setPositionX(
    (system.panel.getXPos() + system.panel.getSprite()->width() / 2) -
    system.title.getWidth() / 2);
  system.title.setPositionY(
    (system.panel.getYPos() + system.panel.getSprite()->height() / 7) -
    system.title.getHeight() / 2);
  for (size_t i = 0; i < 2; ++i)
  {
    system_panel_text[i].setPositionX(system.title.getPosition().x - 50);
    system_panel_text[i].setPositionY(
      (static_cast<float>(i) * 40) + (system.title.getPosition().y + 50));
    system_panel_status_text[i].setPositionX(
      system_panel_text[i].getPosition().x + system_panel_text[1].getWidth() + 20);
    system_panel_status_text[i].setPositionY(
      (static_cast<float>(i) * 40) + (system.title.getPosition().y + 50));
  }
}

void ASGETrek::setupStatusPanelText()
{
  health_text.setPositionX(status.panel.getXPos() + 20);
  health_text.setPositionY(
    (status.panel.getYPos() + status.panel.getSprite()->height() / 3) -
    health_text.getHeight() / 2);
  shields_text.setPositionX(status.panel.getXPos() + 20);
  shields_text.setPositionY(health_text.getPosition().y + 35);
  efficiency_text.setPositionX(status.panel.getXPos() + 20);
  efficiency_text.setPositionY(shields_text.getPosition().y + 35);
  temperature_text.setPositionX(status.panel.getXPos() + 20);
  temperature_text.setPositionY(efficiency_text.getPosition().y + 35);
  mongol_count_text.setPositionX(status.panel.getXPos() + 20);
  mongol_count_text.setPositionY(temperature_text.getPosition().y + 35);
}

void ASGETrek::setupCommandBoxText()
{
  action.text.setPositionX(action.panel.getXPos() + 30);
  action.text.setPositionY(action.panel.getYPos() + 80);
}

void ASGETrek::initNavigationPanel()
{
  navigation.container.getSprite()->xPos(window_width - PANEL_WIDTH - 15);
  navigation.container.getSprite()->yPos(600);
  navigation.container.getSprite()->width(PANEL_WIDTH);
  navigation.container.getSprite()->height(100);
  navigation.panel.createPanelWithoutText(
    navigation.container, ASGE::COLOURS::GRAYBLACK, SECONDARY_TEXT_COLOUR);
  navigation.title.setPositionX(
    (navigation.panel.getXPos() + navigation.panel.getSprite()->width() / 2) -
    navigation.title.getWidth() / 2);
  navigation.title.setPositionY(
    (navigation.panel.getYPos() + navigation.panel.getSprite()->height() / 3) -
    navigation.title.getHeight() / 2);
  navigation.text.setPositionX(
    (navigation.panel.getXPos() + navigation.panel.getSprite()->width() / 2) -
    navigation.text.getWidth() / 2);
  navigation.text.setPositionY(
    navigation.panel.getYPos() + navigation.panel.getSprite()->height() / 1.5F);
}

void ASGETrek::initStatusPanel()
{
  status.container.getSprite()->xPos(225);
  status.container.getSprite()->yPos(92);
  status.container.getSprite()->width(520);
  status.container.getSprite()->height(240);
  status.panel.createPanelWithoutText(
    status.container, ASGE::COLOURS::GRAYBLACK, SECONDARY_TEXT_COLOUR);
  status.title.setPositionX(
    (status.panel.getXPos() + status.panel.getSprite()->width() / 2) -
    status.title.getWidth() / 2);
  status.title.setPositionY(
    (status.panel.getYPos() + status.panel.getSprite()->height() / 7) -
    status.title.getHeight() / 2);
}

void ASGETrek::initCommandBoxPanel()
{
  action.container.getSprite()->xPos(15);
  action.container.getSprite()->yPos(525);
  action.container.getSprite()->width(360);
  action.container.getSprite()->height(100);
  action.panel.createPanelWithoutText(
    action.container, ASGE::COLOURS::GRAYBLACK, SECONDARY_TEXT_COLOUR);
}

void ASGETrek::initWeaponsControlPanel()
{
  weapons.container.getSprite()->xPos(window_width - PANEL_WIDTH - 15);
  weapons.container.getSprite()->yPos(480);
  weapons.container.getSprite()->width(PANEL_WIDTH);
  weapons.container.getSprite()->height(100);
  weapons.panel.createPanelWithoutText(
    weapons.container, ASGE::COLOURS::GRAYBLACK, SECONDARY_TEXT_COLOUR);
  weapons.title.setPositionX(
    (weapons.panel.getXPos() + weapons.panel.getSprite()->width() / 2) -
    weapons.title.getWidth() / 2);
  weapons.title.setPositionY(
    (weapons.panel.getYPos() + weapons.panel.getSprite()->height() / 3) -
    weapons.title.getHeight() / 2);
  weapons.text.setPositionX(
    (weapons.panel.getXPos() + weapons.panel.getSprite()->width() / 2) -
    weapons.text.getWidth() / 2);
  weapons.text.setPositionY(
    weapons.panel.getYPos() + weapons.panel.getSprite()->height() / 1.5F);
}

void ASGETrek::initDamageReportPanel()
{
  damage.container.getSprite()->xPos(window_width - PANEL_WIDTH - 15);
  damage.container.getSprite()->yPos(360);
  damage.container.getSprite()->width(PANEL_WIDTH);
  damage.container.getSprite()->height(100);
  damage.panel.createPanelWithoutText(
    damage.container, ASGE::COLOURS::GRAYBLACK, SECONDARY_TEXT_COLOUR);
  damage.title.setPositionX(
    (damage.panel.getXPos() + damage.panel.getSprite()->width() / 2) -
    damage.title.getWidth() / 2);
  damage.title.setPositionY(
    (damage.panel.getYPos() + damage.panel.getSprite()->height() / 3) -
    damage.title.getHeight() / 2);
  damage.text.setPositionX(
    (damage.panel.getXPos() + damage.panel.getSprite()->width() / 2) -
    damage.text.getWidth() / 2);
  damage.text.setPositionY(
    damage.panel.getYPos() + damage.panel.getSprite()->height() / 1.5F);
}

void ASGETrek::initCommandPanel()
{
  command.text.setPositionX(25);
  command.text.setPositionY(680);
  command.container.getSprite()->xPos(command.text.getPosition().x - 10);
  command.container.getSprite()->yPos(command.text.getPosition().y - 39);
  command.container.getSprite()->width(
    command.text.getPosition().x + command.text.getWidth() + 290);
  command.container.getSprite()->height(60);
  command.panel.createPanelWithoutText(
    command.container, ASGE::COLOURS::GRAYBLACK, SECONDARY_TEXT_COLOUR);
}

void ASGETrek::activeCommandHandler(const ASGE::KeyEvent* key)
{
  switch (active_text)
  {
    case ActiveText::COMMAND:
      commandInputOptions(key);
      break;
    case ActiveText::MOVE:
      openMoveBox();
      break;
    case ActiveText::LASER:
      openLaserBox();
      break;
    case ActiveText::TORPEDO:
      openTorpedoBox();
      break;
    case ActiveText::SELFDESTRUCT:
      openSelfDestructBox();
      break;
  }
  if (active_text >= ActiveText::MOVE)
  {
    incrementScoreAndStardate();
  }
  input_text.clear();
}
void ASGETrek::generalKeyCommands(const ASGE::KeyEvent* key)
{
  switch (key->key)
  {
    case ASGE::KEYS::KEY_BACKSPACE:
      if (!input_text.empty())
      {
        input_text.pop_back();
      }
      break;
    case ASGE::KEYS::KEY_DELETE:
      input_text.clear();
      break;
    case ASGE::KEYS::KEY_ESCAPE:
      signalExit();
      break;
  }
}
void ASGETrek::quadColourCheck()
{
  auto idx = mongol_positions[static_cast<size_t>(selected_mongol)];
  if (getMongolsInQuad() % 48 == 0)
  {
    quadrant_text_grid[idx]->setColour(ASGE::COLOURS::GREY);
  }
  else if (getMongolsInQuad() % 48 > 0)
  {
    quadrant_text_grid[idx]->setColour(ASGE::COLOURS::RED);
  }
}
void ASGETrek::quadMoveLeft()
{
  selected_mongol--;
  if (selected_mongol < 0)
  {
    selected_mongol = 63;
  }
  auto index = mongol_positions[static_cast<size_t>(selected_mongol)];
  grabMongolsInQuadFrom(quadrant_text_grid[index]->getString().front());
}
void ASGETrek::quadMoveRight()
{
  selected_mongol++;
  if (selected_mongol == 64)
  {
    selected_mongol = 0;
  }
  auto index = mongol_positions[static_cast<size_t>(selected_mongol)];
  grabMongolsInQuadFrom(quadrant_text_grid[index]->getString().front());
}
void ASGETrek::incrementScoreAndStardate()
{
  star_date += 10;
  score_counter += 10;
  stardate_text.setString("Star Date: " + std::to_string(star_date));
  score_text.setString("Score: " + std::to_string(score_counter));
}

void ASGETrek::showMongolsInQuads()
{
  for (size_t i = 0; i < mongol_num_in_quad[0]; ++i)
  {
    if (getMongolsInQuad() % 48 == mongol_num_in_quad[0])
    {
      if (!mongol->getEnemyVec()[i]->isKilled())
      {
        mongol->getEnemyVec()[i]->setIsVisible(true);
      }
    }
    else
    {
      mongol->getEnemyVec()[i]->setIsVisible(false);
    }
  }
  for (size_t i = 0; i < mongol_num_in_quad[1]; ++i)
  {
    auto idx = mongol_num_in_quad[0] + i;
    if (getMongolsInQuad() % 48 == mongol_num_in_quad[1])
    {
      if (!mongol->getEnemyVec()[idx]->isKilled())
      {
        mongol->getEnemyVec()[idx]->setIsVisible(true);
      }
    }
    else
    {
      mongol->getEnemyVec()[idx]->setIsVisible(false);
    }
  }
  for (size_t i = 0; i < mongol_num_in_quad[2]; ++i)
  {
    auto sum = mongol_num_in_quad[0] + mongol_num_in_quad[1];
    if (getMongolsInQuad() % 48 == mongol_num_in_quad[2])
    {
      if (!mongol->getEnemyVec()[sum + i]->isKilled())
      {
        mongol->getEnemyVec()[sum + i]->setIsVisible(true);
      }
    }
    else
    {
      mongol->getEnemyVec()[sum + i]->setIsVisible(false);
    }
  }
}

void ASGETrek::openMoveBox()
{
  moveCommandInputOptions();
  closeMoveBox();
}
void ASGETrek::openLaserBox()
{
  moveCommandInputOptions();
  closeLaserBox();
}
void ASGETrek::openTorpedoBox()
{
  moveCommandInputOptions();
  closeTorpedoBox();
}
void ASGETrek::openSelfDestructBox()
{
  if (input_text == menu_scene.getPassword())
  {
    scene.gameState() = SceneManager::GameState::LOSE;
    closeSelfDestructBox();
  }
}

void ASGETrek::closeMoveBox()
{
  text_selected_counter--;
  action.panel.setIsVisible(false);
}
void ASGETrek::closeLaserBox()
{
  text_selected_counter -= 2;
  action.panel.setIsVisible(false);
}
void ASGETrek::closeTorpedoBox()
{
  text_selected_counter -= 3;
  action.panel.setIsVisible(false);
}
void ASGETrek::closeSelfDestructBox()
{
  text_selected_counter -= 4;
  action.panel.setIsVisible(false);
}

void ASGETrek::commandInputOptions(const ASGE::KeyEvent* key)
{
  if (
    input_text.starts_with('M') ||
    (input_text.length() - input_text.size() + 1 == input_text.find('M') &&
     input_text.starts_with(static_cast<char>(key->key == ASGE::KEYS::KEY_ENTER))))
  {
    text_selected_counter++;
    action.panel.setIsVisible(true);
    action.title.setString("MOVE");
  }
  else if (
    input_text.starts_with('L') ||
    (input_text.length() - input_text.size() + 1 == input_text.find('L') &&
     input_text.starts_with(static_cast<char>(key->key == ASGE::KEYS::KEY_ENTER))))
  {
    text_selected_counter += 2;
    action.panel.setIsVisible(true);
    action.title.setString("LASER");
  }
  else if (
    input_text.starts_with('T') ||
    (input_text.length() - input_text.size() + 1 == input_text.find('T') &&
     input_text.starts_with(static_cast<char>(key->key == ASGE::KEYS::KEY_ENTER))))
  {
    text_selected_counter += 3;
    action.panel.setIsVisible(true);
    action.title.setString("TORPEDO");
  }
  else if (
    input_text.starts_with('S') ||
    (input_text.length() - input_text.size() + 1 == input_text.find('S') &&
     input_text.starts_with(static_cast<char>(key->key == ASGE::KEYS::KEY_ENTER))))
  {
    text_selected_counter += 4;
    action.panel.setIsVisible(true);
    action.title.setString("SELF DESTRUCT");
    action.text.setString("Password: ");
  }

  damage.text.setString("...");
  weapons.text.setString("...");
  navigation.text.setString("...");
  navigation.text.setColour(SECONDARY_TEXT_COLOUR);
  damage.text.setColour(SECONDARY_TEXT_COLOUR);
  damage.panel.centralisePanelText(damage.text);
  weapons.panel.centralisePanelText(weapons.text);
  navigation.panel.centralisePanelText(navigation.text);
  action.panel.centralisePanelText(action.title, 20);
}

void ASGETrek::loseGame()
{
  scene.gameState() = SceneManager::GameState::LOSE;
  name_text.setString(
    "OH NO " + menu_scene.getName() +
    ", YOU WERE DEFEATED BY THE MONGOLS! \nYOU SCORED: " +
    std::to_string(score_counter));
  name_text.setColour(ASGE::COLOURS::RED);
  name_text.setPositionX((window_width - name_text.getWidth()) / 2.0F);
  name_text.setPositionY((window_height + name_text.getHeight()) / 1.5F);
}
void ASGETrek::winGame()
{
  scene.gameState() = SceneManager::GameState::WIN;
  name_text.setString(
    "WELL DONE " + menu_scene.getName() +
    ", YOU SCORED: " + std::to_string(score_counter));
  name_text.setPositionX((window_width - name_text.getWidth()) / 2.0F);
  name_text.setPositionY((window_height + name_text.getHeight()) / 1.5F);
}

void ASGETrek::updateGridElementVisibility() const
{
  for (const auto& i : sector_grid->getSectorVec())
  {
    if (i->isOccupied())
    {
      i->setIsVisible(false);
    }
  }

  for (const auto& j : mongol->getEnemyVec())
  {
    if (j->isVisible())
    {
      sector_grid->getSectorVec()[j->getMongolPos()]->setOccupied(true);
    }
    else
    {
      sector_grid->getSectorVec()[j->getMongolPos()]->setOccupied(false);
      sector_grid->getSectorVec()[j->getMongolPos()]->setIsVisible(true);
    }
  }
}

void ASGETrek::initText()
{
  command.text.setString("COMMAND: ")
    .setScale(0.6F)
    .setZOrder(120)
    .setColour(ASGE::COLOURS::WHITE);

  action.text.setString("[C]: ").setScale(0.6F).setZOrder(120).setColour(
    ASGE::COLOURS::WHITE);

  userInput_text.setZOrder(107).setScale(0.65F).setColour(ASGE::COLOURS::YELLOW);

  stardate_text.setString("Star Date: " + std::to_string(star_date))
    .setPositionX(510)
    .setPositionY(660)
    .setScale(0.625F)
    .setZOrder(104)
    .setColour(PRIMARY_TEXT_COLOUR);

  score_text.setString("Score: " + std::to_string(score_counter))
    .setPositionX(510)
    .setPositionY(695)
    .setScale(0.625F)
    .setZOrder(104)
    .setColour(PRIMARY_TEXT_COLOUR);

  sector_pos_text.setString("CURRENT SEC: [ , ]")
    .setPositionX(15)
    .setPositionY(325)
    .setZOrder(107)
    .setScale(0.52F)
    .setColour(PRIMARY_TEXT_COLOUR);

  for (size_t i = 0; i < 8; i++)
  {
    row_axis_text[i]
      .setString(std::to_string(i + 1))
      .setScale(1)
      .setFont(renderer->getDefaultFont())
      .setZOrder(108)
      .setColour(ASGE::COLOURS::WHITE);
  }

  for (size_t i = 0; i < 8; i++)
  {
    column_axis_text[i]
      .setString(std::to_string(i + 1))
      .setScale(1)
      .setFont(renderer->getDefaultFont())
      .setZOrder(109)
      .setColour(ASGE::COLOURS::WHITE);
  }

  action.title.setString("...").setScale(0.675F).setZOrder(100).setColour(
    PRIMARY_TEXT_COLOUR);

  damage.title.setString("DAMAGE REPORT")
    .setScale(0.7F)
    .setZOrder(100)
    .setColour(PRIMARY_TEXT_COLOUR /*ASGE::COLOURS::DARKMAGENTA*/);
  damage.text.setString("...").setScale(0.6F).setZOrder(100);

  weapons.title.setString("WEAPONS CONTROL")
    .setScale(0.7F)
    .setZOrder(100)
    .setColour(PRIMARY_TEXT_COLOUR /*ASGE::COLOURS::DARKRED*/);
  weapons.text.setString("...").setScale(0.6F).setZOrder(100).setColour(
    SECONDARY_TEXT_COLOUR);

  navigation.title.setString("NAVIGATION")
    .setScale(0.7F)
    .setZOrder(100)
    .setColour(PRIMARY_TEXT_COLOUR /*ASGE::COLOURS::DARKGREEN*/);
  navigation.text.setString("...").setScale(0.6F).setZOrder(100).setColour(
    SECONDARY_TEXT_COLOUR);

  status.title.setString("STATUS").setScale(0.7F).setZOrder(100).setColour(
    PRIMARY_TEXT_COLOUR);

  shields_text.setString("SHIELDS: [" + std::to_string(player.getShields()) + "]")
    .setScale(0.60F)
    .setZOrder(100)
    .setColour(PRIMARY_TEXT_COLOUR);

  health_text.setString("HEALTH: [" + std::to_string(player.getHealth()) + "]")
    .setScale(0.60F)
    .setZOrder(100)
    .setColour(ASGE::COLOURS::RED);

  efficiency_text
    .setString("EFFICIENCY: [" + std::to_string(player.getEfficiency()) + "]")
    .setScale(0.60F)
    .setZOrder(100)
    .setColour(ASGE::COLOURS::LIMEGREEN);

  temperature_text
    .setString("TEMP: [" + std::to_string(player.getTemperature()) + "]")
    .setScale(0.60F)
    .setZOrder(100)
    .setColour(ASGE::COLOURS::ORANGE);

  mongol_count_text.setScale(0.60F).setZOrder(100).setColour(ASGE::COLOURS::WHITE);

  system.title.setString("SYSTEMS").setScale(0.7F).setZOrder(100).setColour(
    PRIMARY_TEXT_COLOUR);
  for (size_t i = 0; i < 2; ++i)
  {
    std::array<std::string, 2> system_list = { "LASERS:", "TORPEDOES:" };
    system_panel_text[i].setString(system_list[i]).setScale(0.55F).setZOrder(100);

    std::array<std::string, 2> system_status = { "ONLINE", "ONLINE" };
    system_panel_status_text[i]
      .setString(system_status[i])
      .setScale(0.55F)
      .setZOrder(100)
      .setColour(ASGE::COLOURS::LIME);
  }

  controls.title.setString("CONTROLS")
    .setScale(0.7F)
    .setZOrder(100)
    .setColour(PRIMARY_TEXT_COLOUR);

  controls.text
    .setString("'M' TO MOVE or 'L' TO FIRE LASERS\n'T' TO FIRE TORPEDOES\n'S' TO "
               "SELF DESTRUCT")
    .setScale(0.5F)
    .setZOrder(100)
    .setColour(ASGE::COLOURS::WHITE);

  name_text.setString("")
    .setPositionX(500)
    .setPositionY(500)
    .setZOrder(107)
    .setScale(0.75F)
    .setColour(PRIMARY_TEXT_COLOUR);

  for (size_t i = 0; i < 64; ++i)
  {
    quadrant_text_grid.emplace_back(std::make_unique<ASGE::Text>());
    quadrant_text_grid[i]
      ->setString("0|0|0")
      .setFont(renderer->getDefaultFont())
      .setZOrder(107)
      .setScale(0.95F)
      .setColour(ASGE::COLOURS::GREY);
  }
}

void ASGETrek::textAlignToQuestion(ASGE::Text& question)
{
  float offset = 15.F;
  userInput_text.setPositionX(
    question.getPosition().x + question.getWidth() + offset);
  userInput_text.setPositionY(question.getPosition().y + question.getHeight());
}

void ASGETrek::activeText()
{
  if (active_text == ActiveText::COMMAND)
  {
    textAlignToQuestion(command.text);
    command.text.setColour(PRIMARY_TEXT_COLOUR);
  }
  else
  {
    textAlignToQuestion(action.text);
    action.text.setColour(PRIMARY_TEXT_COLOUR);
  }
}

void ASGETrek::assigningInputBoxToState()
{
  switch (text_selected_counter)
  {
    case 5:
      active_text = ActiveText::COMMAND;
      break;
    case 6:
      active_text = ActiveText::MOVE;
      break;
    case 7:
      active_text = ActiveText::LASER;
      break;
    case 8:
      active_text = ActiveText::TORPEDO;
      break;
    case 9:
      active_text = ActiveText::SELFDESTRUCT;
      break;
  }
}

void ASGETrek::initshortRangeScanner()
{
  sector_grid = std::make_unique<Grid>();
  sector_grid->initSector(8, 4, renderer.get());
  sector_grid->setSectorPos(30, 116);
  sector_grid->setSectorColour(PRIMARY_TEXT_COLOUR, 0.6F);

  auto spacing        = (sector_grid->getSectorVec().size() / 8 + 4) * 2;
  auto scanner_xpos   = sector_grid->getSectorVec()[0]->getSprite()->xPos();
  auto scanner_ypos   = sector_grid->getSectorVec()[0]->getSprite()->yPos();
  auto scanner_width  = sector_grid->getSectorVec()[0]->getSprite()->width();
  auto scanner_height = sector_grid->getSectorVec()[0]->getSprite()->height();
  for (size_t i = 0; i < LENGTH; ++i)
  {
    auto index = static_cast<float>(i * spacing);
    row_axis_text[i].setPositionX((index) + scanner_xpos - scanner_width / 2);
    row_axis_text[i].setPositionY(scanner_ypos - (scanner_height * 1.5F));
    column_axis_text[i].setPositionX(
      (scanner_xpos - scanner_width / 2) - scanner_width * 3);
    column_axis_text[i].setPositionY(
      (index) + scanner_ypos + (scanner_height * 1.5F));
  }
}

void ASGETrek::initLongRangeScanner()
{
  quadrant_grid = std::make_unique<Grid>();
  quadrant_grid->initQuadrant(8, 4, renderer.get());
  quadrant_grid->setQuadrantPos(800, 110);
  quadrant_grid->setQuadrantColour(PRIMARY_TEXT_COLOUR, 0.0F);
}

void ASGETrek::initPlayer()
{
  player.init(1, renderer.get());
  sector_grid->randomisePlayerGridPos(player);
}

void ASGETrek::initMongol()
{
  mongol    = std::make_unique<Enemy>();
  auto size = static_cast<size_t>(getMongolTotal());
  mongol->init(size, renderer.get());
  mongol_counter = mongol->getEnemyVec().size();
  mongol_count_text.setString("MONGOLS: [" + std::to_string(mongol_counter) + "]");
  sector_grid->randomiseMongolGridPos(mongol);

  for (const auto& i : mongol->getEnemyVec())
  {
    i->setIsVisible(false);
  }

  for (size_t i = 0; i < 64; ++i)
  {
    mongol_positions[i] = i;
  }
}

void ASGETrek::initMongolIcon()
{
  mongol_icon = std::make_unique<Enemy>();
  mongol_icon->init(mongol->getEnemyVec().size(), renderer.get());
  for (size_t i = 0; i < mongol_icon->getEnemyVec().size(); ++i)
  {
    auto spacing = static_cast<float>(i * 30);
    auto x = mongol_count_text.getPosition().x + mongol_count_text.getWidth() + 15;
    auto y = mongol_count_text.getPosition().y - 23;
    mongol_icon->getEnemyVec()[i]->getSprite()->xPos(spacing + x);
    mongol_icon->getEnemyVec()[i]->getSprite()->yPos(y);
  }
}

void ASGETrek::initStar()
{
  star = std::make_unique<GameObject>();
  star->initObject(4, "/data/images/star.png", renderer.get());
  sector_grid->randomiseStarGridPos(star);
}

void ASGETrek::initStarbase()
{
  starbase = std::make_unique<GameObject>();
  starbase->initObject(2, "/data/images/starBase.png", renderer.get());
  sector_grid->randomiseStarbaseGridPos(starbase);
}

void ASGETrek::initFont()
{
  auto read = ASGE::FILEIO::File::IOMode::READ;
  if (file.open("/data/fonts/goldman/Goldman-Bold.ttf", read))
  {
    using Buffer  = ASGE::FILEIO::IOBuffer;
    Buffer buffer = file.read();
    if (buffer.length != 0U)
    {
      int font_index = renderer->loadFontFromMem(
        "GOLDMAN_BOLD",
        buffer.as_unsigned_char(),
        static_cast<unsigned int>(buffer.length),
        64);
      command.text.setFont(renderer->getFont(font_index));
      sector_pos_text.setFont(renderer->getFont(font_index));
    }
    file.close();
  }

  if (file3.open("/data/fonts/goldman/Goldman-Regular.ttf", read))
  {
    using Buffer  = ASGE::FILEIO::IOBuffer;
    Buffer buffer = file3.read();
    if (buffer.length != 0U)
    {
      int font_index = renderer->loadFontFromMem(
        "GOLDMAN_REGULAR",
        buffer.as_unsigned_char(),
        static_cast<unsigned int>(buffer.length),
        64);
      userInput_text.setFont(renderer->getFont(font_index));
      action.title.setFont(renderer->getFont(font_index));
      weapons.title.setFont(renderer->getFont(font_index));
      weapons.text.setFont(renderer->getFont(font_index));
      damage.title.setFont(renderer->getFont(font_index));
      damage.text.setFont(renderer->getFont(font_index));
      navigation.title.setFont(renderer->getFont(font_index));
      navigation.text.setFont(renderer->getFont(font_index));
      status.title.setFont(renderer->getFont(font_index));
      system.title.setFont(renderer->getFont(font_index));
      for (size_t i = 0; i < 2; ++i)
      {
        system_panel_text[i].setFont(renderer->getFont(font_index));
        system_panel_status_text[i].setFont(renderer->getFont(font_index));
      }
      controls.title.setFont(renderer->getFont(font_index));
      controls.text.setFont(renderer->getFont(font_index));
      shields_text.setFont(renderer->getFont(font_index));
      health_text.setFont(renderer->getFont(font_index));
      temperature_text.setFont(renderer->getFont(font_index));
      efficiency_text.setFont(renderer->getFont(font_index));
      mongol_count_text.setFont(renderer->getFont(font_index));
      action.text.setFont(renderer->getFont(font_index));
      stardate_text.setFont(renderer->getFont(font_index));
      score_text.setFont(renderer->getFont(font_index));
      name_text.setFont(renderer->getFont(font_index));
    }
    file3.close();
  }
}

void ASGETrek::moveCommandInputOptions()
{
  auto x_input       = input_text.find_first_of(',') - 1;
  auto y_input       = input_text.find_first_of(',') + 1;
  int laser_damage   = laser_dmg_distrib(generator);
  int torpedo_damage = torpedo_dmg_distrib(generator);

  /// checks inputs are within the range before plugging them in
  if (x_input > 0 && x_input < 9 && y_input > 0 && y_input < 9)
  {
    auto grid_x_pos = input_text[x_input] - 48; // Value BEFORE comma
    auto grid_y_pos = input_text[y_input] - 48; // Value AFTER comma
    auto sect_pos =
      (static_cast<size_t>(grid_y_pos - 1)) +
      (static_cast<size_t>(grid_x_pos - 1)) * sector_grid->getSectorVec().size() / 8;

    /// checks calculated value is within grid range, -4 check for multiple commas
    if (static_cast<int>(sect_pos) >= 0 && sect_pos < 64 && grid_y_pos != -4)
    {
      if (active_text == ActiveText::MOVE)
      {
        moveCommand(grid_x_pos, grid_y_pos, sect_pos);
      }
      else if (active_text == ActiveText::LASER || active_text == ActiveText::TORPEDO)
      {
        shootCommand(laser_damage, torpedo_damage, grid_x_pos, grid_y_pos, sect_pos);
      }
    }
    else
    {
      navigation.text.setString("OUT OF RANGE!");
      navigation.panel.centralisePanelText(navigation.text);
    }
  }
}
void ASGETrek::shootCommand(
  int laser_dmg, int torpedo_dmg, int x, int y, size_t sect_pos)
{
  for (const auto& i : mongol->getEnemyVec())
  {
    if (sect_pos == i->getMongolPos() && i->isVisible())
    {
      if (player.getPlayerVec()[0]->getTemperature() < 100)
      {
        playerNearMongolsInSector(laser_dmg, torpedo_dmg, x, y, i);
      }
    }

    weapons.text.setString(
      "Lasers Fired At Sector [" + std::to_string(x) + "][" + std::to_string(y) +
      "]");
    weapons.panel.centralisePanelText(weapons.text);

    if (player.getPlayerVec()[0]->getTemperature() >= 100)
    {
      weapons.text.setString("WEAPONS OVERHEATED!");
      weapons.panel.centralisePanelText(weapons.text);
    }
  }
}

void ASGETrek::playerNearMongolsInSector(
  int laser_dmg, int torpedo_dmg, int x, int y, const auto& i)
{
  auto sector_size = sector_grid->getSectorVec().size();
  if (player.isInVicinity(i->getMongolPos(), sector_size))
  {
    if (active_text == ActiveText::LASER)
    {
      damageMongol(laser_dmg, x, y, i);
    }
    else if (active_text == ActiveText::TORPEDO)
    {
      damageMongol(torpedo_dmg, x, y, i);
    }

    player.getPlayerVec()[0]->takeDamage(10);
    updateStatusBars();

    if (i->getHealth() < 0)
    {
      killMongol(i);
    }
  }
}
void ASGETrek::damageMongol(int dmg, int x, int y, const auto& i)
{
  i->takeDamage(dmg);
  auto str_1 = "Mongol at Sector [" + std::to_string(x) + "][";
  auto str_2 = std::to_string(y) + "] " + "Took " + std::to_string(dmg) + " Damage";
  damage.text.setString(str_1 + str_2);
  damage.panel.centralisePanelText(damage.text);
}
void ASGETrek::killMongol(const auto& i)
{
  i->setIsVisible(false);
  i->setKilled(true);
  mongolDeathReportUpdate();
  mongolCounterUpdate();
  sector_grid->getSectorVec()[i->getMongolPos()]->setOccupied(false);
  sector_grid->getSectorVec()[i->getMongolPos()]->setIsVisible(true);
}
void ASGETrek::moveCommand(auto x, auto y, size_t sect_pos)
{
  for (const auto& i : player.getPlayerVec())
  {
    if (sector_grid->getSectorVec()[sect_pos]->isOccupied())
    {
      navigation.text.setString("SPACE OCCUPIED");
      navigation.panel.centralisePanelText(navigation.text);
    }
    else
    {
      movePlayer(x, y, sect_pos, i);
      objectProximityChecks();
    }
  }
}
void ASGETrek::objectProximityChecks()
{
  auto sector_size = sector_grid->getSectorVec().size();
  for (const auto& j : starbase->getVec())
  {
    if (player.isInVicinity(j->getStarBasePos(), sector_size))
    {
      playerInStarbaseVicinity();
    }
  }
  for (const auto& j : star->getVec())
  {
    if (player.isInVicinity(j->getStarPos(), sector_size))
    {
      playerInStarVicinity();
    }
  }
}

void ASGETrek::movePlayer(auto x, auto y, size_t sect_pos, const auto& i)
{
  auto xpos   = sector_grid->getSectorVec()[sect_pos]->getSprite()->xPos();
  auto ypos   = sector_grid->getSectorVec()[sect_pos]->getSprite()->yPos();
  auto width  = player.getPlayerVec()[0]->getSprite()->width();
  auto height = player.getPlayerVec()[0]->getSprite()->height();

  sector_grid->getSectorVec()[i->getPlayerPos()]->setOccupied(false);
  sector_grid->getSectorVec()[i->getPlayerPos()]->setIsVisible(true);
  i->getSprite()->xPos(xpos - width / 2 + 2);
  i->getSprite()->yPos(ypos - height / 2 + 3);
  i->setPlayerPos(sect_pos);
  sector_grid->getSectorVec()[i->getPlayerPos()]->setOccupied(true);

  sector_pos_text.setString(
    "CURRENT SEC: [" + std::to_string(x) + "," + std::to_string(y) + "]");
  navigation.text.setString(
    "Player Moved To Sector [" + std::to_string(x) + "][" + std::to_string(y) + "]");
  navigation.panel.centralisePanelText(navigation.text);
}
void ASGETrek::playerInStarbaseVicinity()
{
  navigation.text.setString("A Starbase is in your vicinity");
  navigation.panel.centralisePanelText(navigation.text);

  if (player.getPlayerVec()[0]->getEfficiency() < 100)
  {
    efficiency_bar->gain(50);
  }

  player.getPlayerVec()[0]->increaseEfficiency(20);
  auto eff_str = std::to_string(player.getPlayerVec()[0]->getEfficiency());
  efficiency_text.setString("EFFICIENCY: [" + eff_str + "]");

  if (player.getPlayerVec()[0]->getTemperature() > 0)
  {
    temperature_bar->deplete(50);
  }
  player.getPlayerVec()[0]->decreaseWeaponTemp(20);
  auto temp_str = std::to_string(player.getPlayerVec()[0]->getTemperature());
  temperature_text.setString("TEMP: [" + temp_str + "]");

  if (player.getPlayerVec()[0]->getHealth() < 100)
  {
    player.getPlayerVec()[0]->restoreHealth(10);
    auto health_str = std::to_string(player.getPlayerVec()[0]->getHealth());
    health_text.setString("HEALTH: [" + health_str + "]");
    health_bar->gain(12.25F);
    damage.text.setString("Ship made some repairs!");
    damage.text.setColour(ASGE::COLOURS::LIGHTBLUE);
    damage.panel.centralisePanelText(damage.text);
  }
}
void ASGETrek::playerInStarVicinity()
{
  navigation.text.setString("A Star is in your vicinity");
  navigation.panel.centralisePanelText(navigation.text);

  if (player.getPlayerVec()[0]->getEfficiency() > 0)
  {
    efficiency_bar->deplete(50);
  }
  player.getPlayerVec()[0]->decreaseEfficiency(20);

  if (player.getPlayerVec()[0]->getShields() > 0)
  {
    shield_bar->deplete(25.0F);
  }

  else if (player.getPlayerVec()[0]->getShields() < 1)
  {
    health_bar->deplete(20.0F);
  }

  if (player.getPlayerVec()[0]->getTemperature() < 100)
  {
    temperature_bar->gain(50);
    auto damage_str = "Player took " + std::to_string(20) + " heat damage";
    damage.text.setString(damage_str);
    damage.text.setColour(ASGE::COLOURS::ORANGE);
    damage.panel.centralisePanelText(damage.text);
  }

  player.getPlayerVec()[0]->takeDamage(10);
  player.getPlayerVec()[0]->increaseWeaponTemp(20);

  auto health_str = std::to_string(player.getPlayerVec()[0]->getHealth());
  auto shield_str = std::to_string(player.getPlayerVec()[0]->getShields());
  auto eff_str    = std::to_string(player.getPlayerVec()[0]->getEfficiency());
  auto temp_str   = std::to_string(player.getPlayerVec()[0]->getTemperature());
  health_text.setString("HEALTH: [" + health_str + "]");
  shields_text.setString("SHIELDS: [" + shield_str + "]");
  efficiency_text.setString("EFFICIENCY: [" + eff_str + "]");
  temperature_text.setString("TEMP: [" + temp_str + "]");
}

void ASGETrek::updateStatusBars()
{
  efficiency_bar->deplete(50);
  player.getPlayerVec()[0]->decreaseEfficiency(20);
  auto eff_str = std::to_string(player.getPlayerVec()[0]->getEfficiency());
  efficiency_text.setString("EFFICIENCY: [" + eff_str + "]");

  temperature_bar->gain(50);
  player.getPlayerVec()[0]->increaseWeaponTemp(20);
  auto temp_str = std::to_string(player.getPlayerVec()[0]->getTemperature());
  temperature_text.setString("TEMP: [" + temp_str + "]");

  if (player.getPlayerVec()[0]->getShields() > 0)
  {
    shield_bar->deplete(25.0F);
  }

  else if (player.getPlayerVec()[0]->getShields() < 1)
  {
    health_bar->deplete(20.0F);
  }

  auto health_str = std::to_string(player.getPlayerVec()[0]->getHealth());
  auto shield_str = std::to_string(player.getPlayerVec()[0]->getShields());
  health_text.setString("HEALTH: [" + health_str + "]");
  shields_text.setString("SHIELDS: [" + shield_str + "]");
  mongol_count_text.setString("MONGOLS: [" + std::to_string(mongol_counter) + "]");
}

void ASGETrek::mongolCounterUpdate()
{
  mongol_counter--;
  mongol_count_text.setString("MONGOLS: [" + std::to_string(mongol_counter) + "]");
  icon_counter--;
  mongol_icon->getEnemyVec()[icon_counter]->setIsVisible(false);
}

void ASGETrek::mongolDeathReportUpdate()
{
  navigation.text.setString("Startled, the mongols have scattered!");
  navigation.panel.centralisePanelText(navigation.text);
  navigation.text.setColour(ASGE::COLOURS::VIOLET);
  damage.text.setString("Mongol Destroyed!");
  damage.text.setColour(ASGE::COLOURS::LIMEGREEN);
  damage.panel.centralisePanelText(damage.text);
}

int ASGETrek::getMongolsInQuad() const
{
  return FRONT;
}
void ASGETrek::grabMongolsInQuadFrom(int front)
{
  FRONT = front;
}

int ASGETrek::getMongolTotal() const
{
  return MONGOL_TOTAL;
}
void ASGETrek::setMongolTotal(int mongolTotal)
{
  MONGOL_TOTAL = mongolTotal;
}
