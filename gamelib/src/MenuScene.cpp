#include "gamelib/Scenes/MenuScene.hpp"

bool MenuScene::init(ASGE::Renderer* renderer)
{
  brief              = std::make_unique<PanelSetup>();
  background         = std::make_unique<PanelSetup>();
  right_arrow_button = std::make_unique<Button>();

  loadGoldmanBoldFont(renderer);
  loadGoldmanRegularFont(renderer);

  initPanel(brief->panel, renderer);
  initGameObject(brief->container, renderer);

  initBriefTitleText();
  initUserInputText();
  initBriefPanel();
  initQuestionText();
  briefPages();
  loadGoldmanRegularFontFix(renderer);

  float x = brief->panel.getXPos() + brief->panel.getSprite()->width() - 65;
  float y = brief->panel.getYPos() + brief->panel.getSprite()->height() - 70;
  initRightArrowButton(renderer, x, y);
  initBriefText(x, y);

  return true;
}
void MenuScene::initBriefTitleText()
{
  brief->title.setString("BRIEF").setScale(0.7F).setZOrder(100).setColour(
    ASGE::COLOURS::AQUA);
}
void MenuScene::initUserInputText()
{
  userInput_text.setZOrder(1000)
    .setScale(0.65F)
    .setColour(ASGE::COLOURS::YELLOW)
    .setPositionX(500)
    .setPositionY(500);
}
void MenuScene::initRightArrowButton(ASGE::Renderer* renderer, float x, float y)
{
  right_arrow_button->init("/data/images/right_arrow.png", renderer);
  right_arrow_button->size(50, 50);
  right_arrow_button->position(x, y);
  right_arrow_button->setIsVisible(false);
}
void MenuScene::initBriefText(float x, float y)
{
  brief->text.setString("Press Right Arrow")
    .setScale(0.5F)
    .setZOrder(1)
    .setPositionX(x - 160)
    .setPositionY(y + 30)
    .setColour(ASGE::COLOURS::AQUA);
}
void MenuScene::briefPages()
{
  std::array<std::string, 5> pages = { "The Mongols have invaded!,\nYou must "
                                       "eliminate the threat before its\ntoo late!",
                                       "Move anywhere adjacent to a "
                                       "starbase\nstation to recover 10+ health per "
                                       "turn. ",
                                       "However, avoid being anywhere adjacent\nto "
                                       "a star or risk losing 10+ health per\nturn!",
                                       "To attack the mongols, your ship must\nbe "
                                       "situated adjacent to a mongol in "
                                       "the\nsector grid. You will then be able to "
                                       "attack.",
                                       "ENTER 'M' To MOVE\nENTER 'L' To fire "
                                       "LASERS\nENTER 'T' To fire TORPEDOS\nENTER "
                                       "'S' To GIVE UP" };

  for (size_t i = 0; i < 5; i++)
  {
    brief->text_vec.emplace_back(std::make_unique<ASGE::Text>());
    brief->text_vec[i]->setString(pages[i]);
    brief->text_vec[i]->setScale(0.60F);
    brief->text_vec[i]->setColour(ASGE::COLOURS::WHITE);
    brief->text_vec[i]->setPositionX(brief->panel.getXPos() + 20);
    brief->text_vec[i]->setPositionY(brief->panel.getYPos() * 2);
  }
}

void MenuScene::initQuestionText()
{
  std::array<std::string, 4> questions = { "Will you require a briefing?:",
                                           "Please enter your name:",
                                           "For verification, enter your command "
                                           "level (1-5):",
                                           "Lt. Commander, please enter "
                                           "self-destruct password:" };

  for (size_t i = 0; i < 4; i++)
  {
    background->text_vec.emplace_back(std::make_unique<ASGE::Text>());
    background->text_vec[i]->setString(questions[i]);
    background->text_vec[i]->setScale(0.70F);
    background->text_vec[i]->setZOrder(1);
    background->text_vec[i]->setColour(ASGE::COLOURS::WHITE);
    background->text_vec[i]->setPositionX(100);
    background->text_vec[i]->setPositionY((static_cast<float>(i) * 75.F) + 275.0F);
  }
}

void MenuScene::initBriefPanel()
{
  brief->container.getSprite()->width(550);
  brief->container.getSprite()->height(260);
  brief->container.getSprite()->xPos(
    window_width - brief->container.getSprite()->width() - 20);
  brief->container.getSprite()->yPos(brief->container.getSprite()->height() / 2.7F);
  brief->container.getSprite()->setGlobalZOrder(1);
  brief->panel.createPanelWithoutText(
    brief->container, ASGE::COLOURS::GRAYBLACK, ASGE::COLOURS::LIGHTGREY);
  brief->title.setPositionX(
    (brief->panel.getXPos() + brief->panel.getSprite()->width() / 2) -
    brief->title.getWidth() / 2);
  brief->title.setPositionY(
    (brief->panel.getYPos() + brief->panel.getSprite()->height() / 7) -
    brief->title.getHeight() / 2);

  brief->panel.setIsVisible(false);
}

void MenuScene::update()
{
  textAlign();
  assigningInputBoxToState();
  activeQuestionHighlight();
  updateBriefPages();
}
void MenuScene::updateBriefPages()
{
  for (size_t i = 0; i < 5; ++i)
  {
    if (i == brief_counter)
    {
      brief->text_vec[i]->setZOrder(1);
    }
    else
    {
      brief->text_vec[i]->setZOrder(0);
    }
  }
}

void MenuScene::activeQuestionHighlight()
{
  for (size_t i = 0; i < 4; ++i)
  {
    if (static_cast<size_t>(active_text) == i)
    {
      if (i != 0)
      {
        /// colours previous question white as long as not on the first one
        background->text_vec[i - 1]->setColour(ASGE::COLOURS::WHITE);
      }
      background->text_vec[static_cast<size_t>(i)]->setColour(ASGE::COLOURS::BLUE);
    }
  }
}

void MenuScene::textAlign()
{
  for (size_t i = 0; i < background->text_vec.size(); ++i)
  {
    if (static_cast<size_t>(active_text) == i)
    {
      userInput_text.setPositionX(
        background->text_vec[i]->getPosition().x +
        background->text_vec[i]->getWidth() + 15);
      userInput_text.setPositionY(
        background->text_vec[i]->getPosition().y +
        background->text_vec[i]->getHeight());
    }
  }
}

void MenuScene::render(ASGE::Renderer* renderer)
{
  renderer->renderText(userInput_text);

  for (auto& i : background->text_vec)
  {
    renderer->renderText(*i);
  }

  if (brief->panel.isVisible())
  {
    renderer->renderSprite(*brief->container.getSprite());
    brief->panel.renderPanel(renderer);
    renderer->renderText(brief->title);
    renderer->renderText(brief->text);
    for (auto& i : brief->text_vec)
    {
      renderer->renderText(*i);
    }
  }

  if (right_arrow_button->isVisible())
  {
    right_arrow_button->render(renderer);
  }
}

void MenuScene::keyInputs(ASGE::SharedEventData data)
{
  const auto* key = dynamic_cast<const ASGE::KeyEvent*>(data.get());

  generalKeyInputs(key);

  if (brief->panel.isVisible())
  {
    makeBriefControlsActive(key);
  }

  resetBriefPagesCounter();

  if (key->key == ASGE::KEYS::KEY_ENTER && key->action == ASGE::KEYS::KEY_PRESSED)
  {
    nextQuestion();
  }

  if (key->action == ASGE::KEYS::KEY_PRESSED)
  {
    userInput_text.setString((input_text += static_cast<char>(key->key)) + "|");
  }
}
void MenuScene::generalKeyInputs(const ASGE::KeyEvent* key)
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
  }
}
void MenuScene::nextQuestion()
{
  switch (active_text)
  {
    case ActiveText::BRIEF:
      briefInputOptions();
      break;
    case ActiveText::USERNAME:
      usernameInputOptions();
      break;
    case ActiveText::DIFFICULTY:
      difficultyInputOptions();
      break;
    case ActiveText::PASSWORD:
      passwordInputOptions();
      break;
  }
  input_text.clear();
}
void MenuScene::resetBriefPagesCounter()
{
  if (brief_counter == 5)
  {
    brief_counter = 0;
  }
}
void MenuScene::makeBriefControlsActive(const ASGE::KeyEvent* key)
{
  if (key->key == ASGE::KEYS::KEY_RIGHT && key->action == 1)
  {
    brief_counter++;
    right_arrow_button->setColour(ASGE::COLOURS::AQUA);
  }
  else
  {
    right_arrow_button->setColour(ASGE::COLOURS::LIGHTGREY);
  }
}

void MenuScene::briefInputOptions()
{
  if (input_text.find("YES") != std::string::npos)
  {
    std::string brief_string; // TODO: Make global
    brief_string = input_text;
    right_arrow_button->setIsVisible(true);
    brief->panel.setIsVisible(true);
  }
  else if (input_text.find("NO") != std::string::npos)
  {
    background->text_vec[0]->setString(
      background->text_vec[0]->getString() + "  " + input_text);
    text_selected_counter++;
    right_arrow_button->setIsVisible(false);
    brief->panel.setIsVisible(false);
  }
}

void MenuScene::usernameInputOptions()
{
  if (input_text.length() > 1)
  {
    background->text_vec[1]->setString(
      background->text_vec[1]->getString() + "  " + input_text);
    setName(input_text);
    text_selected_counter++;
  }
}

void MenuScene::difficultyInputOptions()
{
  std::array<char, 5> mode = { '1', '2', '3', '4', '5' };
  for (size_t i = 0; i < 5; ++i)
  {
    if (input_text.ends_with(mode[i]))
    {
      background->text_vec[2]->setString(
        background->text_vec[2]->getString() + "  " + mode[i]);
      setDifficulty(static_cast<size_t>(mode[i]) - 48); /// ASCII
      std::cout << "Difficulty choice: " << getDifficulty() << std::endl;
      text_selected_counter++;
    }
  }
}

void MenuScene::passwordInputOptions()
{
  if (input_text.length() > 1)
  {
    std::cout << input_text << std::endl;
    setPassword(input_text);
    gameState() = GameState::GAME;
  }
}

void MenuScene::assigningInputBoxToState() /// Add to scene class
{
  switch (text_selected_counter)
  {
    case 0:
      active_text = ActiveText::BRIEF;
      break;
    case 1:
      active_text = ActiveText::USERNAME;
      break;
    case 2:
      active_text = ActiveText::DIFFICULTY;
      break;
    case 3:
      active_text = ActiveText::PASSWORD;
      break;
  }
}

void MenuScene::setDifficulty(size_t difficulty)
{
  DIFFICULTY = difficulty;
}

size_t MenuScene::getDifficulty() const
{
  return DIFFICULTY;
}

void MenuScene::setPassword(std::string password)
{
  PASSWORD = std::move(password);
}

std::string MenuScene::getPassword() const
{
  return PASSWORD;
}

void MenuScene::setName(std::string name)
{
  NAME = std::move(name);
}

std::string MenuScene::getName() const
{
  return NAME;
}
