/******************************************************************************
    font.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "font.h"

Font::Font ( void )
{
  this->font = NULL;

  if ( TTF_Init () == -1 )
  {
    std::cout << "ERR: " << SDL_GetError() << std::endl;
    exit ( EXIT_FAILURE );
  }

  atexit ( TTF_Quit );
}

Font::~Font ( void )
{
  TTF_CloseFont ( this->font );
  this->font = NULL;
}

bool Font::LoadTTF ( std::string filename, unsigned int font_size )
{
  this->font = TTF_OpenFont ( filename.c_str(), font_size );

  if ( this->font == NULL )
  {
    std::cout << "ERR: " << SDL_GetError() << std::endl;
    exit ( EXIT_FAILURE );
  }

  return true;
}

bool Font::DrawText ( SDL_Surface *video_buffer, std::string text, int x, int y, SDL_Color color )
{
  SDL_Surface *txt_buffer = NULL;
  SDL_Rect offsets;

  offsets.x = x;
  offsets.y = y;

  txt_buffer = TTF_RenderText_Solid ( this->font, text.c_str(), color );

  SDL_BlitSurface ( txt_buffer, NULL, video_buffer, &offsets );

  SDL_FreeSurface ( txt_buffer );

  return true;
}
