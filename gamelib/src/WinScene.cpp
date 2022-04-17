#include "gamelib/Scenes/WinScene.hpp"

bool WinScene::init(ASGE::Renderer* renderer)
{
  loadWinFont(renderer);

  win_title_text.setString("VICTORY")
    .setZOrder(101)
    .setScale(0.05F)
    .setPositionX((window_width - win_title_text.getWidth()) / 2.0F)
    .setPositionY((window_height + win_title_text.getHeight()) / 2.0F)
    .setColour(ASGE::COLOURS::AQUA);

  return false;
}

/*void WinScene::keyInputs(ASGE::SharedEventData data)
{

}*/

void WinScene::update()
{
  float original_size = 1;
  if (win_title_text.getScale() < original_size)
  {
    enlargeVictoryText();
  }
}

void WinScene::render(ASGE::Renderer* renderer)
{
  renderer->renderText(win_title_text);
}

void WinScene::loadWinFont(ASGE::Renderer* renderer)
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
      win_title_text.setFont(renderer->getFont(font_index));
    }
    file2.close();
  }
}

void WinScene::enlargeVictoryText()
{
  win_title_text.setScale(win_title_text.getScale() + 0.01F)
    .setPositionX((window_width - win_title_text.getWidth()) / 2.0F)
    .setPositionY((window_height + win_title_text.getHeight()) / 2.0F);
}
