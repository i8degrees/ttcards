cmake_minimum_required ( VERSION 2.6 )

# CMake Environment
set ( CMAKE_VERBOSE_MAKEFILE OFF CACHE PATH "Verbose Makefile" )
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/" )
set ( CMAKE_TEMPLATE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates" )

# Set the install prefix to the current build folder if the user has not chosen
# their own prefix path.
if ( NOT CMAKE_INSTALL_PREFIX )
  set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Install path prefix" )
endif ( NOT CMAKE_INSTALL_PREFIX )

# Project configuration
set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 10 )
set ( PROJECT_VERSION_PATCH 0 )

# Project options
option ( DEBUG "Enable building with debugging features" off )
option ( DEBUG_ASSERT "Enable building with run-time assertions" off )
option ( DEBUG_TRACE "Enable building with logging of every object's construction & destruction" off )
option ( DOCS "Generate HTML based API documentation with Doxygen" off )

project ( ttcards ) # Sets PROJECT_NAME variable for us

# Platform detection
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/platform.cmake" )

if ( DEBUG )
  if ( PLATFORM_OSX )
    # FIXME; Passing -D DEBUG=1 is a temporary workaround for getting around the
    # compile errors with debugging macros defined within the OS X header file
    # "CoreServices/CoreServices.h" that are conflicting with our
    # (unassigned value) DEBUG macro.
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D DEBUG=1" )
  endif ( PLATFORM_OSX )

  if ( NOT PLATFORM_WINDOWS )
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D NOM_DEBUG -Wall -Wunused" )
  else ( PLATFORM_WINDOWS )
    # TODO: split these options up like we have for other platforms
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D NOM_DEBUG /D NOM_DEBUG_ASSERT /D NOM_DEBUG_TRACE" )
  endif ( NOT PLATFORM_WINDOWS )

  set ( CMAKE_BUILD_TYPE "Debug" )
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

# Utility macros
include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros.cmake" )

# Relative path from project root to compiled source files
set ( SRC_DIR "src" )

# Relative path from project root to header files
set ( TTCARDS_HEADERS_DIR "include" )

# Relative path from project root to third-party in-project jsoncpp header file(s)
set ( JSONCPP_HEADERS_DIR "${TTCARDS_HEADERS_DIR}/jsoncpp" )

# Absolute path to the json_spirit header files
#
#     DEPRECATED
#
# TODO: json_spirit will soon be entirely phased out -- removed & replaced --
# with jsoncpp. In the mean time, though, we are going to try holding onto our
# json_spirit investment here, in order to ease the major transitions ahead of
# us; nomlib first needs to have a stable SDL2 engine up and going.
#set ( HEADERS_JSON_SPIRIT_DIR "${PROJECT_SOURCE_DIR}/${TTCARDS_HEADERS_DIR}/json_spirit" )

# Relative path from project root to the external dependencies directory
set ( EXTERNAL_DEPS_DIR "third-party" )

# Relative path from the from project root to game resources
set ( TTCARDS_RESOURCES_DIR "Resources" )

if ( NOT PLATFORM_OSX )
  # Absolute path without closing backslash to game storage path;
  # must be writable by the user.
  if ( NOT TTCARDS_DATA_DIR )
    set ( TTCARDS_DATA_DIR "$ENV{HOME}/Documents/${PROJECT_NAME}" )
  endif ( NOT TTCARDS_DATA_DIR )
endif ( NOT PLATFORM_OSX )

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

if ( OSXAPP )
  message ( STATUS "Using OSXAPP install profile." )
else ( NOT OSXAPP )
  message ( STATUS "Using POSIX install profile." )
endif ( OSXAPP )

message ( STATUS "Installation Prefix: ${CMAKE_INSTALL_PREFIX}" )

if ( NOT PLATFOMRM_OSX )
  message ( STATUS "Data folder: ${TTCARDS_DATA_DIR}" )
endif ( NOT PLATFOMRM_OSX )

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

#set ( EXT_HEADERS ${EXT_HEADERS} "${HEADERS_JSON_SPIRIT_DIR}" )
#message ( STATUS "Found packaged json_spirit: ${HEADERS_JSON_SPIRIT_DIR}" )

# Required Dependencies
#
# We gather up all of the library & header search paths in two separate
# variables -- EXT_HEADERS & EXTLIBS -- so we can then make a minimal number
# of calls to include_directories & linking of libraries.
#
# TODO: we should be checking for the existence of these directories beforehand!
#
if ( PLATFORM_WINDOWS )
  # Header and library paths -- *INCLUDE_DIR and *LIBRARY_DIR, respectively --
  # *MUST* include trailing backslash for this platform!
  #
  # *LIBRARY_DIR variables are used to ease the install of run-time dependencies.
  # You should have only DLL and LIB file types in the library directories, in
  # order for our file filtering to weed out the proper ones during installation.
  #
  # FIXME: We shouldn't be linking SDL2, Open(AL|GL) or libsndfile dependencies
  # here. It is intended that nomlib satisfies these dependencies for us!
  # (We should be matching OS X dependency variables below, more or less).
  # 2013-11-18
  set ( NOMLIB_INCLUDE_DIR "C:/Users/jeff/Projects/nomlib.git/include/" )
  set ( NOMLIB_LIBRARY "C:/Users/jeff/Projects/nomlib.git/build/Debug/nomlib-d.lib" )
  set ( NOMLIB_LIBRARY_DIR "C:/Users/jeff/Projects/nomlib.git/build/Debug/" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${NOMLIB_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${NOMLIB_LIBRARY} )

  # SDL2 dependency
  set ( SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2/include/" )
  set ( SDL2_LIBRARY
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2/lib/${PLATFORM_ARCH}/SDL2.lib"
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2/lib/${PLATFORM_ARCH}/SDL2main.lib"
      )
  set ( SDL2_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2/lib/${PLATFORM_ARCH}/" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_LIBRARY} )

  # SDL2_image dependency
  set ( SDL2_IMAGE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_image/include/" )
  set ( SDL2_IMAGE_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_image/lib/${PLATFORM_ARCH}/SDL2_image.lib" )
  set ( SDL2_IMAGE_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_image/lib/${PLATFORM_ARCH}/" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_IMAGE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_IMAGE_LIBRARY} )

  # SDL2_ttf dependency
  set ( SDL2_TTF_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_ttf/include/" )
  set ( SDL2_TTF_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_ttf/lib/${PLATFORM_ARCH}/SDL2_ttf.lib" )
  set ( SDL2_TTF_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/SDL2_ttf/lib/${PLATFORM_ARCH}/" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_TTF_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${SDL2_TTF_LIBRARY} )

  # OpenAL dependency
  set ( OPENAL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/OpenAL/include/" )
  set ( OPENAL_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/OpenAL/lib/${PLATFORM_ARCH}/OpenAL32.lib" )
  set ( OPENAL_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/OpenAL/lib/${PLATFORM_ARCH}/" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${OPENAL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${OPENAL_LIBRARY} )

  # libsndfile dependency
  set ( LIBSNDFILE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libsndfile/include/" )
  set ( LIBSNDFILE_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libsndfile/lib/${PLATFORM_ARCH}/" )
  set ( LIBSNDFILE_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libsndfile/lib/${PLATFORM_ARCH}/libsndfile-1.lib" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBSNDFILE_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBSNDFILE_LIBRARY} )

elseif ( PLATFORM_OSX )

  find_package ( nomlib REQUIRED )

  if ( NOMLIB_FOUND )
    set ( EXT_HEADERS ${EXT_HEADERS} ${NOMLIB_INCLUDE_DIR} )
    set ( EXTLIBS ${EXTLIBS} ${NOMLIB_LIBRARY} )
  else ( NOT NOMLIB_FOUND )
    message ( FATAL_ERROR "Required dependency nomlib missing!" )
  endif ( NOMLIB_FOUND )

  # SDL2 dependency
  set ( SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2.framework/Headers" )
  set ( SDL2_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2.framework" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_INCLUDE_DIR} )
  #set ( EXTLIBS ${EXTLIBS} ${SDL2_LIBRARY} )

  # SDL2_image dependency
  set ( SDL2_IMAGE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2_image.framework/Headers" )
  set ( SDL2_IMAGE_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2_image.framework" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_IMAGE_INCLUDE_DIR} )
  #set ( EXTLIBS ${EXTLIBS} ${SDL2_IMAGE_LIBRARY} )

  # SDL2_ttf dependency
  set ( SDL2_TTF_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2_ttf.framework/Headers" )
  set ( SDL2_TTF_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/SDL2_ttf.framework" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${SDL2_TTF_INCLUDE_DIR} )
  #set ( EXTLIBS ${EXTLIBS} ${SDL2_TTF_LIBRARY} )

  # OpenAL dependency
  set ( OPENAL_INCLUDE_DIR "/System/Library/Frameworks/OpenAL.framework" )
  set ( OPENAL_LIBRARY "/System/Library/Frameworks/OpenAL.framework" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${OPENAL_INCLUDE_DIR} )
  #set ( EXTLIBS ${EXTLIBS} ${OPENAL_LIBRARY} )

  # libsndfile dependency
  set ( LIBSNDFILE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/sndfile.framework/Headers" )
  set ( LIBSNDFILE_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/sndfile.framework" )
  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBSNDFILE_INCLUDE_DIR} )
  #set ( EXTLIBS ${EXTLIBS} ${LIBSNDFILE_LIBRARY} )

  find_library ( COCOA_LIBRARY Cocoa )
  mark_as_advanced ( COCOA_LIBRARY )
  set ( EXTLIBS ${EXTLIBS} ${COCOA_LIBRARY} )

elseif ( PLATFORM_LINUX )
  # TODO
endif ( PLATFORM_WINDOWS )

# External header files inclusion
include_directories ( "${EXT_HEADERS}" "${JSONCPP_HEADERS_DIR}" )

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
                          "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
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
            "${SDL2_LIBRARY}" # SDL.framework v2.0.0
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
#file ( MAKE_DIRECTORY "${TTCARDS_DATA_DIR}" )

if ( DOCS )
  # Install generated documentation files
  install ( DIRECTORY ${PROJECT_BINARY_DIR}/docs/html
            DESTINATION "${INSTALL_GENERATED_DOCS_PATH}"
            PATTERN ".*" EXCLUDE # do not include dotfiles
          )
endif ( DOCS )

# Linking phase -- issue link with all defined external dependencies
if ( PLATFORM_WINDOWS )
  target_link_libraries ( ${PROJECT_NAME} ${EXTLIBS} ${NOMLIB_LIBRARY} )
else ( NOT PLATFORM_WINDOWS )
  target_link_libraries ( ${PROJECT_NAME} ${NOMLIB_LIBRARY} ) # ${EXTLIBS}
endif ( PLATFORM_WINDOWS )

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
