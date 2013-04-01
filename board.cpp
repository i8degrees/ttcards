/******************************************************************************
    board.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "board.h"

using namespace std;

Board::Board ( void )
{
  for ( int y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( int x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
        grid[y][x] = 0;
    }
  }

}

bool Board::Init ( void )
{
  SDL_Surface *background = NULL;

  return true;
}

Board::~Board ( void )
{
  SDL_FreeSurface ( background );
}

bool Board::LoadBackground ( void )
{
  background = gfx.LoadBackground ( BOARD_BACKGROUND );

  return true;
}

bool Board::DrawBackground ( SDL_Surface *video_buffer )
{
  SDL_BlitSurface ( background, NULL, video_buffer, NULL );

  return true;
}
