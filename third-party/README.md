# Third-Party Dependencies #

[ttcards's](http://github.com/i8degrees/ttcards/) third-party dependencies. Pre-packaged libraries are available for the OS X and Windows platforms and may be found listed as ttcards_**\<PLATFORM\>-dependencies** at:

* [ttcards's sourceforge.net project files](http://sourceforge.net/projects/ttcards/files/)

## Installation ##

Download the package file for your platform and extract inside the third-party folder inside the project root. You should now have several libraries populating third-party/**\<EXTRACTED_PLATFORM\>**. No other dependency hell to ensure -- yay!

## Packaged Libraries ##

The officially packaged development header files of:

* [SDL v2.0.3](http://libsdl.org/)
* [SDL_image v2.0.0](http://www.libsdl.org/projects/SDL_image/)
* [SDL_ttf v2.0.12](http://www.libsdl.org/projects/SDL_ttf/)
  - [libsndfile v1.0.24](http://www.mega-nerd.com/libsndfile/)
    * Distributed framework is copied from the [SFML2 master branch](https://github.com/LaurentGomila/SFML/tree/master/) with me moving the distribution's **sndfile.h** file to it under a new Headers directory.

  * Packaged only under Mac OS X

    * [SDL2_image.framework v2.0.0](http://libsdl.org/projects/SDL_image) built with SDL_IMAGE_USE_COMMON_BACKEND (in order to fix critical pixel mismatch issue; see also https://forums.libsdl.org/viewtopic.php?t=10013&highlight=perhaps+again+pixel+bug+mac).
      * [libpng v1.5.18](https://github.com/Homebrew/homebrew/commits/master/Library/Formula/libpng.rb) from Homebrew's repository with the --universal build flag.
      * [libtiff v4.0.3](https://github.com/Homebrew/homebrew/commits/master/Library/Formula/libtiff.rb) from Homebrew's repository with the --universal build flag.
      * [libjpeg 8d](https://github.com/Homebrew/homebrew/commits/master/Library/Formula/jpeg.rb) from Homebrew's repository with the --universal build flag.

## Licensing ##

Licensing files for each library can be found at the project root of your ttcards directory under LICENSE_**\<LIBRARY_NAME\>**.
