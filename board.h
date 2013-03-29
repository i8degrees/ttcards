/******************************************************************************
    board.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef BOARD_HEADERS
#define BOARD_HEADERS

#include <SDL.h>
#include <iostream>
#include <string>

#define BOARD_WIDTH 3
#define BOARD_HEIGHT 3

class Board
{

public:
    Board();
    bool CleanUp ( void );
    SDL_Surface *LoadBackground ( std::string filename );
    //bool LoadBackground ( std::string filename );


private:
    //unsigned int player1_score;
    //unsigned int player2_score;
    unsigned int grid[BOARD_WIDTH][BOARD_HEIGHT];
    //SDL_Surface *background = NULL;
};

#endif // BOARD_HEADERS
