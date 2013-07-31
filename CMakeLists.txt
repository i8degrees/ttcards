cmake_minimum_required ( VERSION 2.8 )

#     Project configuration
set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 7 )
set ( PROJECT_VERSION_PATCH 0 )

project ( ttcards ) # $PROJECT_NAME

set ( SRC_DIR "src" ) # ttcards/src

# relative path -- from project source -- to game resources
set ( TTCARDS_RESOURCES_DIR "Resources" )
#set ( TTCARDS_RESOURCES ) # TODO: BUILD_OSXAPP

#     CMake Environment
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules/" )

option ( VERBOSE "Verbose build" off )
option ( BUILD_RELEASE "Build Release target; defaults to Debug" off )
option ( BUILD_UNIVERSAL_LIB "Build as an OSX Universal Library" off )
option ( BUILD_OSXAPP "Build a OSX App Bundle; defaults to off" on )

if ( VERBOSE )
  set ( CMAKE_VERBOSE_MAKEFILE true )
endif ( VERBOSE )

if ( BUILD_UNIVERSAL_LIB )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( "Building an OSX Universal Library" )
endif ( BUILD_UNIVERSAL_LIB )

if ( BUILD_RELEASE )
  set ( CMAKE_BUILD_TYPE Release )
  set ( PROJECT_BUILD_TYPE 1 )
  message ( "Building Release target." )
else ( NOT BUILD_RELEASE )
  set ( CMAKE_BUILD_TYPE Debug )
  set ( PROJECT_BUILD_TYPE 0 )
  message ( "Building Debug target." )
endif ( BUILD_RELEASE )

#     Platform specific
if ( CMAKE_SYSTEM_NAME STREQUAL Linux ) # Tested on Ubuntu v12.04-LTS(?) with
                                        # applicable dependencies installed
  set ( PLATFORM_LINUX true )
  message ( "Linux OS detected " )

  set ( CMAKE_CXX_FLAGS -std=c++0x )
elseif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )

  set ( PLATFORM_OSX true )
  message ( "Darwin OS (Mac OSX) detected" )  # Developed 100% on OSX v10.8.3

  set ( CMAKE_EXE_LINKER_FLAGS "-std=c++11 -stdlib=libc++" )
  set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )

  if ( CMAKE_GENERATOR STREQUAL "Xcode" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
    message ( STATUS "Setting Xcode-specific project settings " )
  endif ( CMAKE_GENERATOR STREQUAL "Xcode" )

else ()

  set ( PLATFORM_UNKNOWN true )
  message ( "Unsupported build system detected... Hopefully CMake gets it right for you; good luck!" )
endif ( CMAKE_SYSTEM_NAME STREQUAL Linux )

# global project doxygen defaults
set ( DOXYGEN_COMMAND doxygen )
set ( DOXYGEN_CONFIG_FILE doxygen.cfg )

set ( TTCARDS_SOURCE
      ${SRC_DIR}/cfg.cpp
      #${SRC_DIR}/cpu_player.cpp
      ${SRC_DIR}/CardsMenuState.cpp
      ${SRC_DIR}/Game.cpp
      ${SRC_DIR}/GameOverState.cpp
      ${SRC_DIR}/Game_debug.cpp
      ${SRC_DIR}/board.cpp
      ${SRC_DIR}/card.cpp
      ${SRC_DIR}/card_collection.cpp
      ${SRC_DIR}/card_debug.cpp
      ${SRC_DIR}/card_hand.cpp
      ${SRC_DIR}/card_rules.cpp
      ${SRC_DIR}/card_view.cpp
      ${SRC_DIR}/main.cpp
      ${SRC_DIR}/player.cpp
      ${SRC_DIR}/GameObject.cpp
    )

# Required Dependencies; we gather up all of the library & header search paths
# in two separate variables -- EXT_HEADERS & EXTLIBS -- so we can then pass
# these variables around further with minimal amount of redundancy.
#
find_package ( SDL REQUIRED )
set ( EXT_HEADERS ${EXT_HEADERS} ${SDL_INCLUDE_DIR} )
set ( EXTLIBS ${EXTLIBS} ${SDL_LIBRARY} "SDLmain" ) #FIXME

find_package ( nomlib REQUIRED )
set ( EXT_HEADERS ${EXT_HEADERS} ${NOMLIB_INCLUDE_DIR} )
set ( EXTLIBS ${EXTLIBS} ${NOMLIB_LIBRARY} )

find_library ( json_spirit REQUIRED )

find_path ( JSON_SPIRIT_INCLUDE_DIR
            json_spirit.h
            json_spirit_reader_template.h json_spirit_writer_template.h
            PATH_SUFFIXES
            include/json_spirit include
          )

set ( EXT_HEADERS ${EXT_HEADERS} ${JSON_SPIRIT_INCLUDE_DIR} )
set ( EXTLIBS ${EXTLIBS} ${JSON_SPIRIT_LIBRARY} )

# Now that we have collected all the header file search paths we need, we
# include them all here
include_directories ( ${EXT_HEADERS} )

# Linking Dependencies & Packaging / Installation

if ( BUILD_OSXAPP ) # OSX App Bundle
  set ( PROJECT_BUILD_TYPE 2 )
  message ( "Building OSX App Bundle" )

  # This allows our Resources, or any other not-quite-standard install command
  # install our data in the right place without further ugliness.
  set ( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR} )

  #set ( TTCARDS_RESOURCES_INSTALL_DIR ${PROJECT_NAME}.app/Contents/${TTCARDS_RESOURCES_DIR} )

  add_executable  ( ${PROJECT_NAME} MACOSX_BUNDLE ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${EXTLIBS} )

  # This is where the output executable added is installed to; which should be:
  #
  # ${PROJECT_NAME}.app/Contents/MacOS
  install ( TARGETS ${PROJECT_NAME} DESTINATION . )

#     Install dependencies
#     TODO: when ${NOMLIB_LIBRARY} is a dylib, put it under bin/${PROJECT_NAME}.app/Contents/Libraries
  if ( IS_DIRECTORY ${NOMLIB_LIBRARY} )

    install ( DIRECTORY ${NOMLIB_LIBRARY} DESTINATION ${PROJECT_NAME}.app/Contents/Frameworks
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )

  else ( NOT IS_DIRECTORY ${NOMLIB_LIBRARY} )
    # FIXME: we need the actual file, not the symbolic link
    install ( FILES ${NOMLIB_LIBRARY} DESTINATION ${PROJECT_NAME}.app/Contents/Frameworks )

  endif ( IS_DIRECTORY ${NOMLIB_LIBRARY} )

#     Install game resources
  install ( DIRECTORY ${TTCARDS_RESOURCES_DIR} DESTINATION ${PROJECT_NAME}.app/Contents
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles
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

else ( NOT BUILD_OSXAPP ) # POSIX Installation

  message ( "Building POSIX install" )

  add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${EXTLIBS} )

  install ( TARGETS ${PROJECT_NAME} DESTINATION bin
            CONFIGURATIONS Release
          )

#     Install game resources
  install ( DIRECTORY ${TTCARDS_RESOURCES_DIR} DESTINATION share/${PROJECT_NAME}
            CONFIGURATIONS Release
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles -- *cough* .DS_Store
          )

endif ( BUILD_OSXAPP )

if ( BUILD_RELEASE OR BUILD_OSXAPP )

  #     Uninstall target

  configure_file  (
                    "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                    IMMEDIATE @ONLY
                  )

  add_custom_target ( uninstall
                      COMMAND ${CMAKE_COMMAND} -P
                      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                    )

endif ( BUILD_RELEASE OR BUILD_OSXAPP )

if ( BUILD_OSXAPP )

#     Run app target (Sublime Text Editor Integration) -- BUILD_OSXAPP variant
  add_custom_target ( run COMMAND
                      open -a ${CMAKE_INSTALL_PREFIX}/ttcards.app
                    )

  add_custom_command  ( TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}
                        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/${TTCARDS_RESOURCES_DIR}
                      )

elseif ( BUILD_RELEASE )

#     Run app target (Sublime Text Editor Integration) -- BUILD_RELEASE variant
  add_custom_target ( run COMMAND
                      open --wait-apps --fresh -a "Terminal"
                      ${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}
                    )

else ( NOT BUILD_RELEASE OR BUILD_OSXAPP )

#     Run app target (Sublime Text Editor Integration) -- debug / development
  add_custom_target ( run COMMAND
                      open --wait-apps --fresh -a "Terminal"
                      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
                    )

  add_custom_command  ( TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}
                        ${CMAKE_CURRENT_BINARY_DIR}/${TTCARDS_RESOURCES_DIR}
                      )

endif ( BUILD_OSXAPP )

# TODO: this is for tracking installation paths, versions, etc.
configure_file  ( "${PROJECT_SOURCE_DIR}/cmake/version.hpp.in"
                  "${PROJECT_SOURCE_DIR}/${SRC_DIR}/version.hpp"
                )

#     FIXME: doxygen docs generation
add_custom_target ( ttcards-docs
                    COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE}
                  )

include ( "${PROJECT_SOURCE_DIR}/cmake/CPackConfig.cmake" )

include ( InstallRequiredSystemLibraries )

# Platform specific generator presets
if ( PLATFORM_OSX )
  set ( CPACK_GENERATOR "DragNDrop" )
elseif ( PLATFORM_LINUX )
  set ( CPACK_GENERATOR "DEB" )
elseif ( PLATFORM_UNKNOWN )
  set ( CPACK_GENERATOR "ZIP" )
endif ( PLATFORM_OSX )

include ( CPack )
