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
#include <algorithm>
#include <functional>
#include <memory>

/*
#include "json_spirit_writer_template.h"
#include "json_spirit_reader_template.h"

#ifndef JSON_SPIRIT_VALUE_ENABLED
  #define JSON_SPIRIT_VALUE_ENABLED
#endif
*/
#include <nomlib/types.hpp>
#include <nomlib/json.hpp>

#include "Card.hpp"
#include "CardCollection.hpp"
#include "config.hpp"

class CardHand
{
  public:
    typedef std::shared_ptr<CardHand> SharedPtr;

    CardHand ( void );
    ~CardHand ( void );

    bool push_back ( Card& card );
    bool erase ( Card& card );

    void clearSelectedCard ( void );
    Card& getSelectedCard ( void );
    void selectCard ( Card& card );

    bool exists ( const Card& card ) const;

    /// Make selected card the first element in stack
    ///
    /// The current card position is reset to zero.
    void front ( void );

    /// Obtain the current position we are at in the cards vector
    ///
    /// Returns a number between zero and the maximum card element in the
    /// vector.
    nom::int32 position ( void );

    /// Increment the position we are at in the cards vector by one.
    ///
    /// If we are at the end of the vector of cards, we reset the position to
    /// zero.
    ///
    /// We always update the selected card to this calculated position.
    void next ( void );

    /// Decrement the position we are at in the cards vector by one.
    ///
    /// If we are at the beginning or the end of the vector of cards, we reset
    /// the position to zero.
    ///
    /// We always update the selected card to this calculated position.
    void previous ( void );

    /// Sets the position we are at in the cards vector
    void set_position ( nom::int32 pos );

    void clear ( void );
    nom::int32 size ( void ) const;
    nom::int32 at ( Card& card );

    /// Creates a randomized player hand with the preferred minimum & maximum
    /// level ranges in mind, with no duplicate cards present.
    ///
    /// Do not forget to set the proper player ID on your new card objects
    /// before heading off into battle!
    void randomize ( nom::int32 level_min, nom::int32 level_max, CardCollection& db, nom::int32 seedling = 0 );

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

    /// Modify card rank values.
    void modifyCardRank ( bool modifier, nom::uint32 direction );

    /// Getter for obtaining the strongest card in the player's hand.
    ///
    /// Note that this does not take into account game rules that may be in
    /// effect!
    const Card strongest ( void );

    /// Getter for obtaining the weakest card in the player's hand.
    ///
    /// Note that this does not take into account game rules that may be in
    /// effect!
    const Card weakest ( void );

    /// \todo Declare in private scope
    Cards cards;

  private:
    /// Track the current position we are at in the cards vector
    nom::int32 card_position;

    /// Player's active card
    Card selectedCard;
};

/// Pretty print the the card attributes.
///
std::ostream& operator << ( std::ostream& os, const CardHand& rhs );

void Free_CardHand ( CardHand* ptr );

#endif // GAMEAPP_CARD_HAND_HEADERS defined
