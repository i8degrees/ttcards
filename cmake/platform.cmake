# Platform detection script for CMake

if ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
  set ( PLATFORM_OSX true )

  option ( OSXAPP "Enable building OS X Application Bundle" on )
  option ( UNIVERSAL "Enable building OSX Universal Application" off )

  # Setup the SDK selection for backwards compatibility
  set ( SDKVER "10.7" )
  set ( DEVROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer" )
  set ( SDKROOT "${DEVROOT}/SDKs/MacOSX${SDKVER}.sdk" )

  if ( EXISTS ${SDKROOT} )
    set ( CMAKE_OSX_SYSROOT "${SDKROOT}" )
    set ( CMAKE_OSX_DEPLOYMENT_TARGET "${SDKVER}" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=${SDKVER}" )
  else ( NOT EXISTS ${SDKROOT} )
    # Binary compatibility will be limited to platform's version
    message ( WARNING "Warning, Mac OS X ${SDKVER} SDK path not found: ${SDKROOT}" )
  endif ( EXISTS ${SDKROOT} )

  # libc++ requires OSX v10.7+
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++" )

  if ( CMAKE_GENERATOR STREQUAL "Xcode" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
  endif ( CMAKE_GENERATOR STREQUAL "Xcode" )

  message ( STATUS "Platform: Darwin (Mac OS X)" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL "Linux" ) # Tested on Ubuntu v12.04-LTS
  set ( PLATFORM_LINUX true )

  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )

  message ( STATUS "Platform: Linux" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  set ( PLATFORM_WINDOWS true )

  option ( ARCH_32 "Compile ${PROJECT_NAME} as a 32-bit library" off )
  option ( ARCH_64 "Compile ${PROJECT_NAME} as a 64-bit library" on )

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
