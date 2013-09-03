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

#include "json_spirit_writer_template.h"
#include "json_spirit_reader_template.h"

#ifndef JSON_SPIRIT_VALUE_ENABLED
  #define JSON_SPIRIT_VALUE_ENABLED
#endif

#include <nomlib/types.hpp>
#include <nomlib/json.hpp>

#include "Card.hpp"
#include "CardCollection.hpp"
#include "config.hpp"

class CardHand
{
  public:
    CardHand ( void );
    ~CardHand ( void );

    bool push_back ( Card& card );
    bool erase ( Card& card );

    void clearSelectedCard ( void );
    Card& getSelectedCard ( void );
    void selectCard ( Card& card );

    bool exists ( const Card& card ) const;

    /// Make selected card the first element in stack
    void front ( void );
    void clear ( void );
    nom::int32 size ( void ) const;
    nom::int32 at ( Card& card );

    void randomize ( nom::uint32 level_min, nom::uint32 level_max, CardCollection& db, nom::uint64 seedling = 0 );

    /// Save the current player's hand to a file as a series of RFC 4627 JSON
    /// compliant objects.
    ///
    /// Order in which data is saved *does* matter and must match to the order
    /// in which it is loaded.
    bool save ( const std::string& filename );

    /// Load saved player hand from a file encoded as RFC 4627 compliant JSON
    /// objects.
    ///
    /// Loading order *does* matter and expects the order in which it was saved
    /// to match precisely.
    bool load ( const std::string& filename );

    void modifyCardRank ( bool modifier, nom::uint32 direction );

    /// \todo Declare in private scope
    std::vector<Card> cards;

  private:
    /// Player's active card
    Card selectedCard;
};

#endif // GAMEAPP_CARD_HAND_HEADERS defined
