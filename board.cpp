/******************************************************************************
    board.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "board.h"

using namespace std;

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

bool Board::LoadBackground ( void )
{
  this->background = this->gfx.LoadBackground ( BOARD_BACKGROUND );

  return true;
}

bool Board::DrawBackground ( SDL_Surface *video_buffer )
{
  SDL_BlitSurface ( this->background, NULL, video_buffer, NULL );

  return true;
}
