/******************************************************************************
    board.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "board.h"

Board::Board ( void )
{
  this->background = NULL;

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
  SDL_FreeSurface ( this->background );
  this->background = NULL;
}

bool Board::LoadBackground ( Gfx &engine, std::string filename )
{
  this->background = engine.LoadImage ( filename );

  return true;
}

bool Board::DrawBackground ( Gfx &engine )
{
  engine.DrawSurface ( this->background, 0, 0 );

  return true;
}
