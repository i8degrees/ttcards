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
#include "audio.h"
#include "card.h"
#include "card_debug.h"
#include "card_hand.h"
#include "card_view.h"

#include "font.h"
#include "sprite.h"

#define DEBUG_PLAYER

class Player: public CardHand
{
public:
  Player ( void );
  ~Player ( void );

  SDL_Rect GetXY ( void );
  void SetXY ( unsigned int x, unsigned int y );

  unsigned int GetType ( void );
  void SetType ( unsigned int player_type );

  unsigned int GetState ( void );
  void SetState ( unsigned int state );

  // TODO: Branch off into Score class
  unsigned int GetScore ( void );
  void SetScore ( unsigned int score );
  bool DrawScore ( Gfx &engine, unsigned int x, unsigned int y );

  void Input ( SDL_Event &input );
  bool Draw ( Gfx &engine, unsigned int x, unsigned int y );

private:
  //CardView card;
  Font text_score;

  unsigned int x;
  unsigned int y;
  unsigned int type; // HUMAN, CPU, DEBUG
  unsigned int state; // ...is it my turn turn yet?

  unsigned int score;

  Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects
};

#endif // PLAYERS_HEADERS defined
