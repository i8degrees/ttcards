/******************************************************************************

    Human Player Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_PLAYER_HEADERS
#define GAMEAPP_PLAYER_HEADERS

#include <iostream>
#include <string>

#include <nomlib/math.hpp>

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

    void Init ( CardHand *player_cards );

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
    /// debug support for card attributes
    CardDebug debug;
    /// Card rendering
    CardView card;
    /// pointer reference to player's hand
    CardHand *hand;

    /// x, y origin coords
    nom::Coords coords;
    /// unique identifier for tracking each player in game
    unsigned int id;
    /// not implemented
    unsigned int state;
    /// player's scoreboard
    unsigned int score;
};

#endif // GAMEAPP_PLAYER_HEADERS defined
