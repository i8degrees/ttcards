/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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

void Free_CardHand ( CardHand* player_cards );

class Player
{
  public:
    Player ( void );
    Player ( CardHand* player_cards );
    ~Player ( void );

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
    std::shared_ptr<CardHand> hand;
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
