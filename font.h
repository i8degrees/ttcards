/******************************************************************************
    font.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef FONT_HEADERS
#define FONT_HEADERS

#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"

class Font {
public:
  Font ( void );
  ~Font ( void )
  {
    TTF_CloseFont ( font );
  }

  bool Init ( void );
  bool LoadTTF ( std::string filename, unsigned int size );
  bool DrawText ( SDL_Surface *video_buffer, std::string text, int x, int y, SDL_Color color );

private:
  unsigned int font_size;
  unsigned int x;
  unsigned int y;

  TTF_Font *font = NULL;

  SDL_Surface *text_buffer = NULL;
};

#endif // FONT_HEADERS
