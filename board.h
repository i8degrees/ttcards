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
    Board();
    ~Board ( void );

    bool LoadBackground ( void );
    bool DrawBackground ( SDL_Surface *video_buffer );

    Gfx gfx;

  private:
    //unsigned int player1_score;
    //unsigned int player2_score;
    SDL_Surface *background;
    int grid[3][3] = {  0, 0, 0,
                        0, 0, 0,
                        0, 0, 0 };
};

#endif // BOARD_HEADERS defined
