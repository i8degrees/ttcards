cmake_minimum_required ( VERSION 2.8 )

set ( EMCC off )
set ( NOMJSON_LIBRARY off )

set ( CMAKE_VERBOSE_MAKEFILE true )
set ( CMAKE_BUILD_TYPE Debug )
#set ( CMAKE_OSX_ARCHITECTURES i386;x86_64 ) # OSX Universal Library

if ( CMAKE_GENERATOR STREQUAL Xcode )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
endif ()

if ( CMAKE_SYSTEM_NAME STREQUAL Linux ) # Tested on Ubuntu v12.04-LTS(?) with
                                        # applicable dependencies installed
    message ( "Linux OS detected " )
    set ( CMAKE_CXX_COMPILER "/usr/bin/c++" )
    set ( CMAKE_CXX_FLAGS -std=gnu++0x )
elseif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )
    message ( "Darwin OS (Mac OSX) detected" )  # Developed 100% on OSX v10.8.3
    set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
    set ( CMAKE_EXE_LINKER_FLAGS "-std=c++11 -stdlib=libc++" )
    set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
    set ( CMAKE_CXX_FLAGS_DEBUG "-gfull -O0 -Wall" )
    set ( CMAKE_CXX_FLAGS_RELEASE "-O2" )
else ()
    message ( "Unsupported build system detected... Hopefully CMake gets it right for you; good luck!" )
endif ()

if ( EMCC STREQUAL on )
    set ( EMCC_BIN "$ENV{HOME}/local/src/emscripten/emcc" )
    set ( EMCC_CFLAGS --jcache )
    set ( EMCC_TARGET libgamelib.bc )
    set ( LLVM_ANALYZER_BIN "$ENV{HOME}/local/checker/scan-build" )
    set ( LLVM_ANALYZER_ARGS -k --use-c++=/usr/bin/clang++ /usr/bin/clang++ -c )
endif ()

set ( SRC_DIR src ) # project_dir/src

SUBDIRS ( json_spirit gamelib ttcards )

include_directories ( json_spirit )
include_directories ( gamelib/src )

if ( NOMJSON_LIBRARY STREQUAL on )
    add_subdirectory ( nomJSON )
    include_directories ( nomjson/src )
endif ()
