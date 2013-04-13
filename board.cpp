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

  this->background = NULL;
  this->engine = NULL;

  for ( int y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( int x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
        this->grid[y][x] = 0;
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

  this->engine = NULL;
}

bool Board::LoadBackground ( std::string filename )
{
  this->background = this->engine->LoadImage ( filename );

  return true;
}

bool Board::DrawBackground ( Gfx &engine )
{
  engine.DrawSurface ( this->background, 0, 0 );

  return true;
}
