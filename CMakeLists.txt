cmake_minimum_required ( VERSION 2.6 )

set ( CMAKE_VERBOSE_MAKEFILE true )
set ( CMAKE_BUILD_TYPE Debug )

if ( EMCC ) # FIXME
  set ( CMAKE_CXX_COMPILER "/Users/jeff/local/src/emscripten/emcc" )
else ( EMCC )
  set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
endif ( EMCC )

set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )

set ( CMAKE_CXX_FLAGS_DEBUG "-gfull -O0 -Wall" )
set ( CMAKE_CXX_FLAGS_RELEASE "-O2" )

project(ttcards)

set ( SRC_DIR src )
set ( ttcards_SRC
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
      ${SRC_DIR}/cpu_player.cpp
      ${SRC_DIR}/ttcards.cpp
    )

set ( EXECUTABLE_OUTPUT_PATH .. )

find_package ( SDL REQUIRED )
include_directories ( /usr/local/include/gamelib )
link_libraries ( ${SDL_LIBRARY} SDLmain gamelib )
add_executable ( ttcards ${ttcards_SRC} )
