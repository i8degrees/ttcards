/******************************************************************************
    sprite.h

  Basic Sprite Engine written for SDL in C++

  Copyright (c) 2013 Jeffrey Carpenter

      TODO
  1. id, x_offset, y_offset in Load
  2. animation
  3. z-axis ordering (think: layering)
  4. state management? (think: alive, dying, dead, etc.)
******************************************************************************/
#ifndef SPRITE_HEADERS
#define SPRITE_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "SDL.h"

class Sprite
{
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
  unsigned int x_offset; // sprite input x-axis location
  unsigned int y_offset; // sprite input y-axis location

  SDL_Rect src, dest;
  SDL_Surface *sprite_buffer = NULL; // memory buffer allocation

};

#endif // SPRITE_HEADERS
