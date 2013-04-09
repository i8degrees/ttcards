/******************************************************************************
    sprite.h

  Basic Sprite Engine API using SDL in C++

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef SPRITE_HEADERS
#define SPRITE_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "SDL/SDL.h"

class Sprite {
  public:

    unsigned int x; // sprite x-axis blitting location
    unsigned int y; // sprite y-axis blitting location
    unsigned int z;
    unsigned int id; // crude sprite sheet functionality
    unsigned int state;

    Sprite ( int width, int height );
    ~Sprite ( void );

    bool LoadImage ( std::string filename );
    bool Draw ( SDL_Surface *video_buffer );

  private:
    unsigned int width; // sprite width
    unsigned int height; // sprite height
    //unsigned int x_offset; // sprite input x-axis location
    //unsigned int y_offset; // sprite input y-axis location

    SDL_Rect src, dest;
    SDL_Surface *sprite_buffer; // memory buffer allocation
};

#endif // SPRITE_HEADERS defined
