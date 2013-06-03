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

  this->cpu_difficulty = 2; // easy = 1, hard = 2

  for ( int cols = 0; cols < 3; cols++ )
  {
    for ( int rows = 0; rows < 3; rows++ )
    {
      this->grid[rows][cols].x = rows;
      this->grid[rows][cols].y = cols;
    }
  }
}

CPUPlayer::~CPUPlayer ( void )
{
  #ifdef DEBUG_CPU_PLAYER_OBJ
    std::cout << "CPUPlayer::~CPUPlayer (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}

void CPUPlayer::Init ( Board *board, CardHand *cards )
{
  this->board = board; // initialize game board
  this->hand = cards;
}

void CPUPlayer::randomMove ( void )
{
  unsigned int moveX = std::rand() % BOARD_GRID_WIDTH;
  unsigned int moveY = std::rand() % BOARD_GRID_HEIGHT;
  unsigned int cID = std::rand() % MAX_PLAYER_HAND;

  std::cout << "CPU:" << " " << "Easy Mode [random]" << std::endl;
  this->hand->selectCard ( this->hand->cards[cID] );
  this->moveTo ( moveX, moveY );
}

bool CPUPlayer::randomEdgeMove ( void )
{
  if ( this->board->getPlayerCount ( 0 ) == 0 && this->hand->cards.size() > 0 ) // first turn
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

      if ( this->board->getStatus ( edge[rand_choice].x, edge[rand_choice].y ) == false )
      {
        return false;
      }
    }
    return true;
}

void CPUPlayer::moveTo ( unsigned int x, unsigned int y )
{
  // ...
}
