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
#ifndef GAMEAPP_BOARD_HEADERS
#define GAMEAPP_BOARD_HEADERS

#include <iostream>
#include <string>
#include <utility>
#include <memory>

#include <nomlib/types.hpp>

#include "card.h"
#include "card_debug.h"
#include "card_rules.h"
#include "GameObject.hpp"
#include "cfg.h"

class Board
{
  public:
    Board ( void );
    Board ( CardRules& rules, std::shared_ptr<GameObject> object );
    ~Board ( void );

    std::vector<std::pair<nom::int32, nom::int32>> checkBoard ( nom::int32 x, nom::int32 y );

    // TODO: Consider branching this into Score class
    unsigned int getCount ( void );
    unsigned int getPlayerCount ( unsigned int player_id );

    nom::int32 getStatus ( nom::int32 x, nom::int32 y );
    void updateStatus ( unsigned int x, unsigned int y, Card &card );
    unsigned int getPlayerID ( unsigned int x, unsigned int y );
    void flipCard ( unsigned int x, unsigned int y, unsigned int player_id );
    std::string getName ( unsigned int x, unsigned int y );
    Card &getCard ( unsigned int x, unsigned int y );

    void Update ( unsigned int x, unsigned int y ); // TODO
    void Draw ( void* video_buffer );
    void List ( void );

    const nom::int32 operator() ( const nom::int32 x, const nom::int32 y );

  private:
    void initialize ( void );
    std::shared_ptr<GameObject> state;
    /// Debug support for card attributes
    CardDebug debug;
    /// Card rules
    CardRules rules;
    /// 2D vector of Card data containers
    std::vector<std::vector<Card>> grid;
};

#endif // GAMEAPP_BOARD_HEADERS defined
