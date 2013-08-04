# json_spirit #

Project fork of [json_spirit](http://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented) v4.06 library, primarily with OS X distribution in mind.

## Why ##

Originally, it was simply to include this library into my project as a git submodule for linking to. I had trouble linking to the json_spirit library provided by brew (OSX). As with all things in life, it slowly grew from there.

## Changes From Original ##

Except for the minor last two items, no changes have been made to the library source. All changes found are within the build script -- CMakeLists.txt. The first two, perhaps four items are important (to me).

* Option to build as either a shared or static library (shared by default).
    > I had trouble using json_spirit on OSX when compiled as a static library -- something to do with the linking stage and json_spirit_value.o not having symbols. Building as a shared library fixes my problem.

* Add option to build as either debug or release binary (release by default)
    > What a difference it makes in built library file sizes!

* Add install_name_dir CMake property to build library so we do not have to manually "fix" it with install_name_tool when bundling it with third-party software; this only affects the build if you are building within the OS X environment.
* Add OSX Framework bundle support; this option is only available within the OS X environment.

* CMake option for OSX Universal Libraries. By default, this feature is disabled. This option is only available within the OS X environment.
* Add soname version property to target library
* CMake module for finding json_spirit
* 'make uninstall' target support
* Drop the minimum required CMake version to 2.0
* Add CMake options for building demos and tests; default is enabled.
    > I actually never intended on doing this, but this is intended to be a temporary fix until I feel like diagnosing why my [Travis-CI build](http://travis-ci.org/i8degrees/json_spirit) does not succeed due to the examples being included.

* Add CMake options to build the features you want (so you do not have to edit any header files). By default, all features are enabled.
* Fix compile warning (see my second commit log for details)

## Projects Using This Fork ##

* [i8degrees/ttcards](https://github.com/i8degrees/ttcards)

## Building Instructions ##

### Dependencies ###

* cmake v2.0+
* Boost v1.34+

### Build Status ###

[![Build Status](https://travis-ci.org/i8degrees/json_spirit.png?branch=master)](https://travis-ci.org/i8degrees/json_spirit)

After obtaining the necessary dependencies listed above:

```
  git clone https://github.com/i8degrees/json_spirit
  cd json_spirit
  mkdir build && cd build
  cmake ..
  make
  make install
```

Uninstall support is provided by running 'make uninstall' within the build directory.

## Pull Requests ##

Pull requests that pertain to OS X packaging / distribution are welcome as long as they do ***NOT*** apply only to a specific project.
