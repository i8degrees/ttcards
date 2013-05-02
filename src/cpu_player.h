/******************************************************************************
    cpu_player.h

    CPU Player Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CPU_PLAYER_HEADERS
#define GAMEAPP_CPU_PLAYER_HEADERS

#include <iostream>
#include <string>

#include "gamelib/gamelib.h"

#include "cfg.h"
#include "card_debug.h"
#include "player.h"
#include "board.h"

class CPUPlayer: public Player
{
public:
  CPUPlayer ( void );
  ~CPUPlayer ( void );

  void Init ( Board *board );

  void calcEdgePos ( void );
  void moveTo ( unsigned int x, unsigned int y );

private:
  Board *board;
};

#endif // CPU_PLAYERS_HEADERS defined
