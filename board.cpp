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

SDL_Surface *Board::LoadBackground ( std::string filename )
{
  SDL_Surface *tmpBuffer = NULL;
  SDL_Surface *vBuffer = NULL;

  if ( ( tmpBuffer = SDL_LoadBMP ( filename.c_str() ) ) == NULL )
  {
    std::cout << "ERR -255: " << SDL_GetError() << std::endl;
    return NULL;
  }

/*
  SDL_SetColorKey ( tmpBuffer, SDL_SRCCOLORKEY,
                   SDL_MapRGB ( tmpBuffer->format, alpha.r, alpha.g, alpha.b ) );
*/

  vBuffer = SDL_DisplayFormatAlpha ( tmpBuffer );

  SDL_FreeSurface ( tmpBuffer );

  return vBuffer;
}

bool Board::DrawBackground ( SDL_Surface *video_buffer )
{
  background = Board::LoadBackground ( BOARD_BACKGROUND );

  SDL_BlitSurface ( background, NULL, video_buffer, NULL );

  return true;
}
