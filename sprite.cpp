/******************************************************************************
    sprite.cpp

  Basic Sprite Engine written for SDL in C++

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/

#include "sprite.h"

Sprite::Sprite ( int x, int y, int z, int w, int h, int id )
{
  //sprite_buffer = NULL;

  Sprite::id = id;
  Sprite::x = x;
  Sprite::y = y;
  Sprite::z = z;
  Sprite::width = w;
  Sprite::height = h;
}

bool Sprite::Load ( string filename )
{

  if ( ( sprite_buffer = SDL_LoadBMP ( filename.c_str() ) ) == NULL )
  {
#ifdef DEBUG
    std::cout << "ERROR: Could not load " << filename << " " << SDL_GetError() );
#endif
    return false;
  }

  //printf("%d %d %d %d %d %s\n", sprite_ptr->id, sprite_ptr->x, sprite_ptr->y, sprite_ptr->width, sprite_ptr->height, filename);

  return true;
}

bool Sprite::Draw ( SDL_Surface *video_buffer )
{
  SDL_Rect dest;
  SDL_Rect src;

  src.x = Sprite::id * Sprite::width; // crude sprite sheet
  src.y = 0;
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
