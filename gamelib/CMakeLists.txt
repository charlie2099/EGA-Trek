## local variable for managing the gamelib target
project(GameLib)
set(GAMELIB "GameLib")

## glob is useful but not reliant in compilation so just use it for headers ##
file(GLOB_RECURSE HEADER_LIST
        LIST_DIRECTORIES FALSE
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )

set(GAMELIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game_component.cpp include/gamelib/GameObject.hpp src/GameObject.cpp include/gamelib/Grid.hpp src/Grid.cpp include/gamelib/Panel.hpp src/Panel.cpp include/gamelib/Ships/Player.hpp src/Player.cpp include/gamelib/Ships/Enemy.hpp src/Enemy.cpp include/gamelib/Ships/Starship.hpp src/Starship.cpp src/Button.cpp include/gamelib/Button.hpp include/gamelib/EnergyBar.hpp src/EnergyBar.cpp include/gamelib/Scenes/MenuScene.hpp src/MenuScene.cpp include/gamelib/Scenes/SceneManager.hpp src/SceneManager.cpp include/gamelib/Scenes/WinScene.hpp src/WinScene.cpp include/gamelib/Scenes/LoseScene.hpp src/LoseScene.cpp)

add_library(${GAMELIB} STATIC ${GAMELIB_SOURCES} "${HEADER_LIST}")
add_library(libs::${GAMELIB} ALIAS ${GAMELIB})
configure_target(${GAMELIB} ${CMAKE_CURRENT_SOURCE_DIR}/include "${HEADER_LIST}" "")
target_include_directories(${GAMELIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src )
add_asge_dependency(${GAMELIB})
