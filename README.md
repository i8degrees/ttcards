# TTcards #

Remake of a beloved card game from my youth -- SquareSoft's Final Fantasy VIII Triple Triad -- no machine reverse engineering has been done (per se), other than from the game sprites. The code base derives entirely from many hours of amateur visual study and speculation (personal judgment) of how the game logic works based on reaction.

## Building Instructions ##

### Dependencies ###

* cmake v2.6+
* [nomlib](https://github.com/i8degrees/nomlib)
* SDL v1.2.15
* clang (v3.x) with libc++ libraries or recent GCC version (known to work with v4.6.x) with c++0x

### OSX ###

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

Uninstall support is provided by running 'make uninstall' within the build directory.

* Without the full resources folder package, you won't get very far into execution
before a crash occurs -- this will be fixed in the future.

### Linux ###

* Until I get around to writing proper instructions, you may take a look at my
.travis.yml build script in the root of this repository.

#### Build Status ####

[![Build Status](https://travis-ci.org/i8degrees/ttcards.png?branch=master,dev)](https://travis-ci.org/i8degrees/ttcards)
