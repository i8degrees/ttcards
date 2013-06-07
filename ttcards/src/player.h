/******************************************************************************
    player.h

    Human Player Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_PLAYER_HEADERS
#define GAMEAPP_PLAYER_HEADERS

#include <iostream>
#include <string>

#include "cfg.h"
#include "card_debug.h"
#include "card_hand.h"
#include "card_view.h"

#include "Coords.h"

class Player
{
  public:
    Player ( void );
    ~Player ( void );

    void Init ( CardHand *player_cards, CardView *card_gfx );

    unsigned int getX ( void );
    unsigned int getY ( void );

    std::pair <int, int> getXY ( void );
    void setXY ( unsigned int x, unsigned int y );

    unsigned int getID ( void );
    void setID ( unsigned int id_ );

    unsigned int getState ( void );
    void setState ( unsigned int state );

    // TODO: Consider branching this into Score class
    unsigned int getScore ( void );
    void setScore ( unsigned int score );

    void Update ( void );
    void Draw ( void* video_buffer );

  private:
    CardDebug debug; // debug support for card attributes
    CardView *card; // pointer referencing our card rendering class
    CardHand *hand; // pointer reference to player's hand

    nom::Coords coords; // x, y origin coords
    unsigned int id; // unique identifier for tracking each player in game
    unsigned int state; // not implemented
    unsigned int score; // player's scoreboard
};

#endif // GAMEAPP_PLAYER_HEADERS defined
