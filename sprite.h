/******************************************************************************
    sprite.h

    SDL-based Sprite Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef SPRITE_HEADERS
#define SPRITE_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "SDL/SDL.h"

#include "gfx.h"

#define DEBUG_SPRITE

class Sprite {
  public:
    Sprite ( void );
    Sprite ( unsigned int width, unsigned int height );
    ~Sprite ( void );

    unsigned int GetX ( void );
    unsigned int GetY ( void );
    unsigned int GetWidth ( void );
    unsigned int GetHeight ( void );

    unsigned int GetXOffset ( void );
    unsigned int GetYOffset ( void );
    unsigned int GetWidthOffset ( void );
    unsigned int GetHeightOffset ( void );

    void SetX ( unsigned int x );
    void SetY ( unsigned int y );
    void SetWidth ( unsigned int width );
    void SetHeight ( unsigned int height );

    void SetXOffset ( unsigned int x_offset );
    void SetYOffset ( unsigned int y_offset );
    void SetWidthOffset ( unsigned int width_offset );
    void SetHeightOffset ( unsigned int height_offset );

    unsigned int GetSheetID ( void );
    unsigned int GetState ( void );
    void SetSheetID ( unsigned int id );
    void SetState ( unsigned int state );

    bool LoadImage ( std::string filename, SDL_Color colorkey = { 0, 0, 0 }, unsigned int flags = SDL_SRCCOLORKEY | SDL_RLEACCEL );
    bool Draw ( Gfx &engine );

  private:
    SDL_Surface *sprite_buffer; // memory buffer allocation

    SDL_Rect coords; // blitting coords struct
    SDL_Rect offsets; // source coords AKA clipping struct

    unsigned int sheet_id; // sprite sheets
    unsigned int state; // alive, dying, dead, ...

    Gfx engine;
};

#endif // SPRITE_HEADERS defined
