#include "ASGEGame.hpp"
#include <Engine/GameSettings.hpp>
#include <Engine/Logger.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  ASGE::GameSettings game_settings;
  game_settings.window_title  = "ASGETrek";
  game_settings.window_width  = 1280;
  game_settings.window_height = 720;
  game_settings.mode          = ASGE::GameSettings::WindowMode::WINDOWED;
  game_settings.fixed_ts      = 30;
  game_settings.fps_limit     = 120;
  game_settings.msaa_level    = 1;

  Logging::INFO("Launching Game!");
  ASGETrek game(game_settings);
  if (game.init())
  {
    game.run();
  }
  return 0;
}
