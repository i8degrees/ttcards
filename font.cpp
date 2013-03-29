/******************************************************************************
    font.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "font.h"

using namespace std;

Font::Font ( void )
{
}

bool Font::Init ( void )
{
  if ( TTF_Init () == -1 )
  {
    std::cout << "ERR: " << SDL_GetError() << endl;
    exit(-1);
  }

  atexit ( TTF_Quit );

  return true;
}

bool Font::LoadTTF ( std::string filename, unsigned int size )
{
  font_size = size;

  font = TTF_OpenFont ( filename.c_str(), font_size );

  if ( font == NULL )
  {
    std::cout << "ERR -255: " << SDL_GetError() << endl;
    exit(-1);
  }

  return true;
}

//~Font ()

bool Font::DrawText ( SDL_Surface *video_buffer, std::string text, int x, int y, SDL_Color color )
{

  SDL_Rect offsets;

  offsets.x = x;
  offsets.y = y;

  SDL_Surface *txt_buffer = TTF_RenderText_Solid ( font, text.c_str(), color );

  SDL_BlitSurface ( txt_buffer, NULL, video_buffer, &offsets );

  return true;
}
