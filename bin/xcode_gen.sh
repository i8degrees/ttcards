#!/bin/sh

# I have no idea **why**, but my Sublime Text 2 build for TTcards fails unless
# I create this directory beforehand ...
/bin/mkdir -p CMakeFiles/ALL_BUILD

cmake -GXcode -DCMAKE_OSX_DEPLOYMENT_TARGET=10.7 \
-DDEBUG=on -DDEBUG_ASSERT=on \
-DNOMLIB_DEPS_PREFIX=${HOME}/Projects/nomlib.git/third-party/osx \
..

