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

#define DEBUG_BOARD
#define DEBUG_BOARD_OBJ

class Board
{
  public:
    Board ( void );
    ~Board ( void );

    unsigned int GetStatus ( unsigned int x, unsigned int y );

    void UpdateBoard ( unsigned int x, unsigned int y, unsigned int state );
    void Draw ( Gfx &engine );

  private:
    unsigned int grid[3][3];
};

#endif // BOARD_HEADERS defined
