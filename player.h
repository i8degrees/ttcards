/******************************************************************************
    player.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef PLAYER_HEADERS
#define PLAYER_HEADERS

#include <iostream>

#include "SDL.h"

#include "cfg.h"
#include "font.h"
#include "sprite.h"

class Player
{
public:
  Player ( void );
  ~Player ( void );

  unsigned int GetScore ( void );
  bool SetScore ( unsigned int value );

  bool Draw ( SDL_Surface *video_buffer, int x, int y );
  bool DrawScore ( SDL_Surface *video_buffer, int x, int y );

private:
  unsigned int state;
  unsigned int score;
  Font txtCard;
  Font txtScore;
  SDL_Surface *card_buffer = NULL;

  bool BuildCard ( void );

};

#endif // PLAYERS_HEADERS defined
