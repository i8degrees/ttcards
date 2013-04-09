/******************************************************************************
    font.h

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

  bool LoadTTF ( std::string filename, unsigned int size );
  bool DrawText ( SDL_Surface *video_buffer, std::string text, int x, int y, SDL_Color color );

private:
  TTF_Font *font;
};

#endif // FONT_HEADERS defined
