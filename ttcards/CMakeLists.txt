cmake_minimum_required ( VERSION 2.8 )

set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 1 )
set ( PROJECT_VERSION_PATCH 0 )

project ( ttcards ) # $PROJECT_NAME

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

find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )
set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${SDL_LIBRARY} )
set ( REQUIRED_LIBS ${REQUIRED_LIBS} "SDLmain" ) # FIXME

if ( NOT TTCARDS_NOMLIB_COMPILE )
  find_package ( nomlib REQUIRED )
  include_directories ( "${NOMLIB_INCLUDE_DIR}/nomlib" ) # FIXME
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${NOMLIB_LIBRARY} )
else ()
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} "gamelib" ) # FIXME
endif ( NOT TTCARDS_NOMLIB_COMPILE )

# FIXME
if ( NOT TTCARDS_JSON_SPIRIT_COMPILE )
  find_library ( json_spirit REQUIRED )
  include_directories ( /usr/local/include/json_spirit ) # FIXME
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${JSON_SPIRIT_LIBRARY} )
else ()
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} "json_spirit" ) # FIXME
endif ( NOT TTCARDS_JSON_SPIRIT_COMPILE )

link_libraries (  ${PROJECT_NAME} # makefile target
                  ${REQUIRED_LIBS}
                  #nomjson
                )

add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )

#     Run app target for Sublime Text Editor Custom Build Settings

add_custom_target ( run
                    COMMAND open --wait-apps --fresh -a "Terminal" ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}
                  )

#     FIXME: doxygen docs generation
add_custom_target ( ttcards-docs
                    COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE}
                  )

if ( CMAKE_BUILD_TYPE STREQUAL Release OR CMAKE_BUILD_TYPE STREQUAL MinSizeRel )

  #     Install executable
  install ( TARGETS ${PROJECT_NAME} DESTINATION bin )

  #     Install game data
  install ( FILES ${TTCARDS_DATA} DESTINATION share/${PROJECT_NAME}/data )

  install ( DIRECTORY ${TTCARDS_DATA_AUDIO_DIR} DESTINATION share/${PROJECT_NAME}/data OPTIONAL )

  install ( DIRECTORY ${TTCARDS_DATA_FONTS_DIR} DESTINATION share/${PROJECT_NAME}/data )

  install ( DIRECTORY ${TTCARDS_DATA_IMAGES_DIR} DESTINATION share/${PROJECT_NAME}/data )

endif ( CMAKE_BUILD_TYPE STREQUAL Release OR CMAKE_BUILD_TYPE STREQUAL MinSizeRel )
