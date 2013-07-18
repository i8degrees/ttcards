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
set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${SDL_LIBRARY} "SDLmain" ) #FIXME

if ( NOT TTCARDS_NOMLIB_COMPILE )
  find_package ( nomlib REQUIRED )
  include_directories ( "${NOMLIB_INCLUDE_DIR}" )
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${NOMLIB_LIBRARY} )
endif ( NOT TTCARDS_NOMLIB_COMPILE )

if ( NOT TTCARDS_JSON_SPIRIT_COMPILE )
  find_library ( json_spirit REQUIRED )
  include_directories ( /usr/local/include/json_spirit ) # FIXME
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${JSON_SPIRIT_LIBRARY} )
endif ( NOT TTCARDS_JSON_SPIRIT_COMPILE )

#     Linking Dependencies & Packaging / Installation:

if ( BUILD_OSXAPP ) # OSX App Bundle
  set ( PROJECT_VERSION_TYPE 2 )
  message ( "Building OSX App Bundle" )

  # Reset bin path; we are basically in Release mode
  # (This might change in the future... need to rethink structure of all this)
  set ( EXECUTABLE_OUTPUT_PATH )

  add_executable  ( ${PROJECT_NAME} MACOSX_BUNDLE ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${REQUIRED_LIBS} )

  install ( TARGETS ${PROJECT_NAME} DESTINATION bin )

#     Install dependencies
#     TODO: when ${NOMLIB_LIBRARY} is a dylib, put it under bin/${PROJECT_NAME}.app/Contents/Libraries
  if ( IS_DIRECTORY ${NOMLIB_LIBRARY} )

    install ( DIRECTORY ${NOMLIB_LIBRARY} DESTINATION bin/${PROJECT_NAME}.app/Contents/Frameworks
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )
  else ( NOT IS_DIRECTORY ${NOMLIB_LIBRARY} )
    # FIXME: we need the actual file, not the symbolic link
    install ( FILES ${NOMLIB_LIBRARY} DESTINATION bin/${PROJECT_NAME}.app/Contents/Frameworks )

  endif ( IS_DIRECTORY ${NOMLIB_LIBRARY} )

#     Install game data
  install ( DIRECTORY ${TTCARDS_DATA_DIR} DESTINATION bin/${PROJECT_NAME}.app/Contents/Resources
            PATTERN ".*" EXCLUDE # do not include dotfiles
            PATTERN "audio/*" EXCLUDE # do not include optional features
          )

  #   Install optional audio data
  install ( DIRECTORY ${TTCARDS_DATA_AUDIO_DIR} DESTINATION bin/${PROJECT_NAME}.app/Contents/Resources/${TTCARDS_DATA_DIR}
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles
          )

#     Install App bundle icon
  install ( FILES ${TTCARDS_DATA_DIR}/ttcards.icns DESTINATION bin/${PROJECT_NAME}.app/Contents/Resources
            OPTIONAL # do not include optional features
          )

  set_target_properties ( ${PROJECT_NAME} PROPERTIES
                          MACOSX_BUNDLE_INFO_PLIST
                          "${PROJECT_SOURCE_DIR}/cmake/Info.plist.in"
                          MACOSX_BUNDLE_BUNDLE_NAME
                          "TTcards" #${PROJECT_NAME}
                          MACOSX_BUNDLE_ICON_FILE
                          "ttcards.icns"
                          MACOSX_BUNDLE_INFO_STRING
                          "${PROJECT_NAME} (Triple Triad Remake) version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} by Jeffrey Carpenter"
                          MACOSX_BUNDLE_GUI_IDENTIFIER
                          "org.dev.${PROJECT_NAME}"
                          MACOSX_BUNDLE_SHORT_VERSION_STRING
                          "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                          MACOSX_BUNDLE_LONG_VERSION_STRING
                          "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
                          MACOSX_BUNDLE_BUNDLE_VERSION
                          "${CMAKE_VERSION}"
                          MACOSX_BUNDLE_COPYRIGHT
                          "Copyright (c) 2013 Jeffrey Carpenter; All rights reserved."
                        )

#     Run app target for Sublime Text Editor Custom Build Settings
  add_custom_target ( run COMMAND open -a "${CMAKE_INSTALL_PREFIX}/bin/ttcards.app" )

else ( NOT BUILD_OSXAPP ) # POSIX Installation

  message ( "Building POSIX install" )

  add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${REQUIRED_LIBS} )

#     Install game data;
  install ( TARGETS ${PROJECT_NAME} DESTINATION bin
            CONFIGURATIONS Release
          )

  install ( DIRECTORY ${TTCARDS_DATA_DIR} DESTINATION share/${PROJECT_NAME}
            CONFIGURATIONS Release
            PATTERN ".*" EXCLUDE # do not include dotfiles -- *cough* .DS_Store
            PATTERN "audio/*" EXCLUDE
          )

#     Install optional audio data
  install ( DIRECTORY ${TTCARDS_DATA_AUDIO_DIR} DESTINATION share/${PROJECT_NAME}/${TTCARDS_DATA_DIR}
            OPTIONAL
            CONFIGURATIONS Release
            PATTERN ".*" EXCLUDE
          )

#     Run app target for Sublime Text Editor Custom Build Settings
  add_custom_target ( run
                      COMMAND open --wait-apps --fresh -a "Terminal" ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}
                    )

endif ( BUILD_OSXAPP )

# TODO: this is for tracking installation paths, versions, etc.
configure_file  ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.hpp.in"
                  "${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/version.hpp"
                )

#     FIXME: doxygen docs generation
add_custom_target ( ttcards-docs
                    COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE}
                  )
