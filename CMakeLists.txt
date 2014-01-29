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
  set ( CMAKE_VERBOSE_MAKEFILE ON )

  if ( NOT PLATFORM_WINDOWS )
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D NOM_DEBUG -Wall -Wunused -O0" ) #-D_GLIBCXX_DEBUG
  else ( PLATFORM_WINDOWS )
    # TODO: split these options up like we have for other platforms
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D NOM_DEBUG /D NOM_DEBUG_ASSERT /D NOM_DEBUG_TRACE" )

    # FIXME: _ITERATOR_DEBUG_LEVEL is intended as a *temporary* patch to get
    # around a GameOverState crash involving vector or iterator access; we still
    # need to hunt down the bugs and eliminate them...
    set ( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_ITERATOR_DEBUG_LEVEL=0" )
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

# Relative path from project root to the external dependencies directory
set ( EXTERNAL_DEPS_DIR "third-party" )

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

  set ( MSVCPP_LIBRARY_REDIST "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/msvcpp2013/${PLATFORM_ARCH}/" )

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
include_directories ( "${EXT_HEADERS}" )

# List of our source code to be compiled
include ( "${SRC_DIR}/CMakeLists.txt" )

# Installation phase
if ( PLATFORM_OSX )

  # CMAKE_INSTALL_PREFIX = ttcards.app

  # Set the path of our library dependencies to search within our app bundle
  set ( INSTALL_NAME_PATH
        "@executable_path/../Frameworks"
      )

  if ( OSXAPP )
    set ( EXECUTABLE_FLAGS "MACOSX_BUNDLE" )

    add_executable  ( ${PROJECT_NAME} ${EXECUTABLE_FLAGS} ${TTCARDS_SOURCE} )

    set ( INSTALL_APP_PATH "${BUNDLE_NAME}.app" )

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
    install ( TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_INSTALL_PREFIX}" )
    #set ( INSTALL_APP_PATH "${CMAKE_INSTALL_PREFIX}" )
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
            "${SDL2_LIBRARY_DIR}"
            "${SDL2_IMAGE_LIBRARY_DIR}"
            "${SDL2_TTF_LIBRARY_DIR}"
            "${OPENAL_LIBRARY_DIR}"
            "${LIBSNDFILE_LIBRARY_DIR}"
            "${MSVCPP_LIBRARY_REDIST}"
            DESTINATION
            "${INSTALL_APP_PATH}"
            FILES_MATCHING PATTERN "*.dll"
          )

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

# Linking phase -- issue link with all defined external dependencies
if ( PLATFORM_WINDOWS )
  target_link_libraries ( ${PROJECT_NAME} ${EXTLIBS} ${NOMLIB_LIBRARY} )
else ( NOT PLATFORM_WINDOWS )
  target_link_libraries ( ${PROJECT_NAME} ${NOMLIB_LIBRARY} ) # ${EXTLIBS}
endif ( PLATFORM_WINDOWS )

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
