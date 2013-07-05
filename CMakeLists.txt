cmake_minimum_required ( VERSION 2.8 )

# CMake configuration:

set ( CMAKE_VERBOSE_MAKEFILE true )
set ( CMAKE_BUILD_TYPE Debug )
#set ( CMAKE_OSX_ARCHITECTURES i386;x86_64 ) # OSX Universal Library

# optional dependencies:
set ( EMCC off )
set ( NOMJSON_LIBRARY off )

#     Required Dependencies
#
# set false to build & install nomlib & json_spirit from external sources;
# otherwise let us take over and build our own dependencies
set ( TTCARDS_NOMLIB_COMPILE off )
set ( TTCARDS_JSON_SPIRIT_COMPILE off )

if ( CMAKE_GENERATOR STREQUAL Xcode )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
endif ()

# Platform specific:

if ( CMAKE_SYSTEM_NAME STREQUAL Linux ) # Tested on Ubuntu v12.04-LTS(?) with
                                        # applicable dependencies installed
  message ( "Linux OS detected " )

  set ( CMAKE_CXX_COMPILER "/usr/bin/c++" )
  set ( CMAKE_CXX_FLAGS -std=gnu++0x )
  set ( CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wsign-compare" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )
  message ( "Darwin OS (Mac OSX) detected" )  # Developed 100% on OSX v10.8.3

  set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
  set ( CMAKE_EXE_LINKER_FLAGS "-std=c++11 -stdlib=libc++" )
  set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
  set ( CMAKE_CXX_FLAGS_DEBUG "-gfull -O0 -Wall -Wsign-compare" )
else ()
  message ( "Unsupported build system detected... Hopefully CMake gets it right for you; good luck!" )
endif ()

set ( CMAKE_CXX_FLAGS_RELEASE "-O2 -msse2 -mmx" )

if ( EMCC STREQUAL on )
  set ( EMCC_BIN "$ENV{HOME}/local/src/emscripten/emcc" )
  set ( EMCC_CFLAGS --jcache )
  set ( EMCC_TARGET libgamelib.bc )
  set ( LLVM_ANALYZER_BIN "$ENV{HOME}/local/checker/scan-build" )
  set ( LLVM_ANALYZER_ARGS -k --use-c++=/usr/bin/clang++ /usr/bin/clang++ -c )
endif ()

# global project doxygen defaults
set ( DOXYGEN_COMMAND doxygen )
set ( DOXYGEN_CONFIG_FILE doxygen.cfg )

set ( SRC_DIR src ) # project_dir/src

# TTcards dependencies

if ( TTCARDS_JSON_SPIRIT_COMPILE STREQUAL on )
  add_subdirectory ( json_spirit )
  include_directories ( json_spirit )
endif ()

if ( TTCARDS_NOMLIB_COMPILE STREQUAL on )
  add_subdirectory ( gamelib )
  include_directories ( gamelib/src )
  include_directories ( gamelib/src/audio )
  include_directories ( gamelib/src/gfx )
  include_directories ( gamelib/src/gui )
  include_directories ( gamelib/src/math )
  include_directories ( gamelib/src/sys )
endif ()

if ( NOMJSON_LIBRARY STREQUAL on )
    add_subdirectory ( nomJSON )
    include_directories ( nomjson/src )
endif ()

add_subdirectory ( ttcards )
