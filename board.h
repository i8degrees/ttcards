/******************************************************************************
    board.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/

#include <SDL.h>

#define BOARD_WIDTH 3
#define BOARD_HEIGHT 3

class Board
{

public:
    Board();

private:
    unsigned int grid[BOARD_WIDTH][BOARD_HEIGHT];
};
