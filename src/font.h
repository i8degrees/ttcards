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

#include "gfx.h"

#define DEBUG_FONT
#define DEBUG_FONT_OBJ

class Font {
public:
  Font ( void );
  ~Font ( void );

  unsigned int GetTextWidth ( void );
  unsigned int GetTextHeight ( void );
  SDL_Color GetTextColor ( void );
  std::string GetTextBuffer ( void );

  void SetTextColor ( unsigned r, unsigned g, unsigned b );
  void SetTextBuffer ( std::string text );

  bool LoadTTF ( std::string filename, unsigned int size );
  bool DrawText ( Gfx *engine, unsigned int x, unsigned int y );

private:
  TTF_Font *font;
  SDL_Color text_color;
  std::string text_buffer;
  SDL_Rect coords;
};

#endif // FONT_HEADERS defined
