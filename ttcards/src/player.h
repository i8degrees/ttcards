/******************************************************************************

    Human Player Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_PLAYER_HEADERS
#define GAMEAPP_PLAYER_HEADERS

#include <iostream>
#include <string>

#include <nomlib_types.hpp>
#include <math/Coords.hpp>

#include "card.h"
#include "card_debug.h"
#include "card_hand.h"
#include "card_view.h"
#include "cfg.h"

class Player
{
  public:
    Player ( void );
    ~Player ( void );

    void Init ( CardHand *player_cards, CardView *card_gfx );

    nom::int32 getX ( void );
    nom::int32 getY ( void );

    const nom::Coords getPosition ( void ) const;
    void setPosition ( nom::int32 x, nom::int32 y );

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
