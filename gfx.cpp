/******************************************************************************
    gfx.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "gfx.h"

using namespace std;

Gfx::Gfx ( void )
{
  SDL_Surface *video_buffer = NULL;
}

Gfx::~Gfx ( void )
{
  //SDL_FreeSurface ( video_buffer );
}

bool Gfx::Init ( void )
{
  return true;
}

SDL_Surface *Gfx::LoadBackground ( std::string filename )
{
  SDL_Surface *tmpBuffer = NULL;

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
