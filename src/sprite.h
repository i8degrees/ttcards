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
#include <cmath>

#define DEBUG_SPRITE
//#define DEBUG_SPRITE_OBJ

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
    void SetXY ( unsigned int x, unsigned int y );
    void UpdateXY ( unsigned int x, unsigned int y );
    void SetWidth ( unsigned int width );
    void SetHeight ( unsigned int height );
    void SetXOffset ( unsigned int x_offset );
    void SetYOffset ( unsigned int y_offset );
    void SetWidthOffset ( unsigned int width_offset );
    void SetHeightOffset ( unsigned int height_offset );
    unsigned int GetState ( void );
    void SetState ( unsigned int state );

    signed int GetSheetID ( void );
    void SetSheetID ( signed int id );
    //struct sheet GetSheetDimensions ( void );
    void SetSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding );

    bool LoadImage ( std::string filename, SDL_Color colorkey = { 0, 0, 0 }, unsigned int flags = SDL_SRCCOLORKEY | SDL_RLEACCEL );
    bool Draw ( Gfx *engine );

  private:
    SDL_Surface *sprite_buffer; // memory buffer allocation

    struct {
      unsigned int x; // sprite x-axis coord for blitting onto video surface
      unsigned int y; // sprite y-axis coord for blitting onto video surface
      unsigned int width; // sprite width
      unsigned int height; // sprite height
    } coords;

    /* These are not used at the moment and are reserved for future implementation */
    struct {
      unsigned int x; // clipping offset X coord
      unsigned int y; // clipping offset Y coord
      unsigned int width; // clipping offset width
      unsigned int height; // clipping offset height
    } offsets;

    unsigned int state; // alive, dying, dead, ...

    struct {
      signed int id; // maps a specific sprite within sheet
      unsigned int sprite_width; // width of sprite in sheet
      unsigned int sprite_height; // height of sprite in sheet
      unsigned int width; // width of sprite sheet
      unsigned int height; // height of sprite sheet
      unsigned int spacing; // applied between each sheet tile
      unsigned int padding; // applied on all four sides of sheet tile
    } sheet;

};

#endif // SPRITE_HEADERS defined
