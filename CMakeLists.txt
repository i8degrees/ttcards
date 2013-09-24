cmake_minimum_required ( VERSION 2.6 )

# CMake Environment
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/" )
set ( CMAKE_TEMPLATE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates" )

# Set the install prefix to the current build folder if the user has not chosen
# their own prefix path.
if ( NOT CMAKE_INSTALL_PREFIX )
  set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Install path prefix" )
endif ( NOT CMAKE_INSTALL_PREFIX )

# Project configuration
set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 9 )
set ( PROJECT_VERSION_PATCH 0 )

# Project options
option ( DEBUG "Enable building with debugging features" off )
option ( DEBUG_ASSERT "Enable building with run-time assertions" off )
option ( DEBUG_TRACE "Enable building with logging of every object's construction & destruction" off )
option ( DOCS "Generate HTML based API documentation with Doxygen" off )

project ( ttcards ) # Sets PROJECT_NAME variable for us

if ( DEBUG )
  set ( CMAKE_BUILD_TYPE "Debug" )
  set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D NOM_DEBUG -Wall -Wunused" )
  set ( CMAKE_VERBOSE_MAKEFILE on )
  message ( STATUS "Building ${PROJECT_NAME} with debugging." )
else () # Build with optimizations for maximum speed and minimal size
  set ( CMAKE_BUILD_TYPE "Release" )
  message ( STATUS "Building ${PROJECT_NAME} with high speed, low drag!" )
endif ( DEBUG )

if ( DEBUG_ASSERT )
  add_definitions ( "-D NOM_DEBUG_ASSERT" )
  message ( STATUS "Building ${PROJECT_NAME} with run-time assertions." )
endif ( DEBUG_ASSERT )

if ( DEBUG_TRACE )
  # We intentionally do not set CMAKE_BUILD_TYPE to "Debug" or "Release" here so
  # we can enable this feature in either target mode to maximize debugging
  # capabilities.
  add_definitions ( "-D NOM_DEBUG_TRACE" )
  message ( STATUS "Building ${PROJECT_NAME} with class object call tracing." )
endif ( DEBUG_TRACE )

# Platform detection
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/platform.cmake" )

# Utility macros
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros.cmake" )

# Relative path from project root to compiled source files
set ( SRC_DIR "src" )

# Relative path from project root to header files
set ( HEADERS_DIR "include" )

# Absolute path to the json_spirit header files
set ( HEADERS_JSON_SPIRIT_DIR "${PROJECT_SOURCE_DIR}/${HEADERS_DIR}/json_spirit" )

# Relative path from the from project root to game resources
set ( TTCARDS_RESOURCES_DIR "Resources" )

# Absolute path without closing backslash to game storage path;
# must be writable by the user.
if ( NOT TTCARDS_DATA_DIR )
  set ( TTCARDS_DATA_DIR "$ENV{HOME}/Documents/${PROJECT_NAME}" )
endif ( NOT TTCARDS_DATA_DIR )

# Files used with documentation generation
set ( PROJECT_DOXYGEN_DIR "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/doxygen" )

# OS X application bundle variables used in generating the Info.plist
set ( BUNDLE_TEMPLATE_PLIST "${CMAKE_TEMPLATE_PATH}/Info.plist.in" )
set ( BUNDLE_NAME "TTcards" )
set ( BUNDLE_DESCRIPTION "Triple Triad Remake" )
set ( BUNDLE_ICON "ttcards.icns" )
set ( BUNDLE_IDENTIFIER "org.i8degrees.${PROJECT_NAME}" )
set ( BUNDLE_COPYRIGHT_YEAR "2013" )
set ( BUNDLE_COPYRIGHT_NAME "Jeffrey Carpenter" )

# Installation paths; CMAKE_INSTALL_PREFIX will be appended to these.
# Depending on the install profile, these paths are updated; such as when we
# are building an OS X application bundle.
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

if ( UNIVERSAL )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( STATUS "Enabled building OSX Universal Application." )
endif ( UNIVERSAL )

if ( OSXAPP )
  message ( STATUS "Using OSXAPP install profile." )
else ( NOT OSXAPP )
  message ( STATUS "Using POSIX install profile." )
endif ( OSXAPP )

message ( STATUS "Installation Prefix: ${CMAKE_INSTALL_PREFIX}" )
message ( STATUS "Data folder: ${TTCARDS_DATA_DIR}" )

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

# We need GTK2 only for its header files (nomlib links to GTK2 for us)
# (This will be fixed sometime in the future)
if ( PLATFORM_LINUX )
  find_package ( GTK2 REQUIRED gtk )
endif ( PLATFORM_LINUX )

if ( SDL_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL_LIBRARY} )
else ( NOT SDL_FOUND )
  message ( FATAL_ERROR "Required dependency SDL missing!" )
endif ( SDL_FOUND )

if ( NOMLIB_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${NOMLIB_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${NOMLIB_LIBRARY} )
else ( NOT NOMLIB_FOUND )
  message ( FATAL_ERROR "Required dependency nomlib missing!" )
endif ( NOMLIB_FOUND )

# First, try finding our packaged header files for json_spirit;
# ...if this fails, attempt to find the headers directory of a system install of
# json_spirit
if ( IS_DIRECTORY "${HEADERS_JSON_SPIRIT_DIR}" )

  set ( EXT_HEADERS ${EXT_HEADERS} "${HEADERS_JSON_SPIRIT_DIR}" )
  message ( STATUS "Found packaged json_spirit: ${HEADERS_JSON_SPIRIT_DIR}" )
elseif ( NOT IS_DIRECTORY "${HEADERS_JSON_SPIRIT_DIR}" )

  find_package ( json_spirit REQUIRED )

  if ( JSON_SPIRIT_FOUND )
    set ( EXT_HEADERS ${EXT_HEADERS} ${JSON_SPIRIT_INCLUDE_DIR} )
    message ( WARNING "Found json_spirit: ${JSON_SPIRIT_INCLUDE_DIR}" )
  else ( NOT JSON_SPIRIT_FOUND )
    message ( FATAL_ERROR "Required dependency json_spirit (headers only) missing!" )
  endif ( JSON_SPIRIT_FOUND )

endif ( IS_DIRECTORY "${HEADERS_JSON_SPIRIT_DIR}" )

if ( PLATFORM_OSX )
  # The SDL_LIBRARY variable for OS X is appended with the "-framework Cocoa"
  # linkage flag, so we have to sanitize this before hand if we want to use the
  # value in anything other than linking -- such as packaging the library into a
  # bundle.
  #
  # From here on out, the burden of linking to the Cocoa framework when need be
  # is entirely on us. I say good riddance!
  string ( REPLACE "-framework Cocoa" "" SDL_LIBRARY ${SDL_LIBRARY} )
endif ( PLATFORM_OSX )

if ( PLATFORM_LINUX )
  if ( GTK2_FOUND )
    set ( EXT_HEADERS ${EXT_HEADERS} ${GTK2_INCLUDE_DIRS} )
  else ( NOT GTK2_FOUND )
    message ( FATAL_ERROR "Required dependency GTK2 (headers) missing!" )
  endif ( GTK2_FOUND )
endif ( PLATFORM_LINUX )

# External header files inclusion
include_directories ( ${EXT_HEADERS} )

# Installation phase
if ( OSXAPP )

  add_executable  ( ${PROJECT_NAME} MACOSX_BUNDLE ${TTCARDS_SOURCE} )

  # CMAKE_INSTALL_PREFIX = ttcards.app
  set ( INSTALL_NAME_PATH
        "@executable_path/../Frameworks"
      )

  set ( INSTALL_APP_PATH "${BUNDLE_NAME}.app" )

  set ( INSTALL_ROOT_PATH "${INSTALL_APP_PATH}/Contents" )

  set ( INSTALL_RESOURCES_PATH "${INSTALL_ROOT_PATH}" )

  set ( INSTALL_DOCS_PATH "${INSTALL_RESOURCES_PATH}/Resources" )
  set ( INSTALL_DOCS_HELP_PATH "${INSTALL_DOCS_PATH}" )
  set ( INSTALL_GENERATED_DOCS_PATH "${INSTALL_DOCS_PATH}/Documentation" )

  # Used for Sublime make run target and fix-up of external frameworks
  set ( INSTALL_BINARY_PATH "${INSTALL_ROOT_PATH}/MacOS/${PROJECT_NAME}" )
  set ( SUBLIME_RUN_PATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_BINARY_PATH}" )

  # Relative path for where to install our external dependencies
  set ( INSTALL_FRAMEWORK_PATH "${INSTALL_ROOT_PATH}/Frameworks" )

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

  add_rpath ( "${INSTALL_NAME_PATH}" "${INSTALL_BINARY_PATH}" )

  # Install our external dependencies into our application bundle
  install ( DIRECTORY
            "${NOMLIB_LIBRARY}" # nomlib.framework
            "${SDL_LIBRARY}" # SDL.framework v1.2.15
            DESTINATION "${INSTALL_FRAMEWORK_PATH}"
            PATTERN ".*" EXCLUDE # do not include dotfiles
            PATTERN "Headers" EXCLUDE # do not include header files
            PATTERN "CMake" EXCLUDE # do not include CMake config scripts
          )

  # CPack refuses to work with me on anything other than a single dot as the
  # destination target path when we are building a package.
  install ( TARGETS ${PROJECT_NAME} DESTINATION . )

else ( NOT OSXAPP ) # POSIX install profile

  add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )
  # Install executables
  install ( TARGETS ${PROJECT_NAME} DESTINATION "${INSTALL_APP_PATH}" )

endif ( OSXAPP )

# Install game resources
install ( DIRECTORY ${TTCARDS_RESOURCES_DIR}
          DESTINATION "${INSTALL_RESOURCES_PATH}"
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

# Create the data folder for game storage if it does not exist
file ( MAKE_DIRECTORY "${TTCARDS_DATA_DIR}" )

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

configure_file  (
                  "${CMAKE_TEMPLATE_PATH}/version.cpp.in"
                  "${PROJECT_SOURCE_DIR}/${SRC_DIR}/version.cpp"
                )

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
