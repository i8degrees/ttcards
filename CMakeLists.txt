cmake_minimum_required ( VERSION 2.6 )

# Project configuration
set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 8 )
set ( PROJECT_VERSION_PATCH 0 )

# Project options -- needs to be done before any kind of project declaration
option ( VERBOSE "Enable verbose makefile" off )
option ( DEBUG "Enable building with debugging features" off )
option ( DEBUG_ASSERT "Enable building with run-time assertions" off )
option ( DEBUG_TRACE "Enable building with logging of every object's construction & destruction" off )

option ( POSIX "Enable POSIX install profile" off )
option ( DOCS "Generate HTML based API documentation with Doxygen" off )

project ( ttcards ) # $PROJECT_NAME

# Platform detection
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/platform.cmake" )

# Utility macros
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros.cmake" )

set ( SRC_DIR "src" ) # ttcards/src

# Relative path -- from project source -- to game resources
set ( TTCARDS_RESOURCES_DIR "Resources" )
set ( PROJECT_DOXYGEN_DIR "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/doxygen" )

# Assume a local development profile initially -- this likely changes depending
# on which build profile is chosen.
set ( SUBLIME_RUN_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}" )

# CMake Environment
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules/" )
set ( CMAKE_TEMPLATE_PATH "${PROJECT_SOURCE_DIR}/cmake/templates" )

# Uninstall make target support script
configure_file  ( "${CMAKE_TEMPLATE_PATH}/uninstall.cmake.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
                  IMMEDIATE @ONLY
                )

# Uninstall make target
add_custom_target ( uninstall
                    COMMAND ${CMAKE_COMMAND} -P
                    "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
                  )

# Check which options have been set
if ( PLATFORM_OSX )
  option ( OSXAPP "Enable building OS X Application Bundle" off )
  option ( UNIVERSAL "Enable building OSX Universal Application" off )
endif ( PLATFORM_OSX )

if ( VERBOSE )
  set ( CMAKE_VERBOSE_MAKEFILE true )
endif ( VERBOSE )

if ( DEBUG )
  set ( CMAKE_BUILD_TYPE "Debug" )
  set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTTCARDS_DEBUG" )
  message ( STATUS "Building ${PROJECT_NAME} with debugging." )
else () # Build with optimizations for maximum speed and minimal size
  set ( CMAKE_BUILD_TYPE "Release" )
  message ( STATUS "Building ${PROJECT_NAME} with high speed, low drag!" )
endif ( DEBUG )

if ( DEBUG_ASSERT )
  add_definitions ( "-DTTCARDS_DEBUG_ASSERT" )
  message ( STATUS "Building ${PROJECT_NAME} with run-time assertions." )
endif ( DEBUG_ASSERT )

if ( DEBUG_TRACE )
  # We intentionally do not set CMAKE_BUILD_TYPE to "Debug" or "Release" here so
  # we can enable this feature in either target mode to maximize debugging
  # capabilities.
  add_definitions ( "-DTTCARDS_DEBUG_TRACE" )
  message ( STATUS "Building ${PROJECT_NAME} with class object call tracing." )
endif ( DEBUG_TRACE )

if ( UNIVERSAL )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( STATUS "Enabled building OSX Universal Application." )
endif ( UNIVERSAL )

if ( POSIX )
  message ( STATUS "Enabled POSIX install profile." )
elseif ( OSXAPP )
  message ( STATUS "Enabled OSXAPP install profile." )
else ( NOT POSIX OR OSXAPP ) # default
  message ( STATUS "Enabled development install profile." )
endif ( POSIX )

if ( DOCS ) # Generation of documentation via doxygen option
  include ( "${PROJECT_SOURCE_DIR}/cmake/doxygen.cmake" )
endif ( DOCS )

# Platform specific
if ( PLATFORM_LINUX ) # Tested on Ubuntu v12.04-LTS

  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )

elseif ( PLATFORM_OSX )

  # libc++ requires OSX v10.7+
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++" )

  if ( CMAKE_GENERATOR STREQUAL "Xcode" )

    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )

  endif ( CMAKE_GENERATOR STREQUAL "Xcode" )

else ()

  # Use whatever CMake gathers for us and hope for the best!

endif ( PLATFORM_LINUX )

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

# Required Dependencies
#
# Add libraries & header file inclusion paths to EXT_HEADERS & EXTLIBS as we
# find them.
find_package ( SDL REQUIRED )
find_package ( nomlib REQUIRED )
find_package ( json_spirit REQUIRED )

if ( SDL_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL_LIBRARY} "SDLmain" ) #FIXME
endif ( SDL_FOUND )

if ( NOMLIB_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${NOMLIB_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${NOMLIB_LIBRARY} )
endif ( NOMLIB_FOUND )

if ( JSON_SPIRIT_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${JSON_SPIRIT_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${JSON_SPIRIT_LIBRARY} )
endif ( JSON_SPIRIT_FOUND )

include_directories ( ${EXT_HEADERS} )

# Linking Dependencies & Packaging / Installation

if ( OSXAPP )

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
                          "TTcards"
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

    # Install software license & general project information
    install ( FILES
              ${PROJECT_SOURCE_DIR}/BUGS.md
              ${PROJECT_SOURCE_DIR}/LICENSE.md
              ${PROJECT_SOURCE_DIR}/README.md
              ${PROJECT_SOURCE_DIR}/TODO.md
              DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/${TTCARDS_RESOURCES_DIR}/Documentation
            )

  if ( DOCS )

    # Install generated documentation files
    install ( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/html
              DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/${TTCARDS_RESOURCES_DIR}/Documentation
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )

  endif ( DOCS )

elseif ( POSIX )

  set ( SUBLIME_RUN_PATH "${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}" )

  add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${EXTLIBS} )

  # Install executables
  install ( TARGETS ${PROJECT_NAME} DESTINATION bin )

  # Install game resources
  install ( DIRECTORY ${TTCARDS_RESOURCES_DIR} DESTINATION share/${PROJECT_NAME}
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles
            PATTERN "Help" EXCLUDE # do not include documentation here
          )

    # Install software license & general project information
    install ( FILES
              ${PROJECT_SOURCE_DIR}/BUGS.md
              ${PROJECT_SOURCE_DIR}/LICENSE.md
              ${PROJECT_SOURCE_DIR}/README.md
              ${PROJECT_SOURCE_DIR}/TODO.md
              DESTINATION share/doc/${PROJECT_NAME}
            )

    # Install general game information -- rules, how to play, etc.
    install ( DIRECTORY
              ${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/Help
              DESTINATION share/doc/${PROJECT_NAME}
            )

  if ( DOCS )

    # Install generated documentation files
    install ( DIRECTORY ${PROJECT_BINARY_DIR}/docs/html
              DESTINATION share/doc/${PROJECT_NAME}
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )

  endif ( DOCS )

else () # Default

  set ( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR} )

  add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${EXTLIBS} )

  # Install executables
  install ( TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

  # Install game resources
  install ( DIRECTORY ${TTCARDS_RESOURCES_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles
          )

endif ( OSXAPP )

# make run target (Sublime Text Editor Integration)
#
# Needs to be added *AFTER* all modifications to SUBLIME_RUN_PATH are made.
add_custom_target ( run COMMAND
                    open --wait-apps --fresh -a "Terminal"
                    "${SUBLIME_RUN_PATH}"
                  )

# Auto-generated build-time variables result in modification of the built
# binary's run-time execution.
#
# CMake dependency variables (of which control the location of this insert)
#
#   a) PROJECT_VERSION_*
#   b) CMAKE_INSTALL_PREFIX
#
# This means that we must include this configured file *AFTER* any and all
# modifications -- that matter to us -- occur in this build script.
configure_file  ( "${CMAKE_TEMPLATE_PATH}/version.hpp.in"
                  "${PROJECT_SOURCE_DIR}/${SRC_DIR}/version.hpp"
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
