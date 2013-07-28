#     CPack Linux "DEB" generator

#set ( CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-dev libmagic-dev libsndfile1-dev libopenal-dev libsdl1.2-dev libsdl1.2debian libsdl-image1.2-dev libsdl-image1.2 libsdl-ttf2.0-dev libsdl-ttf2.0-0" )
set ( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
set ( CPACK_DEBIAN_PACKAGE_SECTION "games" )
set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )

set ( CPACK_PACKAGING_INSTALL_PREFIX "/usr" )
