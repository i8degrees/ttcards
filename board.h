/******************************************************************************
    board.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef BOARD_HEADERS
#define BOARD_HEADERS

#include "SDL.h"
#include <iostream>
#include <string>

#include "cfg.h"
#include "gfx.h"

class Board
{

public:
    Board();
    ~Board ( void );

    bool Init ( void );

    bool LoadBackground ( void );
    bool DrawBackground ( SDL_Surface *video_buffer );

    Gfx gfx;

private:
    //unsigned int player1_score;
    //unsigned int player2_score;
    unsigned int grid[BOARD_GRID_WIDTH][BOARD_GRID_HEIGHT];
    SDL_Surface *background;
};

#endif // BOARD_HEADERS defined
