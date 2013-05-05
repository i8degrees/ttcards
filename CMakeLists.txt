cmake_minimum_required ( VERSION 2.6 )

set ( CMAKE_VERBOSE_MAKEFILE true )
set ( CMAKE_BUILD_TYPE Debug )
#set ( CMAKE_OSX_ARCHITECTURES i386;x86_64 ) # OSX Universal Library

set ( EMCC_BIN "$ENV{HOME}/local/src/emscripten/emcc" )
set ( EMCC_CFLAGS --jcache )
set ( EMCC_TARGET public/index.html )
set ( EMCC_PRELOAD "${CMAKE_CURRENT_SOURCE_DIR}/data/images" )
set ( LLVM_ANALYZER_BIN "$ENV{HOME}/local/checker/scan-build" )
set ( LLVM_ANALYZER_ARGS -k --use-c++=/usr/bin/clang++ /usr/bin/clang++ -c )

set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
set ( CMAKE_CXX_FLAGS_DEBUG "-gfull -O0 -Wall" )
set ( CMAKE_CXX_FLAGS_RELEASE "-O2" )
set ( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR} )
set ( GAMELIB_HEADERS /usr/local/include/gamelib )

include_directories ( ${GAMELIB_HEADERS} )

project(ttcards)

set ( SRC_DIR src ) # project_dir/src

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
      ${SRC_DIR}/cpu_player.cpp
      ${SRC_DIR}/ttcards.cpp
    )

find_package ( SDL REQUIRED )

link_libraries (  ttcards # makefile target
                  ${SDL_LIBRARY}
                  SDLmain
                  gamelib
                )

add_executable ( ttcards ${TTCARDS_SOURCE} )

file ( GLOB files ${TTCARDS_SOURCE} )
foreach ( filename ${files} )
  set ( PROJECT_FILES ${files} )
endforeach()

# analyze target

add_custom_target ( analyze
                    COMMAND ${LLVM_ANALYZER_BIN} ${LLVM_ANALYZER_ARGS}
                    ${SDL_CFLAGS} ${PROJECT_FILES}
                  )

# EMCC (web) target

add_custom_target ( web
                    ${EMCC_BIN} ${EMCC_CFLAGS} ${CFLAGS} ${LDFLAGS}
                    ${PROJECT_FILES} -o ${EMCC_TARGET}
                  )
