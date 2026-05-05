# Platform detection script for CMake

option ( ARCH_32 "Compile ${PROJECT_NAME} as a 32-bit library" off )
option ( ARCH_64 "Compile ${PROJECT_NAME} as a 64-bit library" on )

if ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
  set ( PLATFORM_OSX true )

  option ( OSXAPP "Enable building OS X Application Bundle" on )
  option ( UNIVERSAL "Enable building OSX Universal Application" off )

  # This variable influences the system header files version we build against,
  # which in turn determines the **minimum** version of OS X this build will
  # be binary compatible with (run on).
  #
  # Internally, this influences the search paths for system header files and
  # also instructs CMake to pass -mmacosx-version-min to the compiler.
  #
  # To modify this variable's resulting value, you should pass
  # -DCMAKE_OSX_DEPLOYMENT_TARGET=<ver> to CMake at the time of project files
  # generation -- 'cmake ..' from your out-of-source build directory.
  message( STATUS "Using SDK: ${CMAKE_OSX_SYSROOT}" )

  # libc++ requires OSX v10.7+
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++" )

  if ( CMAKE_GENERATOR STREQUAL "Xcode" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
  endif ( CMAKE_GENERATOR STREQUAL "Xcode" )

  message ( STATUS "Platform: Darwin (Mac OS X)" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL "Linux" ) # Tested on Ubuntu v12.04-LTS
  set ( PLATFORM_LINUX true )
  set ( PLATFORM_POSIX true )

  # IMPORTANT(JEFF): clang is the only supported compiler as of now
  if ( CMAKE_CXX_COMPILER MATCHES "clang" )
    message ( STATUS "Using clang based platform to build..." )
    # nomlib began its life under c++14 on Intel Darwin Mac OSX
    set ( CMAKE_CXX_STANDARD 14 )
    set ( CMAKE_CXX_STANDARD_REQUIRED ON )
    set ( CMAKE_CXX_EXTENSIONS OFF)

    # libc++ requires OSX v10.7+
    #set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++" )
  elseif ( CMAKE_C_COMPILER MATCHES "gcc" )
    message ( STATUS "Using gcc based platform to build..." )
    message ( FATAL_ERROR "nomlib only supports building with clang." )

    # !! GoogleTest unit testing framework v1.10.x requires a minimum C++ level 11
    set( CMAKE_CXX_STANDARD 11 )
    # !! Our engine is based on a c++ level of 14
    set( CMAKE_CXX_STANDARD 14 )
    # NOTE(JEFF): This should only be set when GNU GCC is enabled?
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )
  endif( CMAKE_CXX_COMPILER MATCHES "clang" )

  message ( STATUS "Platform: Linux" )
  message ( STATUS "Build platform: ${CMAKE_CXX_COMPILER}" )
  message ( STATUS "Compiler C++ level: ${CMAKE_CXX_STANDARD}" )
  message ( STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  set ( PLATFORM_WINDOWS true )

  set ( CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}" )

  message ( STATUS "Platform: Windows" )
else () # Not OSX, Linux or Windows OS
  set ( PLATFORM_UNKNOWN true )

  # Use whatever CMake gathers for us and pray for the best!

  message ( STATUS "Platform: Unknown" )
endif ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )

message ( STATUS "Generating build files for: ${CMAKE_GENERATOR}" )

# TODO; we might be able to put these platform checks shown below in the
# CMAKE_SYSTEM_NAME checks above.
if ( UNIVERSAL )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  set ( PLATFORM_ARCH "x86; x64" ) # Reserved for future use

else ( NOT UNIVERSAL )
  # TODO: We should probably consider bumping the architecture down to i386 $(ARCHS_STANDARD_32_BIT)
  set ( CMAKE_OSX_ARCHITECTURES x86_64 )
  set ( PLATFORM_ARCH "x64" ) # Reserved for future use

endif ( UNIVERSAL )

if ( PLATFORM_WINDOWS AND ARCH_32 )
  set ( PLATFORM_ARCH "x86" )
elseif ( PLATFORM_WINDOWS AND ARCH_64 )
  set ( PLATFORM_ARCH "x64" )
endif ( PLATFORM_WINDOWS AND ARCH_32 )

message ( STATUS "Platform Architecture: ${PLATFORM_ARCH}" )
