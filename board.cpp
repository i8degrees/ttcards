/******************************************************************************
    board.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "board.h"

Board::Board ( void )
{
  int x, y = 0;

  #ifdef DEBUG_BOARD_OBJ
    std::cout << "Board::Board (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
  {
    for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
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

void Board::Draw ( void ) //void Board::Draw ( Gfx &engine )
{
  int x, y = 0;

  for ( x = 0; x < BOARD_GRID_WIDTH; x++ )
  {
    for ( y = 0; y < BOARD_GRID_HEIGHT; y++ )
    {
      std::cout << this->grid[x][y] << " ";
    }
    std::cout << "\n" << std::endl;
  }
}
