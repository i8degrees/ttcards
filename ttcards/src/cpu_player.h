/******************************************************************************
    cpu_player.h

    CPU Player Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CPU_PLAYER_HEADERS
#define GAMEAPP_CPU_PLAYER_HEADERS

#include <iostream>
#include <string>

#include "gamelib.h"

#include "cfg.h"
#include "card_debug.h"
#include "player.h"
#include "board.h"

class CPUPlayer
{
public:
  CPUPlayer ( void );
  ~CPUPlayer ( void );

  void Init ( Board *board, CardHand *cards );

  void randomMove ( void );
  bool randomEdgeMove ( void );
  bool checkBoard ( void );
  void moveTo ( unsigned int x, unsigned int y );


private:
  Board *board;
  CardHand *hand;
  SDL_Rect grid[3][3]; // lookup table
  unsigned int cpu_difficulty; // easy = 1, hard = 2
};

#endif // CPU_PLAYERS_HEADERS defined
