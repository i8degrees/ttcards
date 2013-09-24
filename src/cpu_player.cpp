/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "cpu_player.hpp"

CPUPlayer::CPUPlayer ( void )
{
NOM_LOG_TRACE ( TTCARDS );

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
NOM_LOG_TRACE ( TTCARDS );
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
