/******************************************************************************
    board.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "board.h"

Board::Board ( void )
{
  #ifdef DEBUG_BOARD_OBJ
    std::cout << "Board::Board (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  for ( int y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( int x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      this->grid[x][y] = 0;
    }
  }
}

Board::~Board ( void )
{
  #ifdef DEBUG_BOARD_OBJ
    std::cout << "Board::~Board (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

unsigned int Board::GetStatus ( unsigned int x, unsigned int y )
{
  return this->grid[x][y];
}

void Board::UpdateBoard ( unsigned int x, unsigned int y, unsigned int state )
{
  this->grid[x][y] = state;
}

void Board::Draw ( Gfx &engine )
{
  // ...
}
