/******************************************************************************
    player.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef PLAYER_HEADERS
#define PLAYER_HEADERS

#include <iostream>

#include "SDL.h"

class Player
{
public:
  Player ( void );
  ~Player ( void );

  unsigned int GetScore ( void );
  bool SetScore ( unsigned int value );

/*
  unsigned int GetState ( void );
  bool SetState ( unsigned int value);
*/

private:
  //unsigned int state;
  unsigned int score;
};

#endif // PLAYERS_HEADERS defined
