# Project source files

# Use common build output directories for MSVCPP && Xcode project files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")

# This module is required for finding the current git SHA we are at
include(GetGitRevisionNumber)

# Used in version.hpp.in
git_rev_number(GIT_REVISION "--short")
set(PROJECT_VERSION_GIT_REV "${GIT_REVISION}")

# >> Auto-generated build-time variables result in modification of the built
# >> binary's run-time execution.
configure_file("${CMAKE_TEMPLATE_PATH}/version.hpp.in"
  "${CMAKE_BINARY_DIR}/version.hpp"
  @ONLY
)

add_executable(ttcards
  WIN32
  MACOSX_BUNDLE
  main.cpp
)

target_sources(ttcards
  PRIVATE
    #${CMAKE_BINARY_DIR}/version.cpp # auto-generated
    Board.cpp
    BoardTile.cpp
    Card.cpp
    CardCollection.cpp
    CardHand.cpp
    CardRenderer.cpp
    CardResourceLoader.cpp
    CardRules.cpp
    CardsPageDataSource.cpp
    CPU_Player.cpp
    Game.cpp
    GameConfig.cpp
    helpers.cpp
    HumanPlayer.cpp
    IPlayer.cpp
    main.cpp
    states/CardsMenuState.cpp
    states/ConfirmationDialogState.cpp
    states/DialogCursor.cpp
    states/GameOverState.cpp
    states/GameOverStateCursor.cpp
    states/MainMenuState.cpp
    states/PauseState.cpp
    states/PlayState.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES
      Board.hpp
      BoardTile.hpp
      Card.hpp
      CardCollection.hpp
      CardHand.hpp
      CardRules.hpp
      CardsPageDataSource.hpp
      config.hpp
      CPU_Player.hpp
      debug.hpp
      Game.hpp
      GameConfig.hpp
      HumanPlayer.hpp
      IPlayer.hpp
      states/CardsMenuState.hpp
      states/ConfirmationDialogState.hpp
      states/DialogCursor.hpp
      states/GameOverState.hpp
      states/GameOverStateCursor.hpp
      states/PauseState.hpp
      states/PlayState.hpp
)

set_target_properties(ttcards
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    DEBUG_POSTFIX "-d"
    INSTALL_RPATH "$ORIGIN/../lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

target_compile_features(ttcards
  PRIVATE
    cxx_std_14
)

if(TTCARDS_ENABLE_AUDIO_UNIT)
  # >> TTCARDS_ENABLE_AUDIO is a compile definition taht is likely to go away once
  # >> it has served its purpose.
  target_compile_definitions("TTCARDS_ENABLE_AUDIO")
endif()

target_include_directories(ttcards
  PRIVATE
    ${CMAKE_BINARY_DIR} # version.hpp
    ${PROJECT_SOURCE_DIR}/src # game header files
    "~/local/include" # nomlib
)

# See WARNINGS_AS_ERRORS cmake project option
target_link_libraries(ttcards
  PRIVATE
    project_warnings
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(ttcards
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_link_libraries(ttcards
  PUBLIC
    nomlib-actions
    nomlib-core
    nomlib-core
    nomlib-graphics
    nomlib-gui
    nomlib-ptree
    nomlib-serializers
    nomlib-system
    nomlib-system
  PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    $<IF:$<TARGET_EXISTS:RocketCore>,RocketCore,RocketCore-static>
    $<IF:$<TARGET_EXISTS:RocketControls>,RocketControls,RocketControls-static>
    $<IF:$<TARGET_EXISTS:RocketDebugger>,RocketDebugger,RocketDebugger-static>
)

if(NOM_USE_OPENAL AND TTCARDS_ENABLE_AUDIO_UNIT)
  target_link_libraries(ttcards
    PUBLIC
      nomlib-audio
    PRIVATE
      $<IF:$<TARGET_EXISTS:OpenAL::OpenAL>,OpenAL::OpenAL,OpenAL::OpenAL-static>
      $<IF:$<TARGET_EXISTS:Sndfile::sndfile>,Sndfile::sndfile,Sndfile::sndfile-static>
  )
endif(NOM_USE_OPENAL AND TTCARDS_ENABLE_AUDIO_UNIT)

