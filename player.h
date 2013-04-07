/******************************************************************************
    player.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef PLAYER_HEADERS
#define PLAYER_HEADERS

#include <iostream>
#include <string>

#include "SDL/SDL.h"

#include "cfg.h"
#include "card.h"
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
/*
  Must be kept initialized to NULL within private scope for the time being. Once
  the Player constructor is cleaned up -- some really shitty code exists as is --
  we can remove the initialization call here and do elsewhere (ex. our constructor).
*/
  SDL_Surface *card_buffer = NULL;

  //Card *card;

  //Pile *deck;
  //Hand card;

  Font txtCard;
  Font txtScore;

  unsigned int state;
  unsigned int score;

  bool BuildCard ( void );

};

#endif // PLAYERS_HEADERS defined
