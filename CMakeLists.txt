cmake_minimum_required ( VERSION 2.8 )

set ( CMAKE_VERBOSE_MAKEFILE true )
set ( CMAKE_BUILD_TYPE Debug )
#set ( CMAKE_OSX_ARCHITECTURES i386;x86_64 ) # OSX Universal Library

set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
#set ( CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++" )
set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
set ( CMAKE_CXX_FLAGS_DEBUG "-gfull -O0 -Wall" )
set ( CMAKE_CXX_FLAGS_RELEASE "-O2" )

set ( GAMELIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/gamelib/src )
#set ( NOMJSON_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/nomJSON/src )
set ( JSON_SPIRIT ${CMAKE_CURRENT_SOURCE_DIR}/json_spirit )

project ( ttcards )

SUBDIRS ( json_spirit gamelib ttcards )

include_directories ( json_spirit )
#include_directories ( nomjson/src )
#include_directories ( gamelib/src )
