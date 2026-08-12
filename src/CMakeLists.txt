# Project source files

# >> Auto-generated build-time variables result in modification of the built
# >> binary's run-time execution.
configure_file("${CMAKE_TEMPLATE_PATH}/version.cpp.in"
  "${CMAKE_BINARY_DIR}/version.cpp"
)

configure_file("${CMAKE_TEMPLATE_PATH}/version.hpp.in"
  "${CMAKE_BINARY_DIR}/version.hpp"
)

add_executable(ttcards
  WIN32
  MACOSX_BUNDLE
  main.cpp
)

target_sources(ttcards
  PRIVATE
    ${CMAKE_BINARY_DIR}/version.cpp # auto-generated
    Board.cpp
    BoardTile.cpp
    CPU_Player.cpp
    Card.cpp
    CardCollection.cpp
    CardDebug.cpp
    CardHand.cpp
    CardRules.cpp
    CardView.cpp
    CardsPageDataSource.cpp
    Game.cpp
    GameConfig.cpp
    HumanPlayer.cpp
    IPlayer.cpp
    main.cpp
    resources.cpp
    states/CardsMenuState.cpp
    states/ConfirmationDialogState.cpp
    states/DialogCursor.cpp
    states/GameOverState.cpp
    states/GameOverStateCursor.cpp
    states/PauseState.cpp
    states/PlayState.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES
      Board.hpp
      BoardTile.hpp
      CPU_Player.hpp
      Card.hpp
      CardCollection.hpp
      CardDebug.hpp
      CardHand.hpp
      CardRules.hpp
      CardView.hpp
      CardsPageDataSource.hpp
      Game.hpp
      GameConfig.hpp
      HumanPlayer.hpp
      IPlayer.hpp
      config.hpp
      debug.hpp
      resources.hpp
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

target_include_directories(ttcards
  PRIVATE
    ${CMAKE_BINARY_DIR} # version.hpp
    "~/local/include" # nomlib
    ${PROJECT_SOURCE_DIR}/src # game header files
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(ttcards
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_link_libraries(ttcards
  PUBLIC
    #nomlib-audio
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

