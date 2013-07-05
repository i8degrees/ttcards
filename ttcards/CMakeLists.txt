project ( ttcards )

set ( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR} )

# path to game data
set ( TTCARDS_DATA_DIR data )

# path to game audio assets
set ( TTCARDS_DATA_AUDIO_DIR ${TTCARDS_DATA_DIR}/audio )

# path to game font assets
set ( TTCARDS_DATA_FONTS_DIR ${TTCARDS_DATA_DIR}/fonts )

# path to game image assets
set ( TTCARDS_DATA_IMAGES_DIR ${TTCARDS_DATA_DIR}/images )

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
      ${SRC_DIR}/Game.cpp
      ${SRC_DIR}/Game_debug.cpp
      ${SRC_DIR}/GameOverState.cpp
      ${SRC_DIR}/CardsMenuState.cpp
)

# General game assets
set ( TTCARDS_DATA
      ${TTCARDS_DATA_DIR}/cards.json
      ${TTCARDS_DATA_DIR}/icon.bmp
)

# Audio assets
set ( TTCARDS_DATA_AUDIO
      ${TTCARDS_DATA_AUDIO_DIR}/05_TheWinner.ogg
      ${TTCARDS_DATA_AUDIO_DIR}/06_ShuffleOrBoogie.ogg
      ${TTCARDS_DATA_AUDIO_DIR}/15_ShuffleOrBoogie.ogg
      ${TTCARDS_DATA_AUDIO_DIR}/cursor_cancel.wav
      ${TTCARDS_DATA_AUDIO_DIR}/cursor_move.wav
      ${TTCARDS_DATA_AUDIO_DIR}/cursor_wrong.wav
)

# Font assets
set ( TTCARDS_DATA_FONTS
      ${TTCARDS_DATA_FONTS_DIR}/EnvyCodeRb.ttf
      ${TTCARDS_DATA_FONTS_DIR}/VIII.png
      ${TTCARDS_DATA_FONTS_DIR}/VIII_small.png
      ${TTCARDS_DATA_FONTS_DIR}/VIII_cards.png
)

# Image assets
set ( TTCARDS_DATA_IMAGES
      ${TTCARDS_DATA_IMAGES_DIR}/Backgrounds.png
      ${TTCARDS_DATA_IMAGES_DIR}/board.png
      ${TTCARDS_DATA_IMAGES_DIR}/cursors.png
      ${TTCARDS_DATA_IMAGES_DIR}/elements.png
      ${TTCARDS_DATA_IMAGES_DIR}/faces.png
      ${TTCARDS_DATA_IMAGES_DIR}/game-over_background.png
      ${TTCARDS_DATA_IMAGES_DIR}/menu_elements.png
)

find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )

find_package ( SDL_image REQUIRED )
include_directories ( ${SDLIMAGE_INCLUDE_DIR} )

find_package ( SDL_mixer REQUIRED )
include_directories ( ${SDLMIXER_INCLUDE_DIR} )

find_package ( SDL_ttf REQUIRED )
include_directories ( ${SDLTTF_INCLUDE_DIR} )

# FIXME
if ( TTCARDS_NOMLIB_COMPILE STREQUAL off )
  include_directories ( /usr/local/include/gamelib )
  #find_package ( gamelib REQUIRED )
  #include_directories ( ${GAMELIB_INCLUDE_DIR} )
endif ()

# FIXME
if ( TTCARDS_JSON_SPIRIT_COMPILE STREQUAL off )
  include_directories ( /usr/local/include/json_spirit )
  #find_package ( json_spirit REQUIRED )
  #include_directories ( ${JSON_SPIRIT_INCLUDE_DIR} )
endif ()

link_libraries (  ttcards # makefile target
                  ${SDL_LIBRARY}
                  SDLmain
                  gamelib
                  #nomjson
                  json_spirit
                )

add_executable ( ttcards ${TTCARDS_SOURCE} )

#     Run app target for Sublime Text Editor Custom Build Settings

add_custom_target ( run
                    COMMAND open --wait-apps --fresh -a "Terminal" ${EXECUTABLE_OUTPUT_PATH}/ttcards
                  )

#     FIXME: doxygen docs generation
add_custom_target ( ttcards-docs
                    COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE}
                  )

#     Install executable

install ( TARGETS ttcards
          DESTINATION bin
        )

#     Install game data

install ( FILES ${TTCARDS_DATA}
          DESTINATION share/ttcards
        )

install ( FILES ${TTCARDS_DATA_AUDIO}
          DESTINATION share/ttcards/audio
        )

install ( FILES ${TTCARDS_DATA_FONTS}
          DESTINATION share/ttcards/fonts
        )

install ( FILES ${TTCARDS_DATA_IMAGES}
          DESTINATION share/ttcards/images
        )

#     Uninstall target

configure_file  (
                  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                  IMMEDIATE @ONLY
                )

add_custom_target ( uninstall
                    COMMAND ${CMAKE_COMMAND} -P
                    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                  )
