/******************************************************************************
    font.cpp

    SDL-based Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "font.h"

Font::Font ( void )
{
  #ifdef DEBUG_FONT_OBJ
    std::cout << "Font::Font (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->font = NULL;

  if ( TTF_Init () == -1 )
  {
    #ifdef DEBUG_FONT
      std::cout << "ERR in Font::Font (): " << TTF_GetError() << std::endl;
    #endif
    exit ( EXIT_FAILURE );
  }

  this->text_color = { 0, 0, 0 };
  this->coords = { 0, 0, 0, 0 };
}

Font::~Font ( void )
{
  #ifdef DEBUG_FONT_OBJ
    std::cout << "Font::~Font (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  TTF_CloseFont ( this->font );
  this->font = NULL;

  TTF_Quit ();
}

SDL_Color Font::GetTextColor ( void )
{
  return this->text_color;
}

void Font::SetTextColor ( unsigned r, unsigned g, unsigned b )
{
  this->text_color.r = r;
  this->text_color.g = g;
  this->text_color.b = b;
}

bool Font::LoadTTF ( std::string filename, unsigned int font_size )
{
  this->font = TTF_OpenFont ( filename.c_str(), font_size );

  if ( this->font == NULL )
  {
    #ifdef DEBUG_FONT
      std::cout << "ERR: " << TTF_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool Font::DrawText ( Gfx &engine, std::string text, unsigned int x, unsigned int y )
{
  SDL_Surface *text_buffer = NULL;
  coords.x = x;
  coords.y = y;

  text_buffer = TTF_RenderText_Solid ( this->font, text.c_str(), this->text_color );

  engine.DrawSurface ( text_buffer, x, y );

  SDL_FreeSurface ( text_buffer );

  return true;
}
