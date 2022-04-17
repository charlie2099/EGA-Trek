#include "gamelib/Scenes/SceneManager.hpp"

bool SceneManager::initPanel(
  Panel& pan /*, const std::string& path*/, ASGE::Renderer* rend)
{
  if (!pan.loadSprite(rend, "data/images/cube.png"))
  {
    return false;
  }
  return false;
}

bool SceneManager::initGameObject(
  GameObject& obj /*, const std::string& path*/, ASGE::Renderer* rend)
{
  if (!obj.loadSprite(rend, "data/images/cube.png"))
  {
    return false;
  }
  return false;
}

void SceneManager::loadGoldmanRegularFontFix(ASGE::Renderer* renderer)
{
  if (file3.open("/data/fonts/goldman/Goldman-Regular.ttf", File::IOMode::READ))
  {
    using Buffer  = ASGE::FILEIO::IOBuffer;
    Buffer buffer = file3.read();
    if (buffer.length != 0U)
    {
      int font_index = renderer->loadFontFromMem(
        "GOLDMAN_REGULAR_NEW",
        buffer.as_unsigned_char(),
        static_cast<unsigned int>(buffer.length),
        64);
      for (auto& i : background->text_vec)
      {
        i->setFont(renderer->getFont(font_index));
      }
      for (auto& i : brief->text_vec)
      {
        i->setFont(renderer->getFont(font_index));
      }
    }
    file3.close();
  }
}
void SceneManager::loadGoldmanRegularFont(ASGE::Renderer* renderer)
{
  if (file2.open("/data/fonts/goldman/Goldman-Regular.ttf", File::IOMode::READ))
  {
    using Buffer  = ASGE::FILEIO::IOBuffer;
    Buffer buffer = file2.read();
    if (buffer.length != 0U)
    {
      int font_index = renderer->loadFontFromMem(
        "GOLDMAN_REGULAR",
        buffer.as_unsigned_char(),
        static_cast<unsigned int>(buffer.length),
        64);
      brief->title.setFont(renderer->getFont(font_index));
      brief->text.setFont(renderer->getFont(font_index));
      userInput_text.setFont(renderer->getFont(font_index));
    }
    file2.close();
  }
}
void SceneManager::loadGoldmanBoldFont(ASGE::Renderer* renderer)
{
  if (file.open("/data/fonts/goldman/Goldman-Bold.ttf", File::IOMode::READ))
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
      background->title.setFont(renderer->getFont(font_index));
    }
    file.close();
  }
}
bool SceneManager::init(ASGE::Renderer* renderer)
{
  background = std::make_unique<PanelSetup>();

  loadGoldmanBoldFont(renderer);

  initPanel(background->panel, renderer);
  initGameObject(background->container, renderer);

  background->title.setString("EGA Trek")
    .setZOrder(101)
    .setPositionX((window_width - background->title.getWidth()) / 2.0F)
    .setPositionY((window_height + background->title.getHeight()) / 12.0F)
    .setColour(ASGE::COLOURS::WHITE);

  initBackgroundPanel();
  return false;
}

void SceneManager::renderScene(ASGE::Renderer* renderer)
{
  background->panel.renderPanel(renderer);
  renderer->renderSprite(*background->container.getSprite());
  renderer->renderText(background->title);
}

void SceneManager::initBackgroundPanel()
{
  background->container.getSprite()->xPos(5);
  background->container.getSprite()->yPos(80);
  background->container.getSprite()->width(window_width - 10);
  background->container.getSprite()->height(window_height - 85);
  background->panel.createPanelWithoutText(
    background->container, ASGE::COLOURS::BLACK, ASGE::COLOURS::LIGHTGREY);
}
