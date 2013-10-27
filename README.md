# TTcards #

Remake of a beloved card game from my youth -- SquareSoft's Final Fantasy VIII Triple Triad -- no machine reverse engineering has been done (per se), other than from the game sprites. The code base derives entirely from many hours of amateur visual study and speculation (personal judgment) of how the game logic works based on reaction.

## Building Instructions ##

### Dependencies ###

* [cmake v2.6+](http://www.cmake.org/)
* clang (v3.x) with libc++ libraries or recent GCC version (known to work with v4.6.x) with c++0x

* [SDL v2.0.0](http://libsdl.org/)
* [nomlib from dev branch](https://github.com/i8degrees/nomlib)

* [doxygen v1.8.x](http://www.stack.nl/~dimitri/doxygen/) and [graphviz](http://www.graphviz.org/) are needed if you plan on generating the documentation.

### OSX v10.7+ ###

After installing the necessary dependencies listed above -- either through brew
or perhaps hunting down each individual dependency Framework:

```
git clone https://github.com/i8degrees/ttcards
cd ttcards
mkdir build && cd build
```

* Binary & resources built in current build directory:

```
cmake ..
make
./ttcards
```

* OSX App Bundle:

```
cmake -D OSXAPP=on ..
make
make install
open ttcards.app
```

* Binary installed under default CMAKE_INSTALL_PREFIX (/usr or /usr/local):

```
cmake -D OSXAPP=off ..
make
make install
ttcards
```

Uninstall is provided by executing **make uninstall** inside your local build directory; or manually doing so by looking at the **cmake_uninstall.cmake** list provided in the local cmake build directory.

* Without the full resources folder package, you won't get very far into execution
before a crash occurs -- this will be fixed in the future.

### Linux ###

Until I get around to writing proper instructions, you may take a look at my
.travis.yml build script in the root of this repository for hints.

#### Build Status ####

[![Build Status](https://travis-ci.org/i8degrees/ttcards.png?branch=master,dev)](https://travis-ci.org/i8degrees/ttcards)
