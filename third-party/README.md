# Third-Party Dependencies #

[ttcards's](http://github.com/i8degrees/ttcards/) third-party dependencies. Pre-packaged libraries are available for the OS X and Windows platforms and may be found listed as ttcards_**\<PLATFORM\>-dependencies** at:

* [ttcards's sourceforge.net project files](http://sourceforge.net/projects/ttcards/files/)

## Installation ##

Download the package file for your platform and extract inside the root of your project folder for ttcards. You should now have several libraries populating third-party/**\<EXTRACTED_PLATFORM\>**. No other dependency hell to ensure -- yay!

## Packaged Libraries ##

The officially packaged development header files of:

* [SDL v2.0.1](http://libsdl.org/)
* [SDL_image v2.0.0](http://www.libsdl.org/projects/SDL_image/)
* [SDL_ttf v2.0.12](http://www.libsdl.org/projects/SDL_ttf/)
  - [libsndfile v1.0.24](http://www.mega-nerd.com/libsndfile/)
    * Distributed framework is copied from the [SFML2 master branch](https://github.com/LaurentGomila/SFML/tree/master/) with me moving the distribution's **sndfile.h** file to it under a new Headers directory.

## Licensing ##

Licensing files for each library can be found at the project root of your ttcards directory under LICENSE_**\<LIBRARY_NAME\>**.
