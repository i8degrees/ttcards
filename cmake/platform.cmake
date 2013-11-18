# Platform detection script for CMake

if ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )

  set ( PLATFORM_OSX true )

  option ( OSXAPP "Enable building OS X Application Bundle" on )
  option ( UNIVERSAL "Enable building OSX Universal Application" off )

  message ( STATUS "Platform: Darwin (OS X)" )

elseif ( CMAKE_SYSTEM_NAME STREQUAL "Linux" )

  set ( PLATFORM_LINUX true )
  message ( STATUS "Platform: Linux" )

elseif ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )

  set ( PLATFORM_WINDOWS true )

  option ( ARCH_32 "Compile ${PROJECT_NAME} as a 32-bit library" off )
  option ( ARCH_64 "Compile ${PROJECT_NAME} as a 64-bit library" on )

  message ( STATUS "Platform: Windows" )

else () # Not Linux nor OSX

  set ( PLATFORM_UNKNOWN true )
  message ( STATUS "Platform: Unknown" )

endif ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )

# TODO; we might be able to put these platform checks shown below in the
# CMAKE_SYSTEM_NAME checks above.
if ( UNIVERSAL )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  set ( PLATFORM_ARCH "x86; x64" ) # Reserved for future use

else ( NOT UNIVERSAL )
  set ( CMAKE_OSX_ARCHITECTURES x86_64 )
  set ( PLATFORM_ARCH "x64" ) # Reserved for future use

endif ( UNIVERSAL )

if ( PLATFORM_WINDOWS AND ARCH_32 )
  set ( PLATFORM_ARCH "x86" )
elseif ( PLATFORM_WINDOWS AND ARCH_64 )
  set ( PLATFORM_ARCH "x64" )
endif ( PLATFORM_WINDOWS AND ARCH_32 )

message ( STATUS "Platform Architecture: ${PLATFORM_ARCH}" )
