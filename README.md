TTcards
=======

Remake (clone) of SquareSoft's Final Fantasy VIII Triple Triad card game.

Source Build instructions
-------------------------

This is mostly a stub for the time being, but in theory, provided that you have
the necessary libraries and build tools installed:

OSX
---

1. git clone https://github.com/i8degrees/ttcards
2. cd ttcards
3. git submodule init
4. git submodule update
5. mkdir build; cd build
6. cmake ..
7. make
8. cd ../ttcards; ./ttcards

* Without the full resources folder package, you won't get very far into execution
before a crash occurs!

Dependencies
------------

* cmake v2.8.10.2
* Boost v1.53.0
* SDL v1.2.15
* SDL_image v1.2.12
* SDL_mixer v1.2.12
* SDL_ttf v2.0.11
* A recent compiler that has support for the latest C++11 extensions
