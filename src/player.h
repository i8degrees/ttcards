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

  unsigned int getID ( void );
  void setID ( unsigned int id );

  unsigned int getState ( void );
  void setState ( unsigned int state );

  // TODO: Consider branching this into Score class
  unsigned int getScore ( void );
  void setScore ( unsigned int score );

  void Update ( Gfx *engine );
  void Draw ( Gfx *engine );

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
