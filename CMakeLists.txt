cmake_minimum_required ( VERSION 2.8 )

#     CMake Environment
set ( CMAKE_VERBOSE_MAKEFILE true )
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/" )

option ( BUILD_RELEASE "Build Release target; defaults to Debug" off )
option ( BUILD_UNIVERSAL_LIB "Build as OSX Universal Library; i386 and x86_64" off )
option ( BUILD_OSXAPP "Build a OSX App Bundle; defaults to off" off )

if ( BUILD_UNIVERSAL_LIB )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( "Building an OSX Universal Library" )
endif ( BUILD_UNIVERSAL_LIB )

if ( BUILD_RELEASE ) # ...why do I have to even do this? -_-
  set ( CMAKE_BUILD_TYPE Release )
  set ( PROJECT_VERSION_TYPE 1 )
  message ( "Release target activated." )
else ( NOT BUILD_RELEASE )
  set ( CMAKE_BUILD_TYPE Debug )
  set ( PROJECT_VERSION_TYPE 0 )
  message ( "Debug target activated." )
endif ( BUILD_RELEASE )

if ( CMAKE_GENERATOR STREQUAL Xcode )
    message ( "Setting Xcode-specific project settings " )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
endif ( CMAKE_GENERATOR STREQUAL Xcode )

#     Required Dependencies
#
# set false to build & install nomlib & json_spirit from external sources;
# otherwise let us take over and build our own dependencies
option ( TTCARDS_NOMLIB_COMPILE "Let TTcards compile nomlib" off )
option ( TTCARDS_JSON_SPIRIT_COMPILE "Let TTcards compile json_spirit" off )

#     Optional Dependencies
option ( EMCC "EMCC" off )
option ( NOMJSON_LIBRARY "nomJSON" off )

#     Platform specific
if ( CMAKE_SYSTEM_NAME STREQUAL Linux ) # Tested on Ubuntu v12.04-LTS(?) with
                                        # applicable dependencies installed
  message ( "Linux OS detected " )

  #set ( CMAKE_CXX_COMPILER "/usr/bin/c++" )
  set ( CMAKE_CXX_FLAGS -std=c++0x )
elseif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )
  message ( "Darwin OS (Mac OSX) detected" )  # Developed 100% on OSX v10.8.3

  #set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
  set ( CMAKE_EXE_LINKER_FLAGS "-std=c++11 -stdlib=libc++" )
  set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
else ()
  message ( "Unsupported build system detected... Hopefully CMake gets it right for you; good luck!" )
endif ( CMAKE_SYSTEM_NAME STREQUAL Linux )

if ( EMCC )
  set ( EMCC_BIN "$ENV{HOME}/local/src/emscripten/emcc" )
  set ( EMCC_CFLAGS --jcache )
  set ( EMCC_TARGET libgamelib.bc )
endif ( EMCC )

# global project doxygen defaults
set ( DOXYGEN_COMMAND doxygen )
set ( DOXYGEN_CONFIG_FILE doxygen.cfg )

set ( SRC_DIR src ) # project_dir/src

# TTcards dependencies
if ( TTCARDS_JSON_SPIRIT_COMPILE )
  message ( "Building dependency json_spirit" )
  add_subdirectory ( json_spirit )
  include_directories ( json_spirit )
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} "json_spirit" ) # FIXME
endif ( TTCARDS_JSON_SPIRIT_COMPILE )

if ( TTCARDS_NOMLIB_COMPILE )
  message ( "Building dependency nomlib" )
  add_subdirectory ( gamelib )
  include_directories ( gamelib/include )
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} "gamelib" ) # FIXME
endif ( TTCARDS_NOMLIB_COMPILE )

if ( NOMJSON_LIBRARY )
  message ( "Building dependency nomJSON" )
  add_subdirectory ( nomJSON )
  include_directories ( nomjson/src )
  # set ( REQUIRED_LIBS ${REQUIRED_LIBS} "nomjson" )
endif ( NOMJSON_LIBRARY )

add_subdirectory ( ttcards )

if ( BUILD_RELEASE OR BUILD_OSXAPP )

  #     Uninstall target

  configure_file  (
                    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                    IMMEDIATE @ONLY
                  )

  add_custom_target ( uninstall
                      COMMAND ${CMAKE_COMMAND} -P
                      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                    )

endif ( BUILD_RELEASE OR BUILD_OSXAPP )
