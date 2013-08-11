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
#ifndef GAMEAPP_CARD_HAND_HEADERS
#define GAMEAPP_CARD_HAND_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>

#include <nomlib/types.hpp>

#include "card.hpp"
#include "card_collection.hpp"
#include "cfg.hpp"

class CardHand
{
  public:
    CardHand ( void );
    ~CardHand ( void );

    bool addCard ( Card& card );
    bool removeCard ( Card& card );

    void clearSelectedCard ( void );
    Card& getSelectedCard ( void );
    bool selectCard ( Card& card );

    bool exists ( const Card& card ) const;

    void clear ( void );
    bool empty ( void );
    nom::int32 size ( void ) const;
    nom::int32 pos ( Card& card );

    void randomize ( nom::uint32 level_min, nom::uint32 level_max, Collection& db, nom::uint64 seedling = 0 );

    /// \todo Declare in private scope
    std::vector<Card> cards;

  private:
    /// holds player's selected (think: ready to place) card
    Card selectedCard;
};

#endif // GAMEAPP_CARD_HAND_HEADERS defined