/******************************************************************************
    cpu_player.cpp

    CPU Player Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "cpu_player.h"

CPUPlayer::CPUPlayer ( void )
{
  #ifdef DEBUG_CPU_PLAYER_OBJ
    std::cout << "CPUPlayer::CPUPlayer (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}

CPUPlayer::~CPUPlayer ( void )
{
  #ifdef DEBUG_CPU_PLAYER_OBJ
    std::cout << "CPUPlayer::~CPUPlayer (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}

void CPUPlayer::Init ( Board *board )
{
  this->board = board; // initialize game board
}

void CPUPlayer::calcEdgePos ( void )
{
  if ( this->board->GetPlayerCardCount ( 0 ) == 0 && this->hand->cards.size() > 0 ) // first turn
    {
      SDL_Rect edge[4];

      edge[0].x = 0;
      edge[0].y = 0;

      edge[1].x = 2;
      edge[1].y = 0;

      edge[2].x = 0;
      edge[2].y = 2;

      edge[3].x = 2;
      edge[3].y = 2;

      unsigned int rand_choice = std::rand() % 4;

      if ( this->board->GetStatus ( edge[rand_choice].x, edge[rand_choice].y ) == false )
      {
        return;
      }
    }
}

void CPUPlayer::moveTo ( unsigned int x, unsigned int y )
{
  // ...
}
