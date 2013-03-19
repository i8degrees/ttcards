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

#include <SDL.h>
#include <string>
#include <vector>

using namespace std;

class Sprite
{
public:
  unsigned int id; // crude sprite sheet functionality
  unsigned int x; // sprite x-axis blitting location
  unsigned int y; // sprite y-axis blitting location
  unsigned int z;
  unsigned int x_offset; // sprite input x-axis location
  unsigned int y_offset; // sprite input y-axis location
  unsigned int width; // sprite width
  unsigned int height; // sprite height
  unsigned int state;

  Sprite ( int x, int y, int z, int w, int h, int id = 0);

  bool Load ( string filename );
  //bool Load ( char filename, int id = NULL, int x_offset = 0, int y_offset = 0 );
  bool Draw ( SDL_Surface *video_buffer );
  bool Destroy ( void );

private:
  SDL_Surface *sprite_buffer = NULL; // memory buffer allocation
};
