/******************************************************************************
    gfx.h

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GFX_HEADERS
#define GFX_HEADERS

#include <iostream>
#include <string>

#include "cfg.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define DEBUG_GFX
#define DEBUG_GFX_OBJ

class Gfx
{
  public:
    Gfx ( unsigned int screen_width = 320,
          unsigned int screen_height = 240,
          unsigned int screen_bpp = 8,
          unsigned int video_flags = 0,
          unsigned int sdl_flags = SDL_INIT_EVERYTHING,
          unsigned int img_flags = IMG_INIT_PNG
          );

    ~Gfx ( void );

    bool SetSurfaceTransparency ( SDL_Surface *video_buffer,
                                  unsigned int r, unsigned int g, unsigned int b,
                                  unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    static SDL_Surface *LoadImage ( std::string filename, SDL_Color colorkey = { 0, 0, 0 },
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
                          unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

  private:
    SDL_Surface *screen; // primary (think: visible) video memory
};

#endif // GFX_HEADERS defined
