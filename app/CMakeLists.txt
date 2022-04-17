## local variable for managing the game target ##
set(GAME ${PROJECT_NAME})

## glob is useful but not reliant in compilation so just use it for headers ##
file(GLOB_RECURSE HEADER_LIST
    LIST_DIRECTORIES FALSE
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h )

## source files get listed here ##
set(GAME_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ASGEGame.cpp)

## the ASGE Game, which spawns the Game window ##
add_executable(${GAME} ${HEADER_LIST} ${GAME_SOURCES})

## configure the target. Dependency targets go in the LIBS variable i.e. soloud ##
list(APPEND LIBS "GameLib")
configure_target(${GAME} "${CMAKE_CURRENT_SOURCE_DIR}/src" "${HEADER_LIST}" "${LIBS}")
add_asge_dependency(${GAME})

## icon support, because why not ##
if (WIN32)
    set(ICON Resources/game.rc)
elseif (APPLE)
    set(ICON Resources/game.icns)
    set_source_files_properties(
            Resources/game.icns PROPERTIES
            MAXOSX_PACKAGE_LOCATION "Resources")
endif()

## game data ##
add_gamedata(${GAME} ${CMAKE_CURRENT_SOURCE_DIR}/data)