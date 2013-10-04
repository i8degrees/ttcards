# Locate SDL2_ttf
#
# This module defines:
#
# SDL2_TTF_LIBRARY
# SDL2_TTF_FOUND          if false, do not try to link to sdl2_ttf
# SDL2_TTF_INCLUDE_DIR    where to find the headers
#
# Created by Jeffrey Carpenter. Credit & thanks goes to whomever authored the
# FindOpenAL.cmake module -- thanks! :)
#
# CMake should search for OSX frameworks first in the following order:
#
# ~/Library/Frameworks/SDL2_ttf.framework/Headers
# /Library/Frameworks/SDL2_ttf.framework/Headers
# /System/Library/Frameworks/SDL2_ttf.framework/Headers
#
# If found, this will prefer the Framework version over others.
#

find_path ( SDL2_TTF_INCLUDE_DIR SDL_ttf.h
            NAMES
            SDL2_ttf
            PATH_SUFFIXES
            include/SDL2_ttf SDL2_ttf
            PATHS
            ~/Library/Frameworks
            /Library/Frameworks
            /usr/local/include # homebrew
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
            #[HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
          )

find_library  ( SDL2_TTF_LIBRARY
                NAMES
                sdl2_ttf SDL2_ttf # Release version
                PATH_SUFFIXES
                lib64 lib libs64 libs libs/Win32 libs/Win64
                PATHS
                ~/Library/Frameworks
                /Library/Frameworks
                /usr/local/lib # homebrew
                /sw # Fink
                /opt/local # DarwinPorts
                /opt/csw # Blastwave
                /opt
                #[HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
              )


# handle the QUIETLY and REQUIRED arguments and set NOMLIB_FOUND to TRUE if
# all listed variables are TRUE
#include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

#FIND_PACKAGE_HANDLE_STANDARD_ARGS ( SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_INCLUDE_DIR )

#mark_as_advanced ( SDL2_LIBRARY SDL2_INCLUDE_DIR )

if ( SDL2_TTF_INCLUDE_DIR AND SDL2_TTF_LIBRARY )
  set ( SDL2_TTF_FOUND TRUE)
endif ( SDL2_TTF_INCLUDE_DIR AND SDL2_TTF_LIBRARY )

if ( SDL2_TTF_FOUND )
  message ( STATUS "Found SDL2_ttf: ${SDL2_TTF_LIBRARY}" )
else ( NOT SDL2_TTF_FOUND )
  message ( FATAL_ERROR "Could not find required dependency SDL2_ttf" )
endif ( SDL2_TTF_FOUND )
