# TTcards #

A remake of SquareSoft's Final Fantasy VIII Triple Triad card game in C++. The underlying code base spawned another project, [nomlib](https://github.com/i8degrees/nomlib/), while developing the game.

No machine reverse engineering has been done here, other than using ripped audio samples & sprite images; see CREDITS.md for details). This clone is derived in its entirety by many hours of my own study and speculation of how the game logic might be implemented.

## Building ##

First, ensure that you have the following core dependencies installed:

  - A recent compiler supporting C++11:

    * clang with the libc++ standard library (v3.x through v5.x are known to work for me on OS X).

    * GNU's GCC build chain tools using the c++0x standard library (known to compile with v4.6.x+ on a Ubuntu v12.04 LTS virtual machine).

* [nomlib](https://github.com/i8degrees/nomlib/)
* [cmake v2.6+](http://www.cmake.org/)
* [git](http://git-scm.com/download/)


  - Optional
    *  [nomdev](https://github.com/i8degrees/nomdev/), helper scripts for generating project files and consequent building thereof. (See .travis.yml for hints on how to build without nomdev).

    * [doxygen v1.8.x](http://www.stack.nl/~dimitri/doxygen/) and [graphviz](http://www.graphviz.org/) are needed if you plan on generating the documentation.

Next, you should visit the dependencies section for your platform. After these chores are complete, you should be ready to start the building process for your platform!

### OS X ###

After you have the dependencies taken care of, execute the following commands at your terminal prompt:

```
git clone https://github.com/i8degrees/ttcards
cd ttcards
nomdev.rb gen --dev
nomdev.rb build
nomdev.rb install
```

Upon a successful build, you should have an OS X application bundle under your current build directory, named **ttcards.app**.

### Linux ###

Linux builds are broken at the moment.

~~Until I get around to writing the proper instructions, you may take a look at my
.travis.yml build script in the project root of ttcards for hints!~~

#### Linux Build Status #####

[![Build Status](https://travis-ci.org/i8degrees/ttcards.png?branch=master,dev)](https://travis-ci.org/i8degrees/ttcards/)

### Windows ###

After you have the dependencies taken care of, execute the following commands at the DOS prompt:

```
git clone https://github.com/i8degrees/ttcards
cd ttcards
nomdev.rb gen --dev
nomdev.rb build
```

Upon a successful generation, you should have populated your current build directory with several Visual Studio project files: **ttcards.vcxproj** is the one you will want to work with.

You may also be interested in the functionality of **nomdev.rb build**. (Uses the MSVCPP build tools chain with MSBuild to compile the project via command line).

### Project Options ###

Build options are passed to cmake with the -D option. For example, to change the installation prefix:

```
cmake -D CMAKE_INSTALL_PREFIX=$HOME/Applications/Games ..
```

  - Installation path: -D CMAKE_INSTALL_PREFIX=**\<DIRECTORY_PREFIX\>**
    * Defaults to your current **build** directory

  - Documentation: -D DOCS=**\<BOOLEAN\>**
    * Defaults to **OFF**
    * When built (**ON**), the resulting documentation will reside in a new directory named **docs**

  - Debugging: -D DEBUG=**\<BOOLEAN\>** -D DEBUG_ASSERT=**\<BOOLEAN\>** -D DEBUG_TRACE=**\<BOOLEAN\>**
    * Defaults to **OFF**

  - Universal binary: -D UNIVERSAL=**\<BOOLEAN\>**
    * Defaults to **OFF**

Removal is provided by executing **make uninstall** within your current build directory.

## OS X Dependencies ##

* See **third-party/README.md** for where to obtain pre-packaged libraries and how to install them.

## Linux Dependencies ##

* ~~SDL v2.0.1~~
* ~~SDL_image v2.0.0~~
* ~~SDL_ttf v2.0.12~~
* ~~libsndfile v1.0.25~~
* ~~OpenAL~~
* ~~Recent version of GNU's GCC compiler with support for C++11 (known to work with v4.6.x)~~

## Windows Dependencies ##

* See **third-party/README.md** for where to obtain pre-packaged libraries and how to install them.
* [Ruby 1.9.3-p448](http://rubyinstaller.org/downloads/)

* [Microsoft Visual Studio Express 2013 for Windows](http://www.microsoft.com/visualstudio/eng#downloads)

  - Required software tools in your system PATH
    * MSVSCPP -- **\<MSVSCPP_INSTALL_PATH\>\bin**
    * CMake -- **\<CMAKE_INSTALL_PATH\>\bin**
    * Ruby -- **\<RUBY_INSTALL_PATH\>\bin**
    * Git -- **\<GIT_INSTALL_PATH\>\bin**

If this is your first time installing the tools, you might be happy to know that the official installers can do this task for you automatically!

## Why ##

I was interested in getting back into game programming, and thought this would be an excellent long-term exercise in re-acquainting myself with C++, object-oriented design and general video game technology.

## Other Remakes ##

- Other remakes I am aware of
  * [Triple Triad Advance](http://www.ttadvance.ca/index.php/)
  * [Triple Triad Flash Online](http://www.tripletriadflashonline.com/)
  * [Android Triple Triad](https://github.com/Viish/Android-Triple-Triad/)
  * [The 7 Elders](http://www.kongregate.com/games/TadeuBAS/the-7-elders/)
