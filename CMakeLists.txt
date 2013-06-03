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
set ( SRC_DIR src ) # project_dir/src
set ( GAMELIB_SRC gamelib/src ) # project_dir/gamelib/src
set ( NOMJSON_SRC nomjson/src ) # project_dir/nomJSON/src

# LIBGAMELIB
#set ( GAMELIB_HEADERS /usr/local/include/gamelib )

set ( GAMELIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/gamelib/src )
set ( NOMJSON_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/nomJSON/src )

project ( ttcards )

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
      ${SRC_DIR}/ttcards.cpp
      ${SRC_DIR}/GameOverState.cpp
      ${SRC_DIR}/CardsMenuState.cpp
    )

set ( GAMELIB_SOURCE
      ${GAMELIB_SRC}/Coords.cpp
      ${GAMELIB_SRC}/Color.cpp
      ${GAMELIB_SRC}/Logger.cpp
      ${GAMELIB_SRC}/audio.cpp
      #${GAMELIB_SRC}/font.cpp
      ${GAMELIB_SRC}/SDL_TFont.cpp
      ${GAMELIB_SRC}/SDLGradient.cpp
      ${GAMELIB_SRC}/SDLWindowFrame.cpp
      ${GAMELIB_SRC}/SDLMessageBox.cpp
      ${GAMELIB_SRC}/SDL_BitmapFont.cpp
      ${GAMELIB_SRC}/SDL_Cursor.cpp
      ${GAMELIB_SRC}/gfx.cpp
      ${GAMELIB_SRC}/sprite.cpp
      ${GAMELIB_SRC}/SDLInput.cpp
      ${GAMELIB_SRC}/timer.cpp
      ${GAMELIB_SRC}/fps.cpp
      ${GAMELIB_SRC}/OSXFs.cpp
    )

set ( NOMJSON_SOURCE
      ${NOMJSON_SRC}/JSON_Array.cpp
      ${NOMJSON_SRC}/JSON_FileReader.cpp
      ${NOMJSON_SRC}/JSON_FileWriter.cpp
      ${NOMJSON_SRC}/JSON_Object.cpp
      ${NOMJSON_SRC}/JSON_Value.cpp
    )

find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )

find_package ( SDL_image REQUIRED )
include_directories ( ${SDLIMAGE_INCLUDE_DIR} )

find_package ( SDL_mixer REQUIRED )
include_directories ( ${SDLMIXER_INCLUDE_DIR} )

find_package ( SDL_ttf REQUIRED )
include_directories ( ${SDLTTF_INCLUDE_DIR} )

include_directories ( ${GAMELIB_HEADERS} )

include_directories ( ${NOMJSON_HEADERS} )

link_libraries (  ttcards # makefile target
                  ${SDL_LIBRARY}
                  ${SDL_IMAGE_LIBRARY}
                  ${SDL_MIXER_LIBRARY}
                  ${SDL_TTF_LIBRARY}
                  SDLmain
                  #gamelib # LIBGAMELIB
                )

add_executable ( ttcards ${TTCARDS_SOURCE} ${GAMELIB_SOURCE} ${NOMJSON_SOURCE} ) # GAMELIB_SOURCE_COMPILE

file ( GLOB files ${TTCARDS_SOURCE} )
foreach ( filename ${files} )
  set ( PROJECT_FILES ${files} )
endforeach()

# app target for Sublime Build

add_custom_target ( run
                    COMMAND open --wait-apps --fresh -a "Terminal" ${EXECUTABLE_OUTPUT_PATH}/ttcards
                  )

# analyze target

add_custom_target ( analyze
                    COMMAND ${LLVM_ANALYZER_BIN} ${LLVM_ANALYZER_ARGS}
                    ${SDL_CFLAGS} ${PROJECT_FILES}
                  )

# EMCC (web) target

add_custom_target ( emcc
                    ${EMCC_BIN} ${EMCC_CFLAGS} ${CFLAGS} ${LDFLAGS}
                    ${PROJECT_FILES} -o ${EMCC_TARGET}
                  )
