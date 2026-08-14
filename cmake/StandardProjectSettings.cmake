# cmake/StandardProjectSettings.cmake:jeff
#

if(TTCARDS_USE_CCACHE)
  find_program(CCACHE ccache)
  if(CCACHE)
    message(STATUS "Using ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
  else(NOT CCACHE)
    message(STATUS "Ccache not found")
  endif()
endif()

# EXPERIMENTAL(JEFF): This is a WIP and not ready for general consumption.
if(TTCARDS_USE_DISTCC)
  find_program(DISTCC distcc)
  if(DISTCC)
    message(STATUS "Using distcc")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_CXX_COMPILER_LAUNCHER};${DISTCC}")
  else(NOT DISTCC)
    message(STATUS "distcc not found")
  endif(DISTCC)
endif()

# ttcards began its life using c++14 with stdlib libc++ on MacOSX v10.8
# (i386; x86_64) arch
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)

  # Set the possible values of build type for cmake-gui, ccmake
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
  set(CMAKE_VERBOSE_MAKEFILE ON)
  add_compile_definitions("TTCARDS_DEBUG")
  message(STATUS "[TTCARDS_DEBUG] Debugging macros and logging activated.")
endif()

#option(DEBUG "Enable debug statements and asserts" OFF)
#if(DEBUG OR CMAKE_BUILD_TYPE STREQUAL "Debug")
#add_compile_definitions(DEBUG APP_PROFILE)
#endif()

if(DEBUG_ASSERT)
  # Enable TTCARDS_ASSERT macros
  add_compile_definitions("TTCARDS_DEBUG_ASSERT")
  # FIXME(JEFF): NOM_ASSERT macros break if we do not explicitly add it here
  add_compile_definitions("NOM_DEBUG_ASSERT")
  message(STATUS "[TTCARDS_DEBUG_ASSERT] Run-time assertions are enabled.")
else(NOT DEBUG_ASSERT)
  # Disable TTCARDS_ASSERT macros
  add_compile_definitions("NDEBUG")
  message(STATUS "[NDEBUG] Disabling run-time assertions")
endif()

# Generate compile_commands.json to make it easier to work with clang based
# tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# cmake option NOLOG - disable engine logging
option(NOLOG "Disable logging" OFF)
if(NOLOG)
  # TODO(JEFF): This needs to be implemented in config.hpp.in;
  # rewrap our logging macros
endif()

message(STATUS "Generating CMake project files with ${CMAKE_GENERATOR}")
