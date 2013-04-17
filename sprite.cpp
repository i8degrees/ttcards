/******************************************************************************
    sprite.cpp

    SDL-based Sprite Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "sprite.h"

Sprite::Sprite ( void )
{
  #ifdef DEBUG_SPRITE_OBJ
    std::cout << "Sprite::Sprite (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  // Stub constructor
  this->sprite_buffer = NULL;

  this->coords.x = 0;
  this->coords.y = 0;
  this->coords.w = 0;
  this->coords.h = 0;
  this->offsets.x = 0;
  this->offsets.y = 0;
  this->offsets.w = 0;
  this->offsets.h = 0;

  this->sheet_id = 000000;

  this->offsets.x = 0;
  this->offsets.y = 0;
  this->offsets.w = 0;
  this->offsets.h = 0;
}

Sprite::Sprite ( unsigned int width, unsigned int height )
{
  #ifdef DEBUG_SPRITE_OBJ
    std::cout << "Sprite::Sprite (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->sprite_buffer = NULL;

  this->coords.x = 0;
  this->coords.y = 0;
  this->coords.w = width;
  this->coords.h = height;
  this->offsets.x = 0;
  this->offsets.y = 0;
  this->offsets.w = width;
  this->offsets.h = height;

  this->sheet_id = 000000;

  this->offsets.x = 0;
  this->offsets.y = 0;
  this->offsets.w = width;
  this->offsets.h = height;
}

Sprite::~Sprite ( void )
{
  #ifdef DEBUG_SPRITE_OBJ
    std::cout << "Sprite::~Sprite (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  SDL_FreeSurface ( this->sprite_buffer );
  this->sprite_buffer = NULL;
}

unsigned int Sprite::GetX ( void )
{
  return this->coords.x;
}

unsigned int Sprite::GetY ( void )
{
  return this->coords.y;
}

unsigned int Sprite::GetWidth ( void )
{
  return this->coords.w;
}

unsigned int Sprite::GetHeight ( void )
{
  return this->coords.h;
}

unsigned int Sprite::GetXOffset ( void )
{
  return this->offsets.x;
}

unsigned int Sprite::GetYOffset ( void )
{
  return this->offsets.y;
}

unsigned int Sprite::GetWidthOffset ( void )
{
  return this->offsets.w;
}

unsigned int Sprite::GetHeightOffset ( void )
{
  return this->offsets.h;
}

void Sprite::SetX ( unsigned int x )
{
  this->coords.x = x;
}

void Sprite::SetY ( unsigned int y )
{
  this->coords.y = y;
}

void Sprite::SetXY ( unsigned int x, unsigned int y )
{
  this->coords.x = x;
  this->coords.y = y;
}

void Sprite::UpdateXY ( unsigned int x, unsigned int y )
{
  this->coords.x += x;
  this->coords.y += y;
}

void Sprite::SetWidth ( unsigned int width )
{
  this->coords.w = width;
}

void Sprite::SetHeight ( unsigned int height )
{
  this->coords.h = height;
}

void Sprite::SetXOffset ( unsigned int x_offset )
{
  this->offsets.x = x_offset;
}

void Sprite::SetYOffset ( unsigned int y_offset )
{
  this->offsets.y = y_offset;
}

void Sprite::SetWidthOffset ( unsigned int width_offset )
{
  this->offsets.w = width_offset;
}

void Sprite::SetHeightOffset ( unsigned int height_offset )
{
  this->offsets.h = height_offset;
}

unsigned int Sprite::GetSheetID ( void )
{
  return this->sheet_id;
}

unsigned int Sprite::GetState ( void )
{
  return this->state;
}

void Sprite::SetSheetID ( unsigned int id )
{
  this->sheet_id = id;
}

void Sprite::SetState ( unsigned int state )
{
  this->state = state;
}

/*
bool Sprite::LoadImage ( std::string filename, SDL_Color colorkey, unsigned int flags )
{
  // Ensure that we have a clean surface available
  if ( this->sprite_buffer != NULL )
  {
    SDL_FreeSurface ( this->sprite_buffer );
    this->sprite_buffer = NULL;
  }

  this->sprite_buffer = this->engine->LoadImage ( filename, colorkey, flags );

  if ( this->sprite_buffer == NULL )
  {
    #ifdef DEBUG_SPRITE
      //std::cout << "ERR in Sprite::LoadImage (): " << SDL_GetError() << std::endl;
    #endif
    SDL_FreeSurface ( this->sprite_buffer );
    this->sprite_buffer = NULL;

    return false;
  }

  return true;
}
*/

bool Sprite::LoadImage ( std::string filename )
{
  // Ensure that we have a clean surface available
  if ( this->sprite_buffer != NULL )
  {
    SDL_FreeSurface ( this->sprite_buffer );
    this->sprite_buffer = NULL;
  }

  this->sprite_buffer = IMG_Load ( filename.c_str() );

  if ( this->sprite_buffer == NULL )
  {
    #ifdef DEBUG_SPRITE
      std::cout << "ERR in Sprite::LoadImage (): " << SDL_GetError() << std::endl;
    #endif
    SDL_FreeSurface ( this->sprite_buffer );
    this->sprite_buffer = NULL;

    return false;
  }

  return true;
}

bool Sprite::Draw ( Gfx &engine )
{
  if ( this->sheet_id != 000000 )
  {
    this->offsets.x = ( this->sheet_id * this->offsets.w );
    this->offsets.y = 0;
  }

  if ( this->sprite_buffer == NULL )
  {
    #ifdef DEBUG_SPRITE
      std::cout << "ERR in Sprite::Draw(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  if ( engine.DrawSurface ( this->sprite_buffer, this->GetX(), this->GetY(), &offsets ) == false )
  {
    return false;
  }

  return true;
}
