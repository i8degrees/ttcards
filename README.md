TTcards
=======

Remake (clone) of SquareSoft's Final Fantasy VIII Triple Triad card game.

Building Instructions
---------------------

### Dependencies ###

* cmake v2.6+
* [nomlib](https://github.com/i8degrees/nomlib)
* [json_spirit](https://github.com/i8degrees/json_spirit)
* SDL v1.2.15
* A recent compiler that has support for the latest C++11 extensions

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
  cmake -DBUILD_OSXAPP=on ..
  make
  make install
  open ttcards.app
```

* Binary installed under default CMAKE_INSTALL_PREFIX (/usr or /usr/local):

```
  cmake -DBUILD_POSIX=on ..
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
