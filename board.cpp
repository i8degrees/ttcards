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

SDL_Surface *Board::LoadBackground ( std::string filename )
{
  SDL_Surface *tmpBuffer = NULL;
  SDL_Surface *vBuffer = NULL;
  SDL_Color alpha = { 0, 0, 0 };

  if ( ( tmpBuffer = SDL_LoadBMP ( filename.c_str() ) ) == NULL )
  {
    std::cout << "ERR -255: " << SDL_GetError() << std::endl;
    return NULL;
  }

  SDL_SetColorKey ( tmpBuffer, SDL_SRCCOLORKEY,
                   SDL_MapRGB ( tmpBuffer->format, alpha.r, alpha.g, alpha.b ) );

  vBuffer = SDL_DisplayFormatAlpha ( tmpBuffer );

  SDL_FreeSurface ( tmpBuffer );

  return vBuffer;

  //return true;
}

bool Board::CleanUp ( void )
{
  // SDL_FreeSurface ( tmpBuffer );
  // SDL_FreeSurface ( background );

  return true;
}
