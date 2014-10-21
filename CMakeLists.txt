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

# Common dependencies (shared between all platforms)

# libRocket dependency
find_package( OpenGL REQUIRED )
if( OPENGL_FOUND )
  set ( EXT_HEADERS ${EXT_HEADERS} ${OPENGL_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${OPENGL_LIBRARY} )
endif( OPENGL_FOUND )

# This might be a short-term kludge; we are linking statically (by default) so
# that we are able to start using both nomlib and libRocket in TTcards with a
# minimum of packaging fuss for the project (statically linking here lets us
# remove libRocket from the linking of TTcards, and just requires the header
# files).
#
# I'm also not sure that it makes sense to abstract libRocket's higher-level
# APIs...
#
# This only affects builds on OS X. Windows builds always links statically.
set( NOM_USE_STATIC_LIBROCKET FALSE )
set( NOM_USE_LIBROCKET_LUA FALSE )

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

  # libRocket dependency
  set ( GLEW_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/glew/include/GL/" )
  # Paths must be: a) absolute; b) directory; c) end with a slash
  set ( GLEW_REDIST_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/glew/bin/${PLATFORM_ARCH}/" )
  set ( GLEW_LIBRARY
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/glew/lib/${PLATFORM_ARCH}/glew32.lib"
      )
  set ( EXT_HEADERS ${EXT_HEADERS} "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/glew/include/GL" )
  set ( EXTLIBS ${EXTLIBS} ${GLEW_LIBRARY} )

  # libRocket dependency (GUI)
  set ( LIBROCKET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/include" )
  # Paths must be: a) absolute; b) directory; c) end with a slash
  set ( LIBROCKET_REDIST_DIRS
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/lib/${PLATFORM_ARCH}/;${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/bin/${PLATFORM_ARCH}/" )
  set ( LIBROCKET_LIBRARY
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/lib/${PLATFORM_ARCH}/RocketControls.lib"
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/lib/${PLATFORM_ARCH}/RocketCore.lib"
        "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/windows/libRocket/lib/${PLATFORM_ARCH}/RocketDebugger.lib"
      )
  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBROCKET_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBROCKET_LIBRARY} )

elseif ( PLATFORM_OSX )

  find_package ( nomlib REQUIRED )

  if ( NOMLIB_FOUND )
    set ( EXT_HEADERS ${EXT_HEADERS} ${NOMLIB_INCLUDE_DIR} )
    set ( EXTLIBS ${EXTLIBS} ${NOMLIB_LIBRARY} )
    message( STATUS "Found nomlib: ${NOMLIB_LIBRARY}" )
  else ( NOT NOMLIB_FOUND )
    message ( FATAL_ERROR
              "Not found: nomlib.\n"
              "\nNOMLIB_INCLUDE_DIR: ${NOMLIB_INCLUDE_DIR}"
              "\nNOMLIB_LIBRARY: ${NOMLIB_LIBRARY}"
            )
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

  # libRocket dependency (FreeType2)
  #
  # FIXME: This file needs its RPATH modified; it is **not** being used as the
  # library for executables yet. We are using brew's version installed at
  # /usr/local
  set ( FREETYPE_LIBRARY "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/bin/libfreetype.6.dylib" )
  set ( EXTLIBS ${EXTLIBS} ${FREETYPE_LIBRARY} )

  # libRocket dependency
  if( NOM_USE_LIBROCKET_LUA )
    find_package( Lua REQUIRED )

    if( LUA_FOUND )
      set ( EXT_HEADERS ${EXT_HEADERS} ${LUA_INCLUDE_DIR} )
      set ( EXTLIBS ${EXTLIBS} ${LUA_LIBRARY} )
    endif( LUA_FOUND )
  endif( NOM_USE_LIBROCKET_LUA )

  # libRocket dependency (GUI)

  set ( LIBROCKET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/include" )

  if( NOM_USE_STATIC_LIBROCKET )
    # Note that these are built without debugging (Release) because the
    # resulting file size was insane otherwise (+158MB).
    set ( LIBROCKET_LIBRARY
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketControls.a"
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketCore.a"
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketDebugger.a"
        )

    if( NOM_USE_LIBROCKET_LUA )
      set ( LIBROCKET_LIBRARY
            ${LIBROCKET_LIBRARY}
            "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketControlsLua.a"
            "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketCoreLua.a"
          )
    endif( NOM_USE_LIBROCKET_LUA )

  else( NOT NOM_USE_STATIC_LIBROCKET )
    # Debug libraries
    set ( LIBROCKET_LIBRARY
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketControls.1.dylib"
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketCore.1.dylib"
          "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketDebugger.1.dylib"
        )

    # NOTE: Lua bindings for the dynamic library build has not been tested!
    if( NOM_USE_LIBROCKET_LUA )
      set ( LIBROCKET_LIBRARY
            ${LIBROCKET_LIBRARY}
            "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketControlsLua.1.dylib"
            "${PROJECT_SOURCE_DIR}/${EXTERNAL_DEPS_DIR}/osx/librocket/lib/libRocketCoreLua.1.dylib"
          )
    endif( NOM_USE_LIBROCKET_LUA )

  endif( NOM_USE_STATIC_LIBROCKET )

  set ( EXT_HEADERS ${EXT_HEADERS} ${LIBROCKET_INCLUDE_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBROCKET_LIBRARY} )

  find_library ( COCOA_LIBRARY Cocoa )
  mark_as_advanced ( COCOA_LIBRARY )
  set ( EXTLIBS ${EXTLIBS} ${COCOA_LIBRARY} )

elseif ( PLATFORM_LINUX )
  # TODO
endif ( PLATFORM_WINDOWS )

# External header files inclusion
include_directories (
                      ${EXT_HEADERS}
                    )

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

    if( NOT NOM_USE_STATIC_LIBROCKET )
      # Bundle dynamic libraries we depend on
      #
      # TODO: Use "Libraries" directory for dynamic libraries.
      install ( FILES
                ${LIBROCKET_LIBRARY}
                DESTINATION "${INSTALL_FRAMEWORK_PATH}"
              )
    endif( NOT NOM_USE_STATIC_LIBROCKET )

    # CPack refuses to work with me on anything other than a single dot as the
    # destination target path when we are building a package.
    install ( TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_INSTALL_PREFIX}" )
    #set ( INSTALL_APP_PATH "${CMAKE_INSTALL_PREFIX}" )


    install ( FILES
              "${PROJECT_SOURCE_DIR}/${TTCARDS_RESOURCES_DIR}/config.json"
              # DESTINATION "${INSTALL_APP_PATH}/Resources"
              DESTINATION "$ENV{HOME}/Documents/ttcards"
            )
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
  target_link_libraries( ${PROJECT_NAME} ${NOMLIB_LIBRARY} ${EXTLIBS} )
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
