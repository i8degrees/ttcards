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

class Board
{

public:
    Board();
    ~Board ( void );

    bool Init ( void );

    SDL_Surface *LoadBackground ( std::string filename );

    bool DrawBackground ( void );
    bool DrawBackground ( SDL_Surface *video_buffer );
    //bool LoadBackground ( std::string filename );


private:
    //unsigned int player1_score;
    //unsigned int player2_score;
    unsigned int grid[BOARD_GRID_WIDTH][BOARD_GRID_HEIGHT];
    SDL_Surface *background;
};

#endif // BOARD_HEADERS defined
