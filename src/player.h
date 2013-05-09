/******************************************************************************
    player.h

    Human Player Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_PLAYER_HEADERS
#define GAMEAPP_PLAYER_HEADERS

#include <iostream>
#include <string>
#include <utility>

#include "gamelib.h"

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

  void Init ( CardHand *player_cards, CardView *card_gfx );

  std::pair <int, int> getXY ( void );
  void setXY ( unsigned int x, unsigned int y );

  unsigned int GetID ( void );
  void SetID ( unsigned int id );

  unsigned int GetState ( void );
  void SetState ( unsigned int state );

  // TODO: Consider branching this into Score class
  unsigned int GetScore ( void );
  void SetScore ( unsigned int score );

  void Draw ( Gfx *engine );
  void Update ( Gfx *engine );

private:
  CardDebug debug;
  CardView *card;
  CardHand *hand;

  std::pair <int, int> coords; // x, y coords
  //unsigned int x;
  //unsigned int y;
  unsigned int id; // unique identifier for tracking each player in game
  unsigned int state;
  unsigned int score;
};

#endif // PLAYERS_HEADERS defined
