/******************************************************************************
    font.h

    SDL-based Font Rendering API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef FONT_HEADERS
#define FONT_HEADERS

#include <iostream>
#include <string>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

class Font {
public:
  Font ( void );
  ~Font ( void );

  SDL_Color GetTextColor ( void );
  void SetTextColor ( unsigned r, unsigned g, unsigned b );

  bool LoadTTF ( std::string filename, unsigned int size );
  bool DrawText ( Gfx &engine, std::string text, unsigned int x, unsigned int y );

private:
  TTF_Font *font;
  SDL_Color text_color = { 0, 0, 0 };
  SDL_Rect coords = { 0, 0, 0, 0 };
};

#endif // FONT_HEADERS defined
