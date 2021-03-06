# Locate SDL2_image
#
# This module defines:
#
# SDL2_IMAGE_LIBRARY
# SDL2_IMAGE_FOUND          if false, do not try to link to sdl2_image
# SDL2_IMAGE_INCLUDE_DIR    where to find the headers
#
# Created by Jeffrey Carpenter. Credit & thanks goes to whomever authored the
# FindOpenAL.cmake module -- thanks! :)
#
# CMake should search for OSX frameworks first in the following order:
#
# ~/Library/Frameworks/SDL2_image.framework/Headers
# /Library/Frameworks/SDL2_image.framework/Headers
# /System/Library/Frameworks/SDL2_image.framework/Headers
#
# If found, this will prefer the Framework version over others.
#

find_path ( SDL2_IMAGE_INCLUDE_DIR SDL_image.h
            NAMES
            SDL2_image
            PATH_SUFFIXES
            include/SDL2_image SDL2_image
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

find_library  ( SDL2_IMAGE_LIBRARY
                NAMES
                sdl2_image SDL2_image # Release version
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

if ( SDL2_IMAGE_INCLUDE_DIR AND SDL2_IMAGE_LIBRARY )
  set ( SDL2_IMAGE_FOUND TRUE)
endif ( SDL2_IMAGE_INCLUDE_DIR AND SDL2_IMAGE_LIBRARY )

if ( SDL2_IMAGE_FOUND )
  message ( STATUS "Found SDL2_image: ${SDL2_IMAGE_LIBRARY}" )
else ( NOT SDL2_IMAGE_FOUND )
  message ( FATAL_ERROR "Could not find required dependency SDL2_image" )
endif ( SDL2_IMAGE_FOUND )
