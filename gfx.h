/******************************************************************************
    gfx.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GFX_HEADERS
#define GFX_HEADERS

#include <iostream>

#include "SDL.h"

class Gfx {
public:
  Gfx ( void );
  ~Gfx ( void );

  bool Init ( void );

  SDL_Surface *LoadBackground ( std::string filename );
private:
  SDL_Surface *video_buffer;
};

#endif // GFX_HEADERS defined
