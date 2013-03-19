/******************************************************************************
    board.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "board.h"

Board::Board ( void )
{
    for ( int y = 0; y < BOARD_HEIGHT; y++ )
    {
        for ( int x = 0; x < BOARD_WIDTH; x++ )
        {
            grid[y][x] = 0;
        }
    }
}
