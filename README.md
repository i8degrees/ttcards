# TTcards

A remake of SquareSoft's Final Fantasy VIII Triple Triad card game in C++. The underlying code base spawned another project, [nomlib](https://github.com/i8degrees/nomlib/), while developing the game.

No machine reverse engineering has been done here, other than using ripped audio samples & sprite images; see CREDITS.md for details). This clone is derived in its entirety by many hours of my own study and speculation of how the game logic might be implemented.

## Building

First, ensure that you have the following core dependencies installed:

* [nomlib](https://github.com/i8degrees/nomlib/)
* [cmake v2.6+](http://www.cmake.org/)
* [git](http://git-scm.com/download/)

### Library Dependencies

The same dependencies used in nomlib can be used in TTcards. See nomlib's [sourceforge.net project files page](http://sourceforge.net/projects/nomlib/files/) for downloading pre-packaged dependencies for your platform. 

1. Create a new directory under the game's source repository called ```third-party```.
2. Download the latest dependencies for your platform in the ```third-party``` directory and extract.
3. Profit?

### Mac OS X

Requires clang with the libc++ standard library (v3.x through v5.x are known to work for me on OS X).

```
git clone https://github.com/i8degrees/ttcards
cd ttcards
mkdir build && cd build
cmake -DDEBUG=on -DDEBUG_ASSERT=on -DNOMLIB_DEPS_PREFIX=<your_library_deps_path>/osx -DCMAKE_OSX_DEPLOYMENT_TARGET=10.7 ..
make
make install
```

Replace **<library_deps_path>** with the full directory path of the library package you have extracted, i.e.: ```~/Downloads/ttcards/third-party```. Be sure that you do not forget to append ```/osx``` to the end of path.

Upon a successful build, you should have an OS X application bundle under your current build directory called ```ttcards.app```.

### Linux

Linux builds are broken at the moment.

~~Until I get around to writing the proper instructions, you may take a look at my
.travis.yml build script in the project root of ttcards for hints!~~

### Microsoft Windows

Requires [Microsoft Visual Studio Express 2013 for Windows](http://www.microsoft.com/visualstudio/eng#downloads).

  - Required software tools in your system PATH
    * MSVSCPP -- ```<MSVSCPP_INSTALL_PATH>\bin```
    * CMake -- ```<CMAKE_INSTALL_PATH>\bin```

**NOTE:** Alpha build quality. This has only been tested on MS Windows 7.

```
git clone https://github.com/i8degrees/ttcards
cd ttcards
mkdir build && cd build
cmake -G"Visual Studio 12" -DARCH_32=on -DDEBUG=on -DDEBUG_ASSERT=on -DNOMLIB_DEPS_PREFIX=<your_library_deps_path>/windows ..
```

Replace **<library_deps_path>** with the full directory path of the library package you have extracted, i.e.: ```~/Downloads/ttcards/third-party```. Be sure that you do not forget to append ```/windows``` to the end of path.

Upon a successful generation, you should have populated your current build directory with a Visual Studio solution file called ```ttcards.vcxproj``` that you can then open and build from.

### Project Options ###

Build options are passed to cmake with the -D option. For example, to change the installation prefix:

```
cmake -D CMAKE_INSTALL_PREFIX=$HOME/Applications ..
```

  - Installation path: -D CMAKE_INSTALL_PREFIX=**\<DIRECTORY_PREFIX\>**
    * Defaults to your current **build** directory

  - Debugging: -D DEBUG=**\<BOOLEAN\>** -D DEBUG_ASSERT=**\<BOOLEAN\>**
    * Defaults to **OFF**

  - Universal binary: -D UNIVERSAL=**\<BOOLEAN\>**
    * Defaults to **OFF**

Removal is provided by executing **make uninstall** within your current build directory.

## Why ##

I was interested in getting back into game programming, and thought this would be an excellent long-term exercise in re-acquainting myself with C++, object-oriented design and general video game technology.

## Other Remakes ##

- Other remakes I am aware of
  * [Triple Triad Advance](http://www.ttadvance.ca/index.php/)
  * [Triple Triad Flash Online](http://www.tripletriadflashonline.com/)
  * [Android Triple Triad](https://github.com/Viish/Android-Triple-Triad/)
  * [The 7 Elders](http://www.kongregate.com/games/TadeuBAS/the-7-elders/)
