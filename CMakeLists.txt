cmake_minimum_required ( VERSION 2.6 )

# CMake Environment
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/" )
set ( CMAKE_TEMPLATE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates" )

# CMake options
option ( VERBOSE "Enable verbose makefile" off )
option ( CMAKE_INSTALL_PREFIX "Install project in a custom location" ${CMAKE_CURRENT_BINARY_DIR} )

if ( VERBOSE )
  set ( CMAKE_VERBOSE_MAKEFILE true )
endif ( VERBOSE )

# Set the install prefix to the current build folder if this option has not been
# set.
if ( NOT CMAKE_INSTALL_PREFIX )
  set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" )
endif ( NOT CMAKE_INSTALL_PREFIX )

# Project configuration
set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 8 )
set ( PROJECT_VERSION_PATCH 0 )

# Project options
option ( DEBUG "Enable building with debugging features" off )
option ( DEBUG_ASSERT "Enable building with run-time assertions" off )
option ( DEBUG_TRACE "Enable building with logging of every object's construction & destruction" off )
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

# Information used in the generation of the packaged Info.plist
set ( BUNDLE_TEMPLATE_PLIST "${CMAKE_TEMPLATE_PATH}/Info.plist.in" )
set ( BUNDLE_NAME "TTcards" )
set ( BUNDLE_DESCRIPTION "Triple Triad Remake" )
set ( BUNDLE_ICON "ttcards.icns" )
set ( BUNDLE_IDENTIFIER "org.i8degrees.${PROJECT_NAME}" )
set ( BUNDLE_COPYRIGHT_YEAR "2013" )
set ( BUNDLE_COPYRIGHT_NAME "Jeffrey Carpenter" )

# Installation paths are relative to CMAKE_INSTALL_PREFIX and must be without
# the closing backslash character.
#
# Unless you specify either the option OSXAPP or POSIX, CMAKE_INSTALL_PREFIX is
# overwritten.
#
# All installation profiles must supply these variables; the default assumes
# that neither POSIX or OSXAPP is defined, thus the local development profile
# takes effect -- meaning everything is installed into the current build folder.
set ( INSTALL_APP_PATH "bin" )
set ( INSTALL_RESOURCES_PATH "share/${PROJECT_NAME}" )
set ( INSTALL_DOCS_PATH "share/doc/${PROJECT_NAME}" )
set ( INSTALL_DOCS_HELP_PATH "${INSTALL_DOCS_PATH}" )
set ( INSTALL_GENERATED_DOCS_PATH "share/doc/${PROJECT_NAME}" )

include ( "${PROJECT_SOURCE_DIR}/cmake/uninstall.cmake" )

# Check which options have been set

# Assume a local development profile initially -- this likely changes depending
# on which build profile is chosen.
set ( SUBLIME_RUN_PATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_APP_PATH}/${PROJECT_NAME}" )

if ( PLATFORM_OSX )
  option ( OSXAPP "Enable building OS X Application Bundle" on )
  option ( UNIVERSAL "Enable building OSX Universal Application" off )
endif ( PLATFORM_OSX )

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

if ( OSXAPP )
  message ( STATUS "Using OSXAPP install profile." )
else ( NOT OSXAPP )
  message ( STATUS "Using POSIX install profile." )
endif ( OSXAPP )

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

else ( NOT PLATFORM_LINUX OR PLATFORM_OSX )
  # Use whatever CMake gathers for us and pray for the best!
endif ( PLATFORM_LINUX )

# List of our source code to be compiled
include ( "${SRC_DIR}/CMakeLists.txt" )

# Required Dependencies
#
# Add libraries & header file inclusion paths to EXT_HEADERS & EXTLIBS as we
# find them.
find_package ( SDL REQUIRED )
find_package ( nomlib REQUIRED )
find_package ( json_spirit REQUIRED )

if ( SDL_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL_LIBRARY} )
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

# Installation phase
if ( OSXAPP )

  # CMAKE_INSTALL_PREFIX = ttcards.app
  set ( INSTALL_NAME_PATH "@executable_path/../Frameworks" )

  set ( INSTALL_APP_PATH "${BUNDLE_NAME}.app" )

  set ( INSTALL_ROOT_PATH "${INSTALL_APP_PATH}/Contents" )

  set ( INSTALL_RESOURCES_PATH "${INSTALL_ROOT_PATH}" )

  set ( INSTALL_DOCS_PATH "${INSTALL_RESOURCES_PATH}/Resources" )
  set ( INSTALL_DOCS_HELP_PATH "${INSTALL_DOCS_PATH}" )
  set ( INSTALL_GENERATED_DOCS_PATH "${INSTALL_DOCS_PATH}/Documentation" )

  # Used for Sublime make run target and fix-up of external frameworks
  set ( INSTALL_BINARY_PATH "${INSTALL_ROOT_PATH}/MacOS/${PROJECT_NAME}" )
  set ( SUBLIME_RUN_PATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_BINARY_PATH}" )

  add_executable  ( ${PROJECT_NAME} MACOSX_BUNDLE ${TTCARDS_SOURCE} )

  # Generate Info.plist for distribution within our soon-to-be application bundle
  set_target_properties ( ${PROJECT_NAME} PROPERTIES
                          MACOSX_BUNDLE_INFO_PLIST
                          "${BUNDLE_TEMPLATE_PLIST}"
                          MACOSX_BUNDLE_BUNDLE_NAME
                          "${BUNDLE_NAME}"
                          MACOSX_BUNDLE_ICON_FILE
                          "${BUNDLE_ICON}"
                          MACOSX_BUNDLE_INFO_STRING
                          "${BUNDLE_DESCRIPTION} version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} by ${BUNDLE_COPYRIGHT_NAME}"
                          MACOSX_BUNDLE_GUI_IDENTIFIER
                          ${BUNDLE_IDENTIFIER}
                          MACOSX_BUNDLE_SHORT_VERSION_STRING
                          "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                          MACOSX_BUNDLE_LONG_VERSION_STRING
                          "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
                          MACOSX_BUNDLE_BUNDLE_VERSION
                          "${CMAKE_VERSION}"
                          MACOSX_BUNDLE_COPYRIGHT
                          "Copyright (c) ${BUNDLE_COPYRIGHT_YEAR} ${BUNDLE_COPYRIGHT_NAME} All rights reserved."
                        )

  # Bundle external dependencies within our application bundle for distribution
  set ( INSTALL_FRAMEWORK_PATH "${INSTALL_ROOT_PATH}/Frameworks" )

  install_rpath ( "${INSTALL_NAME_PATH}" "${INSTALL_BINARY_PATH}" )

  # The SDL_LIBRARY variable for OS X is appended with the "-framework Cocoa"
  # linkage flag, so we have to sanitize this before hand for use in locating
  # and bundling this dependency.
  string ( REPLACE "-framework Cocoa" "" SDL_REAL_LIBRARY ${SDL_LIBRARY} )

  # Install our external dependencies into our application bundle
  install ( DIRECTORY
            "${NOMLIB_LIBRARY}" # nomlib.framework
            "${SDL_REAL_LIBRARY}" # SDL.framework v1.2.15
            "${JSON_SPIRIT_LIBRARY}" # json_spirit.framework v4.06.1
            DESTINATION "${INSTALL_FRAMEWORK_PATH}"
            PATTERN ".*" EXCLUDE # do not include dotfiles
          )

else ( NOT OSXAPP )

  add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )

  # Install executables
  install ( TARGETS ${PROJECT_NAME} DESTINATION "${INSTALL_APP_PATH}" )

endif ( OSXAPP )

  # Install game resources
  install ( DIRECTORY ${TTCARDS_RESOURCES_DIR} DESTINATION "${INSTALL_RESOURCES_PATH}"
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
              DESTINATION "${INSTALL_DOCS_PATH}"
            )

    # Install general game information -- rules, how to play, etc.
    install ( DIRECTORY
              ${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/Help
              DESTINATION "${INSTALL_DOCS_HELP_PATH}"
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )

  if ( DOCS )
    # Install generated documentation files
    install ( DIRECTORY ${PROJECT_BINARY_DIR}/docs/html
              DESTINATION "${INSTALL_GENERATED_DOCS_PATH}"
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )
  endif ( DOCS )

# Linking phase -- issue link with all defined external dependencies
target_link_libraries ( ${PROJECT_NAME} ${EXTLIBS} "SDLmain" )

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
