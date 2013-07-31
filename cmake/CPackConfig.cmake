# This file controls the generation details of our packages through CPack
# variables set below.
#
# These variables should be set in the CMake list file of the project *BEFORE*
# the CPack module is included.

set ( CPACK_PACKAGE_NAME "${PROJECT_NAME}" )
set ( CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )
set ( CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )

set ( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )

# PackageMaker refuses to let us use the Markdown extension (MD) -_-
#
# TODO / Generate these text files on the fly from the Markdown sources
set ( CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/cmake/templates/LICENSE.txt" )
set ( CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/cmake/templates/README.txt" )
set ( CPACK_RESOURCE_FILE_WELCOME "${PROJECT_SOURCE_DIR}/cmake/templates/Welcome.txt" )
set ( CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/cmake/templates/Description.txt" )

set ( CPACK_PACKAGE_VENDOR "org.github-i8degrees" )
set ( CPACK_PACKAGE_CONTACT "jeffrey.carp@gmail.com" )
set ( CPACK_PACKAGE_DESCRIPTION "Triple Triad Remake" )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "TTcards - Triple Triad Remake" )
# TODO / Debian package dependencies
#set ( CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-dev libmagic-dev libsndfile1-dev libopenal-dev libsdl1.2-dev libsdl1.2debian libsdl-image1.2-dev libsdl-image1.2 libsdl-ttf2.0-dev libsdl-ttf2.0-0" )
set ( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
set ( CPACK_DEBIAN_PACKAGE_SECTION "games" )
set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )

set ( CPACK_STRIP_FILES true )
#set ( CPACK_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR} )
#set ( CPACK_PACKAGING_INSTALL_PREFIX "/usr" )
