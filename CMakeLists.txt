cmake_minimum_required ( VERSION 2.6 )

# TODO: Use CMake v2.8.12 OSX RPath features
# http://www.kitware.com/blog/home/post/510
# cmake_minimum_required ( VERSION 2.8.12 )

# CMake Environment

# Opt out of using CMake v3.0 PROJECT_VERSION variables management for the
# project.
# http://www.cmake.org/cmake/help/v3.0/command/project.html#command:project
if( POLICY CMP0048 )
  cmake_policy( SET CMP0048 OLD )
endif( POLICY CMP0048 )

set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/" )
set ( CMAKE_TEMPLATE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates" )

# Set the install prefix to the current build folder if the user has not chosen
# their own prefix path.
if ( NOT CMAKE_INSTALL_PREFIX )
  set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Install path prefix" )
endif ( NOT CMAKE_INSTALL_PREFIX )

# Project configuration
set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 11 )
set ( PROJECT_VERSION_PATCH 0 )

# Project options
option ( DEBUG "Enable building with debugging features" off )
option ( DEBUG_ASSERT "Enable building with run-time assertions" off )
option ( DOCS "Generate HTML based API documentation with Doxygen" off )

project ( ttcards ) # Sets PROJECT_NAME variable for us

# Platform detection
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/platform.cmake" )

if ( DEBUG )
  set ( CMAKE_VERBOSE_MAKEFILE ON )

  if ( NOT PLATFORM_WINDOWS )
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D NOM_DEBUG -Wall -Wunused -O0" ) #-D_GLIBCXX_DEBUG
  else ( PLATFORM_WINDOWS )
    # TODO: split these options up like we have for other platforms
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D NOM_DEBUG /D NOM_DEBUG_ASSERT" )
  endif ( NOT PLATFORM_WINDOWS )

  set ( CMAKE_BUILD_TYPE "Debug" )
  message ( STATUS "Building ${PROJECT_NAME} with debugging." )
  message ( STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}" )
else () # Build with optimizations for maximum speed and minimal size
  set ( CMAKE_BUILD_TYPE "Release" )
  message ( STATUS "Building ${PROJECT_NAME} with high speed, low drag!" )
  message ( STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}" )
endif ( DEBUG )

if ( DEBUG_ASSERT )
  add_definitions ( "-DNOM_DEBUG_ASSERT" ) # Enable NOM_ASSERT macros
  message ( STATUS "Run-time assertions are ON." )
else ( NOT DEBUG_ASSERT )
  add_definitions ( "-DNDEBUG" ) # Disable NOM_ASSERT macros
  message ( STATUS "Run-time assertions are OFF." )
endif ( DEBUG_ASSERT )

# Utility macros
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros.cmake" )

# Relative path from project root to compiled source files
set ( SRC_DIR "src" )

# Relative path from project root to header files
set ( TTCARDS_HEADERS_DIR "include" )

# Relative path from the from project root to game resources
set ( TTCARDS_RESOURCES_DIR "Resources" )

# Additional flags to pass add_executable
set ( EXECUTABLE_FLAGS "" )

# Files used with documentation generation
set ( PROJECT_DOXYGEN_DIR "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/doxygen" )

# OS X application bundle variables used in generating the Info.plist
set ( BUNDLE_TEMPLATE_PLIST "${CMAKE_TEMPLATE_PATH}/Info.plist.in" )
set ( BUNDLE_NAME "TTcards" )
set ( BUNDLE_DESCRIPTION "Triple Triad Remake" )
set ( BUNDLE_ICON "icon.icns" )
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

# CMAKE_SYSTEM_PREFIX_PATH is searched to find libraries when the find_package
# command is used
message ( STATUS "Library Search Prefix: ${CMAKE_SYSTEM_PREFIX_PATH}" )

# Installation prefix path set for our project
message ( STATUS "Installation Prefix: ${CMAKE_INSTALL_PREFIX}" )

if ( DOCS ) # Generation of documentation via doxygen option
  include ( "${PROJECT_SOURCE_DIR}/cmake/doxygen.cmake" )
endif ( DOCS )

# Third-party dependencies

if( PLATFORM_WINDOWS )

  # Set the search path for where to find nomlib's CMake package script
  if( PLATFORM_ARCH STREQUAL "x86" )
    set( NOMLIB_ROOT "$ENV{SystemDrive}/Program Files (x86)/nomlib" )
  elseif( PLATFORM_ARCH STREQUAL "x64" )
    set( NOMLIB_ROOT "$ENV{SystemDrive}/Program Files/nomlib" )
  endif( PLATFORM_ARCH STREQUAL "x86" )

endif( PLATFORM_WINDOWS )

# External dependencies prefix (absolute path to nomlib's third-party directory)
if( NOT NOMLIB_DEPS_PREFIX )
  message( FATAL_ERROR "The location of the third-party dependencies must be set. See README.md for details." )
else()
  # Used by nomlib's find module
  set(  NOMLIB_DEPS_ROOT ${NOMLIB_DEPS_PREFIX} CACHE PATH
        "nomlib's third-party dependencies path" )
  message( STATUS "NOMLIB_DEPS_ROOT: ${NOMLIB_DEPS_PREFIX}" )
endif()

find_package( nomlib COMPONENTS
              core file math system graphics gui ptree serializers REQUIRED
              OPTIONAL_COMPONENTS audio
)

if( PLATFORM_WINDOWS )
  # Redistribution paths must be absolute, a directory and end with a trailing
  # slash character (these are fed to CMake's install command). Only filenames
  # with the extension of '.dll' (case-insensitive) will be copied.

  set( SDL2_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/SDL2/lib/${PLATFORM_ARCH}/" )
  set( SDL2_IMAGE_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/SDL2_image/lib/${PLATFORM_ARCH}/" )
  set( SDL2_TTF_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/SDL2_ttf/lib/${PLATFORM_ARCH}/" )

  if( PLATFORM_ARCH STREQUAL "x86" )
    set( OPENAL_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/OpenAL/libs/Win32/" )
  elseif( PLATFORM_ARCH STREQUAL "x64" )
    set( OPENAL_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/OpenAL/libs/Win64/" )
  endif( PLATFORM_ARCH STREQUAL "x86" )

  set( LIBSNDFILE_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/libsndfile/lib/${PLATFORM_ARCH}/" )
  set( LIBROCKET_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/libRocket/lib/${PLATFORM_ARCH}/" )
  set( LIBROCKET_FREETYPE_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/libRocket/bin/${PLATFORM_ARCH}/" )
  set( MSVCPP_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/msvcpp2013/${PLATFORM_ARCH}/" )

elseif( PLATFORM_OSX )

  # FIXME: This file needs its RPATH modified; it is **not** being used as the
  # library for executables yet. We are using brew's version installed at
  # /usr/local
  set( LIBROCKET_FREETYPE_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/librocket/bin/libfreetype.6.dylib" )

elseif( PLATFORM_LINUX )
  # TODO
endif( PLATFORM_WINDOWS )

# Project source tree
include( "${SRC_DIR}/CMakeLists.txt" )
# add_subdirectory( ${SRC_DIR} )

# Installation phase
if ( PLATFORM_OSX )

  # CMAKE_INSTALL_PREFIX = ttcards.app

  # Set the run-time library search path for where to find external library
  # dependencies
  set ( RUNTIME_LIBRARIES_SEARCH_PATH
        "@executable_path/../Frameworks"
        "@executable_path/../Frameworks/nomlib-core.framework/Frameworks"
        "@executable_path/../Frameworks/nomlib-system.framework/Frameworks"
        "@executable_path/../Frameworks/nomlib-audio.framework/Frameworks"
        "@executable_path/../Frameworks/nomlib-gui.framework/Libraries"
      )

  if ( OSXAPP )
    set ( EXECUTABLE_FLAGS "MACOSX_BUNDLE" )

    add_executable  ( ${PROJECT_NAME} ${EXECUTABLE_FLAGS} ${TTCARDS_SOURCE} )

    set ( INSTALL_APP_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BUNDLE_NAME}.app" )

    set ( INSTALL_ROOT_PATH "${INSTALL_APP_PATH}/Contents" )

    set ( INSTALL_RESOURCES_PATH "${INSTALL_ROOT_PATH}" )

    set ( INSTALL_DOCS_PATH "${INSTALL_RESOURCES_PATH}/Resources" )
    set ( INSTALL_DOCS_HELP_PATH "${INSTALL_DOCS_PATH}" )
    set ( INSTALL_GENERATED_DOCS_PATH "${INSTALL_DOCS_PATH}/Documentation" )

    # Directory path to fixing up external frameworks with install_name_tool(1)
    set ( INSTALL_BINARY_PATH "${INSTALL_ROOT_PATH}/MacOS/${PROJECT_NAME}" )

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
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
                            MACOSX_BUNDLE_LONG_VERSION_STRING
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
                            MACOSX_BUNDLE_BUNDLE_VERSION
                            "${CMAKE_VERSION}"
                            MACOSX_BUNDLE_COPYRIGHT
                            "Copyright (c) ${BUNDLE_COPYRIGHT_YEAR} ${BUNDLE_COPYRIGHT_NAME} All rights reserved."
                          )

    add_rpath("${RUNTIME_LIBRARIES_SEARCH_PATH}" "${INSTALL_BINARY_PATH}")

    # Bundle our dependencies into our application bundle
    install(  DIRECTORY
              ${NOMLIB_LIBRARIES} # nomlib-component.framework
              DESTINATION "${INSTALL_FRAMEWORK_PATH}"
              PATTERN ".*" EXCLUDE ) # do not include dotfiles

    install(  FILES
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_game.json"
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_assets-low-res.json"
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_assets-hi-res.json"
              DESTINATION "$ENV{HOME}/Library/Application\ Support/TTcards" )

    file( MAKE_DIRECTORY
          "$ENV{HOME}/Library/Application\ Support/TTcards/SavedGames"
          "$ENV{HOME}/Library/Application\ Support/TTcards/Screenshots" )

  endif ( OSXAPP )

# ...PLATFORM_OSX...

elseif ( PLATFORM_WINDOWS )

  set ( INSTALL_APP_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}" )
  set ( INSTALL_RESOURCES_PATH "${INSTALL_APP_PATH}" )

  # TODO:
  #set ( INSTALL_DOCS_PATH "share/doc/${PROJECT_NAME}" )
  #set ( INSTALL_DOCS_HELP_PATH "${INSTALL_DOCS_PATH}" )
  #set ( INSTALL_GENERATED_DOCS_PATH "share/doc/${PROJECT_NAME}" )

  # Build as console app -- shows cmd.exe window for logging -- only when
  # debugging is toggled on
  if ( NOT DEBUG )
    set ( EXECUTABLE_FLAGS "WIN32" ) # Implies WIN_MAIN
  else ( DEBUG )
    # MSVCPP should default to Console if we set EXECUTABLE_FLAGS empty
    set ( EXECUTABLE_FLAGS "" )
  endif ( NOT DEBUG )

  add_executable ( ${PROJECT_NAME} ${EXECUTABLE_FLAGS} ${TTCARDS_SOURCE} )

  # FIXME: Install binaries to our platform-dependent paths we've now set
  #install ( TARGETS ${PROJECT_NAME} DESTINATION "${INSTALL_APP_PATH}" )

  # Install library dependencies into binary output directory
  install ( DIRECTORY
            "${SDL2_REDIST_DIR}"
            "${SDL2_IMAGE_REDIST_DIR}"
            "${SDL2_TTF_REDIST_DIR}"
            "${OPENAL_REDIST_DIR}"
            "${LIBSNDFILE_REDIST_DIR}"
            "${MSVCPP_REDIST_DIR}"
            "${LIBROCKET_REDIST_DIR}"
            "${LIBROCKET_FREETYPE_REDIST_DIR}"
            DESTINATION
            "${INSTALL_APP_PATH}"
            FILES_MATCHING PATTERN "*.dll" )

    # NOTE: The LOCALAPPDATA environment variable does not exist on Windows XP
    install(  FILES
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_game.json"
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_assets-low-res.json"
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config_assets-hi-res.json"
              DESTINATION "$ENV{LOCALAPPDATA}/TTcards" )

    file( MAKE_DIRECTORY
          "$ENV{LOCALAPPDATA}/TTcards/SavedGames"
          "$ENV{LOCALAPPDATA}/TTcards/Screenshots" )

elseif ( PLATFORM_LINUX ) # POSIX install layout
  # TODO: set paths, etc. like we do above with OSX & Windows platforms

  add_executable ( ${PROJECT_NAME} ${EXECUTABLE_FLAGS} ${TTCARDS_SOURCE} )

  # Install binaries to our platform-dependent paths we've now set
  install ( TARGETS ${PROJECT_NAME} DESTINATION "${INSTALL_APP_PATH}" )

endif ( PLATFORM_OSX )

# FIXME: If it weren't for the infinite recursion issue we have with the OS X
# Application Bundle, we'd be able to use a single install targets line here:
#
# install ( TARGETS ${PROJECT_NAME} DESTINATION "${INSTALL_APP_PATH}" )

# Install game resources
install ( DIRECTORY "${TTCARDS_RESOURCES_DIR}"
          DESTINATION "${INSTALL_RESOURCES_PATH}"
          OPTIONAL
          PATTERN ".*" EXCLUDE # do not include dotfiles
          PATTERN "Documentation" EXCLUDE # do not include documentation here
        )

# Install software license & general project information
install ( FILES
          ${PROJECT_SOURCE_DIR}/LICENSE.md
          ${PROJECT_SOURCE_DIR}/README.md
          DESTINATION "${INSTALL_DOCS_PATH}"
        )

# Install general game information -- rules, how to play, etc.
install ( DIRECTORY
          ${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/Documentation
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

# Linking phase
target_link_libraries(  ${PROJECT_NAME} ${NOMLIB_LIBRARIES}
                        ${NOMLIB_DEPENDENCIES}
)

# TODO: Windows platform support
if ( NOT PLATFORM_WINDOWS )
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
endif ( NOT PLATFORM_WINDOWS )
