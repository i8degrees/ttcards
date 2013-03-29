/******************************************************************************
    sprite.cpp

  Basic Sprite Engine written for SDL in C++

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "sprite.h"

using namespace std;

Sprite::Sprite ( int width, int height )
{
  Sprite::width = width;
  Sprite::height = height;

  Sprite::id = 0;

  src.x = 0;
  src.y = 0;
  src.w = Sprite::width;
  src.h = Sprite::height;
}

bool Sprite::LoadImage ( std::string filename )
{

  SDL_Surface *tmpBuffer = NULL;
  SDL_Color alpha = { 0, 0, 0 };

  if ( ( tmpBuffer = SDL_LoadBMP ( filename.c_str() ) )  == NULL )
  {
    std::cout << "ERR -255: " << SDL_GetError() << endl;
    return false;
  }

  SDL_SetColorKey ( tmpBuffer, SDL_SRCCOLORKEY,
                   SDL_MapRGB ( tmpBuffer->format, alpha.r, alpha.g, alpha.b ) );

  sprite_buffer = SDL_DisplayFormatAlpha ( tmpBuffer );

  SDL_FreeSurface ( tmpBuffer );

  //printf("%d %d %d %d %d %s\n", sprite_ptr->id, sprite_ptr->x, sprite_ptr->y, sprite_ptr->width, sprite_ptr->height, filename);

  return true;
}

bool Sprite::Draw ( SDL_Surface *video_buffer )
{
  if ( Sprite::id != 0 )
  {
    src.x = (Sprite::id * Sprite::width);
    src.y = 0;
  }
  else
  {
    src.x = 0;
    src.y = 0;
  }

  src.w = Sprite::width;
  src.h = Sprite::height;

  dest.x = Sprite::x;
  dest.y = Sprite::y;
  dest.w = Sprite::width;
  dest.h = Sprite::height;

  SDL_BlitSurface(sprite_buffer, &src, video_buffer, &dest);

  return true;
}

bool Sprite::Destroy ( void )
{
  SDL_FreeSurface(sprite_buffer);

  return true;
}
