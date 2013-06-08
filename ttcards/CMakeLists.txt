project ( ttcards )

set ( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR} )

set ( TTCARDS_SOURCE
      ${SRC_DIR}/board.cpp
      ${SRC_DIR}/card.cpp
      ${SRC_DIR}/card_debug.cpp
      ${SRC_DIR}/card_view.cpp
      ${SRC_DIR}/card_collection.cpp
      ${SRC_DIR}/card_hand.cpp
      ${SRC_DIR}/card_rules.cpp
      ${SRC_DIR}/cfg.cpp
      ${SRC_DIR}/main.cpp
      ${SRC_DIR}/player.cpp
      #${SRC_DIR}/cpu_player.cpp
      ${SRC_DIR}/ttcards.cpp
      ${SRC_DIR}/GameOverState.cpp
      ${SRC_DIR}/CardsMenuState.cpp
    )

find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )

find_package ( SDL_image REQUIRED )
include_directories ( ${SDLIMAGE_INCLUDE_DIR} )

find_package ( SDL_mixer REQUIRED )
include_directories ( ${SDLMIXER_INCLUDE_DIR} )

find_package ( SDL_ttf REQUIRED )
include_directories ( ${SDLTTF_INCLUDE_DIR} )

link_libraries (  ttcards # makefile target
                  ${SDL_LIBRARY}
                  SDLmain
                  gamelib
                  #nomjson
                  json_spirit
                )

add_executable ( ttcards ${TTCARDS_SOURCE} )

# app target for Sublime Build

add_custom_target ( run
                    COMMAND open --wait-apps --fresh -a "Terminal" ${EXECUTABLE_OUTPUT_PATH}/ttcards
                  )
