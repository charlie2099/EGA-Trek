#include "gamelib/Scenes/LoseScene.hpp"

bool LoseScene::init(ASGE::Renderer* renderer)
{
  loadLoseFont(renderer);

  lose_title_text.setString("DEFEAT")
    .setZOrder(101)
    .setScale(0.05F)
    .setPositionX((window_width - lose_title_text.getWidth()) / 2.0F)
    .setPositionY((window_height + lose_title_text.getHeight()) / 2.0F)
    .setColour(ASGE::COLOURS::ORANGERED);

  return false;
}

void LoseScene::update()
{
  float original_size = 1;
  if (lose_title_text.getScale() < original_size)
  {
    enlargeDefeatText();
  }
}

void LoseScene::render(ASGE::Renderer* renderer)
{
  renderer->renderText(lose_title_text);
}

void LoseScene::loadLoseFont(ASGE::Renderer* renderer)
{
  if (file2.open(
        "/data/fonts/goldman/Goldman-Bold.ttf", ASGE::FILEIO::File::IOMode::READ))
  {
    using Buffer  = ASGE::FILEIO::IOBuffer;
    Buffer buffer = file2.read();
    if (buffer.length != 0U)
    {
      int font_index = renderer->loadFontFromMem(
        "GOLDMAN_BOLD",
        buffer.as_unsigned_char(),
        static_cast<unsigned int>(buffer.length),
        100);
      lose_title_text.setFont(renderer->getFont(font_index));
    }
    file2.close();
  }
}

void LoseScene::enlargeDefeatText()
{
  lose_title_text.setScale(lose_title_text.getScale() + 0.01F)
    .setPositionX((window_width - lose_title_text.getWidth()) / 2.0F)
    .setPositionY((window_height + lose_title_text.getHeight()) / 2.0F);
}
