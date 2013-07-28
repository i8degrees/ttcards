cmake_minimum_required ( VERSION 2.8 )

#     Project configuration
set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 7 )
set ( PROJECT_VERSION_PATCH 0 )

project ( ttcards ) # $PROJECT_NAME

set ( SRC_DIR "src" ) # ttcards/src

set ( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR} )

# relative path -- from project source -- to game resources
set ( TTCARDS_RESOURCES_DIR "Resources" )
#set ( TTCARDS_RESOURCES ) # TODO: BUILD_OSXAPP

# relative path -- from resources directory -- to audio assets
set ( TTCARDS_RESOURCES_AUDIO_DIR "${TTCARDS_RESOURCES_DIR}/audio" )

#     CMake Environment
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules/" )

option ( VERBOSE "Verbose build" off )
option ( BUILD_RELEASE "Build Release target; defaults to Debug" off )
option ( BUILD_UNIVERSAL_LIB "Build as OSX Universal Library; i386 and x86_64" off )
option ( BUILD_OSXAPP "Build a OSX App Bundle; defaults to off" off )

if ( VERBOSE )
  set ( CMAKE_VERBOSE_MAKEFILE true )
endif ( VERBOSE )

if ( BUILD_UNIVERSAL_LIB )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( "Building an OSX Universal Library" )
endif ( BUILD_UNIVERSAL_LIB )

if ( BUILD_RELEASE )
  set ( CMAKE_BUILD_TYPE Release )
  set ( PROJECT_VERSION_TYPE 1 )
  message ( "Building Release target." )
else ( NOT BUILD_RELEASE )
  set ( CMAKE_BUILD_TYPE Debug )
  set ( PROJECT_VERSION_TYPE 0 )
  message ( "Building Debug target." )
endif ( BUILD_RELEASE )

if ( CMAKE_GENERATOR STREQUAL Xcode )

    message ( "Setting Xcode-specific project settings " )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )

endif ( CMAKE_GENERATOR STREQUAL Xcode )

#     Required Dependencies
option ( TTCARDS_JSON_SPIRIT_COMPILE "Let TTcards compile json_spirit" off )
#     Optional Dependencies
option ( EMCC "EMCC" off )
option ( NOMJSON_LIBRARY "nomJSON" off )

#     Platform specific
if ( CMAKE_SYSTEM_NAME STREQUAL Linux ) # Tested on Ubuntu v12.04-LTS(?) with
                                        # applicable dependencies installed
  set ( PLATFORM_LINUX true )
  message ( "Linux OS detected " )

  #set ( CMAKE_CXX_COMPILER "/usr/bin/c++" )
  set ( CMAKE_CXX_FLAGS -std=c++0x )
elseif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )

  set ( PLATFORM_OSX true )
  message ( "Darwin OS (Mac OSX) detected" )  # Developed 100% on OSX v10.8.3

  #set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
  set ( CMAKE_EXE_LINKER_FLAGS "-std=c++11 -stdlib=libc++" )
  set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
else ()

  set ( PLATFORM_UNKNOWN true )
  message ( "Unsupported build system detected... Hopefully CMake gets it right for you; good luck!" )
endif ( CMAKE_SYSTEM_NAME STREQUAL Linux )

if ( EMCC )

  set ( EMCC_BIN "$ENV{HOME}/local/src/emscripten/emcc" )
  set ( EMCC_CFLAGS --jcache )
  set ( EMCC_TARGET libnomlib.bc )

endif ( EMCC )

# global project doxygen defaults
set ( DOXYGEN_COMMAND doxygen )
set ( DOXYGEN_CONFIG_FILE doxygen.cfg )

#     TTcards dependencies
if ( TTCARDS_JSON_SPIRIT_COMPILE )
  message ( "Building dependency json_spirit." )
  add_subdirectory ( extlibs/json_spirit )
  include_directories ( extlibs/json_spirit )
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} "json_spirit" ) # FIXME
endif ( TTCARDS_JSON_SPIRIT_COMPILE )

if ( NOMJSON_LIBRARY )
  message ( "Building dependency nomJSON." )
  add_subdirectory ( extlibs/nomJSON )
  include_directories ( extlibs/nomJSON/src )
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} "nomJSON" )
endif ( NOMJSON_LIBRARY )

set ( TTCARDS_SOURCE
       ${SRC_DIR}/board.cpp
       ${SRC_DIR}/card.cpp
       ${SRC_DIR}/card_debug.cpp
       ${SRC_DIR}/card_view.cpp
       ${SRC_DIR}/card_collection.cpp
       ${SRC_DIR}/card_hand.cpp
       ${SRC_DIR}/card_rules.cpp
       ${SRC_DIR}/cfg.cpp
       ${SRC_DIR}/main.cpp
       ${SRC_DIR}/player.cpp
       #${SRC_DIR}/cpu_player.cpp
       ${SRC_DIR}/Game.cpp
       ${SRC_DIR}/Game_debug.cpp
       ${SRC_DIR}/GameOverState.cpp
       ${SRC_DIR}/CardsMenuState.cpp
    )

#     Required Dependencies:
find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )
set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${SDL_LIBRARY} "SDLmain" ) #FIXME

find_package ( nomlib REQUIRED )
include_directories ( "${NOMLIB_INCLUDE_DIR}" )
set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${NOMLIB_LIBRARY} )

if ( NOT TTCARDS_JSON_SPIRIT_COMPILE )
  find_library ( json_spirit REQUIRED )
  include_directories ( /usr/local/include/json_spirit ) # FIXME
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${JSON_SPIRIT_LIBRARY} )
endif ( NOT TTCARDS_JSON_SPIRIT_COMPILE )

#     Linking Dependencies & Packaging / Installation:

if ( BUILD_OSXAPP ) # OSX App Bundle
  set ( PROJECT_VERSION_TYPE 2 )
  message ( "Building OSX App Bundle" )

  # Reset bin path; we are basically in Release mode
  # (This might change in the future... need to rethink structure of all this)
  set ( EXECUTABLE_OUTPUT_PATH )

  add_executable  ( ${PROJECT_NAME} MACOSX_BUNDLE ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${REQUIRED_LIBS} )

  install ( TARGETS ${PROJECT_NAME} DESTINATION bin )

#     Install dependencies
#     TODO: when ${NOMLIB_LIBRARY} is a dylib, put it under bin/${PROJECT_NAME}.app/Contents/Libraries
  if ( IS_DIRECTORY ${NOMLIB_LIBRARY} )

    install ( DIRECTORY ${NOMLIB_LIBRARY} DESTINATION bin/${PROJECT_NAME}.app/Contents/Frameworks
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )
  else ( NOT IS_DIRECTORY ${NOMLIB_LIBRARY} )
    # FIXME: we need the actual file, not the symbolic link
    install ( FILES ${NOMLIB_LIBRARY} DESTINATION bin/${PROJECT_NAME}.app/Contents/Frameworks )

  endif ( IS_DIRECTORY ${NOMLIB_LIBRARY} )

#     Install game resources
  install ( DIRECTORY ${TTCARDS_RESOURCES_DIR} DESTINATION bin/${PROJECT_NAME}.app/Contents
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles
          )

#     Install App bundle icon
  install ( FILES ${TTCARDS_RESOURCES_DIR}/ttcards.icns DESTINATION bin/${PROJECT_NAME}.app/Contents
            OPTIONAL # do not include optional features
          )

  set_target_properties ( ${PROJECT_NAME} PROPERTIES
                          MACOSX_BUNDLE_INFO_PLIST
                          "${PROJECT_SOURCE_DIR}/cmake/Info.plist.in"
                          MACOSX_BUNDLE_BUNDLE_NAME
                          "TTcards" #${PROJECT_NAME}
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

#     Run app target for Sublime Text Editor Custom Build Settings
  add_custom_target ( run COMMAND open -a "${CMAKE_INSTALL_PREFIX}/bin/ttcards.app" )

else ( NOT BUILD_OSXAPP ) # POSIX Installation

  message ( "Building POSIX install" )

  add_executable ( ${PROJECT_NAME} ${TTCARDS_SOURCE} )
  target_link_libraries ( ${PROJECT_NAME} ${REQUIRED_LIBS} )

  install ( TARGETS ${PROJECT_NAME} DESTINATION bin
            CONFIGURATIONS Release
          )

#     Install game resources
  install ( DIRECTORY ${TTCARDS_RESOURCES_DIR} DESTINATION share/${PROJECT_NAME}
            CONFIGURATIONS Release
            OPTIONAL
            PATTERN ".*" EXCLUDE # do not include dotfiles -- *cough* .DS_Store
          )

#     Run app target for Sublime Text Editor Custom Build Settings
  add_custom_target ( run
                      COMMAND open --wait-apps --fresh -a "Terminal" ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}
                    )

endif ( BUILD_OSXAPP )

if ( BUILD_RELEASE OR BUILD_OSXAPP )

  #     Uninstall target

  configure_file  (
                    "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                    IMMEDIATE @ONLY
                  )

  add_custom_target ( uninstall
                      COMMAND ${CMAKE_COMMAND} -P
                      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                    )

endif ( BUILD_RELEASE OR BUILD_OSXAPP )


# TODO: this is for tracking installation paths, versions, etc.
configure_file  ( "${PROJECT_SOURCE_DIR}/cmake/version.hpp.in"
                  "${PROJECT_SOURCE_DIR}/${SRC_DIR}/version.hpp"
                )

#     FIXME: doxygen docs generation
add_custom_target ( ttcards-docs
                    COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE}
                  )

#     CPack configuration
include ( InstallRequiredSystemLibraries )

set ( CPACK_PACKAGE_NAME "${PROJECT_NAME}" )
set ( CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )
set ( CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )
set ( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )

set ( CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.md" )
set ( CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md" )
set ( CPACK_RESOURCE_FILE_WELCOME "${CPACK_RESOURCE_FILE_README}" )

#     Used for RPM & Debian packages
set ( CPACK_PACKAGE_VENDOR "org.github-i8degrees" )
set ( CPACK_PACKAGE_CONTACT "jeffrey.carp@gmail.com" )
set ( CPACK_PACKAGE_DESCRIPTION "Triple Triad Remake" )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "TTcards - Triple Triad Remake" )

set ( CPACK_STRIP_FILES true )

if ( PLATFORM_OSX )

  set ( CPACK_GENERATOR "DragNDrop" )
  set ( CPACK_PROJECT_CONFIG_FILE "${PROJECT_SOURCE_DIR}/cmake/cpack/OSXGenerator.cmake" )
  message ( "CPack Generator is set to: OSX package." )

elseif ( PLATFORM_LINUX )

  set ( CPACK_GENERATOR "DEB" )
  set ( CPACK_PROJECT_CONFIG_FILE "${PROJECT_SOURCE_DIR}/cmake/cpack/DebGenerator.cmake" )
  message ( "CPack Generator is set to: Debian package." )

elseif ( PLATFORM_UNKNOWN )

  set ( CPACK_GENERATOR "ZIP" )
  message ( "CPack Generator is set to: ZIP package." )

endif ( PLATFORM_OSX )

include ( CPack )
