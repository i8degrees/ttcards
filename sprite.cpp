/******************************************************************************
    sprite.cpp

  Basic Sprite Engine API using SDL in C++

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "sprite.h"

using namespace std;

Sprite::Sprite ( int width, int height )
{
  this->sprite_buffer = NULL;

  this->width = width;
  this->height = height;

  this->id = 0;

  this->src.x = 0;
  this->src.y = 0;
  this->src.w = this->width;
  this->src.h = this->height;
}

Sprite::~Sprite ( void )
{
  SDL_FreeSurface ( this->sprite_buffer );
  this->sprite_buffer = NULL;
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

  SDL_SetColorKey ( tmpBuffer, SDL_RLEACCEL | SDL_SRCCOLORKEY,
                   SDL_MapRGB ( tmpBuffer->format, alpha.r, alpha.g, alpha.b ) );

  this->sprite_buffer = SDL_DisplayFormatAlpha ( tmpBuffer );

  SDL_FreeSurface ( tmpBuffer );

  //printf("%d %d %d %d %d %s\n", sprite_ptr->id, sprite_ptr->x, sprite_ptr->y, sprite_ptr->width, sprite_ptr->height, filename);

  return true;
}

bool Sprite::Draw ( SDL_Surface *video_buffer )
{
  if ( this->id != 0 )
  {
    this->src.x = ( this->id * this->width );
    this->src.y = 0;
  }
  else
  {
    this->src.x = 0;
    this->src.y = 0;
  }

  this->src.w = this->width;
  this->src.h = this->height;

  this->dest.x = this->x;
  this->dest.y = this->y;
  this->dest.w = this->width;
  this->dest.h = this->height;

  SDL_BlitSurface(this->sprite_buffer, &src, video_buffer, &dest);

  return true;
}
