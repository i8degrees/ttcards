/******************************************************************************
    gfx.h

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GFX_HEADERS
#define GFX_HEADERS

#include <iostream>
#include <string>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define DEBUG_GFX
//#define DEBUG_GFX_OBJ

class Gfx {

  public:
    Gfx ( void );
    ~Gfx ( void );

    bool Init ( unsigned int sdl_flags = SDL_INIT_EVERYTHING,
                unsigned int img_flags = IMG_INIT_PNG );

    bool SetVideoMode ( unsigned int screen_width,
                        unsigned int screen_height,
                        unsigned int screen_bpp, unsigned int flags = 0 );

    bool SetSurfaceTransparency ( SDL_Surface *video_buffer,
                                  unsigned int r, unsigned int g, unsigned int b,
                                  unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    SDL_Surface *LoadImage (  std::string filename, SDL_Color colorkey = { 0, 0, 0 },
                              unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    bool DrawSurface (  SDL_Surface *video_buffer, unsigned int x, unsigned int y,
                        SDL_Rect *offsets = NULL );

    bool UpdateScreen ( void );

    bool DrawRectangle (  unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height,
                          unsigned int r, unsigned int g, unsigned int b );

    void SetWindowTitle ( std::string app_name );

    bool SetWindowIcon (  std::string app_icon,
                          SDL_Color colorkey = { 0, 0, 0 },
                          unsigned int flags = SDL_SRCCOLORKEY | SDL_RLEACCEL );

  private:
    SDL_Surface *screen;
};

#endif // GFX_HEADERS defined
