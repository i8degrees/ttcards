/******************************************************************************
    player.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_PLAYER_HEADERS
#define GAMEAPP_PLAYER_HEADERS

#include <iostream>
#include <string>

#include "gamelib/gamelib.h"

#include "cfg.h"
#include "board.h"
#include "card_debug.h"
#include "card_hand.h"
#include "card_view.h"

class Player
{
public:
  Player ( void );
  ~Player ( void );

  void Init ( CardHand *player_cards );

  SDL_Rect GetXY ( void );
  void SetXY ( unsigned int x, unsigned int y );

  unsigned int GetID ( void );
  void SetID ( unsigned int id );

  unsigned int GetState ( void );
  void SetState ( unsigned int state );

  // TODO: Consider branching this into Score class
  unsigned int GetScore ( void );
  void SetScore ( unsigned int score );

  void Draw ( Gfx *engine );
  void Update ( Gfx *engine );

  void DrawScore ( Gfx *engine, unsigned int x, unsigned int y );

private:
  Font text_score;
  CardDebug debug;
  CardView card;
  CardHand *hand;

  unsigned int x;
  unsigned int y;
  unsigned int id; // unique identifier for tracking each player in game
  unsigned int state;
  unsigned int score;
};

#endif // PLAYERS_HEADERS defined
