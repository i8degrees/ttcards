/******************************************************************************
    board.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef BOARD_HEADERS
#define BOARD_HEADERS

#include "SDL/SDL.h"
#include <iostream>
#include <string>

#include "cfg.h"
#include "gfx.h"

class Board
{
  public:
    Board ( void );
    ~Board ( void );

    bool LoadBackground ( std::string filename );
    bool DrawBackground ( Gfx &engine );

  private:
    SDL_Surface *background;
    Gfx *engine;
    unsigned int grid[3][3];
};

#endif // BOARD_HEADERS defined
