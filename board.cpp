/******************************************************************************
    board.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "board.h"

/*
TODO: Add configurable board init, such as:

  Board ( unsigned int board_width, unsigned int board_height );

*/
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

  SDL_FreeSurface ( this->background );
  this->background = NULL;
}

// TODO: Reconsider using an instance of Gfx solely for this method
bool Board::LoadBackground ( std::string filename )
{
  this->background = this->engine.LoadImage ( filename );

  return true;
}

bool Board::DrawBackground ( Gfx &engine )
{
  engine.DrawSurface ( this->background, 0, 0 );

  return true;
}

unsigned int Board::GetStatus ( unsigned int x, unsigned int y )
{
  return this->grid[x][y];
}

void Board::UpdateBoard ( unsigned int x, unsigned int y, unsigned int state )
{
  this->grid[x][y] = state;
}

void Board::Draw ( void ) // void Board::Draw ( Gfx &engine )
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
