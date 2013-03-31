/******************************************************************************
    gfx.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "gfx.h"

using namespace std;

Gfx::Gfx ( void )
{
}

Gfx::~Gfx ( void )
{
}

bool Gfx::Init ( int flags )
{
  if ( SDL_Init ( flags ) == -1 )
  {
    std::cout << "ERR: " << SDL_GetError() << "\n" << endl;
    exit ( EXIT_FAILURE );
  }

  atexit ( SDL_Quit );

  return true;
}

SDL_Surface *Gfx::SetMode ( unsigned int screen_width,
                            unsigned int screen_height,
                            unsigned int screen_bpp, int flags )
{
  SDL_Surface *screen = NULL;

  screen = SDL_SetVideoMode ( screen_width, screen_height, screen_bpp, flags );

  if ( screen == 0 )
  {
    std::cout << "ERR: " << SDL_GetError() << "\n" << endl;
    exit ( EXIT_FAILURE );
  }

  return screen;
}

SDL_Surface *Gfx::LoadBackground ( std::string filename )
{
  SDL_Surface *tmpBuffer = NULL;
  SDL_Surface *video_buffer = NULL;

  if ( ( tmpBuffer = SDL_LoadBMP ( filename.c_str() ) ) == NULL )
  {
    std::cout << "ERR -255: " << SDL_GetError() << std::endl;
    return NULL;
  }

/*
  SDL_SetColorKey ( tmpBuffer, SDL_SRCCOLORKEY,
                   SDL_MapRGB ( tmpBuffer->format, alpha.r, alpha.g, alpha.b ) );
*/

  video_buffer = SDL_DisplayFormatAlpha ( tmpBuffer );

  SDL_FreeSurface ( tmpBuffer );

  return video_buffer;
}

bool Gfx::SetWindowTitle ( const char app_name[255] )
{
  SDL_WM_SetCaption ( app_name, app_name );

  return true;
}

bool Gfx::SetWindowIcon ( const char app_icon[255] )
{
  SDL_Surface *app_icon = NULL;

  app_icon = SDL_LoadBMP ( app_icon );

  //colorkey = SDL_MapRGB(image->format, 0, 0, 0);

  //SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey);

  SDL_WM_SetIcon ( app_icon, NULL );

  SDL_FreeSurface ( app_icon );

  return true;
}
