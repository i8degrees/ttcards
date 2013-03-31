/******************************************************************************
    gfx.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GFX_HEADERS
#define GFX_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

class Gfx {
public:
  Gfx ( void );
  ~Gfx ( void );

  bool Init ( int flags=SDL_INIT_EVERYTHING );

  SDL_Surface *SetMode (  unsigned int screen_width=320,
                          unsigned int screen_height=240,
                          unsigned int screen_bpp=8, int flags=0 );

  bool SetWindowTitle ( const char app_name[255] );
  bool SetWindowIcon ( const char app_icon[255] );

  SDL_Surface *LoadBackground ( std::string filename );

private:
  //
};

#endif // GFX_HEADERS defined
