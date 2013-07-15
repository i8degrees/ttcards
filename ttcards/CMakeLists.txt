cmake_minimum_required ( VERSION 2.8 )

set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 7 )
set ( PROJECT_VERSION_PATCH 0 )

project ( ttcards ) # $PROJECT_NAME

set ( SRC_DIR "src" ) # ttcards/src

set ( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR} )

# relative path -- from project source -- to game data
set ( TTCARDS_DATA_DIR "data" )
#set ( TTCARDS_RESOURCES ) # TODO: BUILD_OSXAPP

# relative path -- from data dir -- to audio assets
set ( TTCARDS_DATA_AUDIO_DIR "${TTCARDS_DATA_DIR}/audio" )

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

#     Required Dependencies:
find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )
set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${SDL_LIBRARY} )
set ( REQUIRED_LIBS ${REQUIRED_LIBS} "SDLmain" ) # FIXME

if ( NOT TTCARDS_NOMLIB_COMPILE )
  find_package ( nomlib REQUIRED )
  include_directories ( "${NOMLIB_INCLUDE_DIR}/nomlib" ) # FIXME
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${NOMLIB_LIBRARY} )
endif ( NOT TTCARDS_NOMLIB_COMPILE )

if ( NOT TTCARDS_JSON_SPIRIT_COMPILE )
  find_library ( json_spirit REQUIRED )
  include_directories ( /usr/local/include/json_spirit ) # FIXME
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${JSON_SPIRIT_LIBRARY} )
endif ( NOT TTCARDS_JSON_SPIRIT_COMPILE )

#     Linking Dependencies & Packaging / Installation:

link_libraries ( ${PROJECT_NAME} ${REQUIRED_LIBS} )

if ( BUILD_OSXAPP ) # OSX App Bundle
  set ( PROJECT_VERSION_TYPE 2 )
  message ( "Building OSX App Bundle" )

  # ... [Copy Resources from $TTCARDS_DATA_DIR ] ...
  #
  # 1. https://github.com/rpavlik/cmake-modules/blob/master/CopyResourcesToBuildTree.cmake

  add_executable  ( ${PROJECT_NAME} MACOSX_BUNDLE
                    ${TTCARDS_SOURCE} ${TTCARDS_RESOURCES}
                  )

  set_target_properties ( ${PROJECT_NAME} PROPERTIES
                          MACOSX_BUNDLE_INFO_PLIST
                          ${PROJECT_SOURCE_DIR}/${TTCARDS_DATA_DIR}/Info.plist
                        )

  set_source_files_properties ( ${TTCARDS_RESOURCES} PROPERTIES
                                MACOSX_PACKAGE_LOCATION Resources
                              )

else ( NOT BUILD_OSXAPP ) # POSIX Installation
  message ( "Building POSIX install" )
  add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )
endif ( BUILD_OSXAPP )

#     Run app target for Sublime Text Editor Custom Build Settings
add_custom_target ( run
                    COMMAND open --wait-apps --fresh -a "Terminal" ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}
                  )

#     FIXME: doxygen docs generation
add_custom_target ( ttcards-docs
                    COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE}
                  )

#     Install game data; do not include dotfiles -- *cough* .DS_Store *cough*
install ( TARGETS ${PROJECT_NAME} DESTINATION bin
          CONFIGURATIONS Release
        )

install ( DIRECTORY ${TTCARDS_DATA_DIR} DESTINATION share/${PROJECT_NAME}
          CONFIGURATIONS Release
          PATTERN ".*" EXCLUDE
          PATTERN "audio/*" EXCLUDE
        )

#     Install optional audio data
install ( DIRECTORY ${TTCARDS_DATA_AUDIO_DIR} DESTINATION share/${PROJECT_NAME}/${TTCARDS_DATA_DIR}
          OPTIONAL
          CONFIGURATIONS Release
          PATTERN ".*" EXCLUDE
        )

# TODO: this is for tracking installation paths, versions, etc.
configure_file  ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.hpp.in"
                  "${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/version.hpp"
                )
