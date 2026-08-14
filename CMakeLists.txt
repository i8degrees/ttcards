# CMakeLists.txt:jeff
#
#cmake_minimum_required(VERSION 2.8.12)
cmake_minimum_required(VERSION 3.24)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
list(APPEND CMAKE_TEMPLATE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates")

# Adhere to a common hierarchy acording to the chosen platform
include(GNUInstallDirs)
include("${CMAKE_SOURCE_DIR}/cmake/project_version.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/policy.cmake")
# include("${CMAKE_SOURCE_DIR}/cmake/functions.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake")
# TODO
# include("${CMAKE_SOURCE_DIR}/cmake/AppleBuild.cmake")

# Set the install prefix to the current build folder if the user has not chosen
# their own prefix path.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Default install path base prefix")
endif()

#
## vcpkg env
#

# !! this must be set *before* the project declaration
# defaults to a static library target
set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")

if(LINUX AND DEFINED BUILD_SHARED_LIBS)
  set(VCPKG_TARGET_TRIPLET "x64-linux-dynamic")
elif(LINUX AND NOT DEFINED BUILD_SHARED_LIBS)
  set(VCPKG_TARGET_TRIPLET "x64-linux")
endif()

if(APPLE AND DEFINED BUILD_SHARED_LIBS)
  set(VCPKG_TARGET_TRIPLET "x64-osx-dynamic")
elif(APPLE AND NOT DEFINED BUILD_SHARED_LIBS)
  set(VCPKG_TARGET_TRIPLET "x64-osx")
endif()

if(WIN32 AND DEFINED BUILD_SHARED_LIBS)
  set(VCPKG_TARGET_TRIPLET "x64-windows-dynamic")
elif(WIN32 AND NOT DEFINED BUILD_SHARED_LIBS)
  set(VCPKG_TARGET_TRIPLET "x64-windows")
endif()

# TODO(JEFF): cmake project option
# TODO(JEFF): Add TTCARDS_ENABLE_AUDIO_UNIT as new cmake project opt
# Project options
option(DEBUG "Enable building with debugging features" OFF)
option(DEBUG_ASSERT "Enable building with run-time assertions" OFF)
option(DOCS "Generate HTML based API documentation with Doxygen" ON)

# IMPORTANT(JEFF): This initializes our engine versioning variables;
# `PROJECT_VERSION_MAJOR` & friends
project(ttcards
  VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
  DESCRIPTION "Re-make of Triple Triad PSX game"
  HOMEPAGE_URL "https://github.com/i8degrees/ttcards"
  #SPDX_LICENSE "BSD-2-Clause-FreeBSD"
  LANGUAGES CXX
)

include("${CMAKE_SOURCE_DIR}/cmake/StandardProjectSettings.cmake")

# CMake Error at /home/jeff/local/lib/cmake/nomlib/nomlib-shared-targets.cmake:42 (message):
#   Some (but not all) targets in this export set were already defined.

#   Targets Defined: project_warnings

#   Targets not yet defined: nomlib-actions, nomlib-audio, nomlib-core,
#   nomlib-file, nomlib-graphics, nomlib-gui, nomlib-math, nomlib-ptree,
#   nomlib-serializers, nomlib-system

# Link this "library" to use the warnings specified in CompilerWarnings.cmake.
# FIXME(JEFF): Fix error that results from enabling this feature (project_warnings)
# add_library(project_warnings INTERFACE)
#include("${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake")
# Add cmake option WARNINGS_AS_ERRORS
#set_project_warnings(project_warnings)

# Platform detection
# TODO(JEFF): removal
# include ( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/platform.cmake" )


# Relative path from the from project root to game resources
set ( TTCARDS_RESOURCES_DIR "Resources" )

# Files used with documentation generation
set ( PROJECT_DOXYGEN_DIR "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/doxygen" )

# TODO(JEFF): Relocate
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

# CMAKE_SYSTEM_PREFIX_PATH is searched to find libraries when the find_package
# command is used
message ( STATUS "Library Search Prefix: ${CMAKE_SYSTEM_PREFIX_PATH}" )

# Installation prefix path set for our project
message ( STATUS "Installation Prefix: ${CMAKE_INSTALL_PREFIX}" )

if(DOCS) # Generation of documentation via doxygen option
  include("${PROJECT_SOURCE_DIR}/cmake/doxygen.cmake")
endif(DOCS)

# Third-party dependencies

# TODO(JEFF): Verify this opt works!
if(BUILD_STATIC_EXE)
  set(VCPKG_TRIPLET_DIR "x64-linux")
  set(BUILD_TYPE "release")
else(NOT BUILD_STATIC_EXE)
  set(VCPKG_TRIPLET_DIR "x64-linux-dynamic")
  set(BUILD_TYPE "release-dynamic")
endif(BUILD_STATIC_EXE)

# FIXME
set(libRocket_INCLUDE_DIRS "/home/jeff/Projects/nomlib.git/build/${BUILD_TYPE}/vcpkg_installed/${VCPKG_TRIPLET_DIR}/include")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${libRocket_INCLUDE_DIRS}")
include_directories("${libRocket_INCLUDE_DIRS}")

find_package(libRocket CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(nomlib CONFIG REQUIRED)

if(WIN32 AND USE_OLD_WIN_CODE)

  # Sanitize file path delimiters for Windows installations; this resolves
  # installation woes within cmake_install.cmake when mixing environment
  # variables -- forward slashes -- and CMake script paths -- backslashes.
  string( REPLACE "\\" "/" NOMLIB_DEPS_PREFIX ${NOMLIB_DEPS_ROOT} )

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

elseif(APPLE AND USE_OLD_OSX_CODE)

  # FIXME: This file needs its RPATH modified; it is **not** being used as the
  # library for executables yet. We are using brew's version installed at
  # /usr/local
  set( LIBROCKET_FREETYPE_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/librocket/bin/libfreetype.6.dylib" )

elseif(UNIX AND USE_OLD_LINUX_CODE)
  set( SDL2_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/SDL2/lib/" )
  set( SDL2_IMAGE_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/SDL2_image/lib" )
  set( SDL2_TTF_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/SDL2_ttf/lib" )
  set( LIBSNDFILE_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/libsndfile/lib" )
  set( LIBROCKET_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/libRocket/lib" )
  set( LIBROCKET_FREETYPE_REDIST_DIR "${NOMLIB_DEPS_PREFIX}/libRocket/bin" )
endif()

# Project source tree to build
add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/packages")

# TODO(JEFF): Relocate
# CPack configuration
include("${PROJECT_SOURCE_DIR}/cmake/CPackConfig.cmake")
include(InstallRequiredSystemLibraries)
include(CPack)

