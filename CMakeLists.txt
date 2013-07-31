cmake_minimum_required ( VERSION 2.6 )

# Project configuration
set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 7 )
set ( PROJECT_VERSION_PATCH 0 )

project ( ttcards ) # $PROJECT_NAME

set ( SRC_DIR "src" ) # ttcards/src

# Relative path -- from project source -- to game resources
set ( TTCARDS_RESOURCES_DIR "Resources" )

# CMake Environment
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules/" )
set ( CMAKE_TEMPLATE_PATH "${PROJECT_SOURCE_DIR}/cmake/templates" )

option ( VERBOSE "Verbose build" off )
option ( BUILD_UNIVERSAL_LIB "Build as an OSX Universal Library" off )

# Distribution / Packaging target options
option ( BUILD_POSIX "Build POSIX target" off )
option ( BUILD_OSXAPP "Build an OSX App Bundle" off )

if ( VERBOSE )
  set ( CMAKE_VERBOSE_MAKEFILE true )
endif ( VERBOSE )

if ( BUILD_UNIVERSAL_LIB )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( "Building an OSX Universal Library" )
endif ( BUILD_UNIVERSAL_LIB )

if ( BUILD_POSIX )
  set ( PROJECT_BUILD_TYPE 1 )
  message ( "Building POSIX target." )
elseif ( BUILD_OSXAPP )
  set ( PROJECT_BUILD_TYPE 2 )
  message ( "Building OSXAPP target." )
else () # default
  set ( PROJECT_BUILD_TYPE 0 )
  message ( "Building Development target." )
endif ( BUILD_POSIX )

if ( NOT DEFINED CMAKE_BUILD_TYPE )
  set ( CMAKE_BUILD_TYPE Debug )
endif ( NOT DEFINED CMAKE_BUILD_TYPE )

# Platform specific
if ( CMAKE_SYSTEM_NAME STREQUAL Linux ) # Tested on Ubuntu v12.04-LTS

  set ( PLATFORM_LINUX true )
  message ( "Linux OS detected " )

  set ( CMAKE_CXX_FLAGS -std=c++0x )
elseif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )

  set ( PLATFORM_OSX true )
  message ( "Darwin OS (Mac OSX) detected" )

  # libc++ requires OSX v10.7+
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

if ( BUILD_OSXAPP )

  set ( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR} )
  set ( SUBLIME_RUN_PATH "${CMAKE_INSTALL_PREFIX}/ttcards.app/Contents/MacOS/ttcards" )

  add_executable  ( ${PROJECT_NAME} MACOSX_BUNDLE ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${EXTLIBS} )

  # Install executables
  install ( TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

  # Install dependencies
  # TODO: when nomlib is a dylib, put it under TTcards.app/Contents/Libraries
  if ( IS_DIRECTORY ${NOMLIB_LIBRARY} )

    install ( DIRECTORY ${NOMLIB_LIBRARY} DESTINATION ${PROJECT_NAME}.app/Contents/Frameworks
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )

  else ( NOT IS_DIRECTORY ${NOMLIB_LIBRARY} )
    # FIXME: we need the actual file, not the symbolic link
    install ( FILES ${NOMLIB_LIBRARY} DESTINATION ${PROJECT_NAME}.app/Contents/Frameworks )

  endif ( IS_DIRECTORY ${NOMLIB_LIBRARY} )

  #install ( DIRECTORY /Library/Frameworks/SDL.framework DESTINATION ${PROJECT_NAME}.app/Contents/Frameworks )

  # Install game resources
  install ( DIRECTORY ${TTCARDS_RESOURCES_DIR} DESTINATION ${PROJECT_NAME}.app/Contents
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles
          )

  set_target_properties ( ${PROJECT_NAME} PROPERTIES
                          MACOSX_BUNDLE_INFO_PLIST
                          "${CMAKE_TEMPLATE_PATH}/Info.plist.in"
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

elseif ( BUILD_POSIX )

  set ( SUBLIME_RUN_PATH "${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}" )

  add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${EXTLIBS} )

  # Install executables
  install ( TARGETS ${PROJECT_NAME} DESTINATION bin )

  # Install game resources
  install ( DIRECTORY ${TTCARDS_RESOURCES_DIR} DESTINATION share/${PROJECT_NAME}
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles
          )

else () # Default

  set ( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR} )
  set ( SUBLIME_RUN_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}" )

  add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${EXTLIBS} )

  # Install executables
  install ( TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

  # Install game resources
  install ( DIRECTORY ${TTCARDS_RESOURCES_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles
          )

endif ( BUILD_OSXAPP )

# Run app make target (Sublime Text Editor Integration)
add_custom_target ( run COMMAND
                    open --wait-apps --fresh -a "Terminal"
                    ${SUBLIME_RUN_PATH}
                  )

# Uninstall make target support script
configure_file  ( "${CMAKE_TEMPLATE_PATH}/cmake_uninstall.cmake.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                  IMMEDIATE @ONLY
                )

# Uninstall make target
add_custom_target ( uninstall
                    COMMAND ${CMAKE_COMMAND} -P
                    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                  )

# Auto-generated compile-time variables related to our build system
configure_file  ( "${CMAKE_TEMPLATE_PATH}/version.hpp.in"
                  "${PROJECT_SOURCE_DIR}/${SRC_DIR}/version.hpp"
                )

# FIXME: doxygen docs generation
add_custom_target ( ttcards-docs
                    COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE}
                  )

# CPack configuration
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
